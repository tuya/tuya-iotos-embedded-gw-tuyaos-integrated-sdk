/*
 * Copyright (C) 2021 Tuya Inc.
 */

#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <termios.h>

#include "tkl_uart.h"
#include "tkl_output.h"

#define MAX_PATH                256

/* system uart name */
#define UART_NAME_PATTERN_SYS   "/dev/ttyS%d"
/* USB uart name */
#define UART_NAME_PATTERN_USB   "/dev/ttyUSB%d"

#define UART_TYPE_VALID(type)   (((type) >= 0) && ((type) < TUYA_UART_MAX_TYPE))
#define UART_DEV_MAX            32
#define UART_DEV_VALID(num)     (((num) >= 0) && ((num) < UART_DEV_MAX))

// uart fd-list
STATIC INT_T uart_list[TUYA_UART_MAX_TYPE][UART_DEV_MAX];

STATIC BOOL_T uart_dev_valid(INT_T type, INT_T num)
{
    if (!UART_TYPE_VALID(type)) {
        tkl_log_output("invalid uart type %d\n", type);
        return FALSE;
    }
    if (!UART_DEV_VALID(num)) {
        tkl_log_output("invalid uart number %d\n", num);
        return FALSE;
    }
    return TRUE;
}

/* save fd into fd-list */
static INT_T save_fd(UINT_T port_id, INT_T fd)
{
    INT_T type = TUYA_UART_GET_PORT_TYPE(port_id);
    INT_T num = TUYA_UART_GET_PORT_NUMBER(port_id);

    if (!uart_dev_valid(type, num)) {
        return -1;
    }

    uart_list[type][num] = fd;
    return 0;
}

/* get fd from fd-list */
STATIC INT_T get_fd(UINT_T port_id)
{
    INT_T type = TUYA_UART_GET_PORT_TYPE(port_id);
    INT_T num = TUYA_UART_GET_PORT_NUMBER(port_id);

    if (!uart_dev_valid(type, num)) {
        return -1;
    }

    return uart_list[type][num];
}

STATIC VOID disable_flowcontrol(INT_T fd)
{
    struct termios tios;

    memset(&tios, 0, sizeof(tios));

    tcgetattr(fd, &tios);
    tios.c_cflag &= ~CRTSCTS;
    tcsetattr(fd, TCSANOW, &tios);
}

OPERATE_RET tkl_uart_init(UINT_T port_id, TUYA_UART_BASE_CFG_T *cfg)
{
    INT_T type = TUYA_UART_GET_PORT_TYPE(port_id);
    INT_T num = TUYA_UART_GET_PORT_NUMBER(port_id);
    CHAR_T pattern[MAX_PATH] = {0};
    CHAR_T path[MAX_PATH] = {0};
    INT_T fd = -1;
    struct termios tty_attr;
    speed_t speed;
    TUYA_UART_FLOWCTRL_TYPE_E flowcntl;

    if (!uart_dev_valid(type, num)) {
        return OPRT_INVALID_PARM;
    }
    if (cfg == NULL) {
        return OPRT_INVALID_PARM;
    }

    // get uart name pattern
    if (type == TUYA_UART_SYS) {
        strncpy(pattern, UART_NAME_PATTERN_SYS, strlen(UART_NAME_PATTERN_SYS) + 1);
    } else if (type == TUYA_UART_USB) {
        strncpy(pattern, UART_NAME_PATTERN_USB, strlen(UART_NAME_PATTERN_USB) + 1);
    } else {
        tkl_log_output("Invalid uart type %d\n", type);
        return OPRT_INVALID_PARM;
    }

    // get uart name
    sprintf(path, pattern, num);

    fd = open(path, O_RDWR|O_NOCTTY|O_NONBLOCK);
    if (fd < 0) {
        tkl_log_output("Failed to open %s\n", path);
        return OPRT_NOT_FOUND;
    }

    // configure uart

    memset(&tty_attr, 0, sizeof(struct termios));
    tcgetattr(fd, &tty_attr);

    /*
     * Enable Raw mode: all special processing of input and output
     * characters is disabled.
     */
    tty_attr.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                          | INLCR | IGNCR | ICRNL | IXON);
    tty_attr.c_oflag &= ~OPOST;
    tty_attr.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

    // set baudrate
    switch (cfg->baudrate)
    {
    case 1800:
        speed = B1800; break;
    case 2500:
        speed = B2400; break;
    case 4800:
        speed = B4800; break;
    case 9600:
        speed = B9600; break;
    case 38400:
        speed = B38400; break;
    case 57600:
        speed = B57600; break;
    case 115200:
        speed = B115200; break;
    case 230400:
        speed = B230400; break;
    case 460800:
        speed = B460800; break;
    case 921600:
        speed = B921600; break;
    default:
        speed = B9600; break;
    }
    cfsetospeed(&tty_attr, speed);
    cfsetispeed(&tty_attr, speed);

    // set databits
    switch (cfg->databits) {
    case TUYA_UART_DATA_LEN_5BIT:
        tty_attr.c_cflag = (tty_attr.c_cflag & ~CSIZE) | CS5;
        break;
    case TUYA_UART_DATA_LEN_6BIT:
        tty_attr.c_cflag = (tty_attr.c_cflag & ~CSIZE) | CS6;
        break;
    case TUYA_UART_DATA_LEN_7BIT:
        tty_attr.c_cflag = (tty_attr.c_cflag & ~CSIZE) | CS7;
        break;
    default: // TUYA_UART_DATA_BIT8
        tty_attr.c_cflag = (tty_attr.c_cflag & ~CSIZE) | CS8;
        break;
    }

    // set parity
    switch (cfg->parity) {
    case TUYA_UART_PARITY_TYPE_EVEN:
        tty_attr.c_cflag &= ~(PARODD /*| CMSPAR*/);
        tty_attr.c_cflag |= PARENB;
        break;
    case TUYA_UART_PARITY_TYPE_ODD:
       // tty_attr.c_cflag &= ~CMSPAR;
        tty_attr.c_cflag |= PARENB | PARODD;
        break;
    default: // TUYA_UART_PARITY_NONE
        tty_attr.c_cflag &= ~(PARENB | PARODD /*| CMSPAR*/);
        break;
    }

    // set stopbits
    switch (cfg->stopbits) {
    case TUYA_UART_STOP_LEN_2BIT:
        tty_attr.c_cflag |= CSTOPB;
        break;
    default: // TUYA_UART_STOP_BIT1
        tty_attr.c_cflag &= ~CSTOPB;
        break;
    }

    // set flowcntrl
    flowcntl = cfg->flowctrl;
    switch (flowcntl) {
    case TUYA_UART_FLOWCTRL_RTSCTS:
        tty_attr.c_cflag |= CRTSCTS;
        tty_attr.c_iflag &= ~(IXON | IXOFF | IXANY);
        break;
    case TUYA_UART_FLOWCTRL_XONXOFF:
        tty_attr.c_cflag &= ~(CRTSCTS);
        tty_attr.c_iflag |= IXON | IXOFF;
        break;
    default: // TUYA_UART_FLOWCTRL_NONE
        tty_attr.c_cflag &= ~(CRTSCTS);
        tty_attr.c_iflag &= ~(IXON | IXOFF | IXANY);
        break;
    }

    // set local: local or modem
    tty_attr.c_cflag |= CLOCAL;
    // tty_attr.c_cflag &= ~CLOCAL;

    // set hupcl: hup-on-close
    tty_attr.c_cflag |= HUPCL;
    // tty_attr.c_cflag &= ~HUPCL;

    tty_attr.c_cflag |= CREAD;  // enable reception of characters
    tty_attr.c_cc[VMIN] = 1;

    if (tcsetattr(fd, TCSANOW, &tty_attr) < 0) {
        close(fd);
        tkl_log_output("Failed to config uart: %s\n", path);
        return OPRT_COM_ERROR;
    }

    // save fd into list
    save_fd(port_id, fd);

    return OPRT_OK;
}

