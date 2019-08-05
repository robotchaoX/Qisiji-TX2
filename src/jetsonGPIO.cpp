#include "jetsonGPIO.hpp"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

GPIO_Device::GPIO_Device() {

  //  gpio_Number = 0;
  //  unsigned int gpio_ID;
  gpio_Direction = inputPin;
  //  gpio_Value = 0; // gpioGetValue
}
GPIO_Device::~GPIO_Device() {
  if (gpioOpen() >= 0) // 如果能打开gpio则说明仍在注册
  {
    gpioClose();
    gpioUnexport(); // 注销gpio
  }
}

// 设置gpio序号
int GPIO_Device::gpioSetGPIONumber(pinGPIONumber gpioX) { gpio_Number = gpioX; }

// 注册gpio
// gpioExport
// Export the given gpio to userspace;
// Return: Success = 0 ; otherwise open file error
int GPIO_Device::gpioExport() {
  int length;
  gpio_FileDescriptor = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
  if (gpio_FileDescriptor < 0) {
    char errorBuffer[128];
    snprintf(errorBuffer, sizeof(errorBuffer),
             "gpioExport unable to open gpio%d", gpio_Number);
    perror(errorBuffer);
    return gpio_FileDescriptor;
  }

  length = snprintf(gpio_CommandBuffer, sizeof(gpio_CommandBuffer), "%d",
                    gpio_Number);
  if (write(gpio_FileDescriptor, gpio_CommandBuffer, length) != length) {
    perror("gpioExport");
    return gpio_FileDescriptor;
  }
  close(gpio_FileDescriptor);
  return 0;
}

// 注销gpiio
// gpioUnexport
// Unexport the given gpio from userspace
// Return: Success = 0 ; otherwise open file error
int GPIO_Device::gpioUnexport() {
  int length;
  gpio_FileDescriptor = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
  if (gpio_FileDescriptor < 0) {
    char errorBuffer[128];
    snprintf(errorBuffer, sizeof(errorBuffer),
             "gpioUnexport unable to open gpio%d", gpio_Number);
    perror(errorBuffer);
    return gpio_FileDescriptor;
  }
  length = snprintf(gpio_CommandBuffer, sizeof(gpio_CommandBuffer), "%d",
                    gpio_Number);
  if (write(gpio_FileDescriptor, gpio_CommandBuffer, length) != length) {
    perror("gpioUnexport");
    return gpio_FileDescriptor;
  }
  close(gpio_FileDescriptor);
  return 0;
}

// 设置gpio方向，输入还是输出
// gpioSetDirection（inputPin:输入，outputPin:输出）
// Set the direction of the GPIO pin
// Return: Success = 0 ; otherwise open file error
int GPIO_Device::gpioSetDirection(pinDirection direction) {
  gpio_Direction = direction;
  snprintf(gpio_CommandBuffer, sizeof(gpio_CommandBuffer),
           SYSFS_GPIO_DIR "/gpio%d/direction", gpio_Number);
  gpio_FileDescriptor = open(gpio_CommandBuffer, O_WRONLY);
  if (gpio_FileDescriptor < 0) {
    char errorBuffer[128];
    snprintf(errorBuffer, sizeof(errorBuffer),
             "gpioSetDirection unable to open gpio%d", gpio_Number);
    perror(errorBuffer);
    return gpio_FileDescriptor;
  }
  if (direction) {
    if (write(gpio_FileDescriptor, "out", 4) != 4) {
      perror("gpioSetDirection");
      return gpio_FileDescriptor;
    }
  } else {
    if (write(gpio_FileDescriptor, "in", 3) != 3) {
      perror("gpioSetDirection");
      return gpio_FileDescriptor;
    }
  }
  close(gpio_FileDescriptor);
  return 0;
}

// 查询gpio方向
int GPIO_Device::gpioGetDirection() { return gpio_Direction; }

// 设置gpio引脚电平
// gpioSetValue
// Set the value of the GPIO pin to 1 or 0
// Return: Success = 0 ; otherwise open file error
int GPIO_Device::gpioSetValue(unsigned int setValue) {
  snprintf(gpio_CommandBuffer, sizeof(gpio_CommandBuffer),
           SYSFS_GPIO_DIR "/gpio%d/value", gpio_Number);
  gpio_FileDescriptor = open(gpio_CommandBuffer, O_WRONLY);
  if (gpio_FileDescriptor < 0) {
    char errorBuffer[128];
    snprintf(errorBuffer, sizeof(errorBuffer),
             "gpioSetValue unable to open gpio%d", gpio_Number);
    perror(errorBuffer);
    return gpio_FileDescriptor;
  }
  if (setValue) {
    if (write(gpio_FileDescriptor, "1", 2) != 2) {
      perror("gpioSetValue");
      return gpio_FileDescriptor;
    }
  } else {
    if (write(gpio_FileDescriptor, "0", 2) != 2) {
      perror("gpioSetValue");
      return gpio_FileDescriptor;
    }
  }
  close(gpio_FileDescriptor);
  return 0;
}

