
import snNet
import snOperator
import snType
import numpy as np
import imageio
import random
import ctypes
import os

# create net
net = snNet.Net()
net.addNode('In', snOperator.Input(), 'F1')
net.addNode('F1', snOperator.FullyConnected(256), 'F2')
net.addNode('F2', snOperator.FullyConnected(10), 'LS')
net.addNode('LS', snOperator.LossFunction(snType.lossType.softMaxToCrossEntropy), 'Output')

# load of weight
if (net.loadAllWeightFromFile('c:/C++/w.dat')):
    print('weight is load')
else:
    print('error load weight')

# loadImg
imgList = []
pathImg = 'c:/C++/skyNet/test/mnist/'
for i in range(10):
   imgList.append(os.listdir(pathImg + str(i)))

bsz = 100
inLayer = np.zeros((bsz, 1, 28, 28), ctypes.c_float)
outLayer = np.zeros((bsz, 1, 1, 10), ctypes.c_float)
targLayer = np.zeros((bsz, 1, 1, 10), ctypes.c_float)

# cycle
accuratSumm = 0.;
for n in range(1000):

    outLayer[...] = 0
    targLayer[...] = 0

    for i in range(bsz):
        ndir = random.randint(0, 10 - 1)
        nimg = random.randint(0, len(imgList[ndir]) - 1)
        inLayer[i][0] = imageio.imread(pathImg + str(ndir) + '/' + imgList[ndir][nimg])

        targLayer[i][0][0][ndir] = 1.

    acc = [0]
    net.training(0.01, inLayer, outLayer, targLayer, acc)

    accuratSumm += acc[0]

    print(n, "accurate", accuratSumm / (n + 1))

# save weight
if (net.saveAllWeightToFile('c:/C++/w.dat')):
    print('weight is save')
else:
    print('error save weight')