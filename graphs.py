import matplotlib.pyplot as plt

#Greedy
x = [2,5,7,10,12]
y1 = [0.0006752,0.00171,0.00265,0.00359,0.00431]
y2 = [41.78,5000,5000,5000,5000]
y3 = [0.000998,0.00212,0.00278,0.02132,2.08]
y4 = [0.0009297,0.00203,0.00287,0.06112,7.95]
y5 = [0.00096083,0.00195,0.00281,0.03187,3.15]
y6 = [0.00104,0.00205,0.00293,0.02463,1.55]

plt.plot(x,y1,label="Greedy")
plt.plot(x,y2,label="Backtracking Pur")
plt.plot(x,y3,label="Backtracking Greedy")
plt.plot(x,y4,label="B&B1")
plt.plot(x,y5,label="B&B2")
plt.plot(x,y6,label="B&B3")
plt.xlabel("nVisites")
plt.ylabel("Temps (s)")
plt.legend()
plt.ylim(0,8)
plt.xlim(2,12)
plt.show()
