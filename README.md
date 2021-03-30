# Hardware Information library

仅用于windows平台下硬件信息库

# 依赖库

- 必需库

    [Jsoncpp](https://github.com/open-source-parsers/jsoncpp)

    [TinyXML](https://github.com/leethomason/tinyxml2)

    [liblog](http://gitlab.xpaas.lenovo.com./cloudservice/windows/liblog)

    [utf-8](https://github.com/nemtrif/utfcpp)

    [nvapi](https://developer.nvidia.com/nvapi)

    [adlsdk](https://github.com/GPUOpen-LibrariesAndSDKs/display-library)

- 可选库

    - 单元测试库

        [gtest](https://github.com/google/googletest)

    - GUI库

        [GacUI](https://github.com/vczh-libraries/GacUI)

    - 管家Ring0库

        [Coolite(可以用自带未签名Ring0库)](http://gitlab.xpaas.lenovo.com./pcm/pcmanager/tree/master/plugin/SystemThermalPlugin/WinRing0/source)

- 使用源码的库

    [CrystalDisk](https://github.com/hiyohiyo/CrystalDiskInfo)

# How to get started

Clone this repository

    git clone http://gitlab.xpaas.lenovo.com/cloudservice/windows/hardwaremaster.git

After cloning this repository, make sure the submodules are up to date , download and unzip [nvapi](https://developer.nvidia.com/nvapi)

    git submodule update --init --recursive
    unzip nvapi.zip ./Libcommon/NVAPI

Build solution

    cd build

# 结构
- build 解决方案目录
- doc 说明文档
- Tools 用于生成UI界面的工具
- src 源码目录
    - libSmbios 基于smbios3.2标准的smbios静态库
    - SmbiosUi 简易展示Smbios信息的App
    - WSHardwarePluginV2 硬件信息的静态库
        - Sys Ring0库，相比管家增加了Trace信息，可根据PDB文件进行追踪，详见MSDN
    - HardwareUI 简易展示硬件信息的App
- uniontest 单元测试目录
    - SmbiosUnionTest libSmbios的单元测试库