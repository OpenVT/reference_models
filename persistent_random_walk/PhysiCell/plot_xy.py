import numpy as np
from pyMCDS import pyMCDS

dir='output_1sec/'
mcds = pyMCDS("output00000000.xml", dir, microenv=False, graph=False, verbose=True)
xvals = mcds.data['discrete_cells']['data']['position_x']
yvals = mcds.data['discrete_cells']['data']['position_y']
#print("len(xvals) = ",len(xvals))
print("t = ",mcds.get_time())
print("xvals[0] = ",xvals[0])

xml_file="output00000001.xml"
mcds = pyMCDS(xml_file, dir, microenv=False, graph=False, verbose=True)
xvals = mcds.data['discrete_cells']['data']['position_x']
yvals = mcds.data['discrete_cells']['data']['position_y']
#print("len(xvals) = ",len(xvals))
print("t = ",mcds.get_time())
print("xvals[0] = ",xvals[0])
#zvals = mcds.data['discrete_cells']['data']['position_z']
#xv = xvals[0]         
#yv = yvals[0]
#zv = zvals[idx]
#print("len(xvals) 
