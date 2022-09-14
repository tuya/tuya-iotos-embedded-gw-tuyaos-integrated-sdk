/**
 * @file tkl_queue.c
 * @brief the default weak implements of tuya os queue, this implement only used when OS=linux
 * @version 0.1
 * @date 2019-08-15
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */

#include "tkl_queue.h"

/**
 * @brief Create message queue
 *
 * @param[in] msgsize message size
 * @param[in] msgcount message number
 * @param[out] queue the queue handle created
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_queue_create_init(TKL_QUEUE_HANDLE *queue, INT_T msgsize, INT_T msgcount)
{
    return OPRT_OK;
}

/**
 * @brief post a message to the message queue
 *
 * @param[in] queue the handle of the queue
 * @param[in] data the data of the message
 * @param[in] timeout timeout time
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_queue_post(CONST TKL_QUEUE_HANDLE queue, VOID_T *data, UINT_T timeout)
{
    return OPRT_OK;
}

/**
 * @brief fetch message from the message queue
 *
 * @param[in] queue the message queue handle
 * @param[inout] msg the message fetch form the message queue
 * @param[in] timeout timeout time
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_queue_fetch(CONST TKL_QUEUE_HANDLE queue, VOID_T *msg, UINT_T timeout)
{
    return OPRT_OK;
}


/**
 * @brief free the message queue
 *
 * @param[in] queue the message queue handle
 *
 * @return VOID_T
 */
TUYA_WEAK_ATTRIBUTE VOID_T tkl_queue_free(CONST TKL_QUEUE_HANDLE queue)
{
    return;
}

