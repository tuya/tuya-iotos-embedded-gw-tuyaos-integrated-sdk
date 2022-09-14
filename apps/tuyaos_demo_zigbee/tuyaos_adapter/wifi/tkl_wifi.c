#include "tuya_iot_config.h"

#if defined(WIFI_GW) && (WIFI_GW==1)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include "tuya_os_adapter.h"
#include "tkl_wifi.h"
#include "tkl_system.h"
#include "uni_log.h"

#define WLAN_DEV              "wlan0"

#define UDHCPD_CONF           "/etc/udhcpd.conf"
#define HOSTAPD_CONF          "/etc/hostapd/hostapd.conf"
#define WPA_SUPPLICANT_CONF   "/etc/wpa_supplicant/wpa_supplicant.conf"

#define ENABLE_WIFI_DEMO      0

#if defined(ENABLE_WIFI_DEMO) && (ENABLE_WIFI_DEMO == 1)
STATIC NW_IP_S g_ap_ip = {0};
STATIC WF_WK_MD_E g_wifi_mode = WWM_STATION;
STATIC WIFI_EVENT_CB  __wifi_event_cb = NULL;

STATIC OPERATE_RET save_conf(CONST CHAR_T *filename, VOID *data, UINT32_T len)
{
    OPERATE_RET wbyte = 0;
    FILE *fp = NULL;

    fp = fopen(filename, "w");
    if (fp == NULL) {
        PR_ERR("open %s failed", filename);
        return OPRT_OPEN_FILE_FAILED;
    }

    wbyte = fwrite(data, 1, len, fp);
    if (wbyte != len) {
        PR_ERR("write error");
        fclose(fp);
        return OPRT_WRITE_FILE_FAILED;
    }

    fflush(fp);
    fsync(fileno(fp));
    fclose(fp);

    return OPRT_OK;
}

STATIC OPERATE_RET exec_command(CONST CHAR_T *cmd, CHAR_T *data, INT_T len)
{
    FILE *fp = NULL;

    fp = popen(cmd, "r");
    if (fp == NULL) {
        return OPRT_COM_ERROR;
    }

    if (data != NULL) {
        memset(data, 0, len);

        fread(data, len, 1, fp);
        len = strlen(data);
        if (data[len - 1] == '\n') {
            data[len - 1] = '\0';
        } 
    }

    pclose(fp);

    return OPRT_OK;
}

STATIC BOOL_T __wifi_status(VOID)
{
    FILE *fp = NULL;
    CHAR_T buf[512] = {0};
    WF_STATION_STAT_E stat = 0;

    fp = popen("wpa_cli -i " WLAN_DEV " status", "r");
    if (fp == NULL) {
        return FALSE;
    }

    while (fgets(buf, SIZEOF(buf), fp)) {
        if (!strstr(buf, "wpa_state"))
            continue;

        char *k = strtok(buf, "=");
        char *v = strtok(NULL, "=");
        if (v && !strncmp(v, "COMPLETED", strlen("COMPLETED"))) {
            tkl_wifi_station_get_status(&stat);
            if (stat == WSS_GOT_IP) {
                return TRUE;
            }
        }
    }

    pclose(fp);

    return FALSE;
}

STATIC VOID *__wifi_status_thread(VOID *arg)
{
    BOOL_T cur_status = FALSE, lst_status = FALSE;

    while (1) {
        if (g_wifi_mode != WWM_STATION) {
            tkl_system_sleep(500);
            continue;
        }

        cur_status = __wifi_status();

        if (cur_status != lst_status) {
            PR_DEBUG("wifi connection status changed, %d -> %d", lst_status, cur_status);
            if (__wifi_event_cb) {
                __wifi_event_cb(cur_status ? WFE_CONNECTED : WFE_DISCONNECTED, NULL);
            }
            lst_status = cur_status;
        }
        tkl_system_sleep(1000);
    }
}
#endif

OPERATE_RET tkl_wifi_init(WIFI_EVENT_CB cb)
{
#if defined(ENABLE_WIFI_DEMO) && (ENABLE_WIFI_DEMO == 1)
    pthread_t tid;

    __wifi_event_cb = cb;

    pthread_create(&tid, NULL, __wifi_status_thread, NULL);
#endif

    return OPRT_OK;
}

