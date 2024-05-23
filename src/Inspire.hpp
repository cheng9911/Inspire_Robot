#ifndef _INSPIRE_HPP_
#define _INSPIRE_HPP_
// 创建因时手运动控制的类和代码
// 本文件函数全用驼峰命名法
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <ECanVci.h>
#include <bitset>
#include <sstream>
#include <iomanip>
#define USBCAN1 3 /* USBCAN-I */
#define USBCAN2 4 /* USBCAN-II */
#define CAN1 0
#define CAN2 1
#define DeviceInd 0
#define Reserved 0
typedef uint8_t Byte;

class Inspire
{
private:
    DWORD dwRel;
    INIT_CONFIG init_config;
    CAN_OBJ frame;

public:
    Inspire(/* args */);

    ~Inspire();
    int InitCan();
    int WriteCan(int address, int value, int handid);
    std::string GetAddressId(int address);
    std::string GetHandId(int handid);
    std::string GetFlageWriteRead(int flag_write_read);
    std::string GetCanId(int Address, int HandId, int flag_write_read);
    void ConvertValueToHex(int value, Byte data[2]);
};

Inspire::Inspire(/* args */)
{
    init_config.AccCode = 0;
    init_config.AccMask = 0xffffffff; // 不滤波
    init_config.Filter = 0;
    init_config.Timing0 = 0;
    init_config.Timing1 = 0x1c; // 500k
    init_config.Mode = 0;
    memset(&frame, 0, sizeof(frame));
    frame.DataLen = 2;
    frame.SendType = 0;
    frame.RemoteFlag = 0;
    frame.ExternFlag = 1;

    
}

Inspire::~Inspire()
{
}
int Inspire::InitCan()
{
    dwRel = OpenDevice(USBCAN2, DeviceInd, Reserved);
    if (dwRel != 1)
    {
        std::cout << "it is not OpenDevice" << std::endl;
        return -1;
    }
    if (InitCAN(USBCAN2, DeviceInd, CAN1, &init_config) != STATUS_OK)
    {
        std::cout << "it is error when init can" << std::endl;
        CloseDevice(USBCAN2, DeviceInd);
        return -1;
    }
    if (StartCAN(USBCAN2, DeviceInd, CAN1) != STATUS_OK)
    {
        std::cout << "it is error when start can" << std::endl;
        CloseDevice(USBCAN2, DeviceInd);
        return -1 ;
    }
    usleep(10000);
    std::cout << "can init success" << std::endl;
    return 0;
}



std::string Inspire::GetAddressId(int address)
{
    // 检查 address 是否在范围内
    if (address >= 1486 && address < 1534)
    {
        // 将 address 转换为二进制字符串
        std::bitset<12> binary(address); // 使用 32 位存储二进制表示
        std::string binaryString = binary.to_string();

        return binaryString; // 返回 1 表示 address 在范围内
    }
    else
    {
        std::cout << "Address " << address << " is out of the range [1486, 1534)" << std::endl;
        std::string binaryString = "";
        return binaryString; // 返回 0 表示 address 不在范围内
    }
}
std::string Inspire::GetHandId(int handid)
{
    // 检查 handid 是否在范围内
    if (handid >= 1 && handid < 16383)
    {
        // 将 handid 转换为二进制字符串
        std::bitset<14> binary(handid); // 使用 32 位存储二进制表示
        std::string binaryString = binary.to_string();

        return binaryString; // 返回 1 表示 handid 在范围内
    }
    else
    {
        std::cout << "Hand " << handid << " is out of the range [1, 7)" << std::endl;
        std::string binaryString = "";
        return binaryString; // 返回 0 表示 handid 不在范围内
    }
}
std::string Inspire::GetFlageWriteRead(int flag_write_read)
{
    // 检查 flag_write_read 是否在范围内
    if (flag_write_read >= 0 && flag_write_read < 8)
    {
        // 将 flag_write_read 转换为二进制字符串
        std::bitset<3> binary(flag_write_read); // 使用 32 位存储二进制表示
        std::string binaryString = binary.to_string();

        return binaryString; // 返回 1 表示 flag_write_read 在范围内
    }
    else
    {
        std::cout << "flag_write_read " << flag_write_read << " is out of the range [0, 8)" << std::endl;
        std::string binaryString = "";
        return binaryString; // 返回 0 表示 flag_write_read 不在范围内
    }
}
std::string Inspire::GetCanId(int Address, int handid, int flag)
{
    std::string AddressId = GetAddressId(Address);
    std::string HandId = GetHandId(handid);
    std::string flag_write_read = GetFlageWriteRead(flag);

    // 检查 AddressId 和 HandId 是否为空
    if (AddressId.size() == 0 || HandId.size() == 0 || flag_write_read.size() == 0)
    {
        std::cout << "AddressId or HandId or flag_write_read is empty" << std::endl;
        return "";
    }
    // 检查 AddressId 和 HandId 是否为 12 位和 14 位
    if (AddressId.size() != 12 || HandId.size() != 14 || flag_write_read.size() != 3)
    {
        std::cout << "AddressId or HandId or flag_write_read is not 12 or 14 or 3 bits " << std::endl;
        return "";
    }

    std::string tmp = "000";
    // 拼接 AddressId 和 HandId
    std::string CanId = tmp + flag_write_read + AddressId + HandId;
    return CanId;
}
void Inspire::ConvertValueToHex(int value, Byte data[2])
{
    // 检查 value 是否在范围内
    if (value < 0 || value > 1000)
    {
        std::cerr << "Error: Value out of range [0, 1000]" << std::endl;
        return;
    }

    // 将 value 转换为十六进制并存储到 data 数组中
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << value;
    std::string hexString = ss.str();

    // 检查是否需要补充 0，确保有 4 位
    if (hexString.length() < 4)
    {
        hexString = std::string(4 - hexString.length(), '0') + hexString;
    }

    // 交换高低字节
    data[0] = std::stoi(hexString.substr(2, 2), nullptr, 16); // 高字节
    data[1] = std::stoi(hexString.substr(0, 2), nullptr, 16); // 低字节
}
int Inspire::WriteCan(int address, int value, int handid)
{
    std::string canid = GetCanId(address, handid, 1);
    if (canid.size() != 32)
    {
        std::cout << "canid is error" << std::endl;
        return -1;
    }
    std::cout << "canid:" << canid << std::endl;
    unsigned int Canid = std::bitset<32>(canid).to_ulong();

   
    // frame.ID = 0000 0101 0111 0101 0000 0000 0000 0001

    Byte data[2];
    ConvertValueToHex(value, data);
    frame.ID = Canid;

    memcpy(frame.Data, data, frame.DataLen);
    if (Transmit(USBCAN2, DeviceInd, CAN1, &frame, 1) != STATUS_OK)
    {
        std::cout << "it is error when transmit" << std::endl;
        CloseDevice(USBCAN2, DeviceInd);
        return -1;
    }
    usleep(1);
    return 0;

}

#endif