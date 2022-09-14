#include "uni_log.h"
#include "tuya_zigbee_lc_mgr.h"

#include "user_zigbee_local.h"

STATIC VOID __z3_lc_dev_join_cb(CONST CHAR_T *dev_id, CONST CHAR_T *pid, CONST CHAR_T *ver)
{
    PR_DEBUG("tuya zigbee joined, dev_id: %s, pid: %s", dev_id, pid);
}

STATIC VOID __z3_lc_dev_leave_cb(CONST CHAR_T *dev_id)
{
    PR_DEBUG("tuya zigbee leave, dev_id: %s", dev_id);
}

STATIC VOID __z3_lc_dev_offline_cb(CONST CHAR_T *dev_id)
{
    /**
     * 涂鸦生态子设备支持防误删功能，本地重置只让设备强制离线，不移除，防误删子设备
     * 本地重置进入该回调，与离网回调是等效的。
     */
    PR_DEBUG("tuya zigbee offline, dev_id: %s", dev_id);
}

STATIC VOID __z3_lc_dev_hb_query_cb(CONST CHAR_T *dev_id, CONST CHAR_T *ver)
{
    PR_DEBUG("tuya zigbee hb query response, dev_id: %s", dev_id);
}

STATIC VOID __z3_lc_dev_upgrade_status_cb(CONST CHAR_T *dev_id, INT_T status)
{
    PR_DEBUG("tuya zigbee upgrade result, dev_id: %s, status: %d", dev_id, status);
}

STATIC VOID __z3_lc_dev_upgrade_progress_cb(CONST CHAR_T *dev_id, UINT_T total, UINT_T offset)
{
    PR_DEBUG("tuya zigbee upgrading progress, offset: %d", offset);
}

STATIC VOID __z3_lc_dev_obj_dp_report_cb(CONST CHAR_T *dev_id, CONST TY_OBJ_DP_S *dps, CONST UINT_T dp_cnt)
{
    INT_T i = 0;

    PR_DEBUG("tuya zigbee report obj dp");

    for (i = 0; i < dp_cnt; i++) {
        PR_DEBUG("dpid: %d", dps[i].dpid);
        switch (dps[i].type) {
        case PROP_BOOL:
            PR_DEBUG("value: %d", dps[i].value.dp_bool);
            break;
        case PROP_VALUE:
            PR_DEBUG("value: %d", dps[i].value.dp_value);
            break;
        case PROP_ENUM:
            PR_DEBUG("value: %d", dps[i].value.dp_enum);
            break;
        case PROP_STR:
            PR_DEBUG("value: %s", dps[i].value.dp_str);
            break;
        }
    }
}

STATIC VOID __z3_lc_dev_raw_dp_report_cb(CONST CHAR_T *dev_id, BYTE_T dpid, CONST BYTE_T *data, UINT_T len)
{
    PR_DEBUG("tuya zigbee report raw dp");
}

OPERATE_RET user_zigbee_local_init(VOID)
{
    TY_Z3_DEV_LC_MGR_S __z3_dev_lc_mgr = {
        .dev_join_cb             = __z3_lc_dev_join_cb,
        .dev_leave_cb            = __z3_lc_dev_leave_cb,
        .dev_offline_cb          = __z3_lc_dev_offline_cb,
        .dev_heartbeat_query_cb  = __z3_lc_dev_hb_query_cb,
        .dev_upgrade_status_cb   = __z3_lc_dev_upgrade_status_cb,
        .dev_upgrade_progress_cb = __z3_lc_dev_upgrade_progress_cb,
        .dev_obj_dp_rept_cb      = __z3_lc_dev_obj_dp_report_cb,
        .dev_raw_dp_rept_cb      = __z3_lc_dev_raw_dp_report_cb,
    };

    return tuya_zigbee_reg_lc_mgr(&__z3_dev_lc_mgr);
}