#ifndef __USER_BLEMESH_SVC_H__
#define __USER_BLEMESH_SVC_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief initiate blemesh service
 */
OPERATE_RET user_blemesh_svc_init(VOID);

/**
 * @brief start blemesh service
 */
OPERATE_RET user_blemesh_svc_start(VOID);

#ifdef __cplusplus
}
#endif
#endif