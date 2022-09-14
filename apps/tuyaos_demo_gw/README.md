author: Tuya
summary: tuyaos-gw-integrated
id: tuyaos-demo-gw
categories: quickstart,markdown
environments: Web
status: Published

# TuyaOS 综合 SDK 网关应用

## 简介

本示例演示了使用 TuyaOS 综合 SDK 把网关设备接入到涂鸦 IoT 平台，并实现子设备的入网、指令控制、数据上报、OTA 等功能。

## 准备工作

1. Ubuntu 开发环境。
2. [涂鸦 IoT 平台](https://iot.tuya.com/) 上创建网关和子设备产品。

## 启动流程

在开始开发前，我们需要了解 SDK 的初始化流程，这里需要重点关注这几个接口的调用顺序：`tuya_iot_init`、`tuya_iot_set_gw_prod_info`、`tuya_iot_sdk_pre_init`、`tuya_iot_sdk_init` 和 `tuya_iot_sdk_start` ，必须按顺序执行。

SDK 初始化流程如下图所示：

![](https://images.tuyacn.com/fe-static/docs/img/b2c69225-c51e-44ac-b210-5e16eda51a0c.png)

## 文件和接口

常用的头文件：

| 头文件名称                  | 功能                                                         |
| --------------------------- | ------------------------------------------------------------ |
| tuya_iot_com_api.h          | 封装了 IoT 通用接口，常用的有获取 SDK 信息接口、IoT 基础服务初始化接口、数据点上报接口、获取数据点属性值接口、OTA 接口等。 |
| tuya_iot_sdk_api.h          | 封装了 SDK 接口，主要是 SDK 预初始化接口、SDK 初始化接口、SDK 启动接口等。 |
| tuya_gw_dp_mgr.h            | 提供了设备数据点管理回调注册接口，SDK -> Application         |
| tuya_gw_protocol_mgr_user.h | 提供了子设备管理回调注册接口，SDK -> Application             |
| uni_log.h                   | 封装了日志打印接口。                                         |

常用的接口：

| 接口名称                          | 接口说明                                                     |
| --------------------------------- | ------------------------------------------------------------ |
| tuya_iot_init()                   | IoT 基础服务初始化接口，SDK 所有服务和数据都依赖该接口。     |
| tuya_iot_set_gw_prod_info()       | 设置产品授权信息接口，也就是 UUID 和 AUTHKEY，必须保证一机一码。 |
| tuya_iot_reg_gw_app_cb()          | 注册网关应用回调接口，如设备升级、设备重置、设备重启、远程获取日志等通知。 |
| tuya_iot_sdk_reg_netstat_cb()     | 注册联网状态、有线状态、无线状态变更通知接口。               |
| tuya_iot_sdk_pre_init()           | SDK 预备初始化接口。                                         |
| tuya_iot_sdk_init()               | SDK 初始化接口。                                             |
| tuya_iot_sdk_start()              | SDK 启动接口。                                               |
| tuya_subdev_user_sigle_type_reg() | 注册子设备管理接口，如子设备添加、移除、重置、升级、指令下发、心跳查询等通知。 |
| tuya_gw_user_dp_reg()             | 注册设备数据点管理接口。                                     |
| dev_report_dp_json_async()        | 上报 OBJ 类型数据点接口。                                    |
| dev_report_dp_raw_sync()          | 上报 RAW 类型数据点接口。                                    |
| tuya_iot_set_dev_hb_cfg()         | 配置子设备心跳超时接口，需要在应用启动和绑定子设备时进行配置。 |
| tuya_iot_fresh_dev_hb()           | 刷新子设备心跳，在 `tuya_iot_set_dev_hb_cfg` 接口设置的超时时间内刷新心跳，在超时时间内没调用该接口子设备则显示为离线。 |
| tuya_iot_gw_bind_dev()            | 子设备绑定到涂鸦 IoT 平台接口。                              |
| tuya_iot_dev_traversal()          | 遍历所有子设备接口。                                         |
| tuya_iot_upgrade_dev()            | 子设备升级固件下载接口。                                     |
| tuya_iot_dev_upgd_progress_rept() | 子设备升级进度上报接口。                                     |

## 配置文件

示例工程使用 JSON 配置文件，把可配置的参数统一放到配置文件中，配置文件如下：

```
{
    "pid": "dn6an7mvbrnxgpi7",
    "uuid": "tuya461dbc63aeeb991f",
    "authkey": "c8X4PR4wx1gMFaQlaZu5dfgVvVRwB8Ug",
    "storage_path": "./",
    "log_level": 4
}
```

字段说明：

| 字段         | 必须 | 说明                                                         |
| ------------ | ---- | ------------------------------------------------------------ |
| pid          | 是   | 设备产品 ID，在 [涂鸦 IoT 平台](https://iot.tuya.com/) 上创建网关产品时由平台生成。 |
| uuid         | 是   | 授权信息，与 authkey 为一对，必须填写自己申请的 UUID。       |
| authkey      | 是   | 授权信息，与 uuid 为一对，必须填写自己申请的 AUTHKEY。       |
| log_level    | 否   | 日志等级，默认是 debug。                                     |
| storage_path | 是   | 存储路径，需要有可读性权限，必须以 '/' 结束。                |

## 系统适配

SDK 是跨平台的设计，定义了一套标准的 TuyaOS Kernel Layer （简称 TKL）接口来屏蔽系统的差异，接口的定义在 SDK 框架开发包的 **adapter** 目录。系统适配接口需要由您实现，本示例代码提供了 Linux 系统通用实现，实现代码在示例工程的 **tuyaos_adapter** 目录。

在本示例中，我们主要关注有线配网的适配接口，使用示例工程提供的通用实现，需要修改 `tuyaos_adapter/wired/tkl_wired.c` 文件，把 **WIRED_IFNAME** 的值修改成您使用的网络接口名称即可。

> 注意，确保指定的网络接口跟手机在同一个局域网下，才能保证配网成功。

详细开发指导请参考 SDK 开发文档中的 **配网指导**。

## 编译运行

在 IDE 上编译本项目，编译没有问题的话，会生成一个可执行程序，位于 `TuyaOS/output/tuyaos_demo_gw_<版本号>` 目录下，版本号为编译时输入的版本号，配置文件也会自动拷贝到该目录下，切换到该目录，直接运行程序即可，会看到类似如下的打印信息：

```
...
[01-01 00:00:00 TUYA D][tuya_ws_db.c:450] init fs. Path: ./
[01-01 00:00:00 TUYA I][kv_storge.c:45] Init Kvs With File:./
[01-01 00:00:00 TUYA D][simplekv.c:928] path:./
[01-01 00:00:00 TUYA D][simplekv.c:964] get encrypt_key[⚌⚌⚌US⚌⚌⚌⚌ykR⚌⚌]
[01-01 00:00:00 TUYA D][simplekv.c:995] both file exist
...
[01-01 00:00:05 TUYA D][tuya_svc_lan.c:838] udp ip: 192.168.1.20
...
```

程序运行后，打开智慧生活 APP，使用 [自动发现功能](https://developer.tuya.com/cn/docs/iot/user-manual-for-tuya-smart-v3177?id=K9obrofrfk4sk#title-13-%E8%87%AA%E5%8A%A8%E6%B7%BB%E5%8A%A0) 添加网关设备。

> 注意，配网时手机和设备要在同一个局域网。

## FAQ

1. 如何修改 PID、UUID 和 AUTHKEY？<br/>

   如果没有修改示例代码，修改 config.json 配置文件即可。
   
1. 自动发现不了网关设备？<br/>

   确认一下手机和网关是不是在同一个局域网，网关可以通过查看日志，观察是否选择正确的网口，打印示例：<br/>
   
   ```
   [01-01 00:00:05 TUYA D][tuya_svc_lan.c:838] udp ip: 192.168.1.20
   ```
