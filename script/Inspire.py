# 创建因时机器人串口通信代码，继承 SerialCommunication 类，重新描述写寄存器和读寄存器的方法。
from serial import PARITY_NONE, STOPBITS_ONE, EIGHTBITS
from SerialCommunication import SerialCommunication as sc
import time


class Inspire():
    def __init__(self, port, baudrate, bytesize=EIGHTBITS, stopbits=STOPBITS_ONE, parity=PARITY_NONE, timeout=1):
        # self.port = port
        # self.baudrate = baudrate
        # self.bytesize = bytesize
        # self.stopbits = stopbits
        # self.parity = parity
        # self.timeout = timeout
        self.sc_comm = sc(port, baudrate, bytesize, stopbits, parity, timeout)

    def gen_Can_id(handid, reg, rorw):
        # 确保hand_id在0-13之间，reg在0-255之间，rorw在0-7之间
        assert 0 <= handid <= 13, "hand_id should be between 0 and 13"
        assert 0 <= reg <= 2**11, "reg should be between 0 and 2^11"
        assert 0 <= rorw <= 1, "rorw should be between 0 and 7"
        # 对hand_id、reg和rorw进行位移
        handid_shifted = handid
        reg_shifted = reg << 14
        rorw_shifted = rorw << 26
        # 按照位宽组合hand_id、reg和rorw
        can_id = handid_shifted | reg_shifted | rorw_shifted
        # 清除30-32位，确保这部分为0
        can_id &= 0x0FFFFFFF
        return can_id

    def writeRegister(ser, id, address, num, val):  # 这个函数 writeRegister 的作用是向设备写入寄存器的数值。
        bytes = [0xAA, 0xAA]
        # 生成can_id
        CAN_id = gen_Can_id(id, address, 1)
        # 生成can_data
        bytes.append((CAN_id >> 24) & 0xff)  # id
        bytes.append((CAN_id >> 16) & 0xff)
        bytes.append((CAN_id >> 8) & 0xff)
        bytes.append((CAN_id >> 0) & 0xff)
        for i in range(8):
            bytes.append(val[i])
        bytes.append(num)
        bytes.append(0)
        bytes.append(1)
        bytes.append(0)
        checksum=0
        for i in range(2, 18):
            checksum += bytes[i]
        bytes.append(checksum&0xff)
        bytes.append(0x55)
        bytes.append(0x55)
        print("bytes",hex(bytes))

    # 发送can帧


def gen_Can_id(handid, reg, rorw):
    # 确保hand_id在0-13之间，reg在0-255之间，rorw在0-7之间
    assert 0 <= handid <= 13, "hand_id should be between 0 and 13"
    assert 0 <= reg <= 2**11, "reg should be between 0 and 2^11"
    assert 0 <= rorw <= 1, "rorw should be between 0 and 7"
    # 对hand_id、reg和rorw进行位移
    handid_shifted = handid
    reg_shifted = reg << 14
    rorw_shifted = rorw << 26
    # 按照位宽组合hand_id、reg和rorw
    can_id = handid_shifted | reg_shifted | rorw_shifted
    # 清除30-32位，确保这部分为0
    can_id &= 0x0FFFFFFF
    return can_id
def writeRegister(id, address, num, val):  # 这个函数 writeRegister 的作用是向设备写入寄存器的数值。
        bytes = [0xAA, 0xAA]
        # 生成can_id
        CAN_id = gen_Can_id(id, address, 1)
        # 生成can_data
        bytes.append((CAN_id >> 24) & 0xff)  # id
        bytes.append((CAN_id >> 16) & 0xff)
        bytes.append((CAN_id >> 8) & 0xff)
        bytes.append((CAN_id >> 0) & 0xff)
        for i in range(8):
            bytes.append(val[i])
        bytes.append(num)
        bytes.append(0)
        bytes.append(1)
        bytes.append(0)
        for i in range(2, 18):
            checksum += bytes[i]
        bytes.append(checksum&0xff)
        bytes.append(0x55)
        bytes.append(0x55)
        print("bytes",hex(bytes))
        

canid = gen_Can_id(0x01, 0x05D4, 1)
print("canid:", hex(canid))
writeRegister(0x01,1492,2,[0x58,0x02,0x00,0x00,0x00,0x00,0x00,0x00])
