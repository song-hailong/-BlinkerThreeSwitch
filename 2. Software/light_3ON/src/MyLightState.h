#ifndef MyLightState_H
#define MyLightState_H
#include <Arduino.h>
#include "main.h"
#include "stdio.h"

#define ONLight  1
#define OFFLight 0 


//灯状态对象
class MyLightState
{
public:
    // 默认构造
    MyLightState();

    // 初始化IO脚
    void MyLightInitIO();

    // 开关灯函数
    void OpenCloseLight(bool ON_OFF, uint8_t LightX);
    
    // 墙壁开关的控制函数
    void WallSwitchControl();
    
    //构造函数中赋初值
    // 存储墙壁开关状态的变量
    bool WallSwitchState[5];

    // 存储灯状态的变量
    bool LedState[5];

    // 存储继电器引脚
    uint8_t RelayIO[5];

    // 存储继电器墙壁开关接入引脚
    uint8_t SwitchIO[5];

    // 信号强度
    float SignalStrength; 
    // 系统运行时间 单位s
    time_t SysRunTime; 
    // 上次获取数据时间 单位ms
    time_t LastGetDataTime;
   
}; 


#endif