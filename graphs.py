import matplotlib.pyplot as plt

#Greedy
x = [100,250,500,1000,2000,5000,10000]
y1 = [0.00007993,0.00029227,0.00018363,0.0003796,0.00137,0.00266,0.00454]
y2 = [123,1008,5000,5000,5000,5000,5000]
y3 = [0.01713,0.01799,0.01733,0.01754,0.01841,0.02017,0.02173]
y4 = [0.02869,0.03619,0.03909,0.03685,0.05103,0.06333,0.06506]
y5 = [0.01242,0.0163,0.01873,0.01831,0.0262,0.0373,0.03502]
y6 = [0.01077,0.01401,0.01465,0.01516,0.01959,0.02662,0.02937]

plt.plot(x,y1,label="Greedy")
plt.plot(x,y2,label="Backtracking Pur")
plt.plot(x,y3,label="Backtracking Greedy")
plt.plot(x,y4,label="B&B1")
plt.plot(x,y5,label="B&B2")
plt.plot(x,y6,label="B&B3")
plt.xlabel("nArestes")
plt.ylabel("Temps (s)")
plt.legend()
plt.ylim(0,0.07)
plt.xlim(100,10000)
plt.show()
