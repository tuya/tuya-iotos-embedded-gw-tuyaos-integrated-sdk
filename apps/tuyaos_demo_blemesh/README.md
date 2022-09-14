author: Tuya
summary: tuyaos-gw-integrated
id: tuyaos-demo-blemesh
categories: quickstart,markdown
environments: Web
status: Published

# TuyaOS 综合 SDK 蓝牙网关应用

## 简介

本示例演示了使用 TuyaOS 综合 SDK 开发涂鸦蓝牙网关，搭配涂鸦网关蓝牙模组零代码接入涂鸦生态下所有蓝牙 BLE&MESH 子设备。

## 准备工作

1. Ubuntu 开发环境。
2. [涂鸦 IoT 平台](https://iot.tuya.com/) 上创建网关。
3. 涂鸦生态蓝牙 BLE 或者 MESH 子设备产品。
4. 涂鸦网关蓝牙模组。

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
        "bt": {
            "dev_name": "/dev/ttyS6",
            "enable_hb": 1,
            "scan_timeout": 60,
            "mode": 258
        }
    }
}
```

字段说明：

| 字段                 | 必须 | 说明                                                         |
| -------------------- | ---- | ------------------------------------------------------------ |
| pid                  | 是   | 设备产品 ID，在 [涂鸦 IoT 平台](https://iot.tuya.com/) 上创建网关产品时由平台生成。 |
| uuid                 | 是   | 授权信息，与 authkey 为一对，必须填写自己申请的 UUID。       |
| authkey              | 是   | 授权信息，与 uuid 为一对，必须填写自己申请的 AUTHKEY。       |
| storage_path         | 是   | 存储路径，需要有可读性权限，必须以 '/' 结束。                |
| cache_path           | 是   | 缓存路径，升级时下载固件保存到此目录，必须以 '/' 结束。      |
| backup_path          | 是   | 备份路径 ，必须以 '/' 结束。                                 |
| log_level            | 否   | 日志等级，默认是 debug。                                     |
| tuya.bt.dev_name     | 是   | 连接蓝牙模组的串口设备名称，如 "/dev/ttyS0"。                |
| tuya.bt.enable_hb    | 是   | 是否开启 BLE 心跳。                                          |
| tuya.bt.scan_timeout | 是   | BLE 扫描周期，单位是秒。                                     |
| tuya.bt.mode         | 是   | 设置蓝牙模组的工作模式，工作模式的定义在 `TAL_BLE_ROLE_E`，支持按位与操作，网关一般是 `TAL_BLE_ROLE_CENTRAL \| TAL_MESH_ROLE_ADV_PROVISIONER`。 |

## 系统适配

SDK 是跨平台的设计，定义了一套标准的 TuyaOS Kernel Layer （简称 TKL）接口来屏蔽系统的差异，接口的定义在 SDK 框架开发包的 **adapter** 目录。系统适配接口需要由您实现，本示例代码提供了 Linux 系统通用实现，实现代码在示例工程的 **tuyaos_adapter** 目录。

在本示例中，我们主要关注有线配网的适配接口，使用示例工程提供的通用实现，需要修改 `tuyaos_adapter/wired/tkl_wired.c` 文件，把 **WIRED_IFNAME** 的值修改成您使用的网络接口名称即可。

> 注意，确保指定的网络接口跟手机在同一个局域网下，才能保证配网成功。

值得注意的是，由于主控和蓝牙模组使用串口通讯，我们还需要实现串口的适配接口。目前通用实现中的串口适配仅支持设备名称为 `/dev/ttSx` 和 `/dev/ttyUSBx` （x 是数字，表示串口号） 这两种格式的设备名称，其他设备名称如 `/dev/ttyAMA` 等需要自行适配，串口适配文件为 `tuyaos_adapter/uart/tkl_uart.c`。

## 编译运行

在 IDE 上编译本项目，编译没有问题的话，会生成一个可执行程序，位于 `TuyaOS/output/tuyaos_demo_blemesh_<版本号>` 目录下，版本号为编译时输入的版本号，配置文件也会自动拷贝到该目录下，切换到该目录，直接运行程序，会打印蓝牙模组的版本号：

```
...
[01-01 00:00:00 TUYA D][tuya_ws_db.c:450] init fs. Path: ./
[01-01 00:00:00 TUYA I][kv_storge.c:45] Init Kvs With File:./
[01-01 00:00:00 TUYA D][simplekv.c:928] path:./
[01-01 00:00:00 TUYA D][simplekv.c:964] get encrypt_key[⚌⚌⚌US⚌⚌⚌⚌ykR⚌⚌]
[01-01 00:00:00 TUYA D][simplekv.c:995] both file exist
...
[01-01 00:00:02 TUYA D][cmd_host.c:1783] prev cmd host stat: BLE_BOOT
[01-01 00:00:02 TUYA D][cmd_host.c:1783] cur cmd host stat: GET_CMD
uart tx[6]:20 02 FF 00 01 A0
uart rx[11]:20 07 FF 00 00 00 04 A0 03 14 0F
[01-01 00:00:02 TUYA D][tuya_ble_host.c:1273] version:3.20.15
...
[01-01 00:00:05 TUYA D][tuya_svc_lan.c:838] udp ip: 192.168.1.20
...
```

程序运行后，打开智慧生活 APP，使用 [自动发现功能](https://developer.tuya.com/cn/docs/iot/user-manual-for-tuya-smart-v3177?id=K9obrofrfk4sk#title-13-%E8%87%AA%E5%8A%A8%E6%B7%BB%E5%8A%A0) 添加网关设备，接着添加涂鸦生态的蓝牙子设备。

> 注意，配网时手机和设备要在同一个局域网。

## FAQ

1. 如何修改 PID、UUID 和 AUTHKEY？<br/>

   如果没有修改示例代码，修改 config.json 配置文件即可。

1. 自动发现不了网关设备？<br/>

   确认一下手机和网关是不是在同一个局域网，网关可以通过查看日志，观察是否选择正确的网口，打印示例：<br/>

   ```
   [01-01 00:00:05 TUYA D][tuya_svc_lan.c:838] udp ip: 192.168.1.20
   ```

1. 网关蓝牙配网失败？<br/>

   使用涂鸦蓝牙模组，默认支持蓝牙配网，蓝牙适配接口不需要适配，但蓝牙配网依赖无线功能，需要适实现无线的适配接口，详细参考 SDK 开发文档的 **配网指南**。<br/>
   
   先确认蓝牙模组串口是否连通，然后观察日志看是否有蓝牙接收到路由器的 SSID 和密码的打印：<br/>
   
   ```
   [01-01 00:02:34 TUYA D][tuya_bt_sdk.c:44] Rev BT user Data. type:33
   [01-01 00:02:34 TUYA N][bt_netcfg.c:55] Rev Net Cfg:{"ssid":"16F-N-3-39","pwd":"12345678","token":"AYAqTsPFHI2xh1"}
   [01-01 00:02:34 TUYA D][tuya_ble_api.c:1033] ble get conn stat:3
   [01-01 00:02:34 TUYA D][tuya_ble_api.c:844] tuya_ble_sdk_send. cmd:0x21 encrpyt:2
   [01-01 00:02:34 TUYA N][tuya_ble_api.c:74] bt_ack_uncrypt 1
   ```
   
1. 添加不了蓝牙子设备？<br/>

   如果输出日志没有显示蓝牙模组的版本号，说明串口通讯异常。检查一下硬件是否连接蓝牙模组，串口设备名称配置是否正确。蓝牙模组要求支持硬件流控，检查硬件串口是否已支持流控。<br/>
   
   蓝牙串口通讯异常打印输出只有 uart tx，没有 uart rx：<br/>
   
   ```
   responese timeout and retry..0
   uart tx[6]:20 02 FF 00 01 A0
   ```
   
   如果确认蓝牙模组串口通讯正常，仍无法添加蓝牙子设备，最好确认一下创建的网关产品 ID 协议有没有选择 `蓝牙Mesh(SIG)` 协议，没有选择该协议则无法添加 MESH 子设备。
