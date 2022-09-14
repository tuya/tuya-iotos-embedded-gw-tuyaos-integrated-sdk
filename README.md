# TuyaOS Integrated Gateway SDK

[English](./README.md) | [中文](./README_zh.md)

## Overview

The gateway SDK is built on top of TuyaOS for gateway device development. It enables you to connect sub-devices that use any protocols to the [Tuya IoT Development Platform](https://iot.tuya.com/), such as Zigbee, Bluetooth Low Energy, Bluetooth mesh, 433 MHz, 868 MHz, and Z-Wave. Interconnection with the Tuya ecosystem can also be achieved.

The SDK includes a host of features and standard APIs to help you develop easily, including device pairing (over a wired connection, wireless access point (AP), or Bluetooth), upstream and downstream communication, sub-device management, local linkage, distributed-LAN gateway deployment, bulk provisioning, failover, and OTA updates.

## SDK solution

It supports connect any sub-devices to Tuya IoT Development Platform. If your gateway uses **third-party modules**, it can connect the corresponding third-party sub-devices to the Tuya IoT Development Platform. If your gateway uses **Tuya standard modules**, it can connect to sub-devices of the Tuya ecosystem.

- If your gateway uses **third-party modules**, its connection to sub-devices is implemented by you.
- If your gateway uses **Tuya standard modules**, its connection to sub-devices of the Tuya ecosystem has been implemented by the SDK.

<img src="https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/content-platform/hestia/1650957841f23cfa05109.png" style="zoom: 50%;" />

## Quick start

### Get SDK

> *Note:* The publish SDK is only available for Ubuntu, if you want to run the SDK on your hardware, please contact us. 

| SDK Packages                                                 | Wired Pairing      | Wi-Fi Pairing      | Bluetooth Pairing  | BLE&MESH Gateway with ZS3L | BLE&MESH Gateway without ZS3L |
| ------------------------------------------------------------ | ------------------ | ------------------ | ------------------ | -------------------------- | ----------------------------- |
| [ubuntu-x64_eth-gw-com](https://images.tuyacn.com/rms-static/f70721f0-33f4-11ed-9020-89eff5bc050d-1663136272783.tar.gz?tyName=tuyaos-gw-integrated_3.5.6_ubuntu-x64_eth-gw-com_0.0.1.tar.gz) | :white_check_mark: | :x:                | :x:                | :x:                        | :x:                           |
| [ubuntu-x64_eth-wifi-gw-com](https://images.tuyacn.com/rms-static/f708cfa0-33f4-11ed-acd5-2f6ea556a3fd-1663136272794.tar.gz?tyName=tuyaos-gw-integrated_3.5.6_ubuntu-x64_eth-wifi-gw-com_0.0.1.tar.gz) | :white_check_mark: | :white_check_mark: | :x:                | :x:                        | :x:                           |
| [ubuntu-x64_eth-wifi-ble-gw-bt-common](https://images.tuyacn.com/rms-static/4bf6ab00-33fe-11ed-acd5-2f6ea556a3fd-1663140280752.tar.gz?tyName=tuyaos-gw-integrated_3.5.6_ubuntu-x64_eth-wifi-ble-gw-bt-common_0.0.1.tar.gz) | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x:                        | :white_check_mark:            |
| [ubuntu-x64_eth-wifi-ble-gw-bt-silabshost](https://images.tuyacn.com/rms-static/3fc8afe0-33fe-11ed-9020-89eff5bc050d-1663140260318.tar.gz?tyName=tuyaos-gw-integrated_3.5.6_ubuntu-x64_eth-wifi-ble-gw-bt-silabshost_0.0.1.tar.gz) | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:         | :x:                           |

### Compile

1. Decompress the downloaded SDK and copy this repository's `apps` folder into its root directory.
2. Run `./build_app.sh apps/tuyaos_demo_gw tuyaos_demo_gw 1.0.0`，it will compile the demo project.
3. After compiling done, you can run the demo program on Ubuntu which is located in `output` folder.

You also can read the details for those demos to know how to use them.

- [tuyaos_demo_gw](./apps/tuyaos_demo_gw/README.md)
- [tuyaos_demo_zigbee](./apps/tuyaos_demo_zigbee/README.md)
- [tuyaos_demo_engr](./apps/tuyaos_demo_gw/README.md)
- [tuyaos_demo_blemesh](./apps/tuyaos_demo_blemesh/README.md)

### Document

The SDK API Reference Manual and Development Guide are located in the `docs` folder, please check it out.

## Technical support

You can get support from Tuya with the following methods:

+ [Tuya Developer Platform](https://developer.tuya.com/en/)
+ [Help Center](https://support.tuya.com/en/help)
+ [Service & Support](https://service.console.tuya.com)

## License

[MIT License](./LICENSE)
