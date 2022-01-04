## 产品概述

WRG2-IPEX 是由杭州涂鸦信息技术有限公司开发的一款网关核心板，提供给开发者用来熟悉涂鸦平台接入的开发流程，提高后续产品开发效率。同时，也能作为产品预研阶段的快速验证，并快速搭建演示 DEMO。

WRG2-IPEX 硬件上集成了蓝牙模组和 Zigbee 模组，软件上使用 RTL8197FS TuyaOS 产物包，可零代码实现网关智能化，无需任何适配可接入涂鸦生态的所有 Zigbee 子设备和蓝牙子设备。

硬件指标详见 [WRG2-IPEX 模组规格书](https://developer.tuya.com/cn/docs/iot/WRG2-IPEX-module-datasheet?id=Kalb76mhh4z7v)。

## 快速开始

### 获取 SDK

为了简化嵌入式开发流程，屏蔽开发环境的差异，涂鸦嵌入式部门开发了一个名为 **Tuya Wind IDE** 的 VSCode 插件，SDK 获取、开发、编译以及烧录都能在 Tuya Wind IDE 上完成。

本小节简单介绍通过 Tuya Wind IDE 获取 RTL8197FS TuyaOS 开发包的必要步骤。

1. 下载并安装 Tuya Wind IDE，安装和使用说明请参考 [Tuya Wind IDE 使用说明](https://developer.tuya.com/cn/docs/iot-device-dev/Development-Tools?id=Kb0vhc1zz50dk)。

2. 使用 [涂鸦 IoT 开发平台](https:iot.tuya.com) 账号登录 IDE。

   ![img](https://images.tuyacn.com/fe-static/docs/img/8e40df26-b9c4-434d-91a5-0107349a8f67.jpg)  

3. 登录后，在主页上点击 **创建新框架**。

   ![img](https://images.tuyacn.com/fe-static/docs/img/4c53da5f-1aef-4fe0-86c6-654ca8e0567a.jpg)

4. 创建框架时，会弹出项目向导。

   ![img](https://images.tuyacn.com/fe-static/docs/img/4bc438e2-bf85-4bfc-90b9-f8700519fd2b.png)

   **名称**：工程名称，自定义字符串。

   **类型**：选择 **网关开发包**。

   **芯片**：选择 “Wireless SoC -> Realtek -> Wi-Fi -> RTL8197FS”。

   **开发框架**：选择芯片后，IDE 会列出该芯片下所有可用的开发包，选择最新的版本即可。

   **位置**：可以选择默认位置，也可以选择自定义位置。

   最后，点击 **完成**。

5. 创建开发框架需要一定的时间，耐心等待完成。SDK 开发包获取成功后，会自动在 VSCode 打开工程目录。

   ![img](https://images.tuyacn.com/fe-static/docs/img/3a8dbce9-4aa4-4e1a-b2a3-bf476124f73f.jpg)  

### 目录结构

本小节对开发包重要的文件进行简单说明，以便开发者更好理解代码的组织结构。

开发包整体目录结构：

```  
.
├── chip_manual          # 芯片手册，存放涂鸦自研模组原厂芯片资料
├── module_manual        # 模组手册，存放涂鸦自研模组相关资料
├── SDKInformation.json  # SDK 信息文件
├── tools                # 工具类目录，存放各种开发相关工具
├── tuya.json
└── TuyaOS               # SDK 开发包目录
    ├── adapter          # TuyaOS kernel adapter layer 定义的标准接口头文件
    ├── apps             # 应用示例代码目录，开发者可以根据需求修改   
    ├── build_app.sh     # 编译脚本
    ├── CHANGELOG.md     # 版本信息
    ├── docs             # API 文档
    ├── include          # SDK 头文件
    ├── libs             # SDK 库文件，开发者可以按需链接
    ├── LICENSE
    ├── README.md
    ├── tools
    └── vendor           # 工具链，编译时会自动把工具链下载到该目录下
```

TuyaOS->apps->tuyaos_demo_rtl8197fs 目录结构：

```
├── LICENSE			# Tuya MIT License
├── Makefile		# 编译Makefile	
├── README.md		# README
├── build			# 网关工程需要包含的资源
├── build.sh		# 编译脚本
├── docs			# 说明文档
├── include			# 源码头文件
└── src				# 源码文件
```

TuyaOS->apps->tuyaos_demo_rtl8197fs->build 目录结构：

```
.
├── board
│   ├── audio_file					# 语音资源
│   ├── keyname.txt					# 固件名称记录，用于模组产测，修改固件key名称后需要及时修改此文件记录内容。固件名称不是项目名称
│   ├── startup.sh					# 启动脚本，系统启动后会先执行此脚本，一般不要修改
│   └── wifi_test_server.sh			# 当WiFi没有校准时，将会使用此脚本
├── image
│   ├── 8197wlan0_udhcpc.script		# 对wlan0的dhcp配置
│   ├── app_upgrade.sh				# 当OTA时会调用此脚本
│   ├── prod_upgrade.sh				# 当使用批量升级工具升级时会调用此脚本
│   ├── show_net_stat.sh			# 打印显示一些网络状态
│   ├── tuya_start.sh				# 开始运行网关工程
│   ├── tuya_user1
│   │   ├── app_detect.sh			# 监控程序是否运行
│   │   ├── app_stop.sh				# 网关程序停止
│   │   ├── config.json				# 网关信息配置
│   │   ├── debugtool				# 对ZigBee的调试工具，可以获取如ZigBee网络信息
│   │   ├── log_detect.sh			# 日志监控脚本，对日志进行归档打包
│   │   ├── process_monitor.sh		# 软喂狗监控程序
│   │   └── tuya_start_children.sh	# 主程序启动
│   ├── udhcpc.script				# 有线dhcp脚本
│   └── udhcpd.conf					# 有线dhcp配置
├── image_create.sh					# QIO和OTA打包脚本，将会调用BSP打包工具
├── ncp_image
│   └── si32_mg21_115200_pta_665_OTA_1.1.9.bin	# NCP OTA 固件
└── pack.sh							# 工程打包脚本
```

下面重要脚本中一些比较关键的配置进行详细说明。

1. *app_upgrade.sh* 和 *prod_upgrade.sh*

   | 字段                                       | 说明                                                         |
   | ------------------------------------------ | ------------------------------------------------------------ |
   | ncp_ver65_flag="/tmp/ncp_ver65_1_1_9_flag" | 应该根据NCP使用版本进行修改，和ncp_image文件夹内NCP版本匹配，比如NCP版本变更为12.34.56，则字段需要修改为*"/tmp/ncp_ver65_12_34_56_flag"*。Host会在/tmp目录下根据NCP版本自动生成*ncp_ver65_x_x_x_flag*文件，当脚本无法匹配到*ncp_ver65_flag*字段时，会执行NCP升级。 |

2. *tuya_start.sh*

   | 字段                            | 说明                   |
   | ------------------------------- | ---------------------- |
   | TY_ENV_USER_TMP_DIR="/tmp"      | 临时目录，按照实际修改 |
   | DEFULT_APP_RUN_DIR="/tmp/tuya/" | 运行目录，按照实际修改 |

3. *tuya_start_children.sh*

   | 字段                                           | 说明                                        |
   | ---------------------------------------------- | ------------------------------------------- |
   | TY_ENV_USER_LOG_DIR="/tuya/data/log_dir"       | log存贮文件夹，按照实际修改                 |
   | TY_ENV_USER_STORAGE_DIR="/tuya/data/user_data" | 用户信息存贮文件夹，按照实际修改            |
   | TY_ENV_USER_TMP_DIR="/tmp"                     | 临时目录，按照实际修改，应该和3中的保持一致 |
   | TY_ENV_USER_BIN_DIR="/tmp/tuya"                | 运行目录，按照实际修改，应该和3中的保持一致 |

4. *process_monitor.sh*

   | 字段                  | 说明                                                         |
   | --------------------- | ------------------------------------------------------------ |
   | PRO_MONITOR_MAX_NUM=6 | 软喂狗失败阈值此时，当达到此值后执行重启应用程序，每次间隔20秒 |

### 配置

在编译固件之前，我们需要修改一下配置，把 PID 修改成前面创建的网关产品 PID。

配置文件位于 TuyaOS->apps->tuyaos_demo_rtl8197fs->build->image->tuya_user1->config.json。

![img](https://images.tuyacn.com/fe-static/docs/img/20420586-1f29-4fd6-931c-34bcb8507bce.jpg)

配置文件config.json详细解析如下：

基本格式为：

```
 {
     "pid": <xxx>,
     "uuid": <...>,
     "authkey_key": <...>,
     "ap_ssid": <... optional>,
     "ap_password": <... optional>,
     "log_level": <level>,
     "storage_path": <...>,
     "start_mode": <xxxx>,
     "tuya": {
         "zigbee": {
                       "storage_path": <xxx, string>,
                       "cache_path": <xxx, string>,
                       "dev_name": <xxx, string>,
                       "cts": <xxx, number>,
                       "thread_mode": <xxx, number>
         },
         "bt": {
                   "enable_hb": <xxx, number>,
                   "scan_timeout": <xxx, number>,
                   "mode": <xxx, number>
         }
     },
     "user": {
         [ { "tp": <DEV_ATTACH_MOD_X, number>, "sw_ver": <version, string> }, ... ]
     },
 }
```

字段解析：

公共配置部分：

| 字段         | 必须 | 说明                                                         |
| ------------ | ---- | ------------------------------------------------------------ |
| PID          | 是   | SDK运行在非OEM模式，PID字段指定此设备PID，本demo的PID为：jaxsdzxvod99ueen |
| uuid         | 否   | 唯一识别码，优先从nvram读取，不存在时使用此处。关于nvram操作参考[备份授权信息](####备份授权信息) |
| authkey_key  | 否   | 授权识别码，优先从nvram读取，不存在时使用此处。关于nvram操作参考[备份授权信息](####备份授权信息) |
| ap_ssid      | 否   | AP配网SSID，未指定则使用默认SmartLife-<WLAN MAC 后四位>      |
| ap_password  | 否   | AP配网密码，默认为空                                         |
| log_level    | 否   | 日志等级，默认为debug级4                                     |
| storage_path | 是   | 文件存储路径，需要可读写文件系统                             |
| start_mode   | 否   | Wi-Fi 配网模式，支持以下类型：<br/>0：WF_START_AP_ONLY：仅支持 AP 配网<br/>1：WF_START_SMART_ONLY：仅支持 EZ 配网<br/>2：WF_START_AP_FIRST：支持 AP+EZ 配网，AP 配网优先<br/>3：WF_START_SMART_FIRST：支持 AP+EZ 配网，EZ 配网优先<br/>4：WF_START_SMART_AP_CONCURRENT：AP 和 EZ 共存模式 |

协议配置部分：

ZigBee：

| 字段         | 必须 | 说明                                                         |
| ------------ | ---- | ------------------------------------------------------------ |
| storage_path | 是   | 文件存储路径，需要可读写文件系统                             |
| cache_path   | 是   | 临时目录，用于文件缓存，如OTA文件                            |
| dev_name     | 是   | 串口名称                                                     |
| cts          | 是   | 是否开启流控，涂鸦NCP均开启了流控                            |
| thread_mode  | 是   | 1：线程模式运行zigbee host程序<br>0：进程默认运行zigbee host程序 |

蓝牙：

| 字段         | 必须 | 说明              |
| ------------ | ---- | ----------------- |
| enable_hb    | 是   | 是否使能心跳      |
| scan_timeout | 是   | 扫描超时时间      |
| mode         | 是   | slave/master/mesh |

解析部分在user_iot_init中实现，可自行参考代码。

### 编译

本小节将会介绍在 VSCode 上编译 WRG2-IPEX 固件。

1. 展开开发包的 TuyaOS -> apps 目录，选择 **tuyaos_demo_rtl8197fs** 目录，并单击右键，在菜单中选择 **Build Project**，再输入版本号按下回车键，开始编译。

   ![img](https://images.tuyacn.com/fe-static/docs/img/2334a7e6-5faf-493a-8867-236ce800f6a8.jpg)

2. 观察 VSCode 下方的 **终端**，会看到编译的输出打印。直到看到如下图所示打印时，代表编译成功，生成的固件位于 output 目录下。

   ![img](https://images.tuyacn.com/fe-static/docs/img/018686db-3785-4784-89d1-36d456c51e65.jpg)
   
   固件说明：
   
   ```  
   a) tuyaos_demo_rtl8197fs_fw_QIO_<版本号>.bin：全量固件，用于整片 FLASH 烧录，需要进入 U-boot 进行烧录，详细参见《WRG2-IPEX QIO烧录指南.pdf》。
   b) tuyaos_demo_rtl8197fs_fw_UG_<版本号>.bin：OTA 固件，用于固件升级，可以上传到涂鸦 IoT 开发平台进行远程OTA升级。
   c) tuyaos_demo_rtl8197fs_<版本号>_debug：保留符号表的主程序，用于 GDB 调试等场景。
   d) gateway.tar.gz：程序包的压缩包，用于临时调试，可以加速调试进程，详见<简易调试>章节。
   ```

### 运行

前置工作基本完成了，接下来我们运行示例程序看下效果。

我们可以把 OTA 固件烧录到 WRG2-IPEX 开发板上，详见 [OTA 烧录](#OTA 烧录)。烧录完成后，断电重启设备。

WRG2-IPEX 支持有线配网和热点配网，我们可以用涂鸦智能 APP 来激活网关，APP 详细教程请参考 [使用指南](https://developer.tuya.com/cn/docs/iot/user-manual-for-tuya-smart-v3177?id=K9obrofrfk4sk)。

### 上传OTA固件

详细步骤请参考文档[固件升级](https://developer.tuya.com/cn/docs/iot/firmware-upgrade-operation-guide?id=K93ixsft1w3to)。

## 串口登录

我们可以通过串口方式登录到 WRG2-IPEX 开发板后台，便于开发调试。

**串口默认波特率为 38400，用户名为 root，默认密码为 tygw@8197**。

:::important

若默认密码不正确，请联系联系涂鸦客户支持，提供 S/N。

:::

调试串口的位置如下图所示：

![img](https://images.tuyacn.com/fe-static/docs/img/43842900-d785-46f2-bac9-85ea704c8f1e.png)

## 简易调试

由于 OTA 固件升级需要擦除和写入 FLASH，并且需要重启系统才能生效，在开发调试过程比较消耗时间。因此，我们可以使用 **gateway.tar.gz** 将程序解压缩到运行目录，手动运行程序，这样可以方便我们进行调试，节省开发时间。

1. 电脑安装 tftp server，并将 gateway.tar.gz 放在 tftp server 的目录下。

2. 通过串口方式登录到 WRG2-IPEX 后台，详见 [串口登录](#串口登录)。

3. 在 WRG2-IPEX 后台执行压缩包下载命令：

   `cd /tmp && tftp <tftp 服务器 IP 地址> -gr gateway.tar.gz`

4. 在 WRG2-IPEX 后台执行停止程序命令：

   `/tmp/tuya/tuya_user1/app_stop.sh`

5. 在 WRG2-IPEX 后台执行压缩包解压命令：

   `cd /tmp && echo "tuya_start_dir=/tmp/tuya/tuya_user1" > /tmp/tuya/start.conf;tar -C /tmp/tuya -xvf gateway.tar.gz`

6. 在 WRG2-IPEX 后台执行启动程序命令：

   `/tmp/tuya/tuya_start.sh /tmp/tuya`

   程序启动后，可以执行 `ps` 命令观察 **tyZ3Gw** 程序是否运行。

7. 在 WRG2-IPEX 后台执行实时日志打印命令：

   `tail -f /tmp/tuya.log`

## OTA 烧录

本小节将会介绍如何在 WRG2-IPEX 开发板烧录 OTA 固件，需要通过串口登录到后台，登录方法请参见 [串口登录](#串口登录)。

1. 电脑安装 tftp server，并将 OTA 固件放在 tftp server 的目录下。

2. 通过串口登录到 WRG2-IPEX 后台，详见 [串口登录](#串口登录)。

3. 在 WRG2-IPEX 后台执行固件下载和升级命令：

   `cd /tmp && tftp <tftp 服务器 IP 地址> -g -l <固件文件名> && unpack -f <固件文件名> -m`

4. 升级完成后断电重启。

## 全量烧录

本小节将会介绍如何在 WRG2-IPEX 开发板烧录全量固件。烧录全量固件需要在串口 以下操作均在 WRG2-IPEX 后台完成，需要通过串口登录到后台，登录方法请参见 [串口登录](#串口登录)。

### 数据备份

烧录全量固件将会把整片 FLASH 擦除，然后写入 32MB QIO 固件。所以，在烧录之前，我们要把开发板上的射频信息和授权信息进行备份，待烧录完成后，把数据进行恢复。

#### 备份射频信息

执行 `flash allhw`，把打印的信息拷贝备份到电脑上，烧录完固件后要通过 `flash set <key> <value>` 命令一条条写入。

#### 备份授权信息

执行 `nvram show`，把打印的信息拷贝备份到电脑上，烧录完固件后要通过 `nvram set <key> <value>` 命令写入，写入完成后执行 `nvram commit` 写入 FLASH。

需要备份以下 key 的内容：

```  
country
AUTHKEY
slave_mac1
bsn
master_mac
UUID
```

### 进入 UBoot

默认是禁用进入UBoot 的，需要先登录到开发板的后台把运行进入 UBoot 标志打开。

执行 `nvram set persist.boot.enter on && nvram commit`，然后断电重启，在重启过程不停地按 “Esc” 键，就会进入UBoot。

![img](https://images.tuyacn.com/fe-static/docs/img/d855e4a3-1ff3-4f71-a584-fe0684d15323.png)

### 固件烧录

1. 电脑跟 WRG2-IPEX 连接到同一局域网，在 UBoot 中设置静态IP地址（跟电脑同一个网段）。

   ![img](https://images.tuyacn.com/fe-static/docs/img/46214077-782f-40fa-8e1a-b7a791703ddf.png)

2. 电脑打开 tftpd64 工具，选择 **Tftp Client**，输入上一步设置的 IP 地址，并且选择全量固件文件，点击 **Put**，把固件上传到 WRG2-IPEX 上。

   ![img](https://images.tuyacn.com/fe-static/docs/img/87e71813-5e52-43a5-a9dd-4c90248c2f8e.png)

3. 电脑和 WRG2-IPEX 网络通的情况下，WRG2-IPEX 会打印上传进度，直到看到 Success 打印表示传输完成。

   ![img](https://images.tuyacn.com/fe-static/docs/img/709539ef-4e31-4cd3-885a-d19c8a2a3ff7.jpg)

4. 执行命令 `FLW 0x0 0xA0A00000 0x02000000` 进行固件烧录，烧录前会询问是否烧录，输入”Y” 后按回车键开始烧录。

   ![img](https://images.tuyacn.com/fe-static/docs/img/67a909b4-c33f-4959-8f2f-b4d39654fb2e.jpg)

全量固件烧录比较久，请耐心登录烧录完成……，烧录完成断电重启即可。

### 数据恢复

全量固件烧录完成并重启后，最后一个步骤是把在 [数据备份](#数据备份) 章节备份的数据重新写入到 FLASH。

示例：

```  
# flash set HW_BOARD_VER 1
# flash set HW_NIC0_ADDR 00e04c8196c1
# flash set HW_NIC1_ADDR 00e04c8196c9
# flash set HW_WLAN0_WLAN_ADDR 00e04c8196c1
# flash set HW_WLAN0_WLAN_ADDR1 00e04c8196c2
# flash set HW_WLAN0_WLAN_ADDR2 00e04c8196c3
# flash set HW_WLAN0_WLAN_ADDR3 00e04c8196c4
# flash set HW_WLAN0_WLAN_ADDR4 00e04c8196c5
# flash set HW_WLAN0_WLAN_ADDR5 00e04c8196c6
# flash set HW_WLAN0_WLAN_ADDR6 00e04c8196c7
# flash set HW_WLAN0_WLAN_ADDR7 00e04c8196c8
# flash set HW_WLAN0_TX_POWER_CCK_A 11
# flash set HW_WLAN0_TX_POWER_CCK_B 11
# flash set HW_WLAN0_TX_POWER_HT40_1S_A 0
# flash set HW_WLAN0_TX_POWER_HT40_1S_B 0
# flash set HW_WLAN0_TX_POWER_DIFF_HT40_2S 0
# flash set HW_WLAN0_TX_POWER_DIFF_HT20 0
# flash set HW_WLAN0_TX_POWER_DIFF_OFDM 0
# flash set HW_WLAN0_REG_DOMAIN 1
# flash set HW_WLAN0_RF_TYPE 10
# flash set HW_WLAN0_LED_TYPE 0
# flash set HW_WLAN0_11N_XCAP 0
# flash set HW_WLAN0_11N_TSSI1 0
# flash set HW_WLAN0_11N_TSSI2 0
# flash set HW_WLAN0_11N_THER 0
# flash set HW_WLAN0_11N_TRSWITCH 0
# flash set HW_WLAN0_11N_TRSWPAPE_C9 0
# flash set HW_WLAN0_11N_TRSWPAPE_CC 0
# flash set HW_WLAN0_11N_TARGET_PWR 0
# flash set HW_WLAN0_11N_PA_TYPE 0
# flash set HW_WLAN0_11N_THER_2 0
# flash set HW_WLAN0_11N_XCAP_2 0
# flash set HW_WLAN0_11N_KFREE_ENABLE 0
# flash set HW_WLAN0_11N_RESERVED9 0
# flash set HW_WLAN0_11N_RESERVED10 0
# flash set HW_WLAN0_TX_POWER_5G_HT40_1S_A 0
# flash set HW_WLAN0_TX_POWER_5G_HT40_1S_B 0 
# flash set HW_WLAN0_TX_POWER_DIFF_5G_HT40_2S 0
# flash set HW_WLAN0_TX_POWER_DIFF_5G_HT20 0
# flash set HW_WLAN0_TX_POWER_DIFF_5G_OFDM 0
# flash set HW_WLAN0_WSC_PIN "68878924"
# flash set BLUETOOTH_HW_BT_ADDR 00e04c819600
# flash set BLUETOOTH_HW_TX_POWER_IDX 000000000000
# flash set BLUETOOTH_HW_THERMAL_VALUE 0
# flash set BLUETOOTH_HW_ANTENNA_S0 0
# flash set BLUETOOTH_HW_ANTENNA_S1 0
# flash set BLUETOOTH_HW_XTAL_CAP_VAL 0
# flash set BLUETOOTH_HW_TX_DAC_VAL 0

# nvram set country CN
# nvram set AUTHKEY h8eHMxNGVCUxzsKLCd2cmgAgFwI6OUaY
# nvram set slave_mac1 105a17f3cc2b
# nvram set bsn JDCJ30UTU00009
# nvram set master_mac 105a17f3cbf2
# nvram set UUID 1420ee8d85e3a146
# nvram commit
```

## 安全设置

作为网关，安全是十分重要的，本demo出于二次开发的方便，会开放一些限制，供用户自行设置，建议用户后续对这些安全选项进行修改。

### root密码

开发板默认密码为`tygw@8197`，建议及时修改root密码，修改指令为：`echo root:password | chpasswd -m`，将password替换为自定义密码即可。

### SSH

SSH的开启可能会遭受到黑客的远程试探和攻击，不使用时关闭SSH，修改默认端口，都可以起到一定的防范作用。

打开`build/board/startup.sh`文件，将`dropbear -p 22`注释，系统启动时就不会自动开启SSH，需要开启时执行此命令即可。通过修改`-p <端口号>`来自定义端口。

### 可执行程序

release的可执行程序应该是经过strip的，本demo中，已在`pack.sh`对可执行程序进行了strip。

### u-boot

非必要可关闭u-boot

nvram设置字段：

```
nvram set persist.boot.enter off
nvram commit
```

即可关闭。

如需打开可设置：

```
nvram set persist.boot.enter on
nvram commit
```

### 其它安全建议

安全的道路永无止境，我们同样对安全怀着一颗敬畏之心，除以上方法，在商用时建议还进行以下安全方法补充：

1. 仅能通过涂鸦云服务对设备进行OTA升级，且OTA固件进行校验。未来涂鸦会提供用户自定义加签解签方式，敬请期待；
2. 移除物理调试接口，如JTAG/SWD等。开发板已经进行了移除，如自行打板请注意此项；
3. 日志信息中不包含任何敏感数据，如UUID/AUTHKEY/密码等；
4. 无涂鸦服务之外的本地网络端口占用；
5. 无涂鸦服务之外的远程网络服务访问；
6. 所有网络交互的数据，仅限上传至/下载自设备所在或法律允许的国家/地区的服务器；
7. 无web server、telnetd等本地网络服务；
8. 固件内不包含硬编码的敏感信息；

