
#ifndef _INSPIRE_HPP_
#define _INSPIRE_HPP_
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
    // virtual void ConvertValueToHex(int value, Byte data[2]);
    virtual ~CanWriter() = default;
};
/**
 * \class Inspire
 * \brief 控制因时手运动的类，实现了基于CAN总线的通信
 *
 * 此类继承自CanWriter，提供了初始化CAN通信、发送数据以及对手指运动的控制功能。
 */
class Inspire : public CanWriter
{
private:
    DWORD dwRel;             /// 返回值
    INIT_CONFIG init_config; /// CAN初始化配置
    CAN_OBJ frame;           /// CAN帧对象

public:
    /**
     * 定义手部各个手指的静态常量
     * 这些常量用于识别和表示人类手部的五个手指：拇指、食指、中指、无名指和小指。
     * 通过分配唯一的整数值来代表每个手指，使得在程序中可以方便地引用和识别它们。
     */
    static const int THUMB_ROT = 1496; /// 拇指旋转
    static const int THUMB = 1494;     /// 拇指
    static const int INDEX = 1492;     /// 食指
    static const int MIDDLE = 1490;    /// 中指
    static const int RING = 1488;      /// 无名指
    static const int PINKY = 1486;     /// 小指

    static const int MAX_SPEED = 1000; /// 各个手指的最大速度限制
    static const int MIN_SPEED = 0;    /// 各个手指的最小速度限制
    static const int MAX_FORCE = 1000; /// 各个手指的最大力限制
    static const int MIN_FORCE = 0;    /// 各个手指的最小力限制
    // 手指速度对应的寄存器数值
    static const int SPEED_THUMB_ROT = 1532; /// 拇指旋转的速度对应的寄存器数值
    static const int SPEED_THUMB = 1530;     /// 拇指的速度对应的寄存器数值
    static const int SPEED_INDEX = 1528;     /// 食指的速度对应的寄存器数值
    static const int SPEED_MIDDLE = 1526;    /// 中指的速度对应的寄存器数值
    static const int SPEED_RING = 1524;      /// 无名指的速度对应的寄存器数值
    static const int SPEED_PINKY = 1522;     /// 小指的速度对应的寄存器数值
    // 手指力对应的寄存器数值
    static const int FORCE_THUMB_ROT = 1508; /// 拇指旋转的力对应的寄存器数值
    static const int FORCE_THUMB = 1506;     /// 拇指的力对应的寄存器数值
    static const int FORCE_INDEX = 1504;     /// 食指的力对应的寄存器数值
    static const int FORCE_MIDDLE = 1502;    /// 中指的力对应的寄存器数值
    static const int FORCE_RING = 1500;      /// 无名指的力对应的寄存器数值
    static const int FORCE_PINKY = 1498;     /// 小指的力对应的寄存器数值
    // 动作序列对应的寄存器数值
    static const int ACTION_SEQ_CHECKDATA1 = 2000; /// 动作序列的校验数据1
    static const int ACTION_SEQ_CHECKDATA2 = 2001; /// 动作序列的校验数据2
    static const int ACTION_SEQ_STEPNUM = 2002;    /// 当前动作序列的步骤数
    static const int ACTION_SEQ_STEP0 = 2016;      /// 动作序列的第0步
    static const int ACTION_SEQ_STEP1 = 2054;      /// 动作序列的第1步
    static const int ACTION_SEQ_STEP2 = 2092;      /// 动作序列的第2步
    static const int ACTION_SEQ_STEP3 = 2130;      /// 动作序列的第3步
    static const int ACTION_SEQ_STEP4 = 2168;      /// 动作序列的第4步
    static const int ACTION_SEQ_STEP5 = 2206;      /// 动作序列的第5步
    static const int ACTION_SEQ_STEP6 = 2244;      /// 动作序列的第6步
    static const int ACTION_SEQ_STEP7 = 2282;      /// 动作序列的第7步
    static const int ACTION_SEQ_INDEX = 2320;      /// 当前动作序列的索引号
    static const int SAVE_ACTION_SEQ = 2321;       /// 开始执行动作序列
    static const int ACTION_SEQ_RUN = 2322;        /// 运行执行动作序列

    Inspire(/* args */);

    ~Inspire();

