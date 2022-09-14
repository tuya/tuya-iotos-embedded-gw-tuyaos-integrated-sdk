author: Tuya
summary: tuyaos-gw-integrated
id: tuyaos-demo-zigbee
categories: quickstart,markdown
environments: Web
status: Published

# TuyaOS 综合 SDK Zigbee 网关应用

## 简介

本示例演示了使用 TuyaOS 综合 SDK 开发涂鸦 Zigbee 网关，搭配涂鸦网关 Zigbee 模组零代码接入涂鸦生态下所有 Zigbee 子设备，并且演示了把第三方 Zigbee 子设备接入到涂鸦生态，实现入网、控制、上报等功能。

本文档旨在介绍本示例的使用，详细开发指导请参考 SDK 开发文档中的 **Zigbee网关开发指南**。

## 准备工作

1. Ubuntu 开发环境。
2. [涂鸦 IoT 平台](https://iot.tuya.com/) 上创建网关。
3. 涂鸦生态 Zigbee 子设备产品。
4. 涂鸦网关 Zigbee 模组。

## 配置文件

示例工程使用 JSON 配置文件，把可配置的参数统一放到配置文件中，配置文件如下：

```
{
    "pid": "dn6an7mvbrnxgpi7",
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
| uuid                    | 是   | 授权信息，与 authkey 为一对，必须填写自己申请的 UUID。       |
| authkey                 | 是   | 授权信息，与 uuid 为一对，必须填写自己申请的 AUTHKEY。       |
| storage_path            | 是   | 存储路径，需要有可读性权限，必须以 '/' 结束。                |
| cache_path              | 是   | 缓存路径，升级时下载固件保存到此目录，必须以 '/' 结束。      |
| backup_path             | 是   | 备份路径 ，必须以 '/' 结束。                                 |
| log_level               | 否   | 日志等级，默认是 debug。                                     |
| tuya.zigbee.dev_name    | 是   | 连接 Zigbee 模组的串口设备名称，如 "/dev/ttyS0"。            |
| tuya.zigbee.cts         | 是   | 串口是否支持硬件流控，以实际使用的模组为准，Zigbee 模组通常要求接硬件流控。 |
| tuya.zigbee.thread_mode | 是   | 0 表示以子进程方式启动 Zigbee 服务，1 表示以线程方式启动 Zigbee 服务。 |

## 系统适配

SDK 是跨平台的设计，定义了一套标准的 TuyaOS Kernel Layer （简称 TKL）接口来屏蔽系统的差异，接口的定义在 SDK 框架开发包的 **adapter** 目录。系统适配接口需要由您实现，本示例代码提供了 Linux 系统通用实现，实现代码在示例工程的 **tuyaos_adapter** 目录。

在本示例中，我们主要关注有线配网的适配接口，使用示例工程提供的通用实现，需要修改 `tuyaos_adapter/wired/tkl_wired.c` 文件，把 **WIRED_IFNAME** 的值修改成您使用的网络接口名称即可。

> 注意，确保指定的网络接口跟手机在同一个局域网下，才能保证配网成功。

值得注意的是，由于主控和 Zigbee 模组使用串口通讯，我们还需要实现串口的适配接口。目前通用实现中的串口适配仅支持设备名称为 `/dev/ttSx` 和 `/dev/ttyUSBx` （x 是数字，表示串口号） 这两种格式的设备名称，其他设备名称如 `/dev/ttyAMA` 等需要自行适配，串口适配文件为 `tuyaos_adapter/uart/tkl_uart.c`。

## 编译运行

在 IDE 上编译本项目，编译没有问题的话，会生成一个可执行程序，位于 `TuyaOS/output/tuyaos_demo_zigbee_<版本号>` 目录下，版本号为编译时输入的版本号，配置文件也会自动拷贝到该目录下，切换到该目录，直接运行程序，会打印 Zigbee 的网络信息：

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
[01-01 00:00:08:591 TUYA N][tal_zg_slabs.c:148] nodeEuiStr = 086bd7fffed0cb51.
[01-01 00:00:08:591 TUYA N][tal_zg_slabs.c:149] nodeId = 0x0000.
[01-01 00:00:08:592 TUYA N][tal_zg_slabs.c:150] panId = 0x6c98.
[01-01 00:00:08:592 TUYA N][tal_zg_slabs.c:151] channel = 0x19.
[01-01 00:00:08:592 TUYA N][tal_zg_slabs.c:152] ver = 1.0.8.
[01-01 00:00:08:592 TUYA N][tal_zg_slabs.c:153] netStatus = 0x02.
[01-01 00:00:08:593 TUYA N][tal_zg_slabs.c:154] extendedPanId: bedf6a04332da2d1
[01-01 00:00:08:593 TUYA N][tal_zg_slabs.c:155] manufacturer_code: 0x0001
[01-01 00:00:08:593 TUYA N][tal_zg_slabs.c:157] nwk_key: bb 71 88 d1 35 a2 15 ff d5 a1 2d a2 c3 97 a2 f1
...
```

程序运行后，打开智慧生活 APP，使用 [自动发现功能](https://developer.tuya.com/cn/docs/iot/user-manual-for-tuya-smart-v3177?id=K9obrofrfk4sk#title-13-%E8%87%AA%E5%8A%A8%E6%B7%BB%E5%8A%A0) 添加网关设备，接着添加涂鸦生态的 Zigbee 子设备。

> 注意，配网时手机和设备要在同一个局域网。

## FAQ

1. 如何修改 PID、UUID 和 AUTHKEY？<br/>

   如果没有修改示例代码，修改 config.json 配置文件即可。

1. 自动发现不了网关设备？<br/>

   确认一下手机和网关是不是在同一个局域网，网关可以通过查看日志，观察是否选择正确的网口，打印示例：<br/>

   ```
   [01-01 00:00:05 TUYA D][tuya_svc_lan.c:838] udp ip: 192.168.1.20
   ```

1. 添加不了 Zigbee 子设备？<br/>

   如果输出日志没有显示 Zigbee 网络信息，说明串口通讯异常。检查一下硬件是否连接 Zigbee 模组，串口设备名称和硬件流控配置是否正确，串口通讯异常会有类似这样的打印输出：<br/>

   ```
   [01-01 00:00:18:927 TUYA D][ash-host-ui.c:441] ASH disconnected: EZSP_ASH_ERROR_RESET_FAIL
   [01-01 00:00:18:927 TUYA D][ash-host-ui.c:442] NCP status: EZSP_NO_ERROR
   ```

1. 使用涂鸦网关 Zigbee 模组如何扩展接入第三方私有 Zigbee 子设备？<br/>

   参考 `user_zigbee_custom.c` 示例代码，并且参考 SDK 开发文档中的 **Zigbee网关开发指南**。<br/>

1. 本地如何获取已添加的涂鸦生态 Zigbee 子设备的数据？<br/>

   SDK 提供了本地管理涂鸦生态 Zigbee 子设备的接口，可以获取到子设备入网、数据上报等数据，并且可以本地控制子设备，参考 `user_zigbee_local.c` 示例代码。