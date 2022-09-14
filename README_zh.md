# TuyaOS 综合 SDK

[English](./README.md) | 中文版

## 概述

网关开发是基于 TuyaOS 系统裁剪得到的适用于网关产品开发的开发包，适用于任意协议类型子设备接入涂鸦 IoT 开发平台，如 Zigbee、蓝牙 BLE、蓝牙 Mesh、433/868 MHz、Zwave 等协议，并且与涂鸦生态下的智能设备互联互通。

开发包将网关配网（有线配网、AP 配网、蓝牙配网等）、上下行数据通信、子设备管理、本地联动、分布式网关局域网联动、施工模式、故障替换和固件 OTA 等能力进行封装，并提供相关函数接口，只需要按照 TuyaOS 提供的标准 API 进行适配，即可实现网关设备接入涂鸦 IoT 开发平台。

## SDK 方案

SDK 支持把任意协议类型的子设备接入涂鸦 IoT 开发平台，可以使用 `第三方子设备接入模组` 实现把子设备接入到涂鸦 IoT 开发平台，也可以使用 `涂鸦标准子设备接入模组` 接入涂鸦生态子设备。

- 使用 `第三方子设备接入模组`，子设备接入由开发者自主开发
- 使用 `涂鸦标准子设备接入模组`，涂鸦生态子设备接入由开发包内部处理，可零代码接入涂鸦生态子设备

![](https://images.tuyacn.com/fe-static/docs/img/13c7190d-2cd9-4354-8b0d-224e6268a0ad.png)

## 快速开始

### 获取 SDK

> 说明：体验版本我们仅提供 Ubuntu 平台的 SDK 开发包，如果需要其他硬件平台的开发包，请联系涂鸦商务。

| 开发包                                                       | 支持有线配网       | 支持无线配网       | 支持蓝牙配网       | 蓝牙网关-涂鸦模组  | 蓝牙网关-第三方模组 |
| ------------------------------------------------------------ | ------------------ | ------------------ | ------------------ | ------------------ | ------------------- |
| [ubuntu-x64_eth-gw-com](https://images.tuyacn.com/rms-static/f70721f0-33f4-11ed-9020-89eff5bc050d-1663136272783.tar.gz?tyName=tuyaos-gw-integrated_3.5.6_ubuntu-x64_eth-gw-com_0.0.1.tar.gz) | :white_check_mark: | :x:                | :x:                | :x:                | :x:                 |
| [ubuntu-x64_eth-wifi-gw-com](https://images.tuyacn.com/rms-static/f708cfa0-33f4-11ed-acd5-2f6ea556a3fd-1663136272794.tar.gz?tyName=tuyaos-gw-integrated_3.5.6_ubuntu-x64_eth-wifi-gw-com_0.0.1.tar.gz) | :white_check_mark: | :white_check_mark: | :x:                | :x:                | :x:                 |
| [ubuntu-x64_eth-wifi-ble-gw-bt-common](https://images.tuyacn.com/rms-static/4bf6ab00-33fe-11ed-acd5-2f6ea556a3fd-1663140280752.tar.gz?tyName=tuyaos-gw-integrated_3.5.6_ubuntu-x64_eth-wifi-ble-gw-bt-common_0.0.1.tar.gz) | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x:                | :white_check_mark:  |
| [ubuntu-x64_eth-wifi-ble-gw-bt-silabshost](https://images.tuyacn.com/rms-static/3fc8afe0-33fe-11ed-9020-89eff5bc050d-1663140260318.tar.gz?tyName=tuyaos-gw-integrated_3.5.6_ubuntu-x64_eth-wifi-ble-gw-bt-silabshost_0.0.1.tar.gz) | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x:                 |

### 编译

1. 解压下载的开发包，并且把本仓库的 apps 文件夹拷贝到开发包的根目录下。
2. 运行 `./build_app.sh apps/tuyaos_demo_gw tuyaos_demo_gw 1.0.0`，编译 DEMO 工程。
3. 编译完成后，程序位于 `output` 目录下，可以直接在 Ubuntu 系统下运行。

DEMO 工程的详细介绍，请参见各 DEMO 的文档：

- [tuyaos_demo_gw](./apps/tuyaos_demo_gw/README.md)
- [tuyaos_demo_zigbee](./apps/tuyaos_demo_zigbee/README.md)
- [tuyaos_demo_engr](./apps/tuyaos_demo_gw/README.md)
- [tuyaos_demo_blemesh](./apps/tuyaos_demo_blemesh/README.md)

### 文档

开发时请先阅读开发文档，开发文档和接口文档在开发包的 `docs` 文件夹下。

## 获取支持

- [涂鸦 IoT 开发者平台](https://developer.tuya.com/cn)

- [涂鸦帮助中心](https://support.tuya.com/zh/help)

- [涂鸦技术工单平台](https://service.console.tuya.com)  

## License

[MIT License](./LICENSE)
