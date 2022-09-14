#include "uni_log.h"

#include "tuya_hal_system.h"
#include "base_os_adapter.h"
#include "tuya_cloud_base_defs.h"
#if defined(WIFI_GW) && (WIFI_GW==1)
#include "tuya_iot_wifi_api.h"
#endif

#include "tuya_iot_base_api.h"
#include "tuya_iot_com_api.h"
#include "tuya_iot_sdk_api.h"
#include "tuya_iot_sdk_defs.h"
#include "tuya_gw_dp_mgr.h"
#include "tuya_app_config.h"

#include "user_blemesh_svc.h"

STATIC VOID __gw_reset_cb(GW_RESET_TYPE_E type)
{
    PR_DEBUG("reset callback, type: %d", type);

    exit(0);
}

STATIC VOID __gw_reboot_cb(VOID)
{
    PR_DEBUG("reboot callback");

    exit(0);
}

STATIC VOID __gw_upgrade_cb(CONST FW_UG_S *fw)
{
    PR_DEBUG("upgrade callback");
    PR_DEBUG("        tp: %d", fw->tp);
    PR_DEBUG("    fw_url: %s", fw->fw_url);
    PR_DEBUG("    sw_ver: %s", fw->sw_ver);
    PR_DEBUG("   fw_hmac: %s", fw->fw_hmac);
    PR_DEBUG(" file_size: %u", fw->file_size);

    return;
}

STATIC VOID __gw_active_stat_cb(GW_STATUS_E stat)
{
    PR_DEBUG("active stat callback, stat: %d", stat);

    return;
}

STATIC VOID __gw_local_log_cb(OUT CHAR_T *path, IN CONST INT_T len)
{
    PR_DEBUG("local log callback");
}

STATIC VOID __gw_net_stat_cb(IN CONST SDK_NW_STAT_T stat)
{
    PR_DEBUG("net stat: %s", (stat == SDK_NWK_OFFLINE ? "offline" : "online"));
}

STATIC VOID __gw_wired_stat_cb(IN CONST SDK_WIRED_NW_STAT_T stat)
{
    PR_DEBUG("wired stat: %d", stat);
}

STATIC VOID __gw_wifi_stat_cb(IN CONST SDK_WIFI_NW_STAT_T stat)
{
    PR_DEBUG("wifi stat: %d", stat);
}

int main(int argc, char **argv)
{
    OPERATE_RET rt = OPRT_OK;
    CHAR_T *cfg_str = NULL;
    ty_cJSON *cfg_json = NULL;
    CHAR_T *storage_path = NULL;
    CHAR_T *pid = NULL;
    TUYA_PLATEFORM_S *cfg = NULL;

#if defined(WIFI_GW) && (WIFI_GW==1)
    WF_GW_PROD_INFO_S prod_info = {0};
#else
    GW_PROD_INFO_S prod_info = {0};
#endif

    TY_GW_APP_CBS_S gw_app_cbs = {
        .gw_reset_cb       = __gw_reset_cb,
        .gw_reboot_cb      = __gw_reboot_cb,
        .gw_upgrade_cb     = __gw_upgrade_cb,
        .gw_active_stat_cb = __gw_active_stat_cb,
        .gw_log_path_cb    = __gw_local_log_cb,
    };

    TUYA_CALL_ERR_RETURN(tuya_load_config("./config.json"));

    cfg = tuya_get_config();
    PR_DEBUG("        pid: %s", cfg->product_id);
    PR_DEBUG("  cache_dir: %s", cfg->tmp_dir);
    PR_DEBUG("storage_dir: %s", cfg->storage_dir);

    /* Core: IoT 基础服务初始化 */
    TUYA_CALL_ERR_RETURN(tuya_iot_init(cfg->storage_dir));

    ty_cJSON *log_level = ty_cJSON_GetObjectItem(cfg->input, "log_level");
    if (log_level && log_level->type == ty_cJSON_Number) {
        PR_DEBUG("log level: %d", log_level->valueint);
        SetLogManageAttr(log_level->valueint);
    }

    /* Core: 读取&设置授权信息 */
    ty_cJSON *uuid = ty_cJSON_GetObjectItem(cfg->input, "uuid");
    TUYA_CHECK_NULL_RETURN(uuid, OPRT_COM_ERROR);
    ty_cJSON *authkey = ty_cJSON_GetObjectItem(cfg->input, "authkey");
    TUYA_CHECK_NULL_RETURN(authkey, OPRT_COM_ERROR);
    prod_info.uuid = uuid->valuestring;
    prod_info.auth_key = authkey->valuestring;
#if defined(WIFI_GW) && (WIFI_GW==1)
    TUYA_CALL_ERR_RETURN(tuya_iot_set_wf_gw_prod_info(&prod_info));
#else
    TUYA_CALL_ERR_RETURN(tuya_iot_set_gw_prod_info(&prod_info));
#endif

    tuya_iot_reg_gw_app_cb(&gw_app_cbs);
    tuya_iot_sdk_reg_netstat_cb(__gw_net_stat_cb, __gw_wired_stat_cb, __gw_wifi_stat_cb);

    /* Core: SDK 预初始化 */
    tuya_iot_sdk_pre_init(TRUE);

    /* 私有业务初始化 */
    TUYA_CALL_ERR_RETURN(user_blemesh_svc_init());

    /* Core: SDK 初始化 */
#if defined(WIFI_GW) && (WIFI_GW==1)
#if defined(GW_SUPPORT_WIRED_WIFI) && (GW_SUPPORT_WIRED_WIFI==1)
    TUYA_CALL_ERR_RETURN(tuya_iot_wr_wf_sdk_init(IOT_GW_NET_WIRED_WIFI, GWCM_OLD, WF_START_AP_ONLY, cfg->product_id, USER_SW_VER, NULL, 0));
#else
    TUYA_CALL_ERR_RETURN(tuya_iot_wf_sdk_init(GWCM_OLD, WF_START_AP_ONLY, cfg->product_id, USER_SW_VER, NULL, 0));
#endif
#else
    TUYA_CALL_ERR_RETURN(tuya_iot_sdk_init(cfg->product_id, USER_SW_VER, NULL, 0));
#endif

    /* Core: SDK 启动 */
    TUYA_CALL_ERR_RETURN(tuya_iot_sdk_start());

    /* 私有业务启动 */
    TUYA_CALL_ERR_RETURN(user_blemesh_svc_start());

    while (1) {
        tuya_hal_system_sleep(10*1000);
    }

    return 0;
}