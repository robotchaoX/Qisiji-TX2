#ifndef CAN_CODE_CAN_BUS_JETSON_HPP
#define CAN_CODE_CAN_BUS_JETSON_HPP

#include "key_jetson.hpp"
#include <array>

using namespace std;

// class can_jetson{
// public:
//    int can_init();
//    int can_closed();
//    int
//};

enum can_baudRate {
    baud_100k = 100000,
    baud_500k = 500000,
    baud_1M = 1000000,
};

enum canX {
    CAN_PORT_0 = 0, // can0
    CAN_PORT_1 = 1, // can1

};

// TODO
// class CAN_Device
//{
//  public:
//    can_baudRate baud_rate;
//    canX can_port;
//
//
//};

int can_init(unsigned int can_device = 0, unsigned int baudRate = 500000);
int can_closed(unsigned int can_device = 0);
void can_send(unsigned int &can_id, unsigned int &can_dlc, const array<int, 8> &a);
void can_receive(unsigned int &id, array<int, 8> &a);

#endif // CAN_CODE_CAN_BUS_JETSON_HPP

// TODO
// parse CAN frame  // 解析命令是否符合规范

// can_send函数使用提示信息
//  Usage: cansend - simple command line tool to send CAN-frames via CAN_RAW
//  sockets. Usage: cansend <device> <can_frame> <can_frame>:
//  <can_id>#{R|data}          for CAN 2.0 frames
//  <can_id>##<flags>{data}    for CAN FD frames
//  <can_id>:
//  can have 3 (SFF) or 8 (EFF) hex chars
//  {data}:
//  has 0..8 (0..64 CAN FD) ASCII hex-values (optionally separated by '.')
//  <flags>:
//  a single ASCII Hex value (0 .. F) which defines canfd_frame.flags
//  Examples:
//  5A1#11.2233.44556677.88 / 123#DEADBEEF / 5AA# / 123##1 / 213##311
//  1F334455#1122334455667788 / 123#R for remote transmission request.
