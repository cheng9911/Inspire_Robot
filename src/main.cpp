#include "ECanVci.h"
#include <iostream>
#include <string.h>
#include <unistd.h>
#define USBCAN1 3 /* USBCAN-I */
#define USBCAN2 4 /* USBCAN-II */
#define CAN1 0
#define CAN2 1
int nDeviceType = USBCAN2;
int nDeviceInd = 0; /* 设备索引号*/
int nReserved = 0;
int nCANInd = 0;
bool m_connect = false; /*设备启动标致符false：表示设备未启动或者已经关闭 true：表示设备已经启动可以正常收发数据 */
DWORD dwRel;
typedef uint8_t Byte;
int main(int argc, char const *argv[])
{
    dwRel = OpenDevice(nDeviceType, nDeviceInd, nReserved);
    if (dwRel != 1)
    {
        std::cout << "it is error when port open" << std::endl;
        return 1;
    }
    INIT_CONFIG init_config;
    init_config.AccCode = 0;
    init_config.AccMask = 0xffffffff; // 不滤波
    init_config.Filter = 0;
    // 500k
    init_config.Timing0 = 0;
    init_config.Timing1 = 0x1c; // 500k
    init_config.Mode = 0;
    if (InitCAN(nDeviceType, nDeviceInd, CAN1, &init_config) != STATUS_OK)
    {
        std::cout << "it is error when init can" << std::endl;
        CloseDevice(nDeviceType, nDeviceInd);
        return 1;
    }
    if (StartCAN(nDeviceType, nDeviceInd, CAN1) != STATUS_OK)
    {
        std::cout << "it is error when start can" << std::endl;
        CloseDevice(nDeviceType, nDeviceInd);
        return -1;
    }

    std::cout << "success" << std::endl;

    usleep(10000);
    m_connect = true;
    CAN_OBJ frame;
    memset(&frame, 0, sizeof(frame));
    // frame.ID = 0000 0101 0111 0101 0000 0000 0000 0001
    frame.ID = 0x05750001;

    frame.DataLen = 2;
    frame.SendType = 0;
    frame.RemoteFlag = 0;
    frame.ExternFlag = 1;
    // frame.ID |= frame.ID << 11; // id: bit11~bit21 == bit0~bit10
    // frame.ID |= frame.ID << 11; // id: bit22~bit28 == bit0~bit7
    Byte data[] = {0x58, 0x02};

    // frame.Data[1] = 0x02;
    // frame.Data[0] = 0x58;

    memcpy(frame.Data, data, frame.DataLen);

    if (Transmit(nDeviceType, nDeviceInd, CAN1, &frame, 1) != STATUS_OK)
    {
        std::cout << "it is error when transmit" << std::endl;
    }
    usleep(10000);

    CAN_OBJ vco[100];
    dwRel = Receive(nDeviceType, nDeviceInd, nCANInd, vco, 100, 0); // 单次
    std::cout << vco->Data[0] << std::endl;
    usleep(1000000);
    INIT_CONFIG vic;
    CAN_STATUS vcs;
    dwRel = ReadCANStatus(nDeviceType, nDeviceInd, nCANInd, &vcs);
    if (dwRel == STATUS_OK)
    {
        std::cout << "status" << vcs.regStatus << std::endl;
    }

    CloseDevice(nDeviceType, nDeviceInd);
    usleep(2000000);

    std::cout << "00";
    return 0;
}
