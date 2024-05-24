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
#include <vector>
#define USBCAN1 3 /* USBCAN-I */
#define USBCAN2 4 /* USBCAN-II */
#define CAN1 0
#define CAN2 1
#define DeviceInd 0
#define Reserved 0
typedef uint8_t Byte;

// 基类
class CanWriter
{
public:
    virtual int WriteCan(int address, int value, int handid) = 0;
    virtual ~CanWriter() = default;
};

class Inspire : public CanWriter
{
private:
    DWORD dwRel;
    INIT_CONFIG init_config;
    CAN_OBJ frame;

public:
    /**
     * 定义手部各个手指的静态常量
     * 这些常量用于识别和表示人类手部的五个手指：拇指、食指、中指、无名指和小指。
     * 通过分配唯一的整数值来代表每个手指，使得在程序中可以方便地引用和识别它们。
     */
    static const int THUMB_ROT = 1496; // 拇指
    static const int THUMB = 1494;     // 拇指
    static const int INDEX = 1492;     // 食指
    static const int MIDDLE = 1490;    // 中指
    static const int RING = 1488;      // 无名指
    static const int PINKY = 1486;     // 小指
    // 各个手指的速度和力的限制
    static const int MAX_SPEED = 1000;
    static const int MIN_SPEED = 0;
    static const int MAX_FORCE = 1000;
    static const int MIN_FORCE = 0;
    // 手指速度对应的寄存器数值
    static const int SPEED_THUMB_ROT = 1532;
    static const int SPEED_THUMB = 1530;
    static const int SPEED_INDEX = 1528;
    static const int SPEED_MIDDLE = 1526;
    static const int SPEED_RING = 1524;
    static const int SPEED_PINKY = 1522;
    // 手指力对应的寄存器数值
    static const int FORCE_THUMB_ROT = 1508;
    static const int FORCE_THUMB = 1506;
    static const int FORCE_INDEX = 1504;
    static const int FORCE_MIDDLE = 1502;
    static const int FORCE_RING = 1500;
    static const int FORCE_PINKY = 1498;

    Inspire(/* args */);

    ~Inspire();

    int InitCan();
    // 重载函数
    int WriteCan(int address, int value, int handid);
    std::string GetAddressId(int address);
    std::string GetHandId(int handid);
    std::string GetFlageWriteRead(int flag_write_read);
    std::string GetCanId(int Address, int HandId, int flag_write_read);
    void ConvertValueToHex(int value, Byte data[2]);
    void InspireCloseDevice();
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
        return -1;
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
    // std::cout << "canid:" << canid << std::endl;
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
    usleep(1000);
    return 0;
}
void Inspire::InspireCloseDevice()

{
    CloseDevice(USBCAN2, DeviceInd);
}

// 接受基类指针，实现多态的动作类
class InspireAction
{
public:
    InspireAction(CanWriter *canwriter) : action_canwriter(canwriter) {}
    // 编写动作函数，对应不同的动作，比如五指单独运动，五指同时运动等
    // 拇指旋转
    void ThumbRotAction(int value)
    {
        action_canwriter->WriteCan(Inspire::THUMB_ROT, value, 1);
    }
    // 拇指运动
    void ThumbAction(int value)
    {
        action_canwriter->WriteCan(Inspire::THUMB, value, 1);
    }
    // 食指运动
    void IndexAction(int value)
    {
        action_canwriter->WriteCan(Inspire::INDEX, value, 1);
    }
    // 中指运动
    void MiddleAction(int value)
    {
        action_canwriter->WriteCan(Inspire::MIDDLE, value, 1);
    }
    // 无名指运动
    void RingAction(int value)
    {
        action_canwriter->WriteCan(Inspire::RING, value, 1);
    }
    // 小指运动
    void PinkyAction(int value)
    {
        action_canwriter->WriteCan(Inspire::PINKY, value, 1);
    }
    // 五指同时运动
    int FiveFingerAction(std::vector<int> value)
    {
        if (value.size() != 6)
        {
            std::cout << "The size of value is not 6" << std::endl;
            return -1;
        }
        action_canwriter->WriteCan(Inspire::THUMB_ROT, value[0], 1);

        action_canwriter->WriteCan(Inspire::THUMB, value[0], 1);

        action_canwriter->WriteCan(Inspire::INDEX, value[1], 1);

        action_canwriter->WriteCan(Inspire::MIDDLE, value[2], 1);

        action_canwriter->WriteCan(Inspire::RING, value[3], 1);

        action_canwriter->WriteCan(Inspire::PINKY, value[4], 1);

        return 0;
    }
// 设置五指的速度
void SetFiveFingerSpeed(std::vector<int> value)
{
    if (value.size() != 6)
    {
        std::cout << "The size of value is not 6" << std::endl;
        return;
    }
    for (int i = 0; i < value.size(); i++)
    {
        if (value[i] < Inspire::MIN_SPEED || value[i] > Inspire::MAX_SPEED)
        {
            std::cout << "The value is out of range [0, 1000]" << std::endl;
            return;
        }
    }
    action_canwriter->WriteCan(Inspire::SPEED_THUMB_ROT, value[0], 1);

    action_canwriter->WriteCan(Inspire::SPEED_THUMB, value[0], 1);

    action_canwriter->WriteCan(Inspire::SPEED_INDEX, value[1], 1);

    action_canwriter->WriteCan(Inspire::SPEED_MIDDLE, value[2], 1);

    action_canwriter->WriteCan(Inspire::SPEED_RING, value[3], 1);

    action_canwriter->WriteCan(Inspire::SPEED_PINKY, value[4], 1);
}
// 设置五指的力
void SetFiveFingerForce(std::vector<int> value)
{
    if (value.size() != 6)
    {
        std::cout << "The size of value is not 6" << std::endl;
        return;
    }
    // 检查value范围是否在0-1000
    for (int i = 0; i < value.size(); i++)
    {
        if (value[i] < Inspire::MIN_FORCE || value[i] > Inspire::MAX_FORCE)
        {
            std::cout << "The value is out of range [0, 1000]" << std::endl;
            return;
        }
    }
    action_canwriter->WriteCan(Inspire::FORCE_THUMB_ROT, value[0], 1);

    action_canwriter->WriteCan(Inspire::FORCE_THUMB, value[0], 1);

    action_canwriter->WriteCan(Inspire::FORCE_INDEX, value[1], 1);

    action_canwriter->WriteCan(Inspire::FORCE_MIDDLE, value[2], 1);

    action_canwriter->WriteCan(Inspire::FORCE_RING, value[3], 1);

    action_canwriter->WriteCan(Inspire::FORCE_PINKY, value[4], 1);
}
    ~InspireAction() {}

private:
    CanWriter *action_canwriter;
};

#endif