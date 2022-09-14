#include "tuya_zigbee_api.h"
#include "tuya_app_config.h"
#include "ty_cJSON.h"
#include "uni_log.h"

#include "user_zigbee_svc.h"

/**
 * {
 *     "tuya": {
 *         "zigbee": {
 *             "dev_name": "/dev/ttyS6",
 *             "cts": 0,
 *             "thread_mode": 1
 *         }
 *     }
 * }
 */
STATIC ty_cJSON *__zig_get_cfg(VOID)
{
    TUYA_PLATEFORM_S *cfg = tuya_get_config();
    ty_cJSON *root = NULL;

    root = ty_cJSON_GetObjectItem(cfg->input, "tuya");
    TUYA_CHECK_NULL_RETURN(root, NULL);

    return ty_cJSON_GetObjectItem(root, "zigbee");
}

STATIC VOID __zig_permit_join_cb(BOOL_T permit, UINT_T timeout)
{
    PR_DEBUG("permit: %d, timeout: %d", permit, timeout);
}

STATIC VOID __zig_ncp_hard_reset_cb(VOID)
{
    PR_DEBUG("ncp is abnormal, hard reset by GIOP");
}

OPERATE_RET user_zigbee_svc_init(VOID)
{
    OPERATE_RET rt = OPRT_OK;
    TY_Z3_NCP_OPS_S zigbee_ncp_ops = {
        .coo_hard_reset_cb = __zig_ncp_hard_reset_cb,
    };

    tuya_zigbee_reg_permit_join_cb(__zig_permit_join_cb);
    tuya_zigbee_reg_ncp_hard_ops_cb(&zigbee_ncp_ops);

    TUYA_CALL_ERR_LOG(tuya_zigbee_svc_init(__zig_get_cfg()));

    return OPRT_OK;
}

OPERATE_RET user_zigbee_svc_start(VOID)
{
    OPERATE_RET rt = OPRT_OK;

    TUYA_CALL_ERR_LOG(tuya_zigbee_svc_start(__zig_get_cfg()));

    return OPRT_OK;
}