/**
 * @file tkl_wired.c
 * @brief the default weak implements of wired driver, this implement only used when OS=linux
 * @version 0.1
 * @date 2019-08-15
 * 
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <net/if.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

#include "uni_log.h"
#include "tkl_wired.h"

#define WIRED_IFNAME "eth0"

TKL_WIRED_STATUS_CHANGE_CB g_link_status_change_cb;

STATIC VOID *link_status_thread(VOID *arg)
{
    INT_T old_status = -1;
    TKL_WIRED_STAT_E new_status;

    while (1) {
        tkl_wired_get_status(&new_status);
        if (old_status != new_status) {
            g_link_status_change_cb(new_status);
            old_status = new_status;
        }
        sleep(1);
    }
}

OPERATE_RET tkl_wired_get_ip(OUT NW_IP_S *ip)
{
    int sock = 0;
    char ipaddr[50] = {0};

    struct sockaddr_in *sin;
    struct ifreq ifr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         PR_ERR("socket create failed");
         return OPRT_COM_ERROR;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, WIRED_IFNAME, sizeof(ifr.ifr_name) - 1);

    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0 ) {
         PR_ERR("ioctl failed");
         close(sock);
         return OPRT_COM_ERROR;
    }

    sin = (struct sockaddr_in *)&ifr.ifr_addr;
    strcpy(ip->ip, inet_ntoa(sin->sin_addr)); 
    close(sock);

    return OPRT_OK;
}

OPERATE_RET tkl_wired_get_status(TKL_WIRED_STAT_E *status)
{
    int sock;
    struct sockaddr_in *sin;
    struct ifreq ifr;
    NW_IP_S ip;

    *status = TKL_WIRED_LINK_DOWN;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         PR_ERR("socket failed");
         return OPRT_COM_ERROR;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, WIRED_IFNAME, sizeof(ifr.ifr_name) - 1);

    if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
        PR_ERR("ioctl failed");
        close(sock);
        return OPRT_COM_ERROR;
    }

    close(sock);

    if ((ifr.ifr_flags & IFF_UP) == 0) {
        return OPRT_OK;
    }
    
    if (tkl_wired_get_ip(&ip) != OPRT_OK) {
    	return OPRT_OK;
    }
    
    *status = TKL_WIRED_LINK_UP;
    
    return OPRT_OK;
}

OPERATE_RET tkl_wired_set_status_cb(TKL_WIRED_STATUS_CHANGE_CB cb)
{
    STATIC BOOL_T s_init = FALSE;
    pthread_t tid;

    if (s_init) {
        return OPRT_OK;
    }

    g_link_status_change_cb = cb;
    pthread_create(&tid, NULL, link_status_thread, NULL);
    s_init = TRUE;

    return OPRT_OK;
}

OPERATE_RET tkl_wired_get_mac(NW_MAC_S *mac)
{
    int i;
    int sock = -1;
    struct ifreq ifr;
    struct sockaddr *addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
         PR_ERR("socket failed");
         return OPRT_SOCK_ERR;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, WIRED_IFNAME, sizeof(ifr.ifr_name) - 1);
    addr = (struct sockaddr *)&ifr.ifr_hwaddr;
    addr->sa_family = 1;

    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
        PR_ERR("ioctl failed");
        close(sock);
        return OPRT_COM_ERROR;
    }

    memcpy(mac->mac, addr->sa_data, MAC_ADDR_LEN);

    close(sock);
    return OPRT_OK;
}

OPERATE_RET tkl_wired_set_mac(CONST NW_MAC_S *mac)
{
    return OPRT_OK;
}