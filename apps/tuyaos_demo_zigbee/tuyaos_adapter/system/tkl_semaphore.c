/**
 * @file tkl_semaphore.c
 * @brief the default weak implements of tuya os semaphore, this implement only used when OS=linux
 * @version 0.1
 * @date 2019-08-15
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */

#include "tkl_semaphore.h"
#include "tkl_memory.h"
#include <semaphore.h>
#include <time.h>

typedef struct
{
    sem_t sem;
}TKL_SEM_MANAGE,*P_TKL_SEM_MANAGE;

/**
* @brief Create semaphore
*
* @param[out] handle: semaphore handle
* @param[in] sem_cnt: semaphore init count
* @param[in] sem_max: semaphore max count
*
* @note This API is used for creating semaphore.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_semaphore_create_init(TKL_SEM_HANDLE *handle, CONST UINT_T sem_cnt, CONST UINT_T sem_max)
{
    if(!handle) {
        return OPRT_INVALID_PARM;
    }

    P_TKL_SEM_MANAGE sem_manage; 
    sem_manage = (P_TKL_SEM_MANAGE)tkl_system_malloc(SIZEOF(TKL_SEM_MANAGE));
    if (sem_manage == NULL) {
        return OPRT_MALLOC_FAILED;
    }

    INT_T ret;
    ret = sem_init(&sem_manage->sem, 0, sem_cnt);
    if(ret != 0) {
        tkl_system_free(sem_manage);
        *handle = NULL;
        return ret;
    }
    
    *handle = (TKL_SEM_HANDLE)sem_manage;
    return OPRT_OK;
}

/**
* @brief Wait semaphore
*
* @param[in] handle: semaphore handle
* @param[in] timeout: wait timeout, SEM_WAIT_FOREVER means wait until get semaphore
*
* @note This API is used for waiting semaphore.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_semaphore_wait(CONST TKL_SEM_HANDLE handle, CONST UINT_T timeout)
{
    if(!handle) {
        return OPRT_INVALID_PARM;
    }
    
    P_TKL_SEM_MANAGE sem_manage;
    sem_manage = (P_TKL_SEM_MANAGE)handle;

    INT_T ret;
    if (timeout == TKL_SEM_WAIT_FOREVER) {
        ret = sem_wait(&sem_manage->sem);
    } else {
        struct timespec ts = {0, 0};
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += (timeout/1000);
        ts.tv_nsec += ((timeout%1000)*1000*1000);
        ret = sem_timedwait(&sem_manage->sem, &ts);
    }
    
    if (0 != ret) {
        return OPRT_OS_ADAPTER_SEM_WAIT_FAILED;
    }
    
    return OPRT_OK;
}

/**
* @brief Post semaphore
*
* @param[in] handle: semaphore handle
*
* @note This API is used for posting semaphore.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_semaphore_post(CONST TKL_SEM_HANDLE handle)
{
    if(!handle) {
        return OPRT_INVALID_PARM;
    }
    
    P_TKL_SEM_MANAGE sem_manage;
    sem_manage = (P_TKL_SEM_MANAGE)handle;

    INT_T ret;
    ret= sem_post(&(sem_manage->sem));
    if(ret != 0) {
        return OPRT_OS_ADAPTER_SEM_POST_FAILED;
    }
    
    return OPRT_OK;
}

/**
* @brief Release semaphore
*
* @param[in] handle: semaphore handle
*
* @note This API is used for releasing semaphore.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_semaphore_release(CONST TKL_SEM_HANDLE handle)
{
    if(!handle) {
        return OPRT_INVALID_PARM;
    }
    
    P_TKL_SEM_MANAGE sem_manage;
    sem_manage = (P_TKL_SEM_MANAGE)handle;

    INT_T ret;
    ret= sem_destroy(&(sem_manage->sem));
    tkl_system_free(handle); // 释放信号量管理结构
    if(ret != 0) {
        return OPRT_OS_ADAPTER_MUTEX_RELEASE_FAILED;
    }
    
    return OPRT_OK;
}


