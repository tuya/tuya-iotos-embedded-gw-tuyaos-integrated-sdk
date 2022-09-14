#include <stdio.h>

#include "uni_log.h"
#include "tal_workq_service.h"
#include "tuya_zigbee_api.h"
#include "tuya_zigbee_stack.h"
#include "tuya_iot_com_api.h"
#include "tuya_gw_subdev_api.h"
#include "tuya_gw_protocol_mgr_user.h"

#include "user_zigbee_custom.h"

#define CMD_READ_ATTRIBUTER_RESPONSE_HEADER     3 /* Attributer ID: 2 Bytes, Status: 1 Btye */
#define CMD_REPORT_ATTRIBUTES_HEADER            2 /* Attributer ID: 2 Bytes */

TY_Z3_DEV_S __custom_devices[] = {
    { "example-manuname", "example-modelid" },
};

TY_Z3_DEVLIST_S __custom_devlist = {
    .devs = __custom_devices,
    .dev_num = CNTSOF(__custom_devices),
};

STATIC OPERATE_RET __z3_onoff_light_get_status(CONST CHAR_T *dev_id)
{
    OPERATE_RET op_ret = OPRT_OK;
    TY_Z3_APS_FRAME_S frame = {0};
    USHORT_T attribute_buf[] = {0x0000, 0x4001, 0x4002, 0x8001, 0x8002, 0x5000};

    strncpy(frame.id, dev_id, SIZEOF(frame.id));
    frame.profile_id = ZHA_PROFILE_ID;
    frame.cluster_id = CLUSTER_ON_OFF_CLUSTER_ID;
    frame.cmd_type = ZG_ZCL_FRAME_TYPE_GLOBAL;
    frame.src_endpoint = 0x01;
    frame.dst_endpoint = 0xFF;
    frame.cmd_id = CMD_READ_ATTRIBUTES_COMMAND_ID;

    frame.msg_length = SIZEOF(attribute_buf);
    frame.message = (UCHAR_T *)attribute_buf;

    op_ret = tuya_zigbee_send_data(&frame);
    if (op_ret != OPRT_OK) {
        PR_ERR("tuya_zigbee_send_data err: %d", op_ret);
        return op_ret;
    }

    return OPRT_OK;
}

STATIC OPERATE_RET __z3_dev_get_status(CONST CHAR_T *dev_id)
{
    OPERATE_RET op_ret = OPRT_OK;
    DEV_DESC_IF_S *dev_if = NULL;

    dev_if = tuya_iot_get_dev_if(dev_id);
    if (dev_if == NULL) {
        PR_WARN("%s is not bind", dev_id);
        return OPRT_COM_ERROR;
    }

    USHORT_T device_id = (dev_if->uddd & 0xFFFF);
    switch (device_id) {
    case ZG_DEVICE_ID_ON_OFF_LIGHT:
        op_ret = __z3_onoff_light_get_status(dev_id);
        break;
    default:
        PR_WARN("device_id: %04x is not supported");
        return OPRT_NOT_SUPPORTED;
    }

    return op_ret;
}

STATIC OPERATE_RET __z3_onoff_light_global_report_data_process(TY_Z3_APS_FRAME_S *frame, UINT_T uddd)
{
    TY_OBJ_DP_S obj_dp = {0};
    
    if (frame->cluster_id == CLUSTER_ON_OFF_CLUSTER_ID) {
        obj_dp.dpid = frame->src_endpoint;
        obj_dp.type = PROP_BOOL;
        if (frame->cmd_id == CMD_READ_ATTRIBUTES_RESPONSE_COMMAND_ID) {
            obj_dp.value.dp_bool = frame->message[CMD_READ_ATTRIBUTER_RESPONSE_HEADER+1];
        } else if (frame->cmd_id == CMD_REPORT_ATTRIBUTES_COMMAND_ID) {
            obj_dp.value.dp_bool = frame->message[CMD_REPORT_ATTRIBUTES_HEADER+1];
        } else {
            return OPRT_COM_ERROR;
        }
        
        dev_report_dp_json_async(frame->id, &obj_dp, 1);
    }

    return OPRT_OK;
}

