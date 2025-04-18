import serial
import time


regdict = {
    'ID' : 1000,
    'baudrate' : 1001,
    'clearErr' : 1004,
    'forceClb' : 1009,
    'angleSet' : 1486,
    'forceSet' : 1498,
    'speedSet' : 1522,
    'angleAct' : 1546,
    'forceAct' : 1582,
    'errCode' : 1606,
    'statusCode' : 1612,
    'temp' : 1618,
    'actionSeq' : 2320,
    'actionRun' : 2322
}

def gen_CAN_id(handid,reg,rorw):
    return reg<<14+handid+rorw<<26

def openSerial(port, baudrate): #这个函数的作用是根据传入的串口端口号和波特率打开串口通信，并返回一个串口对象，以供后续的数据交换。
    ser = serial.Serial()
    ser.port = port
    ser.baudrate = baudrate
    ser.open()
    return ser


def writeRegister(ser, id, add, num, val): #这个函数 writeRegister 的作用是向设备写入寄存器的数值。
    bytes = [0xAA, 0xAA]
    
    bytes.append((CAN_id>>0)&0xff)
    bytes.append((CAN_id>>8)&0xff)
    bytes.append((CAN_id>>16)&0xff)
    bytes.append((CAN_id>>24)&0xff) # id
    for i in range(8):
        if i<num:
            bytes.append(val[i])
        else:
            bytes.append(0)
    bytes.append(num)
    bytes.append(0)
    bytes.append(1)
    bytes.append(0)
    for i in range(2,18):
        checksum+=bytes[i]
    bytes.append(checksum)
    bytes.append(0x55)
    bytes.append(0x55)
    # bytes.append(num + 3) # len
    # bytes.append(0x12) # cmd，0x12 被用作写寄存器的命令字节。在串口通信中，命令字节用于指示接收方应该执行的操作。在特定的通信协议中，0x12 被定义为写寄存器的命令。
    # bytes.append(add & 0xFF)
    # bytes.append((add >> 8) & 0xFF) # add
    # for i in range(num):
    #     bytes.append(val[i])
    # checksum = 0x00
    # for i in range(2, len(bytes)):
    #     checksum += bytes[i]
    # checksum &= 0xFF
    # bytes.append(checksum)
    ser.write(bytes)
    time.sleep(0.01)
    ser.read_all() # 把返回帧读掉，不处理

# [起始标志] [命令字节] [地址字节低位] [地址字节高位] [数据长度] [实际数据] [校验和]
def readRegister(ser, id, add, num, mute=False):
    bytes = [0xAA, 0xAA]
    CAN_id = gen_CAN_id(id, add, 0)
    bytes.append((CAN_id >> 0) & 0xFF)
    bytes.append((CAN_id >> 8) & 0xFF)
    bytes.append((CAN_id >> 16) & 0xFF)
    bytes.append((CAN_id >> 24) & 0xFF)  # id
    bytes.append(8)
    bytes.append(0)
    bytes.append(0)
    bytes.append(0)
    bytes.append(0)
    bytes.append(0)
    bytes.append(0)
    bytes.append(0)
    for i in range(2,18):
        checksum+=bytes[i]
    bytes.append(checksum)
    bytes.extend([0x55, 0x55])

    ser.write(bytes)
    time.sleep(0.01)
    recv = ser.read_all()
    if len(recv) == 0:
        return []
    num = (recv[3] & 0xFF) - 3 #3 表示数据长度字段的长度。数据包的格式中有一些固定的部分占据了字节，比如命令字节、地址低位、地址高位等，这些部分不属于实际的数据内容。
    val = []
    for i in range(num):
        val.append(recv[7 + i]) # 实际上是从接收到的数据包中提取出的实际数据内容。 从接收到的数据中获取实际的数据内容并添加到列表 val 中。命令字节、地址字节
#7 是一个固定的偏移量，用于跳过数据包中的固定部分（比如起始标志、命令字节、地址字节等），而 i 是一个循环变量，用于逐个访问接收到的实际数据。
    if not mute:
        print('读到的寄存器值依次为：', end='') #- 如果 mute 参数为 False，则打印读取到的数据值。
        for i in range(num):
            print(val[i], end=' ')
        print()
    return val

