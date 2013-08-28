import matplotlib.pyplot as plt
import numpy as np

CONFIDENCE = 0.8

def value_to_frequency(value):
    return 1/(1.4*value*6*(10**-9)*1000)

y = []
y_low = []
y_high = []
x = []
for i in range (17, 218):
    value_y = value_to_frequency(i)
    y.append(value_y)
    y_low.append(value_y*CONFIDENCE)
    y_high.append(value_y*(2-CONFIDENCE))

    x.append((i-17)*(515.00)/(218-17))

fig, ax = plt.subplots(1, sharex=True, sharey=True)

ax.fill_between(x, y_low, y_high, alpha=0.5, label='Confidence 80%')
ax.plot(x, y)

b1 = plt.bar(np.array([1,2])-.2,
             2*np.array([2,1]),
             0.4,
             color='b',
             bottom=np.array([10,8])-np.array([2,1]),
             alpha=0.3) 
plt.legend([b1[0]], ['Confidence (80%)'], shadow=True, fancybox=True, numpoints=1) 

plt.axis([0, 515, 0, 7002])
plt.ylabel('Frequency (Hz)')
plt.xlabel('Value')
plt.show()
