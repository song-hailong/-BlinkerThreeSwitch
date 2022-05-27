#define BLINKER_WIFI
//支持小爱同学
//同步设备后可显示一个插座和四个插孔(名为插孔的插座设备)
#define BLINKER_MIOT_MULTI_OUTLET
#define BLINKER_OTA_VERSION_CODE "2.0.0" //版本号

#include <Arduino.h>
#include <Blinker.h>
#include "main.h"

char auth[] = "xxxxxx"; //app中获取到的Secret Key(密钥)
char ssid[] = "xxxxxx"; //WIFI名称
char pswd[] = "xxxxxx"; //WIFI密码

MyLightState LightState; //灯的状态

BlinkerButton ButtonLight1((char*)"btn-light1");         // 按键组件：开/关灯
BlinkerButton ButtonLight2((char*)"btn-light2");         // 按键组件：开/关灯
BlinkerButton ButtonLight3((char*)"btn-light3");         // 按键组件：开/关灯
BlinkerText TextBlinkerTime((char*)"tex-BlinkerTime");   // 文字组件：显示系统运行时间
BlinkerNumber NumberSignal((char*)"num-signal");         // 数据组件：显示WiFi信号强度

// 首页开关的回调函数, 当收到指令时会调用该回调函数
void switch_callback(const String & state)
{
    BLINKER_LOG("get switch state: ", state);

    if (state == BLINKER_CMD_ON)
    {
        LightState.OpenCloseLight(ONLight, Light1); // 开灯
        LightState.OpenCloseLight(ONLight, Light2); // 开灯
        LightState.OpenCloseLight(ONLight, Light3); // 开灯
        // LightState.LedState[0] = ONLight;
    }
    else
    {
        LightState.OpenCloseLight(OFFLight, Light1); // 关灯
        LightState.OpenCloseLight(OFFLight, Light2); // 关灯
        LightState.OpenCloseLight(OFFLight, Light3); // 关灯
        // LightState.LedState[0] = OFFLight;
    }
}

// 开关灯按键回调函数，当收到指令时会调用该回调函数
void BUTTONLight_callback1(const String & state)
{
    BLINKER_LOG("get button state: ", state);

    if (state == BLINKER_CMD_ON)
    {
        LightState.OpenCloseLight(ONLight, Light1); // 开灯
    }
    else if (state == BLINKER_CMD_OFF) {
        LightState.OpenCloseLight(OFFLight, Light1); // 关灯
    }
    Blinker.vibrate();//发送手机振动指令
}
void BUTTONLight_callback2(const String & state)
{
    BLINKER_LOG("get button state: ", state);

    if (state == BLINKER_CMD_ON)
    {
        LightState.OpenCloseLight(ONLight, Light2); // 开灯
    }
    else if (state == BLINKER_CMD_OFF) {
        LightState.OpenCloseLight(OFFLight, Light2); // 关灯
    }
    Blinker.vibrate();//发送手机振动指令
}
void BUTTONLight_callback3(const String & state)
{
    BLINKER_LOG("get button state: ", state);

    if (state == BLINKER_CMD_ON) {
        LightState.OpenCloseLight(ONLight, Light3); // 开灯
    }
    else if (state == BLINKER_CMD_OFF) {
        LightState.OpenCloseLight(OFFLight, Light3); // 关灯
    }
    Blinker.vibrate();//发送手机振动指令
}

//小爱电源类操作的回调函数:
//当小爱同学向设备发起控制, 设备端需要有对应控制处理函数
void miotPowerState(const String & state, uint8_t num)
{
    BLINKER_LOG("need set outlet: ", num, ", power state: ", state);

    if (state == BLINKER_CMD_ON) {
        BlinkerMIOT.powerState("on", num); //给小爱同学反馈控制状态：已打开
        BlinkerMIOT.print();

        if(num == 4)
            return;
        else if(num == 0) {
            LightState.OpenCloseLight(ONLight, Light1); // 开灯
            LightState.OpenCloseLight(ONLight, Light2); // 开灯
            LightState.OpenCloseLight(ONLight, Light3); // 开灯
            // LightState.LedState[num] = ONLight;
        }
        else {
            LightState.OpenCloseLight(ONLight, num); // 开灯
        }
    }
    else if (state == BLINKER_CMD_OFF) {
        BlinkerMIOT.powerState("off", num); //给小爱同学反馈控制状态：已关闭
        BlinkerMIOT.print();

        if(num == 4)
            return;
        else if(num == 0) {
            LightState.OpenCloseLight(OFFLight, Light1); // 关灯
            LightState.OpenCloseLight(OFFLight, Light2); // 关灯
            LightState.OpenCloseLight(OFFLight, Light3); // 关灯
            // LightState.LedState[num] = OFFLight;
        }
        else {
            LightState.OpenCloseLight(OFFLight, num); // 关灯
        }
    }
}



