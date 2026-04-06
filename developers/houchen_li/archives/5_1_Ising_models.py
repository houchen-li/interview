import numpy as np
import matplotlib.pyplot as plt
#from scipy.constants import Boltzmann
from matplotlib.animation import FuncAnimation
from time import time

#plt.rc('text', usetex=True)
#plt.rc('font', family='serif')

class IsingModelsType:

    def __init__(self, _size, _steps, _temperature, _H, _J):
        self.data_ = np.empty([_steps, _size, _size])
        self.data_[0,:,:] = np.random.uniform(0.0, 1.0, (_size, _size))
        for i in range(_size):
            for j in range(_size):
                if self.data_[0,i,j] < 0.5:
                    self.data_[0,i,j] = -1
                else:
                    self.data_[0,i,j] = 1
        self.size_ = _size
        self.steps_ = _steps
        self.temperature_ = _temperature
        self.H_ = _H
        self.J_ = _J
        self.energys_ = np.empty(_steps)
        self.magnetizations_ = np.empty(_steps)
        self.delta_E_ = 0.0
        del i
        del j

    def scanEnergys_(self):
        for k in range(self.steps_):
            for i in range(self.size_):
                for j in range(self.size_):
                    self.energys_[k] = self.H_*self.data_[k,i,j]
                    self.energys_[k] -= self.J_*self.data_[k,i,j]*self.data_[k,(i+1)%self.size_,j]
                    self.energys_[k] -= self.J_*self.data_[k,i,j]*self.data_[k,i,(j+1)%self.size_]
        del k
        del i
        del j
    
    def scanMagnetizations_(self):
        self.magnetizations_ = np.average(self.data_, axis=(1, 2))

    def do_step_(self, _k):
        self.data_[_k,:,:] = self.data_[_k-1,:,:]
        for i in range(self.size_):
            for j in range(self.size_):
                self.delta_E_ = -2.0*self.H_*self.data_[_k,i,j]
                self.delta_E_ += 2.0*self.J_*self.data_[_k,i,j]*self.data_[_k,(i-1)%self.size_,j]
                self.delta_E_ += 2.0*self.J_*self.data_[_k,i,j]*self.data_[_k,i,(j-1)%self.size_]
                self.delta_E_ += 2.0*self.J_*self.data_[_k,i,j]*self.data_[_k,i,(j+1)%self.size_]
                self.delta_E_ += 2.0*self.J_*self.data_[_k,i,j]*self.data_[_k,(i+1)%self.size_,j]
                if (self.delta_E_ <= 0.0) or (np.random.uniform(0, 1) < np.exp(-self.delta_E_/(self.temperature_))):
                    self.data_[_k,i,j] *= -1
        del i
        del j

    def evaluate(self):
        for k in range(1, self.steps_):
            self.do_step_(k)
        self.scanEnergys_()
        self.scanMagnetizations_()
        del k

    def plotEnergyEvolution(self, _file_name):
        plt.figure()
        plt.plot(np.arange(self.steps_), self.energys_, label="Energy Evoluton")
        plt.grid()
        plt.legend()
        #plt.xlabel(r"time step \(n\)")
        plt.xlabel("time step n")
        #plt.ylabel(r"total energy \(E\)")
        plt.ylabel("total energy E")
        plt.savefig(_file_name)
        plt.close()

    def plotMagnetizationEvolution(self, _file_name):
        plt.figure()
        plt.plot(np.arange(self.steps_), self.magnetizations_, label="Magnetization Evoluton")
        plt.grid()
        plt.legend()
        #plt.xlabel(r"time step \(n\)")
        plt.xlabel("time step n")
        #plt.ylabel(r"average of magnetization \(M\)")
        plt.ylabel("average of magnetization M")
        plt.savefig(_file_name)
        plt.close()

    def plotLatticeEvolution(self):
        latticeFig_, latticeAx_ = plt.subplots()
        latticePlot_ = plt.imshow(self.data_[0,:,:], cmap=plt.cm.gist_gray, animated=True)
        def initLattice_():
            #latticeAx_.set_xlabel(r"\(x\)")
            latticeAx_.set_xlabel("x")
            #latticeAx_.set_ylabel(r"\(y\)")
            latticeAx_.set_ylabel("y")
            return latticePlot_,
        def updateLattice_(_frame):
            latticePlot_.set_array(self.data_[_frame,:,:])
            return latticePlot_,
        ani = FuncAnimation(latticeFig_, updateLattice_, frames=range(1, self.steps_), init_func=initLattice_, blit=True)
        plt.show()

def main():
    size = 100
    steps = 100
    temperature = 1.8
    H = 0.0
    J = 1.0
    np.random.seed(int(time()))
    s = IsingModelsType(size, steps, temperature, H, J)
    s.evaluate()
    s.plotEnergyEvolution("energy.pdf")
    s.plotMagnetizationEvolution("magnetization.pdf")
    s.plotLatticeEvolution()
    return 0

if __name__ == "__main__":
    main()
