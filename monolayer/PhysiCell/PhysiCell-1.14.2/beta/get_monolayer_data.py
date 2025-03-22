#

import sys
import glob
import os
import xml.etree.ElementTree as ET
import math
from pathlib import Path

from pyMCDS import pyMCDS
try:
  import matplotlib
  from matplotlib import gridspec
  import matplotlib.colors as mplc
  from matplotlib.patches import Circle, Ellipse, Rectangle
  from matplotlib.collections import PatchCollection
except:
  print("\n---Error: cannot import matplotlib")
  print("---Try: python -m pip install matplotlib")
#  print("---Consider installing Anaconda's Python 3 distribution.\n")
  raise
try:
  import numpy as np  # if mpl was installed, numpy should have been too.
except:
  print("\n---Error: cannot import numpy")
  print("---Try: python -m pip install numpy\n")
  raise
from collections import deque
try:
  # apparently we need mpl's Qt backend to do keypresses 
  matplotlib.use("Qt5Agg")
#   matplotlib.use("TkAgg")
  import matplotlib.pyplot as plt
except:
  print("\n---Error: cannot use matplotlib's TkAgg backend")
#  print("Consider installing Anaconda's Python 3 distribution.")
  raise

current_idx = 0
nargs = len(sys.argv)-1
print("# args=",nargs)
idx_min = 0
idx_max = 1
if nargs == 2: 
    idx_min = int(sys.argv[1])
    idx_max = int(sys.argv[2])

print("idx_min=",idx_min)
print("idx_max=",idx_max)

use_defaults = True
show_nucleus = 0
axes_min = 0.0
axes_max = 1000  

fig = plt.figure(figsize=(5,5))  # square
ax0 = fig.gca()
#-----------------------------------------------------
def get_cells_info():
    global current_idx, axes_max,cax2,ax0,tvals,xpos,xvals,yvals,exit_rate,nbrs

    frame = current_idx 

    xml_file_root = "output%08d.xml" % frame
    # print("plot_cell_scalar():  current_idx= ",current_idx)
    # print("xml_file_root = ",xml_file_root)
    xml_file = os.path.join('.', xml_file_root)

    if not Path(xml_file).is_file():
        print("ERROR: file not found",xml_file)
        return

    mcds = pyMCDS(xml_file_root, microenv=False, graph=True, verbose=True)
    # total_min = mcds.get_time()  # warning: can return float that's epsilon from integer value
    try:
        df_all_cells = mcds.get_cell_df()
    except:
        print("vis_tab.py: plot_cell_scalar(): error performing mcds.get_cell_df()")
        return
        
    xvals = df_all_cells['position_x']/20  # divide to get units of cell radius (10 diam)
    yvals = df_all_cells['position_y']/20

    exit_rate = df_all_cells['current_cycle_phase_exit_rate']
    nbrs = mcds.data['discrete_cells']['graph']['neighbor_cells']

    csv_fname = f'pc_{current_idx:03}.csv'
    # print("----- csv_fname=",csv_fname)
    with open(csv_fname, 'w') as f:
        f.write('x,y,g,n\n')
        for id in xvals.keys():
            growing_flag = 0
            if exit_rate[id] > 1.e-6:
                growing_flag = 1
            # print("---id=",id)
            f.write(f'{xvals[id]},{yvals[id]},{growing_flag},{len(nbrs[id])}\n')
    f.close()

print("\nNOTE: click in plot window to give it focus before using keys.")
for idx in range(idx_min,idx_max+1):
    current_idx = idx
    print("\n------------------------- current_idx= ",current_idx)
    get_cells_info()

# plt.scatter(xvals, yvals,  s=450, marker='o')
# print("exit_rate min,max = ",exit_rate.min(), exit_rate.max()) 
# plt.show()
