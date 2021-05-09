import matplotlib.pyplot as plt
import csv
import numpy as np

numBots = 30
alld = None

for i in range(numBots):
    d = []
    with open("Data/info%d.txt"%i, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            d.append(int(row[0]))
    if alld is None:
        alld = np.array([d[1:]])
    else:
        alld = np.vstack(([d[1:]], alld))

for d in alld:
    plt.scatter(range(len(alld[0])), d, marker='.')
plt.show()