OPERATE_RET tkl_wifi_get_ip(CONST WF_IF_E wf, NW_IP_S *ip)
{
#if defined(ENABLE_WIFI_DEMO) && (ENABLE_WIFI_DEMO == 1)
    struct ifreq ifr;
    int sock = 0;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        PR_ERR("create socket failed");
        return OPRT_COM_ERROR;
    }

    strncpy(ifr.ifr_name, WLAN_DEV, strlen(WLAN_DEV) + 1);

    if (ioctl(sock, SIOCGIFADDR, &ifr) == 0)
        strncpy(ip->ip, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), sizeof(ip->ip));

    if (ioctl(sock, SIOCGIFBRDADDR, &ifr) == 0)
        strncpy(ip->gw, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_broadaddr)->sin_addr), sizeof(ip->gw));

    if (ioctl(sock, SIOCGIFNETMASK, &ifr) == 0)
        strncpy(ip->mask, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), sizeof(ip->mask));

    close(sock);

    PR_DEBUG("WiFi ip->ip: %s", ip->ip);

    return OPRT_OK;
#else
    return OPRT_COM_ERROR;
#endif
}

OPERATE_RET tkl_wifi_station_get_status(WF_STATION_STAT_E *stat)
{
#if defined(ENABLE_WIFI_DEMO) && (ENABLE_WIFI_DEMO == 1)
    OPERATE_RET op_ret = OPRT_OK;
    NW_IP_S ip = {0};

    *stat = WSS_IDLE;

    op_ret = tkl_wifi_get_ip(WF_STATION, &ip);
    if (op_ret != OPRT_OK) {
        return op_ret;
    }

    if ((strlen(ip.ip) > 0) && (strncmp(g_ap_ip.ip, ip.ip, strlen(ip.ip)) != 0)) {
        *stat = WSS_GOT_IP;
    }
#endif

    return OPRT_OK;
}

OPERATE_RET tkl_wifi_get_mac(CONST WF_IF_E wf, NW_MAC_S *mac)
{
#if defined(ENABLE_WIFI_DEMO) && (ENABLE_WIFI_DEMO == 1)
    int i;
    int fd = -1;
    struct ifreq ifr;
    struct sockaddr *addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
         PR_ERR("socket failed");
         return OPRT_SOCK_ERR;
    }

    memset(&ifr, 0, SIZEOF(ifr));
    strncpy(ifr.ifr_name, WLAN_DEV, SIZEOF(ifr.ifr_name) - 1);
    addr = (struct sockaddr *)&ifr.ifr_hwaddr;
    addr->sa_family = 1;

    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
        PR_ERR("ioctl failed");
        close(fd);
        return OPRT_COM_ERROR;
    }

    memcpy(mac->mac, addr->sa_data, MAC_ADDR_LEN);
    PR_DEBUG("WiFi mac->mac: %02X-%02X-%02X-%02X-%02X-%02X", mac->mac[0], mac->mac[1], mac->mac[2], \
                                                             mac->mac[3],mac->mac[4],mac->mac[5]);

    close(fd);
#endif

    return OPRT_OK;
}

OPERATE_RET tkl_wifi_set_work_mode(CONST WF_WK_MD_E mode)
{
#if defined(ENABLE_WIFI_DEMO) && (ENABLE_WIFI_DEMO == 1)
    PR_DEBUG("WiFi set mode: %d", mode);

    g_wifi_mode = mode;

    switch (mode) {
        case WWM_STATION:
            exec_command("iwconfig " WLAN_DEV " mode managed", NULL, 0);
            break;
        case WWM_SOFTAP:
            // exec_command("iwconfig " WLAN_DEV " mode master", NULL, 0);
            break;
        default:
            break;
    }
#endif

    return OPRT_OK;
}

OPERATE_RET tkl_wifi_get_work_mode(WF_WK_MD_E *mode)
{
#if defined(ENABLE_WIFI_DEMO) && (ENABLE_WIFI_DEMO == 1)
    *mode = g_wifi_mode;

    PR_DEBUG("WiFi got mode: %d", *mode);
#endif

    return OPRT_OK;
}