    /**
     * \brief 初始化CAN通信
     *
     * \return 成功返回0，失败返回-1
     */
    int InitCan();

    /**
     * \brief 重载CanWriter的WriteCan函数，实现写入CAN总线
     *
     * \param[in] address 寄存器地址
     * \param[in] value 写入的数值
     * \param[in] handid 手指编号
     * \return 成功写入返回0，失败返回-1
     */
    int WriteCan(int address, int value, int handid);

    /**
     * \brief 获取地址ID的二进制字符串表示
     *
     * \param[in] address 目标地址
     * \return 地址ID的二进制字符串，如果地址不在范围内则返回空字符串
     */
    std::string GetAddressId(int address);

    /**
     * \brief 获取手ID的二进制字符串表示
     *
     * \param[in] handid 手指编号
     * \return 手ID的二进制字符串，如果手ID不在范围内则返回空字符串
     */
    std::string GetHandId(int handid);

    /**
     * \brief 获取写读标志的二进制字符串表示
     *
     * \param[in] flag_write_read 写读标志
     * \return 标志的二进制字符串，如果标志不在范围内则返回空字符串
     */

    std::string GetFlageWriteRead(int flag_write_read);
    /**
     * \brief 获取CAN ID的二进制字符串表示
     *
     * \param[in] Address 寄存器地址
     * \param[in] HandId 手指编号
     * \param[in] flag_write_read 写读标志
     * \return CAN ID的二进制字符串，如果参数不在范围内则返回空字符串
     */
    std::string GetCanId(int Address, int HandId, int flag_write_read);
    /**
     * \brief 将数值转换为16进制字节数组
     *
     * \param[in] value 要转换的数值
     * \param[out] data 转换后的字节数组
     */
    void ConvertValueToHex(int value, Byte data[2]);
    /**
     * \brief 关闭CAN设备
     *
     */
    void InspireCloseDevice();
};

Inspire::Inspire(/* args */)
{
    init_config.AccCode = 0;
    init_config.AccMask = 0xffffffff; // 不滤波
    init_config.Filter = 0;
    init_config.Timing0 = 0;
    init_config.Timing1 = 0x14; // 500k
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
    if (address >= 1486 && address < 1534||address >= 2000 && address <= 2322)
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
    if (value < -1 || value > 1000)
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
   // 打印数据
    // std::cout << "data[0]: 0x" << std::hex << static_cast<int>(data[0]) << std::endl;
    // std::cout << "data[1]: 0x" << std::hex << static_cast<int>(data[1]) << std::endl;
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
    usleep(300000);
    return 0;
}
void Inspire::InspireCloseDevice()

{
    CloseDevice(USBCAN2, DeviceInd);
}

/*
    * \class InspireAction
    * \brief 控制因时手运动的类，实现了基于CAN总线的通信
    *
    * 此类继承自CanWriter，提供了初始化CAN通信、发送数据以及对手指运动的控制功能。

*/
class InspireAction
{
public:
    int handid;
    /**
     * \brief 构造函数
     */
    InspireAction(CanWriter *canwriter, int id) : action_canwriter(canwriter), handid(id) {}
    /*
     * \brief  拇指旋转
     * \param[in] value 旋转角度
     * \return 无
     */
    void ThumbRotAction(int value)
    {
        action_canwriter->WriteCan(Inspire::THUMB_ROT, value, handid);
    }
    /*
     * \brief  拇指运动
     * \param[in] value 旋转角度
     */
    void ThumbAction(int value)
    {
        action_canwriter->WriteCan(Inspire::THUMB, value, handid);
    }
    /*
     * \brief  食指运动
     * \param[in] value 旋转角度
     */
    void IndexAction(int value)
    {
        action_canwriter->WriteCan(Inspire::INDEX, value, handid);
    }
    /*
     * \brief  中指运动
     * \param[in] value 旋转角度
     */
    void MiddleAction(int value)
    {
        action_canwriter->WriteCan(Inspire::MIDDLE, value, handid);
    }
    /*
     * \brief  无名指运动
     * \param[in] value 旋转角度
     */
    void RingAction(int value)
    {
        action_canwriter->WriteCan(Inspire::RING, value, handid);
    }
    /*
     * \brief  小指运动
     * \param[in] value 旋转角度
     */
    void PinkyAction(int value)
    {
        action_canwriter->WriteCan(Inspire::PINKY, value, handid);
    }
    /*
     * \brief  五指同时运动
     * \param[in] value 五个手指的旋转角度
     * \return 无
     */
    int FiveFingerAction(std::vector<int> value)
    {
        if (value.size() != 6)
        {
            std::cout << "The size of value is not 6" << std::endl;
            return -1;
        }
        action_canwriter->WriteCan(Inspire::THUMB_ROT, value[0], handid);

        action_canwriter->WriteCan(Inspire::THUMB, value[1], handid);

        action_canwriter->WriteCan(Inspire::INDEX, value[2], handid);

        action_canwriter->WriteCan(Inspire::MIDDLE, value[3], handid);

        action_canwriter->WriteCan(Inspire::RING, value[4], handid);

        action_canwriter->WriteCan(Inspire::PINKY, value[5], handid);

        return 0;
    }
    /*
     * \brief  设置五指的速度
     * \param[in] value 五个手指的速度
     */
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
        action_canwriter->WriteCan(Inspire::SPEED_THUMB_ROT, value[0], handid);