// 读取gpio值
// gpioGetValue, 结果写入 gpio_Value
// Get the value of the requested GPIO pin ; value return is 0 or 1
// Return: Success = 0 ; otherwise open file error
int GPIO_Device::gpioGetValue() {
  unsigned int gpioValue;
  char ch;
  snprintf(gpio_CommandBuffer, sizeof(gpio_CommandBuffer),
           SYSFS_GPIO_DIR "/gpio%d/value", gpio_Number);
  gpio_FileDescriptor = open(gpio_CommandBuffer, O_RDONLY);
  if (gpio_FileDescriptor < 0) {
    char errorBuffer[128];
    snprintf(errorBuffer, sizeof(errorBuffer),
             "gpioGetValue unable to open gpio%d", gpio_Number);
    perror(errorBuffer);
    //    return gpio_FileDescriptor;
    return -1;
  }
  if (read(gpio_FileDescriptor, &ch, 1) != 1) {
    perror("gpioGetValue");
    //    return gpio_FileDescriptor;
    return -1;
  }
  if (ch != '0') {
    gpio_Value = 1;
  } else {
    gpio_Value = 0;
  }
  gpioValue = gpio_Value;
  close(gpio_FileDescriptor);
  return gpioValue;
}

// 设置中断的触发方式，
//           none表示引脚为输入，不是中断引脚
//           rising表示引脚为中断输入，上升沿触发
//           falling表示引脚为中断输入，下降沿触发
//           both表示引脚为中断输入，边沿触发
// gpioSetEdge
// Set the edge of the GPIO pin
// Valid edges: 'none' 'rising' 'falling' 'both'
// Return: Success = 0 ; otherwise open file error
int GPIO_Device::gpioSetEdge(char *edge) {

  snprintf(gpio_CommandBuffer, sizeof(gpio_CommandBuffer),
           SYSFS_GPIO_DIR "/gpio%d/edge", gpio_Number);

  gpio_FileDescriptor = open(gpio_CommandBuffer, O_WRONLY);
  if (gpio_FileDescriptor < 0) {
    char errorBuffer[128];
    snprintf(errorBuffer, sizeof(errorBuffer),
             "gpioSetEdge unable to open gpio%d", gpio_Number);
    perror(errorBuffer);
    return gpio_FileDescriptor;
  }
  if (write(gpio_FileDescriptor, edge, strlen(edge) + 1) !=
      ((int)(strlen(edge) + 1))) {
    perror("gpioSetEdge");
    return gpio_FileDescriptor;
  }
  close(gpio_FileDescriptor);
  return 0;
}

// 中断监听??

// 打开gpio口
// gpioOpen
// Open the given pin for reading
// Returns the file descriptor of the named pin
int GPIO_Device::gpioOpen() {
  snprintf(gpio_CommandBuffer, sizeof(gpio_CommandBuffer),
           SYSFS_GPIO_DIR "/gpio%d/value", gpio_Number);
  gpio_FileDescriptor = open(gpio_CommandBuffer, O_RDONLY | O_NONBLOCK);
  if (gpio_FileDescriptor < 0) {
    char errorBuffer[128];
    snprintf(errorBuffer, sizeof(errorBuffer), "gpioOpen unable to open gpio%d",
             gpio_Number);
    perror(errorBuffer);
  }
  return gpio_FileDescriptor;
}

// 关闭gpio口
// gpioClose
// Close the given file descriptor
int GPIO_Device::gpioClose() { return close(gpio_FileDescriptor); }

// 低点平有效
// 当值为“1”时，“1”表示低电平，“0”表示高电平
// gpioActiveLow
// Set the active_low attribute of the GPIO pin to 1 or 0
// Return: Success = 0 ; otherwise open file error
int GPIO_Device::gpioActiveLow(bool activeLowValue) {
  snprintf(gpio_CommandBuffer, sizeof(gpio_CommandBuffer),
           SYSFS_GPIO_DIR "/gpio%d/active_low", gpio_Number);
  gpio_FileDescriptor = open(gpio_CommandBuffer, O_WRONLY);
  if (gpio_FileDescriptor < 0) {
    char errorBuffer[128];
    snprintf(errorBuffer, sizeof(errorBuffer),
             "gpioActiveLow unable to open gpio%d", gpio_Number);
    perror(errorBuffer);
    return gpio_FileDescriptor;
  }
  if (activeLowValue) {
    if (write(gpio_FileDescriptor, "1", 2) != 2) {
      perror("gpioActiveLow");
      return gpio_FileDescriptor;
    }
  } else {
    if (write(gpio_FileDescriptor, "0", 2) != 2) {
      perror("gpioActiveLow");
      return gpio_FileDescriptor;
    }
  }
  close(gpio_FileDescriptor);
  return 0;
}