// 用户自定义设备查询的回调函数:
void miotQuery(int32_t queryCode, uint8_t num)
{
  BLINKER_LOG("MIOT Query outlet: ", num,", codes: ", queryCode);

  switch (queryCode)
  {
      case BLINKER_CMD_QUERY_ALL_NUMBER :
          BLINKER_LOG("MIOT Query All");
          BlinkerMIOT.powerState(LightState.LedState[num] ? "on" : "off", num);
          BlinkerMIOT.print();
          break;
      case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
          BLINKER_LOG("MIOT Query Power State");
          BlinkerMIOT.powerState(LightState.LedState[num] ? "on" : "off", num);
          BlinkerMIOT.print();
          break;
      default :
          BlinkerMIOT.powerState(LightState.LedState[num] ? "on" : "off", num);
          BlinkerMIOT.print();
          break;
    }
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    Blinker.vibrate();  //发送手机振动指令
}

// 用户自定义状态返回的心跳包回调函数
void heartbeat()
{
    BUILTIN_SWITCH.print(LightState.LedState[Light0] ? "on" : "off"); // 开/关组件

    ButtonLight1.icon(LightState.LedState[Light1] ? "far fa-lightbulb-on" : "far fa-lightbulb"); //设置图标
    ButtonLight1.color(LightState.LedState[Light1] ? "Blue" : "Black");  //设置颜色
    ButtonLight1.print(LightState.LedState[Light1] ? "on" : "off"); // 按键组件

    ButtonLight2.icon(LightState.LedState[Light2] ? "far fa-lightbulb-on" : "far fa-lightbulb"); //设置图标
    ButtonLight2.color(LightState.LedState[Light2] ? "Blue" : "Black");  //设置颜色
    ButtonLight2.print(LightState.LedState[Light2] ? "on" : "off"); // 按键组件

    ButtonLight3.icon(LightState.LedState[Light3] ? "far fa-lightbulb-on" : "far fa-lightbulb"); //设置图标
    ButtonLight3.color(LightState.LedState[Light3] ? "Blue" : "Black");  //设置颜色
    ButtonLight3.print(LightState.LedState[Light3] ? "on" : "off"); // 按键组件
        
    char SprintfChar[30];
    sprintf(SprintfChar, "%ld天%ld时%ld分", 
           (LightState.SysRunTime)/3600L/24L,(LightState.SysRunTime)/3600L%24L,(LightState.SysRunTime)/60L%60L);
    TextBlinkerTime.icon("far fa-shipping-timed");
    TextBlinkerTime.print(SprintfChar/*,"系统运行时间"*/);

    NumberSignal.icon("fad fa-wifi");
    NumberSignal.color("Green");  //LimeGreen
    //NumberSignal.unit("Dbm");
    NumberSignal.print(LightState.SignalStrength);//信号强度

    BLINKER_LOG("heartbeat!");
}

void setup() {
    // put your setup code here, to run once:
    // 初始化串口，并开启调试信息，调试用可以删除
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_LOG("ok!");


    LightState.MyLightInitIO();

    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead); //注册回调函数，当有设备收到APP发来的数据时会调用对应的回调函数
    Blinker.attachHeartbeat(heartbeat);//注册心跳包回调函数
    BUILTIN_SWITCH.attach(switch_callback); //设置首页开关的回调函数, 当收到指令时会调用该回调函数
    //注册开关灯按键的回调函数, 当收到指令时会调用该回调函数
    ButtonLight1.attach(BUTTONLight_callback1);
    ButtonLight2.attach(BUTTONLight_callback2);
    ButtonLight3.attach(BUTTONLight_callback3);

    //给APP发送当前的状态
    heartbeat();

    //小爱同学务必在回调函数中反馈该控制状态
    BlinkerMIOT.attachPowerState(miotPowerState);//注册小爱电源类的操作接口回调函数
    BlinkerMIOT.attachQuery(miotQuery); //注册小爱设备查询接口回调函数
}

bool state = 0;
void loop() {
    // put your main code here, to run repeatedly:
    Blinker.run();

    //墙壁开关的控制
    LightState.WallSwitchControl();
    if(millis() - LightState.LastGetDataTime >= 1000) //距上次获取数据已经过去0.5s
    {
        LightState.LastGetDataTime = millis();   // 更新获取数据时间，单位为毫秒（ms）
        LightState.SysRunTime = LightState.LastGetDataTime/1000;  // 获取设备运行时间, 单位为秒(s)
        LightState.SignalStrength = WiFi.RSSI(); // WIFI信号强度
    }
}
