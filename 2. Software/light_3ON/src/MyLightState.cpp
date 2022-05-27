#include "MyLightState.h"

MyLightState::MyLightState()
{
    // 储存继电器引脚
    RelayIO[Relay1] = Relay1IO;
    RelayIO[Relay2] = Relay2IO;
    RelayIO[Relay3] = Relay3IO;
    //RelayIO[Relay4] = Relay4IO;

    // 存储继电器墙壁开关接入引脚
    SwitchIO[Switch1] = Switch1IO;
    SwitchIO[Switch2] = Switch2IO;
    SwitchIO[Switch3] = Switch3IO;
    //SwitchIO[Switch4] = Switch4IO;


    this->WallSwitchState[Switch1] = 1;
    this->WallSwitchState[Switch2] = 1;
    this->WallSwitchState[Switch3] = 1;
    this->WallSwitchState[Switch4] = 1;

    // 灯默认为关闭
    this->LedState[Light1] = false;
    this->LedState[Light2] = false;
    this->LedState[Light3] = false;
    this->LedState[Light4] = false;

    this->SignalStrength = 0.0; // 信号强度
    this->SysRunTime = 0;   // 系统运行时间 单位s
    this->LastGetDataTime = 0; // 上次获取数据时间 单位ms
}

// 初始化IO脚
void MyLightState::MyLightInitIO()
{
    // 初始化IO
    pinMode(Switch1IO, INPUT);
    pinMode(Switch2IO, INPUT);
    pinMode(Switch3IO, INPUT);
    pinMode(RelayIO[Relay1], OUTPUT);
    pinMode(RelayIO[Relay2], OUTPUT);
    pinMode(RelayIO[Relay3], OUTPUT);

    // 获取墙壁开关此时的状态
    digitalWrite(Switch1IO, HIGH);
    digitalWrite(Switch2IO, HIGH);
    digitalWrite(Switch3IO, HIGH);
    this->WallSwitchState[Switch1] = digitalRead(Switch1IO);
    this->WallSwitchState[Switch2] = digitalRead(Switch2IO);
    this->WallSwitchState[Switch3] = digitalRead(Switch3IO);
    this->WallSwitchState[Switch4] = 1;

    // 灯默认为关闭
    digitalWrite(Relay1IO, OFFLight);
    digitalWrite(Relay2IO, OFFLight);
    digitalWrite(Relay3IO, OFFLight);
    this->LedState[Light0] = false;
    this->LedState[Light1] = false;
    this->LedState[Light2] = false;
    this->LedState[Light3] = false;
    this->LedState[Light4] = false;
}

// 开/关灯函数
// 参数：
//       ON_OFF   ：  开 ||  关灯
//       num      ：  哪个灯  0 1 2 3
void MyLightState::OpenCloseLight(bool ON_OFF, uint8_t num)
{
    if(ON_OFF != this->LedState[num]) // 如果灯当前的状态与需要切换的状态不一致
    {
        digitalWrite(this->RelayIO[num], ON_OFF);  // 开/关灯
        this->LedState[num] = ON_OFF;      // 切换灯的状态

        //更新首页开关状态
        if (this->LedState[0] == OFFLight && (LedState[1] == ONLight || LedState[2] == ONLight || LedState[3] == ONLight))
        {
            this->LedState[0] = ONLight;
        }
        else if (this->LedState[0] == ONLight && LedState[1] == OFFLight && LedState[2] == OFFLight && LedState[3] == OFFLight)
        {
            this->LedState[0] = OFFLight;
        }

        heartbeat();    // 更新app显示
    }
    
}


// 墙壁开关的控制函数
void MyLightState::WallSwitchControl()
{
    for (size_t num = 1; num < 4; num++)
    {
        if (digitalRead(this->SwitchIO[num]) != this->WallSwitchState[num]) //如果墙壁开关当前状态发生改变
        {
            delay(100); //延时消抖
            if (digitalRead(this->SwitchIO[num]) != this->WallSwitchState[num])
            {
                this->WallSwitchState[num] = digitalRead(this->SwitchIO[num]); //记录墙壁开关当前的状态

                this->OpenCloseLight(!(this->LedState[num]), num); // 改变灯的状态
            }
        }
    }
}
