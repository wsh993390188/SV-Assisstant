# Hardware Information library

Hardware Information dynamic library(only windows platform)

# open source library

- Config library

    [Jsoncpp](https://github.com/open-source-parsers/jsoncpp)

    [TinyXML](https://github.com/leethomason/tinyxml2)

    [liblog](http://gitlab.xpaas.lenovo.com./cloudservice/windows/liblog)

    [utf-8](https://github.com/nemtrif/utfcpp)

- GPU library

    [nvapi](https://developer.nvidia.com/nvapi)

    [adlsdk](https://github.com/GPUOpen-LibrariesAndSDKs/display-library)

- union test library

    [gtest](https://github.com/google/googletest)

- GUI library

    [GacUI](https://github.com/vczh-libraries/GacUI)

- Ring0 library()

    [WinRing0](https://github.com/QCute/WinRing0)

- Modify library

    [CrystalDisk(modify)](https://github.com/hiyohiyo/CrystalDiskInfo)

# How to get started

Clone this repository

    git clone http://gitlab.xpaas.lenovo.com/cloudservice/windows/hardwaremaster.git

After cloning this repository, make sure the submodules are up to date , download and unzip [nvapi](https://developer.nvidia.com/nvapi)

    git submodule update --init --recursive
    unzip nvapi.zip ./Libcommon/NVAPI

Build solution

    cd build

# Directory
- build Solution Directory
- doc Document Directory
- Tools Generate GacUI Tools
- src Source Directory
    - libSmbios SMBIOS Library(based on [version 3.2](https://www.dmtf.org/standards/smbios))
    - SmbiosUi SMBIOS Apps
    - WSHardwarePluginV2 Hardware dynamic library
        - Sys Ring0 library(because of ms Signature, using WinRing0 to replace it)
    - HardwareUI Hardware Apps
- uniontest Union Test Directory
    - SmbiosUnionTest libSmbios union test Directory