OPERATE_RET tkl_wifi_start_ap(CONST WF_AP_CFG_IF_S *cfg)
{
#if defined(ENABLE_WIFI_DEMO) && (ENABLE_WIFI_DEMO == 1)
    OPERATE_RET op_ret = OPRT_OK;
    INT_T len = 0;
    CHAR_T buf[512] = {0};
    CHAR_T cmd[128] = {0};
    CHAR_T *ap_conf_fmt = 
"interface=%s\n"
"ssid=%s\n"
"country_code=CN\n"
"channel=%d\n"
"beacon_int=100\n"
"max_num_sta=%d\n"
"auth_algs=3\n"
"wpa=%d\n"
"wpa_key_mgmt=WPA-PSK\n"
"wpa_pairwise=TKIP CCMP\n"
"rsn_pairwise=CCMP\n";

    CHAR_T *udhcpd_conf_fmt = 
"interface %s\n"
"start %s.100\n"
"end %s.200\n"
"opt subnet %s\n"
"opt lease 28800\n"
"opt router %s\n"
"opt dns %s\n"
"opt domain SmartLift\n";

    INT_T seg1 = 0, seg2 = 0, seg3 = 0, seg4 = 0;
    CHAR_T ip_prefix[12] = {0};

    tkl_wifi_station_disconnect();

    PR_DEBUG("start ap, ssid: %s, ip: %s", cfg->ssid, cfg->ip.ip);

    memcpy(&g_ap_ip, &(cfg->ip), SIZEOF(NW_IP_S));

    sscanf(cfg->ip.ip, "%d.%d.%d.%d", &seg1, &seg2, &seg3, &seg4);
    snprintf(ip_prefix, SIZEOF(ip_prefix), "%d.%d.%d", seg1, seg2, seg3);

    if (cfg->p_len > 0) {
        len = snprintf(buf, SIZEOF(buf), ap_conf_fmt, WLAN_DEV, cfg->ssid, cfg->chan, cfg->max_conn, 2);
        len += snprintf(buf + len, SIZEOF(buf) - len, "wpa_passphrase=%s\n", cfg->passwd);
    } else {
        len = snprintf(buf, SIZEOF(buf), ap_conf_fmt, WLAN_DEV, cfg->ssid, cfg->chan, cfg->max_conn, 0);
    }

    op_ret = save_conf(HOSTAPD_CONF, buf, len);
    if (op_ret != OPRT_OK) {
        PR_ERR("fail to write %s", HOSTAPD_CONF);
    }

    len = snprintf(buf, SIZEOF(buf), udhcpd_conf_fmt, WLAN_DEV, ip_prefix, ip_prefix, cfg->ip.mask, cfg->ip.gw, cfg->ip.gw);
    op_ret = save_conf(UDHCPD_CONF, buf, len);
    if (op_ret != OPRT_OK) {
        PR_ERR("fail to write %s", UDHCPD_CONF);
    }

    snprintf(cmd, SIZEOF(cmd), "ifconfig %s %s netmask %s", WLAN_DEV, cfg->ip.ip, cfg->ip.mask);
    exec_command(cmd, NULL, 0);
    exec_command("ifconfig " WLAN_DEV " up", NULL, 0);
    tkl_system_sleep(1000);
    exec_command("hostapd -B -P /run/hostapd.pid " HOSTAPD_CONF, NULL, 0);
    exec_command("killall udhcpd", NULL, 0);
    exec_command("udhcpd " UDHCPD_CONF, NULL, 0);
#endif

    return OPRT_OK;
}

OPERATE_RET tkl_wifi_stop_ap(VOID_T)
{
#if defined(ENABLE_WIFI_DEMO) && (ENABLE_WIFI_DEMO == 1)
    exec_command("killall udhcpd", NULL, 0);
    exec_command("killall hostapd", NULL, 0);
    exec_command("ifconfig " WLAN_DEV " down", NULL, 0);
#endif

    return OPRT_OK;
}

