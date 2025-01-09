
from cc3d import CompuCellSetup
        


from PressureRelaxationSteppables import ConstraintInitializerSteppable

CompuCellSetup.register_steppable(steppable=ConstraintInitializerSteppable(frequency=1))




from PressureRelaxationSteppables import GrowthSteppable

CompuCellSetup.register_steppable(steppable=GrowthSteppable(frequency=1))


CompuCellSetup.run()
