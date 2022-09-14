#ifndef __USER_ZIGBEE_SVC_H__
#define __USER_ZIGBEE_SVC_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief initiate zigbee service
 */
OPERATE_RET user_zigbee_svc_init(VOID);

/**
 * @brief start zigbee service
 */
OPERATE_RET user_zigbee_svc_start(VOID);

#ifdef __cplusplus
}
#endif
#endif