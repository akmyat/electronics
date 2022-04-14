# import serial
# import time
#
# serial_connection = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
# time.sleep(2)
#
# operation = True
#
# with open('pid.csv', 'w') as f:
#     f.write("Setpoint,Output\n")
#
# for i in range(600001):
#     line = serial_connection.readline()
#     if line:
#         data = line.decode()
#         print(data)
#         with open('pid.csv', 'a') as f:
#             f.write(data)
#
# serial_connection.close()

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv('old_pid.csv')
plt.plot(range(0, 600000), df['Output'].iloc[0:600000].values.tolist())
plt.plot(range(0, 600000), df['Setpoint'].iloc[0:600000].values.tolist())
plt.axvline(14000, color='gray', linestyle='--')
plt.axvline(400000, color='gray', linestyle='--')
plt.axvline(80000, color='gray', linestyle='--')
plt.axhline(630, color='gray', linestyle='--')
plt.xlabel("Time (us)")
plt.ylabel("Lux")
plt.savefig('pid.png')
plt.show()