OPERATE_RET tkl_wifi_station_connect(CONST SCHAR_T *ssid, CONST SCHAR_T *passwd)
{
#if defined(ENABLE_WIFI_DEMO) && (ENABLE_WIFI_DEMO == 1)
    OPERATE_RET op_ret = OPRT_OK;
    INT_T len = 0;
    CHAR_T buf[512] = {0};
    CHAR_T *wpa_conf_fmt = 
"ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev\n"
"update_config=1\n"
"country=CN\n"
"\n"
"network={\n"
        "\tssid=\"%s\"\n"
        "\tpairwise=CCMP TKIP\n"
        "\tkey_mgmt=WPA-PSK\n"
        "\tgroup=CCMP TKIP\n"
        "\tpsk=\"%s\"\n"
"}\n";

    if (!ssid || !passwd) {
        PR_WARN("ssid or passwd is null");
        return OPRT_INVALID_PARM;
    }

    tkl_wifi_stop_ap();

    PR_DEBUG("ssid: %s, passwd: %s", ssid, passwd);

    len = snprintf(buf, SIZEOF(buf), wpa_conf_fmt, ssid, passwd);
    op_ret = save_conf(WPA_SUPPLICANT_CONF, buf, len);
    if (op_ret != OPRT_OK) {
        PR_ERR("fail to write %s", UDHCPD_CONF);
    }

    exec_command("wpa_supplicant -B -Dnl80211 -i" WLAN_DEV " -c" WPA_SUPPLICANT_CONF, NULL, 0);
    exec_command("udhcpc -i " WLAN_DEV " -s /etc/udhcpc/default.script -p /run/udhcpc_wlan0.pid -b", NULL, 0);
#endif

    return OPRT_OK;
}

OPERATE_RET tkl_wifi_station_disconnect(VOID_T)
{
#if defined(ENABLE_WIFI_DEMO) && (ENABLE_WIFI_DEMO == 1)
    exec_command("wpa_cli -i " WLAN_DEV " terminate", NULL, 0);
    exec_command("[ -f /run/udhcpc_wlan0.pid ] && (cat /run/udhcpc_wlan0.pid | xargs sudo kill)", NULL, 0);
    exec_command("ifconfig " WLAN_DEV " down", NULL, 0);
#endif

    return OPRT_OK;
}

OPERATE_RET tkl_wifi_set_mac(CONST WF_IF_E wf, CONST NW_MAC_S *mac)
{
    return OPRT_OK;
}

OPERATE_RET tkl_wifi_scan_ap(CONST SCHAR_T *ssid, AP_IF_S **aps, UINT_T *num)
{
    return OPRT_OK;
}

OPERATE_RET tkl_wifi_release_ap(AP_IF_S *ap)
{
    return OPRT_OK;
}

OPERATE_RET tkl_wifi_set_cur_channel(CONST UCHAR_T chan)
{
    return OPRT_OK;
}

OPERATE_RET tkl_wifi_get_cur_channel(UCHAR_T *chan)
{
    return OPRT_OK;
}

OPERATE_RET tkl_wifi_set_sniffer(CONST BOOL_T en, CONST SNIFFER_CALLBACK cb)
{
    return OPRT_OK;
}

OPERATE_RET tkl_wifi_station_get_conn_ap_rssi(SCHAR_T *rssi)
{
    return OPRT_OK;
}

OPERATE_RET tkl_wifi_get_bssid(UCHAR_T *mac)
{
    return OPRT_OK;
}

OPERATE_RET tkl_wifi_set_country_code(CONST COUNTRY_CODE_E ccode)
{
    return OPRT_OK;
}

OPERATE_RET tkl_wifi_send_mgnt(CONST UCHAR_T *buf, CONST UINT_T len)
{
    return OPRT_OK;
}

OPERATE_RET tkl_wifi_register_recv_mgnt_callback(CONST BOOL_T enable, CONST WIFI_REV_MGNT_CB recv_cb)
{
    return OPRT_OK;
}

OPERATE_RET tkl_wifi_set_lp_mode(CONST BOOL_T enable, CONST UCHAR_T dtim)
{
    return OPRT_OK;
}

OPERATE_RET tkl_wifi_get_connected_ap_info(FAST_WF_CONNECTED_AP_INFO_T **fast_ap_info)
{
    return OPRT_COM_ERROR;
}

OPERATE_RET tkl_wifi_station_fast_connect(CONST FAST_WF_CONNECTED_AP_INFO_T *fast_ap_info)
{
    return OPRT_COM_ERROR;
}

OPERATE_RET tkl_wifi_set_rf_calibrated(VOID_T)
{
    return OPRT_OK;
}

OPERATE_RET tkl_hostap_ioctl_inet(INT_T dev, INT_T vif_index, UINT_T cmd, ULONG_T arg)
{
    return OPRT_OK;
}
#endif