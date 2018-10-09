#
# SkyNet Project
# Copyright (C) 2018 by Contributors <https:#github.com/Tyill/skynet>
#
# This code is licensed under the MIT License.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files(the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions :
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

from snType import *

class Input():
    """Input layer."""

    def __init__(self):
        pass

    def getParams(self):
        return {}

    def name(self):
        return 'Input'


class FullyConnected():
    '''Fully connected layer'''

    _params = {
    'kernel' : '0',                          # Number of hidden neurons. !Required parameter [0..)
    'active' : active.relu.value,            # Activation function type. Optional parameter
    'optimizer' : optimizer.adam.value,      # Optimizer of weights. Optional parameter
    'dropOut' : '0',                         # Random disconnection of neurons. Optional parameter [0..1.F]
    'batchNorm' : batchNormType.none.value,  # Type of batch norm. Optional parameter
    'mode' : calcMode.CPU.value,             # Сalculation mode. Optional parameter
    'gpuDeviceId' : '0',                     # GPU Id. Optional parameter
    'gpuClearMem' : '0',                     # Clear memory GPU. Optional parameter
    'freeze' :'0',                           # Do not change weights. Optional parameter
    'weightInit' : weightInit.he.value,      # Type of initialization of weights. Optional parameter
    'decayMomentDW' : '0.9',                 # Optimizer of weights moment change. Optional parameter [0..1.F]
    'decayMomentWGr' : '0.99',               # Optimizer of weights moment change of prev. Optional parameter
    'lmbRegular' : '0.001',                  # Optimizer of weights l2Norm. Optional parameter [0..1.F]
    'batchNormLr' : '0.001'                  # Learning rate for batch norm coef. Optional parameter [0..)
    }

    def __init__(self,
                 kernel,
                 act=active.relu,
                 opt=optimizer.adam,
                 dropOut=0.0,
                 bnorm=batchNormType.none,
                 mode=calcMode.CPU,
                 gpuDeviceId=0,
                 gpuClearMem=False,
                 freeze=False):
        self._params['kernel'] = str(kernel)
        self._params['act'] = act.value
        self._params['opt'] = opt.value
        self._params['dropOut'] = str(dropOut)
        self._params['bnorm'] = bnorm.value
        self._params['mode'] = mode.value
        self._params['gpuDeviceId'] = str(gpuDeviceId)
        self._params['gpuClearMem'] = '1' if gpuClearMem else '0'
        self._params['freeze'] = '1' if freeze else '0'

    def __init__(self,
                 kernel,
                 mode=calcMode.CPU):
        self._params['kernel'] = str(kernel)
        self._params['mode'] = mode.value

    def getParams(self):
        return self._params

    def name(self):
        return "FullyConnected"


class Convolution():
    '''Convolution layer'''

    _params = {
    'kernel' : '0',                          # Number of output layers. !Required parameter [0..)
    'fWidth': '3',                           # Width of mask. Optional parameter(> 0)
    'fHeight': '3',                          # Height of mask. Optional parameter(> 0)
    'padding': '0',                          # Padding around the edges. Optional parameter
    'stride':'1',                            # Mask movement step. Optional parameter(> 0)
    'dilate': '1',                           # Expansion mask. Optional parameter(> 0)
    'active' : active.relu.value,            # Activation function type. Optional parameter
    'optimizer' : optimizer.adam.value,      # Optimizer of weights. Optional parameter
    'dropOut' : '0',                         # Random disconnection of neurons. Optional parameter [0..1.F]
    'batchNorm' : batchNormType.none.value,  # Type of batch norm. Optional parameter
    'mode' : calcMode.CPU.value,             # Сalculation mode. Optional parameter
    'gpuDeviceId' : '0',                     # GPU Id. Optional parameter
    'gpuClearMem' : '0',                     # Clear memory GPU. Optional parameter
    'freeze' :'0',                           # Do not change weights. Optional parameter
    'weightInit' : weightInit.he.value,      # Type of initialization of weights. Optional parameter
    'decayMomentDW' : '0.9',                 # Optimizer of weights moment change. Optional parameter [0..1.F]
    'decayMomentWGr' : '0.99',               # Optimizer of weights moment change of prev. Optional parameter
    'lmbRegular' : '0.001',                  # Optimizer of weights l2Norm. Optional parameter [0..1.F]
    'batchNormLr' : '0.001'                  # Learning rate for batch norm coef. Optional parameter [0..)
    }

    def __init__(self,
                 kernel,
                 act=active.relu,
                 opt=optimizer.adam,
                 dropOut=0.0,
                 bnorm=batchNormType.none,
                 mode=calcMode.CPU,
                 gpuDeviceId=0,
                 gpuClearMem=False,
                 freeze=False):
        self._params['kernel'] = str(kernel)
        self._params['act'] = act.value
        self._params['opt'] = opt.value
        self._params['dropOut'] = str(dropOut)
        self._params['bnorm'] = bnorm.value
        self._params['mode'] = mode.value
        self._params['gpuDeviceId'] = str(gpuDeviceId)
        self._params['gpuClearMem'] = '1' if gpuClearMem else '0'
        self._params['freeze'] = '1' if freeze else '0'

    def __init__(self,
                 kernel,
                 mode=calcMode.CPU):
        self._params['kernel'] = str(kernel)
        self._params['mode'] = mode.value

    def getParams(self):
        return self._params

    def name(self):
        return "Convolution"


