/**
 * @file tkl_mutex.c
 * @brief the default weak implements of tuya os mutex, this implement only used when OS=linux
 * @version 0.1
 * @date 2019-08-15
 * 
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 * 
 */

#include "tkl_mutex.h"
#include "tkl_memory.h"
#include "tuya_error_code.h"
#include "tuya_iot_config.h"
#include <pthread.h>
#include <errno.h>


typedef pthread_mutex_t TKL_THRD_MUTEX;
typedef struct
{
    TKL_THRD_MUTEX mutex;
}TKL_MUTEX_MANAGE,*P_TKL_MUTEX_MANAGE;

/**
* @brief Create mutex
*
* @param[out] pMutexHandle: mutex handle
*
* @note This API is used to create mutex.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_mutex_create_init(TKL_MUTEX_HANDLE *handle)
{
    if(!handle)
        return OPRT_INVALID_PARM;
    
    P_TKL_MUTEX_MANAGE mutex_manage;
    mutex_manage = (P_TKL_MUTEX_MANAGE)tkl_system_malloc(SIZEOF(TKL_MUTEX_MANAGE));
    if(!(mutex_manage))
        return OPRT_MALLOC_FAILED;
    
    INT_T ret;
#if defined(CREATE_RECURSION_MUTEX)
    pthread_mutexattr_t attr;

    ret = pthread_mutexattr_init(&attr);
    if(0 != ret) {
        return OPRT_OS_ADAPTER_MUTEX_CREAT_FAILED;
    }
    
    ret = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
    if(0 != ret) {
        return OPRT_OS_ADAPTER_MUTEX_CREAT_FAILED;
    }
    
    ret = pthread_mutex_init(&(mutex_manage->mutex),&attr);
    if(0 != ret) {
        return OPRT_OS_ADAPTER_MUTEX_CREAT_FAILED;
    }
    
    ret = pthread_mutexattr_destroy(&attr);
    if(0 != ret) {
        return OPRT_OS_ADAPTER_MUTEX_RELEASE_FAILED;
    }
#else
    ret = pthread_mutex_init(&(mutex_manage->mutex),NULL);
    if(0 != ret) {
        return OPRT_OS_ADAPTER_MUTEX_CREAT_FAILED;
    }
#endif

    *handle = (TKL_MUTEX_HANDLE)mutex_manage;

    return OPRT_OK;
}

/**
* @brief Lock mutex
*
* @param[in] mutexHandle: mutex handle
*
* @note This API is used to lock mutex.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_mutex_lock(CONST TKL_MUTEX_HANDLE handle)
{
    if(!handle) {
        return OPRT_INVALID_PARM;
    }
    
    P_TKL_MUTEX_MANAGE mutex_manage;
    mutex_manage = (P_TKL_MUTEX_MANAGE)handle;

    INT_T ret;
    ret= pthread_mutex_lock(&(mutex_manage->mutex));
    if(ret != 0) {
        return OPRT_OS_ADAPTER_MUTEX_LOCK_FAILED;
    }

    return OPRT_OK;
}

/**
* @brief Unlock mutex
*
* @param[in] mutexHandle: mutex handle
*
* @note This API is used to unlock mutex.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_mutex_unlock(CONST TKL_MUTEX_HANDLE handle)
{
    if(!handle) {
        return OPRT_INVALID_PARM;
    }
    
    P_TKL_MUTEX_MANAGE mutex_manage;
    mutex_manage = (P_TKL_MUTEX_MANAGE)handle;
    
    INT_T ret;
    ret= pthread_mutex_unlock(&(mutex_manage->mutex));
    if(ret != 0) {
        return OPRT_OS_ADAPTER_MUTEX_UNLOCK_FAILED;
    }

    return OPRT_OK;
}

/**
* @brief Release mutex
*
* @param[in] mutexHandle: mutex handle
*
* @note This API is used to release mutex.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_mutex_release(CONST TKL_MUTEX_HANDLE handle)
{
    if(!handle) {
        return OPRT_INVALID_PARM;
    }
    
    P_TKL_MUTEX_MANAGE mutex_manage;
    mutex_manage = (P_TKL_MUTEX_MANAGE)handle;

    INT_T ret;
    ret= pthread_mutex_destroy(&(mutex_manage->mutex));
    if(ret != 0) {
        return OPRT_OS_ADAPTER_MUTEX_RELEASE_FAILED;
    }
    
    tkl_system_free(handle);
    return OPRT_OK;
}

