from time import sleep
from max31855 import MAX31855

thermocoupler = MAX31855(do=12, cs=13, clk=14)

while True:
    temp_c = thermocoupler.read_temp_c()
    temp_f = thermocoupler.read_temp_f()

    print("Temperature in C = {}".format(temp_c))
    print("Temperature in F = {}".format(temp_f))
    print("\n")
    sleep(0.5)
