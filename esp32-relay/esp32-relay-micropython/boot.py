import usocket as socket
import network
import esp
import gc

esp.osdebug(None)
gc.collect()

# WiFi #
ssid = "ST7-14"
password = "MayThazin11"

station = network.WLAN(network.STA_IF)
station.active(True)
station.connect(ssid, password)
while not station.isconnected():
    pass
print("Connection successful")
print(station.ifconfig()[0])
