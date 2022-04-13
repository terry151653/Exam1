import matplotlib.pyplot as plt
import numpy as np
import serial
import time

Fs = 2000.0  # sampling rate
Ts = 1.0/Fs  # sampling interval

serdev = '/COM7'
s = serial.Serial(serdev)
print("OK")
while (1):
    line = s.readline()
    print(line)
    if (line == b'---start---\r\n'):
        break

print("Get original signal")
y = np.linspace(0, 1, 100000)  # signal vector; create numOfSamples samples
x = 0
while (1):
    line = s.readline()  # Read an echo string from B_L4S5I_IOT01A terminated with '\n'
    if (line == b'---stop---\r\n'):
        break
    y[x] = float(line)
    x += 1

line = s.readline()
tmp = int(line)
durationOfSamples = tmp * 0.001
numOfSamples = int(tmp / 100)


# time vector; create Fs samples between 0 and 1.0 sec.
t = np.linspace(0, durationOfSamples, num = numOfSamples)
Y = np.linspace(0, durationOfSamples, num = numOfSamples)
for i in range(len(t)):
    Y[i] = y[i]
#frq = range(2000)  # focus on frequency of interest

#############################################

plt.plot(t, Y)
plt.xlabel('Time')
plt.ylabel('Degree')
# # inset axes....
# axins = ax[1].inset_axes([0.5, 0.2, 0.47, 0.77])
# axins.plot(frq, abs(Y), 'r')
# # sub region of the original image
# x1, x2, y1, y2 = -5, 30, -0.1, 1.1
# axins.set_xlim(x1, x2)
# axins.set_ylim(y1, y2)
# axins.set_xticklabels([])
# axins.set_yticklabels([])
# ax[1].indicate_inset_zoom(axins, edgecolor="black")

plt.show()
s.close()