        action_canwriter->WriteCan(Inspire::SPEED_THUMB, value[1], handid);

        action_canwriter->WriteCan(Inspire::SPEED_INDEX, value[2], handid);

        action_canwriter->WriteCan(Inspire::SPEED_MIDDLE, value[3], handid);

        action_canwriter->WriteCan(Inspire::SPEED_RING, value[4], handid);

        action_canwriter->WriteCan(Inspire::SPEED_PINKY, value[5], handid);
    }
    /*
     * \brief  设置五指的力
     * \param[in] value 五个手指的力
     */
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
        action_canwriter->WriteCan(Inspire::FORCE_THUMB_ROT, value[0], handid);

        action_canwriter->WriteCan(Inspire::FORCE_THUMB, value[1], handid);

        action_canwriter->WriteCan(Inspire::FORCE_INDEX, value[2], handid);

        action_canwriter->WriteCan(Inspire::FORCE_MIDDLE, value[3], handid);

        action_canwriter->WriteCan(Inspire::FORCE_RING, value[4], handid);

        action_canwriter->WriteCan(Inspire::FORCE_PINKY, value[5], handid);
    }
    /// @brief
    /// 先简单实现握拳，假设stepNum=2,第一步不动，第二步闭合手指
    /// @param index
    /// @param stepNum
    /// @param steps
    void setActionSequenceData(int index, int stepNum, const std::vector<int> &steps,const std::vector<int> &steps1)
    {
        action_canwriter->WriteCan(Inspire::ACTION_SEQ_INDEX, index, handid);
        action_canwriter->WriteCan(Inspire::ACTION_SEQ_STEPNUM, stepNum, handid);
        std::vector<int> steps_0 = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 400};
        for (int j = 0; j < steps_0.size(); j++)
        {
            action_canwriter->WriteCan(Inspire::ACTION_SEQ_STEP0 + j * 2, steps_0[j], handid);
        }
        for(int i=0;i<steps.size();i++)
        {
            action_canwriter->WriteCan(Inspire::ACTION_SEQ_STEP0 + i * 2, steps[i], handid);
        }
        for(int i=0;i<steps1.size();i++)
        {
            action_canwriter->WriteCan(Inspire::ACTION_SEQ_STEP0 + i * 2, steps1[i], handid);
        }
            // 4. 将0x90和0xEB写入到ACTION_SEQ_CHECKDATA1和ACTION_SEQ_CHECKDATA2
            action_canwriter->WriteCan(Inspire::ACTION_SEQ_CHECKDATA1, 144, handid);//0x90
            action_canwriter->WriteCan(Inspire::ACTION_SEQ_CHECKDATA2, 235, handid);//0xEB

            // 5. 设置动作序列库保存寄存器
            action_canwriter->WriteCan(Inspire::SAVE_ACTION_SEQ, 1, handid);
    }
    void RunActionSequence(int index)
    {
        action_canwriter->WriteCan(Inspire::ACTION_SEQ_INDEX, index, handid);
        action_canwriter->WriteCan(Inspire::ACTION_SEQ_RUN, 1, handid);
    }

    ~InspireAction() {}

private:
    CanWriter *action_canwriter;
};

#endif