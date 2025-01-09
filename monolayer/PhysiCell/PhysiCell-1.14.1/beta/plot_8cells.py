# Examples (run from directory containing the .mat files):
#  python plot_8cells.py
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

# current_idx = 0
print("# args=",len(sys.argv)-1)

#for idx in range(len(sys.argv)):
use_defaults = True
show_nucleus = 0
current_idx = 0
axes_min = 0.0
axes_max = 1000  


# if (len(sys.argv) == 5):
#   use_defaults = False
#   kdx = 1
#   show_nucleus = int(sys.argv[kdx])
#   kdx += 1
#   current_idx = int(sys.argv[kdx])
#   kdx += 1
#   axes_min = float(sys.argv[kdx])
#   kdx += 1
#   axes_max = float(sys.argv[kdx])
# elif (len(sys.argv) != 1):
#   print("Please provide either no args or 4 args:")
#   usage_str = "show_nucleus start_index axes_min axes_max"
#   print(usage_str)
#   print("e.g.,")
#   eg_str = "%s 0 0 0 2000" % (sys.argv[0])
#   print(eg_str)
#   sys.exit(1)

#"""
# print("axes_min=",axes_min)
# print("axes_max=",axes_max)
#"""

current_idx = 0
print("current_idx=",current_idx)

#d={}   # dictionary to hold all (x,y) positions of cells

""" 
--- for example ---
In [141]: d['cell1599'][0:3]
Out[141]: 
array([[ 4900.  ,  4900.  ],
       [ 4934.17,  4487.91],
       [ 4960.75,  4148.02]])
"""

fig = plt.figure(figsize=(7,5))
ax0 = fig.gca()

tvals = []
xpos = []
#-----------------------------------------------------
def get_cells_xpos():
    global current_idx, axes_max,cax2,ax0,tvals,xpos

    frame = current_idx 

    xml_file_root = "output%08d.xml" % frame
    # print("plot_cell_scalar():  current_idx= ",current_idx)
    # print("xml_file_root = ",xml_file_root)
    xml_file = os.path.join('.', xml_file_root)

    if not Path(xml_file).is_file():
        print("ERROR: file not found",xml_file)
        return

    mcds = pyMCDS(xml_file_root, microenv=False, graph=False, verbose=False)
    total_min = mcds.get_time()  # warning: can return float that's epsilon from integer value
    try:
        df_all_cells = mcds.get_cell_df()
    except:
        print("vis_tab.py: plot_cell_scalar(): error performing mcds.get_cell_df()")
        return
        
    xvals = df_all_cells['position_x']
    # print("type(xvals)= ",type(xvals))
    # print("xvals= ",xvals)

    # yvals = df_cells['position_y']

    xpos.append(xvals)
    # print("xpos= ",xpos)

    # plt.plot(tvals,xvals)

        # else: 
        #     self.cell_scalar_cbar_combobox.setEnabled(True)
        #     self.discrete_variable = None   # memory leak??
        #     self.discrete_variable_observed = set()
            
    title_str = '8 horizontal cells mechanics test (PhysiCell)'

    axes_min = mcds.get_mesh()[0][0][0][0]
    axes_max = mcds.get_mesh()[0][0][-1][0]

    # cbar2.ax.set_xlabel("pressure", fontsize=9)

    ax0.set_title(title_str, fontsize=12)

    # plot_xmin=plot_ymin= -500
    # plot_xmax=plot_ymax= 500
    # ax0.set_xlim(plot_xmin, plot_xmax)
    # ax0.set_ylim(plot_ymin, plot_ymax)

    # ax0.set_aspect('equal')

    # plt.pause(0.001)  # rwh - yipeee, this causes a redraw!!

#for current_idx in range(40):
#  fname = "snapshot%08d.svg" % current_idx
# plot_cell_scalar()
print("\nNOTE: click in plot window to give it focus before using keys.")

for idx in range(0,25):
    xml_file_root = "output%08d.xml" % idx
    # print("xml_file_root = ",xml_file_root)
    xml_file = os.path.join('.', xml_file_root)
    mcds = pyMCDS(xml_file_root, microenv=False, graph=False, verbose=False)
    total_min = mcds.get_time()  # warning: can return float that's epsilon from integer value
    tvals += [total_min]

for idx in range(0,25):
    current_idx = idx
    # plot_cell_scalar()
    get_cells_xpos()

plt.plot(tvals,xpos,'o-')
ax0.set_xlim(0.0, 120)
ax0.set_xlim(0.0, 60)
ax0.set_xlabel("Time (min)", fontsize=14)
ax0.set_ylabel("Cell center (microns)", fontsize=14)

# fig.canvas.mpl_connect('key_press_event', press)

# keep last plot displayed
#plt.ioff()
plt.show()
