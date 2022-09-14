#ifndef __USER_ENGR_SVC_H__
#define __USER_ENGR_SVC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"

/**
 * @brief 设置施工模式存储路径
 * 
 * @param[in] path 普通模式存储路径，施工模式存储路径为普通模式存储目录下的 engineer 目录
 * 
 * @return OPRT_OK: 成功，Others: 失败
 */
OPERATE_RET user_engr_set_storage_path(CONST CHAR_T *path);

/**
 * @brief 获取施工模式存储路径
 */
CHAR_T *user_engr_get_storage_path(VOID);

/**
 * @brief 施工模式服务初始化，普通模式和施工模式都要调用该接口
 * 
 * @param pid_engr 普通模式 PID
 * 
 * @return OPRT_OK: 成功，Others: 失败
 */
OPERATE_RET user_engr_svc_init(IN CONST CHAR_T *pid);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __USER_ENGR_SVC_H__
