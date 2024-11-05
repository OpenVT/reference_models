
from cc3d import CompuCellSetup
        


from SurfaceInstabilityGrowthSteppables import ConstraintInitializerSteppable

CompuCellSetup.register_steppable(steppable=ConstraintInitializerSteppable(frequency=1))




from SurfaceInstabilityGrowthSteppables import GrowthSteppable

CompuCellSetup.register_steppable(steppable=GrowthSteppable(frequency=1))




from SurfaceInstabilityGrowthSteppables import MitosisSteppable

CompuCellSetup.register_steppable(steppable=MitosisSteppable(frequency=1))


CompuCellSetup.run()
