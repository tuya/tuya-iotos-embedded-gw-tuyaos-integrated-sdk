/**
 * @file tkl_output.c
 * @brief the default weak implements of tuya os output, this implement only used when OS=linux
 * @version 0.1
 * @date 2019-08-15
 * 
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 * 
 */

#include "tkl_output.h"
#include <stdio.h>

/**
* @brief Output log information
*
* @param[in] str: log information
*
* @note This API is used for outputing log information
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
TUYA_WEAK_ATTRIBUTE VOID_T tkl_log_output(IN CONST CHAR_T *str, ...)
{
    printf("%s", str);
    fflush(stdout);
    return;
}

/**
* @brief Close log port
*
* @param VOID_T
*
* @note This API is used for closing log port.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_log_close(VOID_T)
{
    return OPRT_OK;
}

/**
* @brief Open log port
*
* @param VOID_T
*
* @note This API is used for openning log port.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_log_open(VOID_T)
{
    return OPRT_OK;
}


