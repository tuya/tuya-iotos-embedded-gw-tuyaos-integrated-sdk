#include "tuya_bt_api.h"
#include "tuya_app_config.h"
#include "ty_cJSON.h"
#include "uni_log.h"

#include "user_blemesh_svc.h"

STATIC VOID __bt_ncp_hard_reset_cb(VOID)
{
    PR_DEBUG("ncp is abnormal, hard reset by GIOP");
}

/**
 * {
 *     "tuya": {
 *         "bt": {
 *             "dev_name": "/dev/ttyS6",
 *             "enable_hb": 1,
 *             "scan_timeout": 60,
 *             "mode": 258
 *         }
 *     }
 * }
 */
STATIC ty_cJSON *__bt_get_cfg(VOID)
{
    TUYA_PLATEFORM_S *cfg = tuya_get_config();
    ty_cJSON *root = NULL;

    root = ty_cJSON_GetObjectItem(cfg->input, "tuya");
    TUYA_CHECK_NULL_RETURN(root, NULL);

    return ty_cJSON_GetObjectItem(root, "bt");
}

OPERATE_RET user_blemesh_svc_init(VOID)
{
    tuya_bt_ncp_hard_reset_reg(__bt_ncp_hard_reset_cb);

    return tuya_bt_svc_init(__bt_get_cfg());
}

OPERATE_RET user_blemesh_svc_start(VOID)
{
    return tuya_bt_svc_start(__bt_get_cfg());
}