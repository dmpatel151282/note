
Documentation/i2c/writing-clients 

i2c 设备：
    RTC、音视频采集芯片、音视频输出芯片、EEROM芯片、gsensor、AD/DA转换芯片等

    --------------------
    |   字符设备        |
    ---------------------
    |                   |
    |  i2c-driver       |                   driver 层
    ---------------------
    | i2c-core          |  (i2c-core.c)
    |                   |                   kernel 层 (i2c总线驱动)
    | i2c_algorithm     |  (algos/) 
    ---------------------
    |                   |
    | i2c-sunxi         |  (busses/)        soc 层
    |                   |
    ---------------------
    |  i2c适配器(多个)  |
    |                   |  hardware
    |   各种设备(多个)  |
    ---------------------


