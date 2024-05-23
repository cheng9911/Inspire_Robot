# 串口通信的代码，建立串口通信的类包含初始化和读取和写入函数以及关闭串口函数。
import serial

class SerialCommunication:
    def __init__(self, port, baudrate, bytesize=serial.EIGHTBITS, stopbits=serial.STOPBITS_ONE, parity=serial.PARITY_NONE, timeout=1):
        """
        初始化串行通信类

        参数:
            port (str): 串行端口号
            baudrate (int): 波特率
            bytesize (int): 数据位数，默认为8位（serial.EIGHTBITS）
            stopbits (int): 停止位数，默认为1位（serial.STOPBITS_ONE）
            parity (int): 校验位，默认为无校验（serial.PARITY_NONE）
            timeout (float): 超时时间（秒），默认为1秒
        """
        self.port = port
        self.baudrate = baudrate
        self.bytesize = bytesize
        self.stopbits = stopbits
        self.parity = parity
        self.timeout = timeout
        try:
            self.ser = serial.Serial(port=self.port, baudrate=self.baudrate, bytesize=self.bytesize, stopbits=self.stopbits, parity=self.parity, timeout=self.timeout)
        except serial.SerialException as e:
            print(f"串行通信初始化失败: {e}")

    def read(self, num_bytes):
        """
        从串行端口读取指定字节数的数据

        参数:
            num_bytes (int): 需要读取的字节数

        返回:
            bytes: 读取到的数据
        """
        return self.ser.read(num_bytes)

    def write(self, data):
        """
        向串行端口写入数据

        参数:
            data (bytes): 需要写入的数据
        """
        self.ser.write(data)

    def send_and_receive(self, command, response_length):
        """
        发送命令并接收指定长度的响应

        参数:
            command (bytes): 命令数据
            response_length (int): 预期响应的字节数

        返回:
            bytes: 接收到的响应数据
        """
        self.write(command)
        return self.read(response_length)

    def close(self):
        """
        关闭串行通信
        """
        self.ser.close()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.close()