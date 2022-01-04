# TuyaOS 综合 SDK

[English](./README.md) | 中文版

## 概述

TuyaOS 综合 SDK 采用分层、插拔式组件的方式设计，开发者可以按需链接对应的组件库，各组件库可以灵活搭配使用，以实现快速构建具有差异性的网关产品。

SDK 除了提供基本的连接涂鸦云的能力，还可以具备网关能力，并且融合红外，中控，涂鸦 Zigbee 网关模组，涂鸦 Bluetooth LE 网关模组，路由器以及中继器，无感配网等业务，这些业务是涂鸦生态智能方案的实现，可以让开发者实现低代码开发涂鸦生态的网关产品。  

SDK 包含但不限于以下的组件：  

- Zigbee 组件，必须搭配涂鸦标准网关 Zigbee 模组使用，模组作为 Zigbee 协调器的角色，零代码开发可直接接入涂鸦生态的 Zigbee 子设备。同时，也提供了接口给开发者自行接入自研的 Zigbee 子设备，并且支持与涂鸦生态 Zigbee 子设备互联互通。  
- 蓝牙组件，必须搭配涂鸦标准网关蓝牙模组使用，零代码开发可直接使用蓝牙配网，并支持接入涂鸦生态的蓝牙子设备。  
- 路由器组件，开发者可以使用路由器组件快速开发涂鸦公版的路由器产品。  
- 基础组件，开发者可以使用基础组件实现网关开发，支持接入各种协议的子设备，如 433、485、Z-Wave 等，提供工程模式、故障替换、安防、本地联动、跨网关局域网联动等功能。   

## 快速开始

[SDK for WRG2-IPEX](./WRG2-IPEX/README_zh.md)

[SDK for Ubuntu](./UBUNTU/README_zh.md)

## 获取支持

涂鸦 IoT 开发者平台：https://developer.tuya.com/cn  
涂鸦帮助中心：https://support.tuya.com/zh/help  
涂鸦技术工单平台：https://service.console.tuya.com  

## License

[MIT License](./LICENSE)
