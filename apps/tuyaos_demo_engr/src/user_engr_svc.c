#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "uni_log.h"
#include "tuya_iot_config.h"

#include "user_engr_svc.h"

#include "tuya_engr_sdk_api.h"
#include "tuya_engr_zig_api.h"
#include "tuya_gw_engr_base_api.h"

#define CLOUD_CFG_FILE          "cloud.config"
#define APP_CFG_FILE            "app.config"
#define SUB_DEV_FILE            "sub_dev.config"

STATIC CHAR_T g_path_engr[256] = {0};
STATIC CHAR_T g_path_norm[128] = {0};

/* 施工模式获取网关日志回调 */
STATIC OPERATE_RET __engr_get_log_cb(OUT CHAR_T *path, IN CONST INT_T len)
{
    /**
     * TODO: 打包日志文件，把文件路径字符串赋值给 path，字符串长度不能超过 len
     */
    return OPRT_OK;
}

/* 施工模式的数据同步到普通模式回调 */
STATIC OPERATE_RET __engr_sync_config_cb(VOID)
{
    CHAR_T cmd[512] = {0};

    if ((strlen(g_path_engr) == 0) || (strlen(g_path_norm) == 0)) {
        PR_ERR("g_path_engr or g_path_norm is empty");
        return OPRT_COM_ERROR;
    }

    snprintf(cmd, SIZEOF(cmd), "cp -rf %s/* %s", g_path_engr, g_path_norm);
    PR_DEBUG("cmd: %s", cmd);
    system(cmd);

    return OPRT_OK;
}

/* 施工模式重置网关回调 */
STATIC OPERATE_RET __engr_reset_cb(VOID)
{
    CHAR_T cmd[1024] = {0};

    if (strlen(g_path_engr) == 0) {
        PR_ERR("g_path_engr is empty");
        return OPRT_COM_ERROR;
    }

    snprintf(cmd, SIZEOF(cmd), "rm -f %s/%s %s/%s %s/%s", \
        g_path_engr, CLOUD_CFG_FILE,\
        g_path_engr, APP_CFG_FILE, \
        g_path_engr, SUB_DEV_FILE);
    PR_DEBUG("cmd: %s", cmd);
    system(cmd);

    return OPRT_OK;
}

/* 施工完成回调 */
STATIC OPERATE_RET __engr_finish_cb(VOID)
{
    PR_DEBUG("engineering mode has been finished, switch to normal mode");

    return OPRT_OK;
}

STATIC OPERATE_RET __engr_gw_process_cb(GW_ENGR_PROCESS_T status)
{
    PR_INFO("__engr_gw_process_cb status: %d", status);
    return OPRT_OK;
}

CHAR_T *tuya_engineer_get_storage_path(VOID)
{
    return g_path_engr;
}

OPERATE_RET user_engr_set_storage_path(CONST CHAR_T *path)
{
    struct stat st = {0};

    if (path == NULL) {
        return OPRT_INVALID_PARM;
    }

    memset(g_path_engr, 0, SIZEOF(g_path_engr));
    memset(g_path_norm, 0, SIZEOF(g_path_norm));

    snprintf(g_path_norm, SIZEOF(g_path_norm), "%s", path);
    snprintf(g_path_engr, SIZEOF(g_path_engr), "%s/engineer/", g_path_norm);

    if (stat(g_path_engr, &st) != 0) {
        if (0 != mkdir(g_path_engr, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) {
            PR_ERR("mkdir error, path: %s", g_path_engr);
            return OPRT_COM_ERROR;
        }
    }

    return OPRT_OK;
}

CHAR_T *user_engr_get_storage_path(VOID)
{
    return g_path_engr;
}

OPERATE_RET user_engr_svc_init(IN CONST CHAR_T *pid)
{
    TY_ENGR_GW_INFRA_CBS_S engr_gw_cbs = {
        .engineer_get_log       = __engr_get_log_cb,
        .engineer_sync_config   = __engr_sync_config_cb,
        .engineer_finish        = __engr_finish_cb,
        .engineer_reset         = __engr_reset_cb,
    };

    if (pid == NULL) {
        return OPRT_INVALID_PARM;
    }

    /* 注册施工模式状态变更通知回调 */
    tuya_iot_engineer_process_register(__engr_gw_process_cb);

    /**
     * 施工模式初始化，两种情景：
     *   1) 使用涂鸦网关 Zigbee 模组，在 `tuya_engineer_sdk_init` 接口之前调用
     *      `tuya_engineer_set_tuya_zigbee_enable` 接口。
     *   2) 使用第三方 Zigbee 模组，应用不调用 `tuya_engineer_set_tuya_zigbee_enable` 接口，实现
     *      `TY_ENGR_GW_ZIG_CBS_S` 相关的回调，作为 `tuya_engineer_sdk_init` 接口的第三个实参。
     * 
     * 说明，本示例代码实现的是第 1 种场景，注意 `tuya_engineer_sdk_init` 接口第一个参数为普通模式 PID。
     */
    tuya_engineer_set_tuya_zigbee_enable();

    tuya_engineer_sdk_init(pid, &engr_gw_cbs, NULL);

    return OPRT_OK;
}