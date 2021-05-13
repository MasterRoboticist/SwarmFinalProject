import matplotlib.pyplot as plt
import csv
import numpy as np

# gets the data and stores it in class and position arrays
def getData(filename):
    classes = np.array([])
    positions = None
    numBots = 0
    with open(filename, 'r') as file:
        reader = csv.reader(file, delimiter="\t")
        for row in reader:
            if row[0] == '3':
                numBots += 1
            classes = np.append(classes, row[2])
            if positions is None:
                positions = np.array([row[3], row[4]])
            else:
                positions = np.vstack((positions, (row[3], row[4])))

    # print(classes)
    # print(positions)

    classes = np.reshape(classes, (-1,numBots)).T
    positions = np.reshape(positions, (-1,numBots,2))
    positions = np.transpose(positions, axes=(1,0,2))

    classes = classes.astype(float)
    positions = positions.astype(float)

    # print(classes)
    # print(positions)
    return classes, positions

# makes a graph where each robot's class has a line
def makeClassGraph(data, filename):
    xs = range(len(data[0]))
    for d in data:
        plt.scatter(xs, d)
    # plt.show()
    plt.title(filename[7:-4])
    plt.savefig(filename)
    plt.clf()

# TODO
def makeStdDevGraph(data, filename):
    dat = np.transpose(data, (1,0,2))
    stdDevs = np.array([])
    for d in dat:
        cx,cy = centroid(d)
        dists = [distance(cx,cy,p[0],p[1]) for p in d]
        stdDevs = np.append(stdDevs, np.std(dists))
    # plt.plot(range(len(stdDevs)), stdDevs)
    # plt.title(filename[7:-4])
    # plt.savefig(filename)
    # plt.clf()
    print("for file %s last std dev was %f" % (filename, stdDevs[-1]))

def centroid(d):
    p = np.average(d, axis=0)
    return p[0],p[1]

def distance(x1,y1,x2,y2):
    return ((x1-x2)**2+(y1-y2)**2)**.5

# the things to be done to the data
def doStuff(filename):
    print("Analyzing file %s"%filename)
    clss, poss = getData("%s.dat"%filename)
    # makeClassGraph(clss, "graphs/%s.png"%filename)
    makeStdDevGraph(poss, "graphs/%sstd.png"%filename)

# if you did every permutation of some combination of R, N, and K, then fill them in to these arrays and
# the script will do all of them automatically
R = [30]
N = [25,100,200]
K = [1,5,10,20]

# if there is some other data, put the file name without the extension in this list
misc = ['R10N200K5','R70N200K5','R30N500K5']

for r in R:
    for n in N:
        for k in K:
            doStuff("R%dN%dK%d"%(r,n,k))
for m in misc:
    doStuff(m)




# numBots = 30
# alld = None
#
# for i in range(numBots):
#     d = []
#     with open("Data/info%d.txt"%i, 'r') as file:
#         reader = csv.reader(file)
#         for row in reader:
#             d.append(int(row[0]))
#     if alld is None:
#         alld = np.array([d[1:]])
#     else:
#         alld = np.vstack(([d[1:]], alld))
#
# for d in alld:
#     plt.scatter(range(len(alld[0])), d, marker='.')
# plt.show()
