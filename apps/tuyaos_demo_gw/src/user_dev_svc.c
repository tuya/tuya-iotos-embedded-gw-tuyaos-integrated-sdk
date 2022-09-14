#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "uni_log.h"
#include "tuya_iot_com_api.h"
#include "tuya_cloud_com_defs.h"

#include "tuya_gw_subdev_api.h"
#include "tuya_gw_protocol_mgr_user.h"
#include "tuya_iot_sdk_api.h"
#include "tuya_iot_sdk_defs.h"

#include "user_dev_svc.h"

#define MY_FW_FILE  "/tmp/user_dev_fw.bin"
#define MY_DEV_TYPE GP_DEV_ATH_1

STATIC CHAR_T upg_dev[DEV_ID_LEN] = {0};

STATIC OPERATE_RET __dev_ota_data(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len, IN CONST UINT_T offset,
                                  IN CONST BYTE_T *data, IN CONST UINT_T len, OUT UINT_T *remain_len, IN PVOID_T pri_data)
{
    INT_T fd = 0;
    ssize_t w_len = 0;

    PR_DEBUG("OTA File Data, total len: %d, len: %d, offset: %d", total_len, len, offset);

    if (pri_data == NULL) {
        PR_ERR("pri_data is null");
        return OPRT_INVALID_PARM;
    }

    fd = *(INT_T *)pri_data;

    w_len = write(fd, data, len);
    if (w_len <= 0) {
        close(fd);
        *remain_len = 0;
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

STATIC OPERATE_RET __dev_ota_notify(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data)
{
    INT_T fd = 0;

    if (pri_data == NULL) {
        PR_ERR("pri_data is null");
        return OPRT_INVALID_PARM;
    }

    fd = *(INT_T *)pri_data;
    close(fd);

    if (download_result == OPRT_OK) {
        PR_DEBUG("OTA File Download Successfully");

        // upgrade progress
        tuya_iot_dev_upgd_progress_rept(98, upg_dev, MY_DEV_TYPE);
    } else {
        PR_WARN("OTA File Download Failure");
    }

    return OPRT_OK;
}

STATIC BOOL_T __dev_add_cb(CONST GW_PERMIT_DEV_TP_T tp, CONST BOOL_T permit, CONST UINT_T timeout)
{
    OPERATE_RET op_ret = OPRT_OK;
    UINT_T uddd = 0x00000001;
    CHAR_T *dev_id = "000d6ffffe67e2ca";
    CHAR_T *pid = "d1xabwgg";
    CHAR_T *ver = "1.0.0";

    PR_DEBUG("dev add callback, tp: %d, permit: %d, timeout: %d", tp, permit, timeout);

    DEV_DESC_IF_S *dev_if = NULL;

    if (permit) {
        dev_if = tuya_iot_get_dev_if(dev_id);
        if (dev_if != NULL) {
            return TRUE;
        }

        PR_DEBUG("test bind device, dev_id: %s", dev_id);

        op_ret = tuya_iot_gw_bind_dev(MY_DEV_TYPE, uddd, dev_id, pid, ver);
        if (op_ret != OPRT_OK) {
            PR_WARN("tuya_iot_gw_bind_dev err: %d", op_ret);
            return TRUE;
        }
    }

    return TRUE;
}

STATIC VOID __dev_del_cb(CONST CHAR_T *dev_id, CONST GW_DELDEV_TYPE type)
{
    if (dev_id == NULL) {
        PR_ERR("invalid param");
        return;
    }

    PR_DEBUG("dev del callback, dev_id: %s, type: %d", dev_id, type);
}

STATIC VOID __dev_reset_cb(CONST CHAR_T *dev_id, DEV_RESET_TYPE_E type)
{
    if (dev_id == NULL) {
        PR_ERR("invalid param");
        return;
    }

    PR_DEBUG("dev reset callback, dev_id: %s, type: %d", dev_id, type);
}

STATIC VOID __dev_bind_cb(CONST CHAR_T *dev_id, CONST OPERATE_RET result)
{
    OPERATE_RET op_ret = OPRT_OK;

    if (dev_id == NULL) {
        PR_ERR("invalid param");
        return;
    }

    PR_DEBUG("dev bind callback, dev_id: %s, result: %d", dev_id, result);

    op_ret = tuya_iot_set_dev_hb_cfg(dev_id, 120, 3, FALSE);
    if (op_ret != OPRT_OK) {
        PR_ERR("tuya_iot_set_dev_hb_cfg err: %d", op_ret);
        return;
    }

    op_ret = tuya_iot_fresh_dev_hb(dev_id);
    if (op_ret != OPRT_OK) {
        PR_ERR("tuya_iot_fresh_dev_hb err: %d", op_ret);
        return;
    }
}

STATIC VOID __dev_cmd_obj_cb(CONST TY_RECV_OBJ_DP_S *cmd)
{
    INT_T i = 0;
    OPERATE_RET op_ret = OPRT_OK;

    PR_DEBUG("dev cmd obj callback");
    PR_DEBUG("cmd_tp: %d, dtt_tp: %d, dps_cnt: %u", cmd->cmd_tp, cmd->dtt_tp, cmd->dps_cnt);

    for (i = 0; i < cmd->dps_cnt; i++) {
        PR_DEBUG("dpid: %d", cmd->dps[i].dpid);
        switch (cmd->dps[i].type) {
        case PROP_BOOL:
            PR_DEBUG("dp_bool value: %d", cmd->dps[i].value.dp_bool);
            break;
        case PROP_VALUE:
            PR_DEBUG("dp_value value: %d", cmd->dps[i].value.dp_value);
            break;
        case PROP_ENUM:
            PR_DEBUG("dp_enum value: %d", cmd->dps[i].value.dp_enum);
            break;
        case PROP_STR:
            PR_DEBUG("dp_str value: %s", cmd->dps[i].value.dp_str);
            break;
        }
    }

    op_ret = dev_report_dp_json_async(cmd->cid, cmd->dps, cmd->dps_cnt);
    if (op_ret != OPRT_OK) {
        PR_ERR("dev_report_dp_json_async err: %d", op_ret);
        return;
    }
}

STATIC VOID __dev_cmd_raw_cb(CONST TY_RECV_RAW_DP_S *dp)
{
    INT_T i = 0;

    PR_DEBUG("dev cmd raw callback");
    PR_DEBUG("cmd_tp: %d, dtt_tp: %d, dpid: %d, len: %u", dp->cmd_tp, dp->dtt_tp, dp->dpid, dp->len);

    PR_DEBUG("raw data: ");
    for (i = 0; i < dp->len; i++) {
        printf("%02x ", dp->data[i]);
    }

    printf("\n");
}

STATIC VOID __dev_hb_cb(CONST CHAR_T *dev_id)
{
    OPERATE_RET op_ret = OPRT_OK;

    if (dev_id == NULL) {
        PR_ERR("invalid param");
        return;
    }

    op_ret = tuya_iot_fresh_dev_hb(dev_id);
    if (op_ret != OPRT_OK) {
        PR_ERR("tuya_iot_fresh_dev_hb err: %d", op_ret);
        return;
    }
}

STATIC OPERATE_RET __dev_upgrade_cb(CONST CHAR_T *dev_id, CONST FW_UG_S *fw)
{
    OPERATE_RET op_ret = OPRT_OK;
    STATIC INT_T fd = 0;

    if ((dev_id == NULL) || (fw == NULL)) {
        PR_ERR("invalid param");
        return OPRT_INVALID_PARM;
    }

    PR_DEBUG("dev upgrade callback, dev_id: %s", dev_id);
    PR_DEBUG("fw->tp       : %d", fw->tp);
    PR_DEBUG("fw->fw_url   : %s", fw->fw_url);
    PR_DEBUG("fw->sw_ver   : %s", fw->sw_ver);
    PR_DEBUG("fw->file_size: %u", fw->file_size);

    remove(MY_FW_FILE);

    memset(upg_dev, 0, DEV_ID_LEN);
    memcpy(upg_dev, dev_id, strlen(dev_id) + 1);

    fd = open(MY_FW_FILE, O_CREAT | O_TRUNC | O_WRONLY, 0755);
    if (fd < 0) {
        PR_ERR("open error");
        return OPRT_COM_ERROR;
    }

    op_ret = tuya_iot_upgrade_dev(dev_id, fw, __dev_ota_data, __dev_ota_notify, (VOID *)&fd);
    if (op_ret != OPRT_OK) {
        PR_ERR("tuya_iot_upgrade_dev err: %d", op_ret);
    }

    return OPRT_OK;
}

OPERATE_RET user_dev_svc_init(VOID)
{
    OPERATE_RET rt = OPRT_OK;
    DEV_DESC_IF_S *dev_if = NULL;
    VOID *iterator = NULL;

    TY_GW_SUBDEV_MGR_CBS_S dev_mgr_cbs = {
        .dev_add       = __dev_add_cb,
        .dev_del       = __dev_del_cb,
        .dev_bind      = __dev_bind_cb,
        .dev_hb        = __dev_hb_cb,
        .dev_upgrade   = __dev_upgrade_cb,
        .dev_reset     = __dev_reset_cb,
        .dp_cmd_obj    = __dev_cmd_obj_cb,
        .dp_cmd_raw    = __dev_cmd_raw_cb,
    };

    TUYA_CALL_ERR_RETURN(tuya_subdev_user_sigle_type_reg(&dev_mgr_cbs, MY_DEV_TYPE));

    /* 遍历子设备，配置心跳 */
    do {
        dev_if = tuya_iot_dev_traversal(&iterator);
        if (dev_if) {
            if (dev_if->tp != MY_DEV_TYPE) {
                continue;
            }

            tuya_iot_set_dev_hb_cfg(dev_if->id, 120, 3, FALSE);
        }
    } while(dev_if);

    return OPRT_OK;
}