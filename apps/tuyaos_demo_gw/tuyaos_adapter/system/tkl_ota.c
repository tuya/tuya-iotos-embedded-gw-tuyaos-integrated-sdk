/**
 * @file tkl_ota.c
 * @brief default weak implements of tuya ota, it only used when OS=linux
 * 
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 * 
 */
#include "tkl_ota.h"
#include "tuya_error_code.h"

/**
* @brief get ota ability
*
* @param[out] image_size:  max image size
* @param[out] type:        bit0, 1 - support full package upgrade
                                 0 - dont support full package upgrade
*                          bit1, 1 - support compress package upgrade
                                 0 - dont support compress package upgrade
*                          bit2, 1 - support difference package upgrade
                                 0 - dont support difference package upgrade
* @note This API is used for get chip ota ability
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tkl_ota_get_ability(UINT_T *image_size, UINT32_T *type)
{
    return OPRT_OK;
}

/**
* @brief ota start notify
*
* @param[in] image_size:  image size
* @param[in] type:        ota type
* @param[in] path:        ota path
*
* @note This API is used for ota start notify
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tkl_ota_start_notify(UINT_T image_size, TUYA_OTA_TYPE_E type, TUYA_OTA_PATH_E path)
{
    return OPRT_OK;
}


/**
* @brief ota data process
*
* @param[in] pack:       point to ota pack
* @param[in] remain_len: ota pack remain len
*
* @note This API is used for ota data process
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tkl_ota_data_process(TUYA_OTA_DATA_T *pack, UINT_T* remain_len)
{
    return OPRT_OK;
}


/**
* @brief ota end notify
*
* @param[in] reset:  ota reset
*
* @note This API is used for ota end notify
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tkl_ota_end_notify(BOOL_T reset)
{
    return OPRT_OK;
}

/**
* @brief get old firmware info
*
* @param[out] image_size:  max image size
* @param[out] type:        ota type
*
* @note This API is used for old firmware info, and only used in resumes transmission at break-points
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tkl_ota_get_old_firmware_info(TUYA_OTA_FIRMWARE_INFO_T **info)
{
    return OPRT_OK;
}

