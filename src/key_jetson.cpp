//  按键
// 按键IO需下拉，检测高电平

// JetsonTX2 原装载板
// function / sysfs_gpio / Pin  / Name
//-----------------------------------------------------
//    KEY1 ->  gpio397  / Pin13 / GPIO_GEN2 -- GND
//    KEY2 ->  gpio389  / Pin33 / GPIO13    -- GND
//    KEY3 ->  gpio481  / Pin18 / GPIO_GEN5 -- GND
//    LED  ->  gpio398  / Pin29 / GPIO5     -- GND

// JetsonTX2 图为007载板
// function / sysfs_gpio / Pin  / Name
//-----------------------------------------------------
//    KEY1 ->  gpio388  / Pin7  / GPIO8         -- GND
//         ->  gpio298  / Pin8  / GPIO9         -- GND
//         //Pin8带上拉电阻无法使用？
//    KEY2 ->  gpio480  / Pin9  / GPIO_EXP0_INT -- GND
//    KEY3 ->  gpio486  / Pin10 / GPIO_EXP1_INT

#include "key_jetson.hpp"
#include "jetsonGPIO.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

extern bool detec;
extern bool key_detection;
extern bool key_height;
extern bool key_background;

// gpio引脚需定义为全局变量
jetsonTX2GPIONumber KEY1 = gpio388;
jetsonTX2GPIONumber KEY2 = gpio480;
jetsonTX2GPIONumber KEY3 = gpio486;

//// 定义引脚常变量
// const unsigned int KEY1_id = 388;
// const unsigned int KEY2_id = 480;
// const unsigned int KEY3_id = 486;

//-----------------------------------------------------------------------------
Key_GPIO::Key_GPIO() { key_Number = gpio_Number; }

Key_GPIO::~Key_GPIO() { keyClose(); }

int Key_GPIO::setKeyGPIONumber(pinGPIONumber keyGPIONumber) {
    gpio_Number = keyGPIONumber;
}

// void Key_GPIO ::delay_ms(int ms) {
//  // sleep(ms??);  // 秒s
//  // select(ms);  // 毫秒ms
//  // pselect()  // 纳秒
//  usleep(ms * 1000); // 微妙us
//  // nanosleep(ms*1000000);  // 纳秒
//}

// 初始化gkey_GPIO端口
int Key_GPIO ::keyInit(pinGPIONumber keyGPIONumber) {
    gpio_Number = keyGPIONumber;
    key_Number = gpio_Number;
    //  key_ID = ( unsigned int)keyGPIONumber;
    keyClose(); // 关闭gpio，查询gpio是否能开启，能开启则注销重启
    //  if (gpioOpen() >= 0) // 如果能打开gpio则说明仍在注册
    //  {
    //    gpioClose();
    //    gpioUnexport(); // 注销key_gpio
    //  }
    gpioExport(); // 注册gpio
    // 设置gpio方向，设为输入引脚初始化电平不确定!!!，为上一次输入电平（开机时为0）！！
    // 作为输入引脚必须上拉或下拉,上拉1k电阻小了100欧电阻不能省，此处下拉
    gpioSetDirection(inputPin); // 设置gpio方向
    //  输入引脚是不允许赋值的，默认引脚状态浮空??
    // Reverse the button wiring; this is for when the button is wired
    // with a pull up resistor
    // 可直接设置检测电平相反
    //  gpioActiveLow(true); // 设置低点平有效

    return 0;
}

int Key_GPIO ::getKeyValue() {
    int keyValue;
    int getResult;
    getResult = gpioGetValue(); // 结果写入 gpio_Value 及 返回值
    if (getResult > 1 || getResult == -1) {
        return -1;
    } else {
        keyValue = gpio_Value;
        //    keyValue = getResult;
        return keyValue;
    }
}

// 关闭key_gpio端口
int Key_GPIO ::keyClose() {
    if (gpioOpen() >= 0) // 如果能打开gpio则说明仍在注册
    {
        gpioClose();
        gpioUnexport(); // 注销key_gpio
    }
    return 0;
}

//------------------------------------------------------------------------------
Key_GPIO key1; // key 对象
Key_GPIO key2;
Key_GPIO key3;

// API 实现
//  int keyInit() {
int gpio_init() {
    key1.keyInit(KEY1);
    key2.keyInit(KEY2);
    key3.keyInit(KEY3);
}

// 关闭key_gpio，析构时会自动调用
//  int keyClose() {
int gpio_close() {
    key1.keyClose();
    key2.keyClose();
    key3.keyClose();
}

// int keyScanValue  // 中间过程
// key_mode:
//    0,不支持连续按，前一次必须为松开的状态，这次检测到低点平才有效,按键按下了没有松开只算一次
//    1,支持连续按,不管前一次的状态，只要检测到低点平就有效,按键按下了没有松开算多次
int keyScanValue(int key_mode = 0) { // 默认模式
    // 读取引脚状态
    const unsigned int KEY1_id = 1;
    const unsigned int KEY2_id = 2;
    const unsigned int KEY3_id = 3;

    unsigned int KEY1_val, KEY2_val, KEY3_val;
    KEY1_val = key1.getKeyValue();
    //  printf("KEY1_val = %d\n", KEY1_val);
    KEY2_val = key2.getKeyValue();
    //  printf("KEY2_val = %d\n", KEY2_val);
    KEY3_val = key3.getKeyValue();
    //  printf("KEY3_val = %d\n", KEY3_val);
    // 键盘扫描
    // 静态变量，只初始化一次,调用之后值仍然保留，用于记录上一次按键的状态,
    static int key_up =
        1; // 按键按松开状态标志,记录上一次按键的状态,一开始是松开的
    if (key_mode ==
        1) // 若支持连按mode=1，则每次开始键盘扫描时按键状态都设为未按下,只要捕获到按下就为有效按下
        key_up = 1; //  按键状态为未按下
    // 检测高电平，检测到高电平为按键按下
    // 读取管脚状态,是否有按键按下
    if ((key_up == 1) &&
        (KEY1_val == 1 || KEY2_val == 1 || KEY3_val == 1)) { // 有按键按下
        //    delay_ms(10);
        usleep(10000); // 延时消抖
        key_up = 0; // 记录这次按键状态为按下还没松开，按键没有按松开标志
        if (KEY1_val == 1)  // 消抖后再判断一次确实有按键按下
            return KEY1_id; // 按键按下有效
        else if (KEY2_val == 1)
            return KEY2_id;
        else if (KEY3_val == 1) // condition is always true 没问题
            return KEY3_id;
    } else if (KEY1_val == 0 && KEY2_val == 0 &&
               KEY3_val == 0) { // 按键松开了或根本没有被按下
        key_up = 1; // 按键松开，按键松开状态标志置１或者没有按下
                    //      printf("key up / free  \n");
    }
    return 0;
}

int key() {
    const unsigned int KEY1_id = 1;
    const unsigned int KEY2_id = 2;
    const unsigned int KEY3_id = 3;
    int key_num = keyScanValue(0);
    if (key_num > 0) {
        switch (key_num) {
        case KEY1_id:
            cout << " ---- KEY1 PRESS ---- \n";
            key_detection = true;
            break;
        case KEY2_id:
            cout << " ---- KEY2 PRESS ---- \n";
            key_height = true;
            break;
        case KEY3_id:
            cout << " ---- KEY3 PRESS ---- \n";
            key_background = !key_background;
            break;
        default:;
        }
    } else
        //    delay_ms(10); // 毫秒级
        usleep(10000);
    return 0;
}