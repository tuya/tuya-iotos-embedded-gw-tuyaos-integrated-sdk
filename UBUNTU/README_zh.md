## 概述

本示例演示了虚拟子设备接入涂鸦云的完整流程，给开发者接入子设备提供参考。同时，开发者也可以运行本示例快速体验。

本文档对 **tuyaos_demo_dev** 示例进行了简要说明，将会介绍如何获取开发包、编译和运行。

## 创建产品  

在涂鸦 IoT 平台创建网关产品是开发的第一步，是其他步骤的前置条件。本小节将会演示在涂鸦 IoT 平台上创建一个网关产品。

本小节我们将会在涂鸦 IoT 平台上创建一个网关产品。

1. 登录 [涂鸦 IoT 平台](https://iot.tuya.com/pmg/solution)。未注册的用户，请先注册账户。

2. 在平台首页，单击 **创建产品**。

   ![](https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/goat/20201109/3df3f0e80fbb4eb0ba5968b18d35f0ec.png)

3. 在产品品类中，单击 **网关中控** 品类，然后选择 **网关**。

   ![](https://images.tuyacn.com/fe-static/docs/img/4df07424-3c3f-411f-b7cb-c41065d04ab8.jpg)

4. 智能化方式选择 **产品开发**。  

   ![](https://images.tuyacn.com/fe-static/docs/img/a9187b07-e219-418f-bc74-aa24f4667b6e.jpg)

5. 产品方案选 **联网SDK**。

   ![](https://images.tuyacn.com/fe-static/docs/img/a8d5ac49-a802-442d-a423-a1ce3e63d802.jpg)

6. 完善产品信息，填写产品名称和产品型号后，单击 **创建产品**。

   ![](https://images.tuyacn.com/fe-static/docs/img/d53224ff-1b14-41b9-8e1b-99918de9090c.png)

7. 创建产品完成后，会弹框让我们添加标准功能点，后面步骤会介绍如何添加功能点，所以这里直接单击 **确定** 进入下一步。

   ![](https://images.tuyacn.com/fe-static/docs/img/a51cf25f-5e74-4b1a-8c0a-5211193ff16e.jpg)

8. 进入产品主页，我们将会看到涂鸦平台已经给产品分配了一个 PID，也称产品 ID。  

   ![](https://images.tuyacn.com/fe-static/docs/img/a348be85-d932-4588-a7fb-4765582d2e96.jpg)

9. 单击 **功能定义** 选项卡，在 **功能定义** 页面可以给产品添加标准功能或自定义功能，可以按需添加。关于功能点说明，请参考 [标准功能说明](https://developer.tuya.com/cn/docs/iot/standard-functions?id=K914jp5h3j6s8) 和 [自定义功能](https://developer.tuya.com/cn/docs/iot/custom-functions?id=K937y38137c64)。

   ![](https://images.tuyacn.com/fe-static/docs/img/ab3fff5a-4f95-4bc4-9c68-7e28d54f4e2f.jpg)

10. 单击 **设备面板** 选项卡，在 **设备面板** 页面选择 APP 面板，单击 **公版面板**，挑选合适的面板，单击 **使用此模板**。关于 APP 面板更多说明，请参考 [配置 App 界面](https://developer.tuya.com/cn/docs/iot/app-ui-design?id=K914jpghswnq0)。

    ![](https://images.tuyacn.com/fe-static/docs/img/80560bb4-990f-4d5b-85b5-311c24c535c5.jpg)

11. 单击 **硬件开发** 选项卡，在 **硬件开发** 页面选择云端接入硬件和获取授权码。<br>

    **云端接入硬件** 选择 **通用CPU**，接着单击 **免费领取2个激活码**，下单后即可免费领取2个激活码用来调试。<br>

    > 注意，激活码必需是一机一对，不能重复使用。免费领取的激活码仅用于调试目的，正式产品的激活码需要联系涂鸦商务购买。
    
    ![](https://images.tuyacn.com/fe-static/docs/img/f3c2d7eb-4c0d-4c50-8ef1-3590721458d2.jpg)

12. 到这里，网关产品已创建完成。

## 获取开发包 

为了简化嵌入式开发流程，屏蔽开发环境的差异，涂鸦嵌入式部门开发了一个名为 **Tuya Wind IDE** 的 VSCode 插件，SDK 获取、开发、编译以及烧录都能在 Tuya Wind IDE 上完成。

本小节简单介绍通过 Tuya Wind IDE 来获取开发包。

1. 下载并安装 Tuya Wind IDE，安装和使用说明请参考 [Tuya Wind IDE 使用说明](https://developer.tuya.com/cn/docs/iot-device-dev/Development-Tools?id=Kb0vhc1zz50dk)。

2. 使用 [涂鸦 IoT 开发平台](https:iot.tuya.com) 账号登录 IDE，登录后，在主页上单击 **创建新框架**。

   ![img](https://images.tuyacn.com/fe-static/docs/img/4139c485-beda-4664-86d2-3aee636745b2.jpg)

3. 创建框架时，会弹出项目向导。

   ![img](https://images.tuyacn.com/fe-static/docs/img/f658fefa-0d80-4a2f-ab08-f870d64c75b6.jpg)

   **名称(Name)**：工程名称，自定义字符串。<br>

   **类型(Type)**：选择 **网关开发包(EmbeddedPackage-Gateway)**。<br>

   **芯片(Chip)**：选择 “SoC -> ubuntu -> Linux -> X86_64”。<br>

   **开发框架(Package)**：选择芯片后，IDE 会列出该芯片下所有可用的开发包，选择最新的版本即可。<br>

   **位置(Location)**：可以选择默认位置，也可以选择自定义位置。<br>

   最后，单击 **完成**。  

4. 创建开发框架需要一定的时间，耐心等待完成。SDK 开发包获取成功后，会自动在 VSCode 打开工程目录。

   ![img](https://images.tuyacn.com/fe-static/docs/img/01b1ecde-803d-466d-890a-d63bb1e93788.jpg)  

## 目录结构

本示例工程位于 software -> TuyaOS -> apps 目录下，**tuyaos_demo_dev** 的目录结构如下：

```  
.
├── config.json  # 配置文件
├── doc          # 文档
├── hal          # 适配层代码实现
├── include      # 示例头文件
├── LICENSE
├── Makefile     # Makefile
├── README.md    # README
└── src          # 示例源文件
```

## 配置文件

为了让开发者快速体验，本示例工程把配置相关独立放到配置文件中，配置文件为 **config.json**，开发者只需要修改配置文件参数，就能把程序运行起来体验。

需要修改的参数是 **pid**，**uuid**，**authkey**，修改成在涂鸦 IoT 平台上创建的网关产品对应的 PID，详见 [创建产品](#创建产品)。

配置文件 **config.json** 基本格式为：

```
 {
     "pid": <xxx>,
     "uuid": <...>,
     "authkey": <...>,
     "ap_ssid": <... optional>,
     "ap_password": <... optional>,
     "log_level": <level>,
     "storage_path": <...>,
     "start_mode": <xxxx>,
     "user": {
         [ { "tp": <DEV_ATTACH_MOD_X, number>, "sw_ver": <version, string> }, ... ]
     },
 }
```

字段解析：

| 字段         | 必须 | 说明                                                         |
| ------------ | ---- | ------------------------------------------------------------ |
| PID          | 是   | 网关的产品 ID                                                |
| uuid         | 是   | 授权码                                                       |
| authkey      | 是   | 授权码                                                       |
| ap_ssid      | 否   | AP配网SSID，未指定则使用默认SmartLife-<WLAN MAC 后四位>      |
| ap_password  | 否   | AP配网密码，默认为空                                         |
| log_level    | 否   | 日志等级，默认为debug级4                                     |
| storage_path | 是   | 文件存储路径，需要可读写文件系统                             |
| start_mode   | 否   | Wi-Fi 配网模式，支持以下类型：<br/>0：WF_START_AP_ONLY：仅支持 AP 配网<br/>1：WF_START_SMART_ONLY：仅支持 EZ 配网<br/>2：WF_START_AP_FIRST：支持 AP+EZ 配网，AP 配网优先<br/>3：WF_START_SMART_FIRST：支持 AP+EZ 配网，EZ 配网优先<br/>4：WF_START_SMART_AP_CONCURRENT：AP 和 EZ 共存模式 |

## 适配

TuyaOS SDK 是跨平台的设计，定义了一套标准的 TuyaOS Kernel Layer 屏蔽硬件差异，所以要求开发者适配自己的硬件平台。

本示例提供了 Ubuntu 平台的适配代码，开发者无需进行适配就能使用，只需要调整有线网络接口。

打开 tuyaos_demo_dev -> hal -> tkl_wired -> tkl_wired.c 文件，把 **WIRED_NAME** 修改成自己系统上的网络接口。

![img](https://images.tuyacn.com/fe-static/docs/img/5254f1f0-59aa-4d1c-a2c3-b43d30ddd2d4.jpg)

## 编译

完成 [适配](#适配) 之后，接下来我们使用 VSCode 编译示例工程。

选择 **tuyaos_demo_dev**，单击右键打开菜单，单击菜单中的 **Build Project**。

![img](https://images.tuyacn.com/fe-static/docs/img/a527a5b6-5908-4821-bb18-aa7e2505cc47.jpg)

单击 **Build Project** 后，会弹出对话框让我们输入版本号，版本号的格式为 x.x.x，x 必须是数字，根据实际情况输入版本号即可。输入版本号并按回车键就会开始编译。

![img](https://images.tuyacn.com/fe-static/docs/img/967e6b7a-149d-4193-aa16-75e98c92f502.jpg)

编译过程的打印信息会显示在VSCode 的终端。如果编译过程没有报错，看到 "Build APP Finish" 打印则表示编译完成。

![img](https://images.tuyacn.com/fe-static/docs/img/252fb80a-17b1-43df-8db3-55fcba91e8ff.jpg)

## 运行

上一小节程序已经编译完成，接下来在 Ubuntu 平台上运行它，把 Ubuntu 作为网关设备接入到涂鸦平台。

编译生成的可执行程序位于 TuyaOS->output->tuyaos_demo_dev_<版本号> 目录下，版本号为编译时输入的版本号，配置文件也会自动拷贝到该目录下。

![img](https://images.tuyacn.com/fe-static/docs/img/39bbc1d5-dc46-4794-9fe6-dca9237ffe57.jpg)

进入到程序所在目录 TuyaOS->output->tuyaos_demo_dev_<版本号>，运行程序 `./tuyaos_demo_dev`。

程序运行时，会在终端显示程序打印的日志。

```  
[01-01 00:00:00 TUYA D][tuya_ws_db.c:450] init fs. Path: ./
[01-01 00:00:00 TUYA I][kv_storge.c:45] Init Kvs With File:./
[01-01 00:00:00 TUYA D][simplekv.c:928] path:./
[01-01 00:00:00 TUYA D][simplekv.c:964] get encrypt_key[⚌⚌⚌US⚌⚌⚌⚌ykR⚌⚌]
[01-01 00:00:00 TUYA D][simplekv.c:995] both file exist
[01-01 00:00:00 TUYA D][simplekv.c:293] size in: 0 and final: 28700 And mag_rec_max: 512
[01-01 00:00:00 TUYA D][simplekv.c:308] create data hd success
[01-01 00:00:00 TUYA D][simplekv.c:1038] read from normal file
[01-01 00:00:00 TUYA D][simplekv.c:748] curr db is V2. No need to upgrade
...
```

最后，打开智慧生活 APP，用 **有线配网** 来激活网关，APP 详细教程请参考 [使用指南](https://developer.tuya.com/cn/docs/iot/user-manual-for-tuya-smart-v3177?id=K9obrofrfk4sk)。

## 参考

了解更多的子设备接入开发细节，请参考子设备接入指南。

> 说明：子设备接入指南位于开发包的 software -> TuyaOS -> docs -> 接入子设备指南.pdf。
