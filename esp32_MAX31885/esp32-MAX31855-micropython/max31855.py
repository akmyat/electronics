from machine import Pin, SPI


class MAX31855:
    def __init__(self, do=12, cs=13, clk=14):
        """

        :param do: Data Output
        :param cs: Chip Select
        :param clk: Clock
        """
        self.hspi = SPI(1, baudrate=10000000, sck=Pin(clk), mosi=Pin(cs), miso=Pin(do))

        self.cs = Pin(cs, Pin.OUT)
        self.cs.value(1)

        self.temp_c = 0
        self.temp_f = 0

    def __read_temp(self):
        self.cs.value(0)
        data = self.hspi.read(4)
        self.cs.value(1)
        return data

    def read_temp_c(self):
        data = self.__read_temp()
        temp = data[0] << 8 | data[1]
        if temp & 0x0001:
            return float('NaN')
        temp >>= 2
        if temp & 0x2000:
            temp -= 16384
        return temp * 0.25

    def read_temp_f(self):
        temp_c = self.read_temp_c()
        temp_f = temp_c * (9/5) + 32
        return temp_f
