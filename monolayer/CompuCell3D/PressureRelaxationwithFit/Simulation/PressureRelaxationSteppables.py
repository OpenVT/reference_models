from cc3d.cpp.PlayerPython import * 
from cc3d import CompuCellSetup
from cc3d.core.PySteppables import *
import numpy as np
from scipy.optimize import curve_fit



class ConstraintInitializerSteppable(SteppableBasePy):
    def __init__(self,frequency=1):
        SteppableBasePy.__init__(self,frequency)
        self.track_cell_level_scalar_attribute(field_name='Pressure', attribute_name='Pressure')
        

    def start(self):

        for cell in self.cell_list:

            cell.targetVolume = 50
            cell.lambdaVolume = 2.0
            
            cell.dict['Pressure'] = -cell.pressure
            
 
       
        
class GrowthSteppable(SteppableBasePy):
    def __init__(self,frequency=1):
        SteppableBasePy.__init__(self, frequency)
    
    def start(self):
        
        self.shared_steppable_vars['pressure_series']=np.empty((0,2))
        self.shared_steppable_vars['position_series']=np.empty((0,2))
        
        self.plot_win = self.add_new_plot_window(title='Pressure vs Position',
                                                 x_axis_title='Position',
                                                 y_axis_title='Man Pressure', x_scale_type='linear', y_scale_type='linear',
                                                 grid=False)
        
        self.plot_win.add_plot("Pressure", style='Dots', color='red', size=5)      
        self.plot_win2 = self.add_new_plot_window(title='Leading Edge Position',
                                                 x_axis_title='Time (MCS)',
                                                 y_axis_title='Leading Edge', x_scale_type='linear', y_scale_type='linear',
                                                 grid=False)
        
        self.plot_win2.add_plot("Position", style='Dots', color='green', size=5)
        self.plot_win3 = self.add_new_plot_window(title='Max Pressure',
                                                 x_axis_title='Time (MCS)',
                                                 y_axis_title='Max Pressure', x_scale_type='linear', y_scale_type='linear',
                                                 grid=False)
        
        self.plot_win3.add_plot("Pmax", style='Dots', color='orange', size=5)
        self.plot_win3.add_plot("Fit", style='Dots', color='yellow', size=5)


    def step(self, mcs):
        if mcs == 500:
            # iterating over cells of type 1
            # list of  cell types (capitalized)
            for cell in self.cell_list_by_type(self.CELL1):
                cell.targetVolume*=2.0
        
        # iterating over all cells in simulation  
        pmax=0.0
        for cell in self.cell_list:
            cell.dict['Pressure'] = -cell.pressure
            pmax=max(pmax,-cell.pressure)
        self.plot_win3.add_data_point("Pmax", mcs, pmax)

        
        pressure=empty_array = np.zeros((self.dim.x, 2))
        if not mcs % 10 and mcs >0 : #Plot pressure vs position
            for x, y, z in self.every_pixel():
                cell=self.cell_field[x, y, z]
                
                if cell:
                    pressure[x,0]-=self.cell_field[x, y, z].pressure
                    pressure[x,1]+=1 #count number of voxels at this postion
                    if abs(cell.pressure)> 1000:
                        print('error, ill defined pressure', cell.id, x, y, z, cell.pressure)
                        
        position=0
        for x in range(0,self.dim.x):
            if pressure[x,1]>0:
                position=x # keep track of leading edge position
            # arguments are (name of the data series, x, y)
                self.plot_win.add_data_point("Pressure", x, pressure[x,0]/float(pressure[x,1]))
            
        self.plot_win2.add_data_point("Position", mcs, position)
    
        # now append the pressure and position info to do the fits        

        if mcs>500: 
            self.shared_steppable_vars['pressure_series']=np.vstack((self.shared_steppable_vars['pressure_series'],(mcs,pmax)))
            self.shared_steppable_vars['position_series']=np.vstack((self.shared_steppable_vars['position_series'],(mcs,position)))
        
        #at time 1200 do the fits
    
        def exponential_func(x, a, b):
          return a * np.exp(-b * x)    
            
        if mcs == 1100:
            xdata = self.shared_steppable_vars['pressure_series'][:,0]-500.0  # Replace 'x_column' with the name of your x-axis column
            ydata = self.shared_steppable_vars['pressure_series'][:,1]  # Replace 'y_column' with the name of your y-axis column
            print(xdata,ydata)
            #initial_guess = [-ydata[1], 0.01, ydata[1]]  # Adjust values as needed
            initial_guess = [ydata[1], 0.01] 
            popt, pcov = curve_fit(exponential_func, xdata, ydata, p0=initial_guess)
            a,b = popt
            #a,b,c = popt
            self.shared_steppable_vars['a']=a
            self.shared_steppable_vars['b']=b
            #self.shared_steppable_vars['c']=c
            print('popt',popt)
            timescale = 1.0 / popt[1]
            timescale_uncertainty = np.sqrt(pcov[1, 1]) / (popt[1]**2) # Uncertainty propagation
            print("timescale",timescale,"+/-",timescale_uncertainty)
            
            for i in range(0,len(xdata)):
                #self.plot_win3.add_data_point("Fit", xdata[i]+500.0, exponential_func(xdata[i],a,b,c))
                self.plot_win3.add_data_point("Fit", xdata[i]+500.0, exponential_func(xdata[i],a,b))

        if mcs > 1110 and not mcs%10:
            #self.plot_win3.add_data_point("Fit", mcs, exponential_func(mcs-500,self.shared_steppable_vars['a'],self.shared_steppable_vars['b'],self.shared_steppable_vars['c']))
            self.plot_win3.add_data_point("Fit", mcs, exponential_func(mcs-500,self.shared_steppable_vars['a'],self.shared_steppable_vars['b']))

        #for cell in self.cell_list:
            #cell.targetVolume += 1        

        # # alternatively if you want to make growth a function of chemical concentration uncomment lines below and comment lines above        

        # field = self.field.CHEMICAL_FIELD_NAME
        
        # for cell in self.cell_list:
            # concentrationAtCOM = field[int(cell.xCOM), int(cell.yCOM), int(cell.zCOM)]

            # # you can use here any fcn of concentrationAtCOM
            # cell.targetVolume += 0.01 * concentrationAtCOM       

        