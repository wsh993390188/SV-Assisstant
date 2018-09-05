# Overview
SV-Assistant is a hardware (and software) information tool.
Software is programming.

Features supported by SV-Assistant hardware modules
- CPU(Based on https://www.techpowerup.com/cpudb/)
    - using CPUID MSR
- MotherBroad(Based on DMI)
- ACPI(Based on http://www.acpi.info/spec.html)
    - Not Completed
- Storage(Based on https://github.com/hiyohiyo/CrystalDiskInfo)
    - using CrystalDiskInfo Src
- EDID(Based on Windows Regedit.exe)
- GPU(Based on https://www.techpowerup.com/gpudb/)
    - Not Completed
    - using NVAPI
    - using ADL SDK
- Memory(Based on https://www.jedec.org/)
    - Not Completed
- SUPERIO(Based on SuperIO DataSheet)
- USB(Based on https://github.com/Microsoft/Windows-driver-samples/tree/master/usb/usbview)

# 概述
SV-Assistant是一个检测硬件、软件信息的工具,如果你愿意的话还可以更改硬件信息。

- 硬件显示:
    - CPU
        - 模仿CPUZ
        - CPU数据库来源于https://www.techpowerup.com/cpudb/
    - 主板
        - 基于DMI的信息
    - ACPI 信息来源于(http://www.acpi.info/spec.html Version6.2)
        - 扫描系统空间(没做完呢)
    - 存储设备(Based on https://github.com/hiyohiyo/CrystalDiskInfo)
        - 引用CrystalDiskInfo源码 非底层操作PMIO or MMIO来读取设备信息
    - EDID
        显示器EDID的信息，基于windows注册表，如果可以会改成底层获取EDID的形式。因为注册表不显示扩展EDID的信息
    - GPU
        - 模仿GPUZ，使用NVAPI & ADLSDK进行对主流显卡信息的获取
        - GPU数据库来源于https://www.techpowerup.com/gpudb/
        - 做的不好,信息没有全面完善
    - Memory
        - 通过SMBUS获取内存SPD的信息
        - 解析内存SPD并展示（目前未解析SPD时序）
    - SUPERIO
        - 透过IO0x2E/0x2F读取SuperIo信息,主要获取其环境控制模块信息,获取CPU及主板电压温度等信息
    - USB
        - 引用Usbview的源码，略改了改，改的不咋地，未来会重写。
    - 网卡声卡
    - 未完待续(完善信息)

- 硬件更改(未以GUI形式展示) 功能等同于RW Everthing:
    - IO
        - 更改IO口数据
    - 内存
        - 更改内存数据(内核内存暂时不支持访问及修改, 以后加入访问的功能)
    - PCI
        - 读取PCIE配置空间
        - 解析B\D\F对应的设备名
        - 没有对Capability ID、Class Code、PCI Root port、PCI Device Port进行解析
        - Option Rom表头解析
    -SMBUS
        - 读写(没做呢，懒))
    - SuperIO
        - 看RW吧
    - EC
        - 读or写 基于ACPI
    - MSR
        - 读 or 写
    - SMBIOS
        - 读并解析
    - 硬盘
        - Identify & S.M.A.R.T 引用CrystalDiskInfo源码获取
        - 读写硬盘(没做呢，懒)
    - Notes
        - RW Everthing中的MP Table, Clock Gen, E820, 串口通讯都没做 个人认为没必要搞
    - 未完待续(GUI展示)

- 软件部分
    - 枚举系统服务(不能更改, 慢慢加入该功能)
    - 系统启动项(不能更改, 慢慢加入该功能)
    - 计划任务(不能更改, 慢慢加入该功能)
    - Tcp\Udp连接(不能更改,没有上下行信息, 慢慢加入该功能)
        - 仿制TcpView
    - 未完待续(完善功能))

- 系统级软件
    - 仿制PC Hunter
      - 啥都没有呢，仅仅能获取简单的信息 这个比较困难一点，以后慢慢学习并加入该功能(会花费的时间久一点)。  
    - 未完待续(学习)

# Reference
- https://github.com/skitaoka/windows-core-audio-api-samples
- https://github.com/opcm/pcm
- https://github.com/Microsoft/Windows-driver-samples
- https://github.com/hiyohiyo/CrystalDiskInfo
- https://sourceforge.net/projects/winring0/
- https://github.com/openhardwaremonitor/openhardwaremonitor
- https://github.com/muflihun/easyloggingpp