class Deconvolution():
    '''Deconvolution layer'''

    _params = {
        'kernel': '0',                         # Number of output layers. !Required parameter [0..)
        'fWidth': '3',                         # Width of mask. Optional parameter(> 0)
        'fHeight': '3',                        # Height of mask. Optional parameter(> 0)
        'stride': '2',                         # Mask movement step. Optional parameter(> 0)
        'active': active.relu.value,           # Activation function type. Optional parameter
        'optimizer': optimizer.adam.value,     # Optimizer of weights. Optional parameter
        'dropOut': '0',                        # Random disconnection of neurons. Optional parameter [0..1.F]
        'batchNorm': batchNormType.none.value, # Type of batch norm. Optional parameter
        'mode': calcMode.CPU.value,            # Сalculation mode. Optional parameter
        'gpuDeviceId': '0',                    # GPU Id. Optional parameter
        'gpuClearMem': '0',                    # Clear memory GPU. Optional parameter
        'freeze': '0',                         # Do not change weights. Optional parameter
        'weightInit': weightInit.he.value,     # Type of initialization of weights. Optional parameter
        'decayMomentDW': '0.9',                # Optimizer of weights moment change. Optional parameter [0..1.F]
        'decayMomentWGr': '0.99',              # Optimizer of weights moment change of prev. Optional parameter
        'lmbRegular': '0.001',                 # Optimizer of weights l2Norm. Optional parameter [0..1.F]
        'batchNormLr': '0.001'                 # Learning rate for batch norm coef. Optional parameter [0..)
    }

    def __init__(self,
                 kernel,
                 act=active.relu,
                 opt=optimizer.adam,
                 dropOut=0.0,
                 bnorm=batchNormType.none,
                 mode=calcMode.CPU,
                 gpuDeviceId=0,
                 gpuClearMem=False,
                 freeze=False):
        self._params['kernel'] = str(kernel)
        self._params['act'] = act.value
        self._params['opt'] = opt.value
        self._params['dropOut'] = str(dropOut)
        self._params['bnorm'] = bnorm.value
        self._params['mode'] = mode.value
        self._params['gpuDeviceId'] = str(gpuDeviceId)
        self._params['gpuClearMem'] = '1' if gpuClearMem else '0'
        self._params['freeze'] = '1' if freeze else '0'

    def __init__(self,
                 kernel,
                 mode=calcMode.CPU):
        self._params['kernel'] = str(kernel)
        self._params['mode'] = mode.value

    def getParams(self):
        return self._params

    def name(self):
        return "Deconvolution"


class Pooling():
    '''Pooling layer'''

    _params = {
        'kernel': '2',              # Square Mask Size. Optional parameter (> 0)
        'pool': poolType.max.value, # Operator Type. Optional parameter
        'mode': calcMode.CPU.value, # Сalculation mode. Optional parameter
        'gpuDeviceId': '0',         # GPU Id. Optional parameter
        'gpuClearMem': '0',         # Clear memory GPU. Optional parameter
    }

    def __init__(self,
                 mode=calcMode.CPU):
        self._params['mode'] = mode.value

    def getParams(self):
        return self._params

    def name(self):
        return "Pooling"


class Lock():
    '''
    Operator to block further calculation at the current location.
    It is designed for the ability to dynamically disconnect the parallel
    branches of the network during operation.
    '''

    _params = {
        'state':lockType.unlock.value   # Blocking activity. Optional parameter
    }

    def __init__(self, lock):
        self._params['state'] = lock.value

    def getParams(self):
        return self._params

    def name(self):
        return "Lock"


class Switch():
    '''
    Operator for transferring data to several nodes at once.
    Data can only be received from one node.
    '''

    _params = {
        'nextWay':''       # next nodes through a space
    }

    def __init__(self, nextWay : str):
        self._params['nextWay'] = nextWay

    def getParams(self):
        return self._params

    def name(self):
        return "Switch"


class Summator():
    '''
    The operator is designed to combine the values of two layers.
    The consolidation can be performed by the following options: "summ", "diff", "mean".
    The dimensions of the input layers must be the same.
    '''

    _params = {
        'type': summatorType.summ.value
    }

    def __init__(self, type : summatorType):
        self._params['type'] = type.value

    def getParams(self):
        return self._params

    def name(self):
        return "Summator"


class Concat():
    """The operator connects the channels with multiple layers."""

    _params = {
        'sequence': ''         # prev nodes through a space
    }
	
    def __init__(self, sequence: str):
        self._params['sequence'] = sequence

    def getParams(self):
        return {}

    def name(self):
        return 'Concat'


class Resize():
    '''
    Change the number of channels
    '''

    _params = {
        'fwdDiap': '0 0',     # diap layer through a space
        'bwdDiap': '0 0'
    }

    def __init__(self, fwdDiap: diap, bwdDiap: diap):
        self._params['fwdDiap'] = fwdDiap.value()
        self._params['bwdDiap'] = bwdDiap.value()

    def getParams(self):
        return self._params

    def name(self):
        return "Resize"


class Crop():
    '''
    ROI clipping in each image of each channel
    '''

    _params = {
        'roi': '0 0 0 0'     # region of interest
    }

    def __init__(self, rct: rect):
        self._params['roi'] = rct.value()

    def getParams(self):
        return self._params

    def name(self):
        return "Crop"


class UserLayer():
    '''
    Custom layer
    '''

    _params = {
        'cbackName': ''
    }

    def __init__(self, cbackName: str):
        self._params['cbackName'] = cbackName

    def getParams(self):
        return self._params

    def name(self):
        return "UserLayer"


class LossFunction():
    '''Error function calculation layer'''

    _params = {
        'loss': lossType.softMaxToCrossEntropy.value,
        'cbackName': ''    # for user cback
    }

    def __init__(self, loss: lossType, userCBackName: str = ''):
        self._params['loss'] = loss.value
        self._params['cbackName'] = userCBackName

    def getParams(self):
        return self._params

    def name(self):
        return "LossFunction"
