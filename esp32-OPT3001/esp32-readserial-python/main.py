import serial
import time

serial_connection = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
time.sleep(2)

operation = True

with open('pwm_lux.csv', 'w') as f:
    f.write("PWM,LUX\n")

if operation:
    line = serial_connection.readline()
    value = int(line.decode().split(",")[0])
    while value != 255:
        line = serial_connection.readline()
        value = int(line.decode().split(",")[0])
        print(value)

for i in range(255):
    line = serial_connection.readline()
    if line:
        data = line.decode()
        print(f'PWM: {data.split(",")[0]}, LUX: {data.split(",")[1]}')
        with open('pwm_lux.csv', 'a') as f:
            f.write(data)

serial_connection.close()

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv('pwm_lux.csv')
if operation:
    plt.plot(df['PWM'].iloc[1:255].values.tolist(), df['LUX'].iloc[1:255].values.tolist())
    pwm = int(np.interp(700, df['LUX'].iloc[1:255].values.tolist(), df['PWM'].iloc[1:255].values.tolist()))
    print(pwm)
    plt.axvline(pwm, color='gray', linestyle='--')
    plt.axhline(700, color='gray', linestyle='--')
    plt.xlabel("PWM")
    plt.ylabel("Lux")
    plt.title("PWM and Lux Relationship")
    plt.savefig('pwm_lux.png')
    plt.show()
else:
    plt.ylim(0, 1000)
    plt.plot(range(0, 255), df['LUX'][0:255].values.tolist())
    plt.xlabel("Time")
    plt.ylabel("Lux")
    plt.title("Lux According to Time")
    plt.savefig('lux_time.png')
    plt.show()
