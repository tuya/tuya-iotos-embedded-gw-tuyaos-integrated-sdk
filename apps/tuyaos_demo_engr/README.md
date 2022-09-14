author: Tuya
summary: tuyaos-gw-integrated
id: tuyaos-demo-engr
categories: quickstart,markdown
environments: Web
status: Published

# TuyaOS 综合 SDK 施工模式应用

## 简介

本示例演示了使用 TuyaOS 综合 SDK 开发网关施工模式功能，本文将介绍本示例的使用，以及施工模式的开发要点。

## 准备工作

1. Ubuntu 开发环境。
2. [涂鸦 IoT 平台](https://iot.tuya.com/) 上创建网关产品。
3. [智慧工程](https://cp.tuya.com/) SaaS 平台上创建施工模板和施工任务。
2. 手机安装智慧工程 APP 和 智慧生活 APP。
2. [可选] 如果要接入涂鸦生态 Zigbee 子设备，还需要准备涂鸦网关 Zigbee 模组以及涂鸦生态 Zigbee 子设备产品。

## 配置文件

示例工程使用 JSON 配置文件，把可配置的参数统一放到配置文件中，配置文件如下：

```
{
    "pid": "dn6an7mvbrnxgpi7",
    "pid_engr": "ZkOA5uJGrtu4R2f8",
    "uuid": "tuya461dbc63aeeb991f",
    "authkey": "c8X4PR4wx1gMFaQlaZu5dfgVvVRwB8Ug",
    "storage_path": "./",
    "cache_path": "/tmp/",
    "backup_path": "./",
    "log_level": 4,
    "tuya": {
        "zigbee": {
            "dev_name": "/dev/ttyS6",
            "cts": 0,
            "thread_mode": 1
        }
    }
}
```

字段说明：

| 字段                    | 必须 | 说明                                                         |
| ----------------------- | ---- | ------------------------------------------------------------ |
| pid                     | 是   | 设备产品 ID，在 [涂鸦 IoT 平台](https://iot.tuya.com/) 上创建网关产品时由平台生成。 |
| pid_engr                | 是   | 施工模式的设备产品 ID。如果有做普通模式和施工模式关联，则使用施工模式 PID，如果未作关联，可以使用默认的施工模式 PID（ZkOA5uJGrtu4R2f8）。 |
| uuid                    | 是   | 授权信息，与 authkey 为一对，必须填写自己申请的 UUID。       |
| authkey                 | 是   | 授权信息，与 uuid 为一对，必须填写自己申请的 AUTHKEY。       |
| storage_path            | 是   | 存储路径，需要有可读性权限，必须以 '/' 结束。                |
| cache_path              | 是   | 缓存路径，升级时下载固件保存到此目录，必须以 '/' 结束。      |
| backup_path             | 是   | 备份路径 ，必须以 '/' 结束。                                 |
| log_level               | 否   | 日志等级，默认是 debug。                                     |
| tuya.zigbee.dev_name    | 是   | 连接 Zigbee 模组的串口设备名称，如 "/dev/ttyS0"。            |
| tuya.zigbee.cts         | 是   | 串口是否支持硬件流控，以实际使用的模组为准，Zigbee 模组通常要求接硬件流控。 |
| tuya.zigbee.thread_mode | 是   | 0 表示以子进程方式启动 Zigbee 服务，1 表示以线程方式启动 Zigbee 服务。 |

**说明，本示例的网关施工模式子设备接入的是涂鸦生态 Zigbee 设备，所以网关是搭配了涂鸦网关 Zigbee 模组来使用的。如果您使用的是第三方 Zigbee 模组，配置文件中把 tuya 字段的内容移除即可。**

## 系统适配

SDK 是跨平台的设计，定义了一套标准的 TuyaOS Kernel Layer （简称 TKL）接口来屏蔽系统的差异，接口的定义在 SDK 框架开发包的 **adapter** 目录。系统适配接口需要由您实现，本示例代码提供了 Linux 系统通用实现，实现代码在示例工程的 **tuyaos_adapter** 目录。

### 适配有线

在本示例中，我们主要关注有线配网的适配接口，使用示例工程提供的通用实现，需要修改 `tuyaos_adapter/wired/tkl_wired.c` 文件，把 **WIRED_IFNAME** 的值修改成您使用的网络接口名称即可。

> 注意，确保指定的网络接口跟手机在同一个局域网下，才能保证配网成功。

### 适配施工

当网关工作在施工模式，需要指定使用哪个网络接口与施工 APP 进行局域网通讯，所以要针对施工模式修改 TKL 接口的实现。

a) SDK 宏定义 `WIFI_GW == 0`，需要调整 `tkl_wired_get_ip` 接口。

b) SDK 宏定义 `WIFI_GW == 1`，需要调整 `tkl_wifi_get_ip` 接口。

通过 `tuya_get_engineer_mode` 接口判断网关当前工作模式是否是施工模式，如果是施工模式，获取 IP 地址的接口处理施工模式的逻辑，否则处理普通模式的逻辑。

本示例 SDK 宏定义为 `WIFI_GW == 1`，施工模式使用有线配网，所以 `tkl_wifi_get_ip` 接口返回有线网络接口的 IP 地址：

```
OPERATE_RET tkl_wifi_get_ip(CONST WF_IF_E wf, NW_IP_S *ip)
{
#if defined(ENABLE_ENGINEER_TO_NORMAL) && (ENABLE_ENGINEER_TO_NORMAL==1)
    extern BOOL_T tuya_get_engineer_mode(VOID);
    extern OPERATE_RET tkl_wired_get_ip(NW_IP_S *ip);
    if (tuya_get_engineer_mode()) {
        return tkl_wired_get_ip(ip);
    }
#endif

	return OPRT_COM_ERROR;
}
```

### 适配串口

如果使用涂鸦网关 Zigbee 模组，由于主控和 Zigbee 模组使用串口通讯，我们还需要实现串口的适配接口。目前通用实现中的串口适配仅支持设备名称为 `/dev/ttSx` 和 `/dev/ttyUSBx` （x 是数字，表示串口号） 这两种格式的设备名称，其他设备名称如 `/dev/ttyAMA` 等需要自行适配，串口适配文件为 `tuyaos_adapter/uart/tkl_uart.c`。

## 编译运行

在 IDE 上编译本项目，编译没有问题的话，会生成一个可执行程序，位于 `TuyaOS/output/tuyaos_demo_engr_<版本号>` 目录下，版本号为编译时输入的版本号，配置文件也会自动拷贝到该目录下，切换到该目录，命令行带参数 `engr` 运行进入施工模式，不带参数运行进入普通模式。

普通模式：`./tuyaos_demo_engr`。

施工模式：`./tuyaos_demo_engr engr`。

进入施工模式会打印局域网广播的数据内容：

```
...
[01-01 00:00:05 TUYA D][app_agent_engr.c:998] Create UDP Socket To Broadcast
[01-01 00:00:05 TUYA D][app_agent_engr.c:1061] BufToSend 151:{"ip":"172.24.155.238","gwId":"tuya461dbc63aeeb991f","active":0,"ablilty":1,"encrypt":true,"productKey":"ZkOA5uJGrtu4R2f8","version":"3.3","token":true}
[01-01 00:00:05 TUYA D][app_agent_engr.c:1101] send len:188
[01-01 00:00:10 TUYA D][app_agent_engr.c:2005] Send udp discover
[01-01 00:00:10 TUYA D][app_agent_engr.c:1061] BufToSend 151:{"ip":"172.24.155.238","gwId":"tuya461dbc63aeeb991f","active":0,"ablilty":1,"encrypt":true,"productKey":"ZkOA5uJGrtu4R2f8","version":"3.3","token":true}
...
```

网关设备进入工程模式后，打开智慧工程 APP，选择施工任务，添加网关和子设备，并验证预配置的场景和自动化，最后提交施工结果上传部署。

施工完成后，网关切换到普通模式，网关处于非激活状态，使用智慧生活 APP 激活网关设备即可，子设备、场景和自动化会自动恢复。

## 开发指导

施工模式的程序整体结构基本与普通模式是一致的，网关设备需要能够在施工模式和普通模式间切换，建议使用命令行参数来控制程序运行在普通模式还是施工模式。

以下对施工模式的开发流程进行简单的说明：

1. 施工模式存储路径为普通模式存储路径 + "\engineer\"。
2. `tuya_iot_init` 接口之前调用 `tuya_set_engineer_mode` 接口进入施工模式。
3. 当设备工作模式为普通模式时，`tuya_iot_init` 使用普通模式存储路径。当设备工作模式为施工模式时，`tuya_iot_init` 使用施工模式存储路径。
4. `tuya_engineer_gw_init` 接口之前调用 `tuya_engineer_sdk_init` 接口初始化施工业务，施工模式和普通模式都需要调用该接口，该接口参数传递的是 **普通模式 PID**。
5. 当设备工作模式为普通模式时，使用 `tuya_iot_sdk_init` / `tuya_iot_wf_sdk_init` / `tuya_iot_wr_wf_sdk_init` 初始化 SDK 业务。当设备工作模式为施工模式时，使用 `tuya_engineer_gw_init` 接口初始化 SDK 业务。

## FAQ

1. 施工配网超时？<br/>

   确认施工 PID 以及网络接口是否正确，可以看输出的打印：<br/>

   ```
   [01-01 00:00:05 TUYA D][app_agent_engr.c:1061] BufToSend 151:{"ip":"172.24.155.238","gwId":"tuya461dbc63aeeb991f","active":0,"ablilty":1,"encrypt":true,"productKey":"ZkOA5uJGrtu4R2f8","version":"3.3","token":true}
   ```

1. 施工完成，用智慧生活 APP 激活网关，子设备、场景和自动化没有恢复？<br/>

   a) 确认施工任务中的网关 PID 与设备使用的 PID 是否是一致的。<br/>

   b) 确认施工任务完成后，在智慧工程 APP 上点击了 **上传部署**。<br/>

   c) 确保施工模式的数据同步回调接口有实现把施工模式的数据迁移到普通模式的存储路径。

1. 施工模式如何本地恢复出厂设置？<br/>

   除了施工模式有其业务的接口之外，其余接口与普通模式是复用的。
