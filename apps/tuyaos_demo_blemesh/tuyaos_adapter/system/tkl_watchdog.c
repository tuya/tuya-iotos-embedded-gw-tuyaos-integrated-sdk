/**
* @file tkl_watchdog.c
* @brief Common process - adapter the watchdog api
* @version 0.1
* @date 2021-08-06
*
* @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
*
*/
#include "tkl_watchdog.h"

TUYA_WEAK_ATTRIBUTE UINT_T tkl_watchdog_init(TUYA_WDOG_BASE_CFG_T *cfg)
{
    return 0;
}

TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_watchdog_deinit(VOID_T)
{
    return OPRT_OK;
}

TUYA_WEAK_ATTRIBUTE OPERATE_RET tkl_watchdog_refresh(VOID_T)
{
    return OPRT_OK;
}
