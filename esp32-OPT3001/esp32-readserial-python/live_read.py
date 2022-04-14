import matplotlib.pyplot as plt
import serial
from itertools import count
from matplotlib.animation import FuncAnimation

serial_connection = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

fig = plt.figure()
xs = []
ys = []
zs = []

index = count()


def read_serial():
    line = serial_connection.readline().decode()
    data = line.split(",")
    setpoint = data[0]
    output = data[1]
    return output, setpoint


def animate(i):
    setpoint, output = read_serial()
    xs.append(next(index))
    ys.append(output)
    zs.append(setpoint)

    plt.cla()
    plt.plot(xs, ys, color="blue")
    plt.plot(xs, zs, color="green")


ani = FuncAnimation(plt.gcf(), animate,frames=range(0,1000), interval=1000)
plt.tight_layout()
plt.show()