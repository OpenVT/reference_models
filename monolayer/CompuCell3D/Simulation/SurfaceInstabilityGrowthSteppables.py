from cc3d.cpp.PlayerPython import * 
from cc3d import CompuCellSetup
from cc3d.core.PySteppables import *
import numpy as np
exponent=2.0


class ConstraintInitializerSteppable(SteppableBasePy):
    def __init__(self,frequency=1):
        SteppableBasePy.__init__(self,frequency)
        self.track_cell_level_scalar_attribute(field_name='GrowthRate', attribute_name='GrowthRate')
        

    def start(self):

        for cell in self.cell_list:

            cell.targetVolume = 25
            cell.lambdaVolume = 30.0
        
        
class GrowthSteppable(SteppableBasePy):
    def __init__(self,frequency=1):
        SteppableBasePy.__init__(self, frequency)

    def step(self, mcs):
    
        for cell in self.cell_list:
            total_area=0.0
            medium_area=0.0
            if cell:
                for neighbor, common_surface_area in self.get_cell_neighbor_data_list(cell):
                    if not neighbor:
                        medium_area+=common_surface_area
                    
                    total_area+=common_surface_area
                print(cell.id,medium_area,total_area)
                
                if total_area>0:
                    
                    cell.dict['GrowthRate'] = (medium_area/total_area)**exponent
                    
                    
                    cell.targetVolume += 1.0*(medium_area/total_area)**exponent       

        # # alternatively if you want to make growth a function of chemical concentration uncomment lines below and comment lines above        

        # field = self.field.CHEMICAL_FIELD_NAME
        
        # for cell in self.cell_list:
            # concentrationAtCOM = field[int(cell.xCOM), int(cell.yCOM), int(cell.zCOM)]

            # # you can use here any fcn of concentrationAtCOM
            # cell.targetVolume += 0.01 * concentrationAtCOM       

        
class MitosisSteppable(MitosisSteppableBase):
    def __init__(self,frequency=1):
        MitosisSteppableBase.__init__(self,frequency)

    def step(self, mcs):

        cells_to_divide=[]
        for cell in self.cell_list:
            if cell.volume>50:
                cells_to_divide.append(cell)

        for cell in cells_to_divide:

            self.divide_cell_random_orientation(cell)
            # Other valid options
            # self.divide_cell_orientation_vector_based(cell,1,1,0)
            # self.divide_cell_along_major_axis(cell)
            # self.divide_cell_along_minor_axis(cell)

    def update_attributes(self):
        # reducing parent target volume
        self.parent_cell.targetVolume /= 2.0                  

        self.clone_parent_2_child()            

        # for more control of what gets copied from parent to child use cloneAttributes function
        # self.clone_attributes(source_cell=self.parent_cell, target_cell=self.child_cell, no_clone_key_dict_list=[attrib1, attrib2]) 
        
  

        