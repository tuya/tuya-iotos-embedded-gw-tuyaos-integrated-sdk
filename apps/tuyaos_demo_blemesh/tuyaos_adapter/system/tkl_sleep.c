/**
 * @file tkl_sleep.c
 * @brief tkl_sleep  api
 * @version 0.1
 * @date 2020-05-15
 * 
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tkl_sleep.h"

OPERATE_RET tkl_cpu_sleep_callback_register(TUYA_SLEEP_CB_T *sleep_cb)
{
    return OPRT_NOT_SUPPORTED;
}

VOID_T tkl_cpu_allow_sleep(VOID_T)
{
    return;
}

VOID_T tkl_cpu_force_wakeup(VOID_T)
{
    return;
}

OPERATE_RET tkl_cpu_sleep_mode_set(BOOL_T enable, TUYA_CPU_SLEEP_MODE_E mode)
{
    return OPRT_NOT_SUPPORTED;
}