#假设这是一个用于控制机器人的函数，可能有以下需求：
#角度设置（angleSet）：设定机器人关节的角度，共有6个关节需要设置。
#力设置（forceSet）：设定每个关节所需的力。
#速度设置（speedSet）：设定每个关节的运动速度。
def write6(ser, id, str, val): #val：要写入的值，如果 str 为 'angleSet', 'forceSet', 'speedSet'，则应为长度为6的列表，每个元素是一个整数，范围为0~1000。
    if str == 'angleSet' or str == 'forceSet' or str == 'speedSet':
        val_reg = []
        for i in range(6):
            val_reg.append(val[i] & 0xFF)
            val_reg.append((val[i] >> 8) & 0xFF)
        writeRegister(ser, id, regdict[str], 12, val_reg) #12：要写入的数据长度，这里是12个字节，因为对于 angleSet, forceSet, speedSet 每个值都占用2个字节，共6个值，所以长度为12。
    else:
        print('函数调用错误，正确方式：str的值为\'angleSet\'/\'forceSet\'/\'speedSet\'，val为长度为6的list，值为0~1000，允许使用-1作为占位符')

#这段代码定义了一个名为 read6 的函数，用于从设备的寄存器中读取数据，并根据读取的数据类型（由 str 参数指定）进行不同的处理和输出。
def read6(ser, id, str):
    if str == 'angleSet' or str == 'forceSet' or str == 'speedSet' or str == 'angleAct' or str == 'forceAct':
        val = readRegister(ser, id, regdict[str], 12, True)
        if len(val) < 12:
            print('没有读到数据')
            return
        val_act = []
        for i in range(6):
            val_act.append((val[2*i] & 0xFF) + (val[1 + 2*i] << 8))
        print('读到的值依次为：', end='')
        for i in range(6):
            print(val_act[i], end=' ')
        print()
    elif str == 'errCode' or str == 'statusCode' or str == 'temp':
        val_act = readRegister(ser, id, regdict[str], 6, True)
        if len(val_act) < 6:
            print('没有读到数据')
            return
        print('读到的值依次为：', end='')
        for i in range(6):
            print(val_act[i], end=' ')
        print()
    else:
        print('函数调用错误，正确方式：str的值为\'angleSet\'/\'forceSet\'/\'speedSet\'/\'angleAct\'/\'forceAct\'/\'errCode\'/\'statusCode\'/\'temp\'')


if __name__ == '__main__':

    CAN_id=gen_CAN_id(0X1,0x1492,1)
    print("CAN_id:",hex(CAN_id))


    # print('打开串口！')
    # ser = openSerial('/dev/ttyUSB0', 115200) # 改成自己的串口号和波特率，波特率默认115200
    # print(ser)
    # bytes = [0xAA, 0xAA,0x01,0x00,0x75,0x05,0x58,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x01,0x00,0,0x55,0x55]
    # checksum=0
    # # 计算校验和
    # for i in range(2,17):
    #     checksum+=bytes[i]
        
    # print(hex(checksum))
    # bytes[18]=checksum&0xff
    # ser.write(bytes)
    # time.sleep(1)
    # print (hex(bytes))

    

    # print('设置灵巧手运动速度参数，-1为不设置该运动速度！')
    # write6(ser, 1, 'speedSet', [100, 100, 100, 100, 100, 100])
    # time.sleep(2)
    # print('设置灵巧手抓握力度参数！')
    # write6(ser, 1, 'forceSet', [500, 500, 500, 500, 500, 500])
    # time.sleep(1)
    # print('设置灵巧手运动角度参数0，-1为不设置该运动角度！')
    # write6(ser, 1, 'angleSet', [0, 0, 0, 0, 400, -1])
    # time.sleep(3)
    # read6(ser, 1, 'angleAct')
    # time.sleep(1)
    # print('设置灵巧手运动角度参数1000，-1为不设置该运动角度！')
    # write6(ser, 1, 'angleSet', [1000, 1000, 1000, 1000, 400, -1])
    # time.sleep(3)
    # read6(ser, 1, 'angleAct')
    # time.sleep(1)
    # read6(ser, 1, 'errCode')
    # time.sleep(1)
    # print('设置灵巧手动作库序列：8！')
    # writeRegister(ser, 1, regdict['actionSeq'], 1, [8])
    # time.sleep(1)
    # print('运行灵巧手当前序列动作！')
    # writeRegister(ser, 1, regdict['actionRun'], 1, [1])
    # writeRegister(ser, 1, regdict['forceClb'], 1, [1])
    # time.sleep(10) # 由于力校准时间较长，请不要漏过这个sleep并尝试重新与手通讯，可能导致插件崩溃