STATIC OPERATE_RET __z3_onoff_light_report_data_process(TY_Z3_APS_FRAME_S *frame, UINT_T uddd)
{
    switch (frame->cmd_type) {
    case ZG_ZCL_FRAME_TYPE_GLOBAL:
        __z3_onoff_light_global_report_data_process(frame, uddd);
        break;
    case ZG_ZCL_FRAME_TYPE_SPEC_TO_CLUSTER:
        PR_WARN("command is specific to a cluster is not supported");
        return OPRT_COM_ERROR;
    default:
        PR_ERR("cmd_type: %02x is invalid", frame->cmd_type);
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

STATIC OPERATE_RET __z3_onoff_light_down_data_process(CONST TY_RECV_OBJ_DP_S *cmd)
{
    INT_T i = 0;
    OPERATE_RET op_ret = OPRT_OK;

    for (i = 0; i < cmd->dps_cnt; i++) {
        BYTE_T dpid = cmd->dps[i].dpid;
        TY_Z3_APS_FRAME_S frame = {0};
        switch (dpid)
        {
        case 1 ... 6:
            if (cmd->dps[i].type != PROP_BOOL) {
                PR_WARN("dp type: %d invalid", cmd->dps[i].type);
                break;
            }
            strncpy(frame.id, cmd->cid, SIZEOF(frame.id));
            frame.profile_id = ZHA_PROFILE_ID;
            frame.cluster_id = CLUSTER_ON_OFF_CLUSTER_ID;
            frame.cmd_type = ZG_ZCL_FRAME_TYPE_SPEC_TO_CLUSTER;
            frame.frame_type = 0;
            frame.src_endpoint = 0x01;
            frame.dst_endpoint = dpid;
            frame.cmd_id = cmd->dps[i].value.dp_bool;
            frame.msg_length = 0;
            op_ret = tuya_zigbee_send_data(&frame);
            if (op_ret != OPRT_OK) {
                PR_ERR("tuya_zigbee_send_data err: %d", op_ret);
            }
            break;

        default:
            PR_WARN("dpid: %d is not supported", cmd->dps[i].dpid);
            break;
        }
    }

    return OPRT_OK;
}

STATIC VOID __z3_dev_join(TY_Z3_DESC_S *dev)
{
    OPERATE_RET rt = OPRT_OK;
    CHAR_T *pid = NULL;
    UINT_T uddd = 0;
    UCHAR_T sw_ver[SW_VER_LEN+1] = {0};

    PR_DEBUG("device join callback, dev_id: %s", dev->id);
    PR_DEBUG("     node_id: 0x%04x", dev->node_id);
    PR_DEBUG("   manu_name: %s", dev->manu_name);
    PR_DEBUG("    model_id: %s", dev->model_id);

    if (!strncasecmp(dev->model_id, "TS0001", strlen("TS0001"))) {
        pid = "nPGIPl5D";
        uddd |= ZG_DEVICE_ID_ON_OFF_LIGHT;
    } else {
        PR_ERR("model_id[%s] is not supported", dev->model_id);
        return;
    }

    snprintf(sw_ver, SIZEOF(sw_ver), "%d.%d.%d", ((dev->version >> 6) & 0x03), \
                                                 ((dev->version >> 4) & 0x03), \
                                                 (dev->version & 0x0F));
    
    rt = tuya_iot_gw_bind_dev(GP_DEV_ATH_1, uddd, dev->id, pid, sw_ver);
    if (rt != OPRT_OK) {
        PR_ERR("tuya_iot_gw_bind_dev err: %d", rt);
        return;
    }
}

STATIC VOID __z3_dev_leave(CONST CHAR_T *dev_id)
{
    OPERATE_RET rt = OPRT_OK;

    PR_DEBUG("device leave callback, dev_id: %s", dev_id);

    rt = tuya_iot_gw_unbind_dev(dev_id);
    if (rt != OPRT_OK) {
        PR_ERR("tuya_iot_gw_unbind_dev err: %d", rt);
        return;
    }
}

STATIC VOID __z3_dev_report(TY_Z3_APS_FRAME_S *frame)
{
    INT_T i = 0;
    DEV_DESC_IF_S *dev_if = NULL;
    OPERATE_RET rt = OPRT_OK;

    if (frame == NULL) {
        PR_ERR("invalid param");
        return;
    }

    tuya_iot_fresh_dev_hb(frame->id);

    if (frame->cluster_id == CLUSTER_BASIC_CLUSTER_ID) {
        return;
    }

    PR_DEBUG("device zcl report callback, dev_id: %s", frame->id);
    PR_DEBUG("profile_id: 0x%04x", frame->profile_id);
    PR_DEBUG("cluster_id: 0x%04x", frame->cluster_id);
    PR_DEBUG("   node_id: 0x%04x", frame->node_id);
    PR_DEBUG("    src_ep: %d", frame->src_endpoint);
    PR_DEBUG("    dst_ep: %d", frame->dst_endpoint);
    PR_DEBUG("  group_id: %d", frame->group_id);
    PR_DEBUG("  cmd_type: %d", frame->cmd_type);
    PR_DEBUG("   command: 0x%02x", frame->cmd_id);
    PR_DEBUG("frame_type: %d", frame->frame_type);
    PR_DEBUG("   msg_len: %d", frame->msg_length);
    PR_DEBUG("       msg: ");
    for (i = 0; i < frame->msg_length; i++)
        printf("%02x ", frame->message[i]);

    printf("\n");

    dev_if = tuya_iot_get_dev_if(frame->id);
    if (dev_if == NULL) {
        PR_WARN("%s is not bind", frame->id);
        return;
    }

    USHORT_T device_id = (dev_if->uddd & 0xFFFF);
    switch (device_id) {
    case ZG_DEVICE_ID_ON_OFF_LIGHT:
        __z3_onoff_light_report_data_process(frame, dev_if->uddd);
        break;
    default:
        PR_WARN("device_id: %04x is not supported");
        return;
    }
}

STATIC VOID __gw_dev_del_cb(CONST CHAR_T *dev_id, CONST GW_DELDEV_TYPE type)
{
    OPERATE_RET op_ret = OPRT_OK;

    PR_DEBUG("gw dev del callback, dev_id: %s, type: %d", dev_id, type);

    op_ret = tuya_zigbee_del_dev(dev_id);
    if (op_ret != OPRT_OK) {
        PR_ERR("tuya_zigbee_del_dev err: %d", op_ret);
        return;
    }
}

STATIC VOID __gw_dev_reset_cb(CONST CHAR_T *dev_id, DEV_RESET_TYPE_E type)
{
    OPERATE_RET op_ret = OPRT_OK;

    PR_DEBUG("gw dev reset callback, dev_id: %s, type: %d", dev_id, type);

    if (type != DEV_REMOTE_RESET_FACTORY) {
        return;
    }

    op_ret = tuya_zigbee_del_dev(dev_id);
    if (op_ret != OPRT_OK) {
        PR_ERR("tuya_zigbee_del_dev err: %d", op_ret);
        return;
    }
}

STATIC VOID __gw_dev_bind_ifm_cb(CONST CHAR_T *dev_id, CONST OPERATE_RET result)
{
    OPERATE_RET op_ret = OPRT_OK;

    PR_DEBUG("gw dev bind ifm callback, dev_id: %s, result: %d", dev_id, result);

    op_ret = tuya_iot_set_dev_hb_cfg(dev_id, 120, 3, FALSE);
    if (op_ret != OPRT_OK) {
        PR_WARN("tuya_iot_set_dev_hb_cfg err: %d", op_ret);
    }

    op_ret = __z3_dev_get_status(dev_id);
    if (op_ret != OPRT_OK) {
        PR_WARN("__z3_dev_get_status err: %d", op_ret);
    }
}

STATIC VOID __gw_dev_cmd_obj_cb(CONST TY_RECV_OBJ_DP_S *cmd)
{
    OPERATE_RET op_ret = OPRT_OK;
    DEV_DESC_IF_S *dev_if = NULL;

    PR_DEBUG("gw dev cmd obj callback");
    PR_DEBUG("cmd_tp: %d, dtt_tp: %d, dps_cnt: %u", cmd->cmd_tp, cmd->dtt_tp, cmd->dps_cnt);

    dev_if = tuya_iot_get_dev_if(cmd->cid);
    if (dev_if == NULL) {
        PR_WARN("%s is not bind", cmd->cid);
        return;
    }

    USHORT_T device_id = (dev_if->uddd & 0xFFFF);
    switch (device_id) {
    case ZG_DEVICE_ID_ON_OFF_LIGHT:
        __z3_onoff_light_down_data_process(cmd);
        break;
    default:
        PR_WARN("device_id: %04x is not supported");
        return;
    }
}

STATIC VOID __gw_dev_hb_cb(CONST CHAR_T *dev_id)
{
    OPERATE_RET op_ret = OPRT_OK;

    op_ret = tuya_iot_fresh_dev_hb(dev_id);
    if (op_ret != OPRT_OK) {
        PR_ERR("tuya_iot_fresh_dev_hb err: %d", op_ret);
        return;
    }
}

STATIC OPERATE_RET __gw_dev_upgrade_cb(CONST CHAR_T *dev_id, CONST FW_UG_S *fw)
{
    OPERATE_RET op_ret = OPRT_OK;

    PR_DEBUG("gw dev upgrade callback, dev_id: %s", dev_id);
    PR_DEBUG("fw->tp       : %d", fw->tp);
    PR_DEBUG("fw->fw_url   : %s", fw->fw_url);
    PR_DEBUG("fw->sw_ver   : %s", fw->sw_ver);
    PR_DEBUG("fw->file_size: %u", fw->file_size);

    op_ret = tuya_zigbee_upgrade_dev(dev_id, fw);
    if (op_ret != OPRT_OK) {
        PR_ERR("tuya_zigbee_upgrade_dev err: %d", op_ret);
    }

    return op_ret;
}

STATIC VOID_T __z3_custom_init_work(VOID_T *data)
{
    OPERATE_RET op_ret = OPRT_OK;
    DEV_DESC_IF_S *dev_if = NULL;
    VOID *iter = NULL;
    dev_if = tuya_iot_dev_traversal(&iter);
    while (dev_if) {
        if (dev_if->tp == GP_DEV_ATH_1) {
            op_ret = tuya_iot_set_dev_hb_cfg(dev_if->id, 120, 3, FALSE);
            if (op_ret != OPRT_OK) {
                PR_WARN("tuya_iot_set_dev_hb_cfg err: %d", op_ret);
            }
        }

        __z3_dev_get_status(dev_if->id);

        dev_if = tuya_iot_dev_traversal(&iter);
    }
}

STATIC VOID __zig_ncp_got_ver_cb(CHAR_T *ver)
{
    PR_DEBUG("ncp initiation completed, version: %s", ver);

    tal_workq_schedule(WORKQ_SYSTEM, __z3_custom_init_work, NULL);
}

OPERATE_RET user_zigbee_custom_init(VOID)
{
    OPERATE_RET rt = OPRT_OK;

    // 上行回调
    TY_Z3_DEV_CBS_S __z3_dev_cbs = {
        .join   = __z3_dev_join,
        .leave  = __z3_dev_leave,
        .report = __z3_dev_report,
    };

    // 下行回调
    TY_GW_SUBDEV_MGR_CBS_S dev_mgr_cbs = {
        .dev_del       = __gw_dev_del_cb,
        .dev_bind      = __gw_dev_bind_ifm_cb,
        .dev_hb        = __gw_dev_hb_cb,
        .dev_upgrade   = __gw_dev_upgrade_cb,
        .dev_reset     = __gw_dev_reset_cb,
        .dp_cmd_obj    = __gw_dev_cmd_obj_cb,
    };

    TUYA_CALL_ERR_RETURN(tuya_zigbee_reg_ncp_got_ver_cb(__zig_ncp_got_ver_cb));
    TUYA_CALL_ERR_RETURN(tuya_subdev_user_sigle_type_reg(&dev_mgr_cbs, GP_DEV_ATH_1));
    TUYA_CALL_ERR_RETURN(tuya_zigbee_custom_dev_mgr_init(&__custom_devlist, &__z3_dev_cbs));

    return OPRT_OK;
}