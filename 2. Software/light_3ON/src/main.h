#ifndef main_H
#define main_H
#include "MyLightState.h"
#include <vector>

// 设置各引脚别名
// 定义继电器输入引脚
#define Relay1IO   4
#define Relay2IO  18
#define Relay3IO  19
#define Relay4IO

// 定义墙壁开关接入引脚
#define Switch1IO 23
#define Switch2IO 10
#define Switch3IO  9
#define Switch4IO  

// 定义灯泡
#define Light1   1
#define Light2   2
#define Light3   3
#define Light4   4
// 定义墙壁开关
#define Switch1  1
#define Switch2  2
#define Switch3  3
#define Switch4  4
// 定义继电器
#define Relay1   1
#define Relay2   2
#define Relay3   3
#define Relay4   4

void heartbeat(); //用户自定义状态返回的心跳包回调函数

#endif