OPERATE_RET tkl_uart_deinit(UINT_T port_id)
{
    INT_T fd = -1;

    fd = get_fd(port_id);
    if (fd == -1 || fd == 0) {
        return OPRT_OK;
    }
    disable_flowcontrol(fd);
    close(fd);
    save_fd(port_id, -1);

    return OPRT_OK;
}

INT_T tkl_uart_write(UINT_T port_id, VOID_T *buff, UINT16_T len)
{
    INT_T fd = -1;
    INT_T ret = 0;

    fd = get_fd(port_id);
    if (fd == -1 || fd == 0) {
        return OPRT_COM_ERROR;
    }

    ret = write(fd, buff, len);
    if (ret < 0) {
        tkl_log_output("uart send failed, port=0x%X\n", port_id);
        return OPRT_SEND_ERR;
    }
    fsync(fd);

    return ret;
}

INT_T tkl_uart_read(UINT_T port_id, VOID_T *buff, UINT16_T len)
{
    INT_T fd = -1;
    INT_T ret = 0;

    fd = get_fd(port_id);
    if (fd == -1 || fd == 0) {
        return OPRT_COM_ERROR;
    }

    ret = read(fd, buff, len);
    if (ret < 0) {
        return OPRT_RECV_ERR;
    }

    return ret;
}

OPERATE_RET tkl_uart_wait_for_data(UINT_T port_id, INT_T timeout_ms)
{
    INT_T fd = -1;
    fd_set read_set;
    INT_T ret = 0;
    struct timeval timeval_uart = {
        .tv_sec = timeout_ms / 1000,
        .tv_usec = (timeout_ms % 1000) * 1000
    };
    struct timeval *p_timeval = NULL;

    if (timeout_ms != -1) {    // -1 : block indefinitely
        p_timeval = &timeval_uart;
    }

    fd = get_fd(port_id);
    if (fd == -1 || fd == 0) {
        return OPRT_COM_ERROR;
    }

    FD_ZERO(&read_set);
    FD_SET(fd, &read_set);

    if (select(fd + 1, &read_set, NULL, NULL, p_timeval) <= 0) {
        tkl_log_output("uart recv timeout\n");
        return OPRT_TIMEOUT;
    }

    if (!FD_ISSET(fd, &read_set)) {
        //hal_log(HAL_LOG_ERR, "unexpected uart fd\n");
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

VOID_T tkl_uart_rx_irq_cb_reg(UINT_T port_id, TUYA_UART_IRQ_CB rx_cb)
{
    // do nothing
    return;
}

VOID_T tkl_uart_tx_irq_cb_reg(UINT_T port_id, TUYA_UART_IRQ_CB tx_cb)
{
    // do nothing
    return;
}

OPERATE_RET tkl_uart_set_tx_int(UINT_T port_id, BOOL_T enable)
{
    return OPRT_NOT_SUPPORTED;
}

OPERATE_RET tkl_uart_set_rx_flowctrl(UINT_T port_id, BOOL_T enable)
{
    // flowctrl should be set in tkl_uart_init
    return OPRT_NOT_SUPPORTED;
}
