#ifndef KEY_JETSON_HPP
#define KEY_JETSON_HPP

#include "jetsonGPIO.hpp"

// 定义引脚
// 常变量
// const unsigned int  LED1_id 398
// const unsigned int  KEY1_id 397
// const unsigned int  KEY2_id 389
// const unsigned int  KEY3_id 481

// gpio引脚需定义为全局变量,在头文件里定义会报重复定义错误
// jetsonTX2GPIONumber LED1 = gpio398;
// jetsonTX2GPIONumber KEY1 = gpio397;
// jetsonTX2GPIONumber KEY2 = gpio389;
// jetsonTX2GPIONumber KEY3 = gpio481;

// 读取引脚状态
//#define KEY1_val bcm2835_gpio_lev(KEY1)
//#define KEY2_val bcm2835_gpio_lev(KEY2)
//#define KEY3_val bcm2835_gpio_lev(KEY3)
// int KEY1_val,KEY2_val,KEY3_val;
// gpioGetValue(KEY1, &KEY1_val);

class Key_GPIO : GPIO_Device {
public:
  unsigned int key_Number; // gpio序号的数值
  //  unsigned int key_ID; // key1 key2 key3 的123序号
  unsigned int key_Value; // gpioGetValue
  //  pinValue gpio_Value; // gpioGetValue 的结果

  Key_GPIO();
  ~Key_GPIO();
  int setKeyGPIONumber(pinGPIONumber keyGPIONumber);
  //  void delay_ms(int);
  int keyInit(pinGPIONumber keyGPIONumber);
  int getKeyValue();
  int keyClose();
};

//------------------------------------------------------------------------------
// Key_GPIO key1; // key 对象
// Key_GPIO key2;
// Key_GPIO key3;
// API
int gpio_init();
// int keyInit();
int key();        // 单次按键扫描
int gpio_close(); // 析构时会自动调用，可不用
// int keyClose(); // 析构时会自动调用，可不用

#endif // KEY_JETSON_HPP
