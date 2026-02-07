import matplotlib.pyplot as plt
import pandas as pd

# Read the CSV file
# df = pd.read_csv('save_2runs.csv')
# df = pd.read_csv('save.csv')

csv_file = 'model4_TF.csv'
csv_file = 'model4_physicell.csv'
csv_file = 'save_Jan2026.csv'
csv_file = 'data_Dom.csv'   # Feb 2026
csv_file = 'physicell.csv'   # Feb 2026
df = pd.read_csv(csv_file)

# Extract x and y values
x = df['com_1']
y = df['com_2']

# Create the plot
# plt.plot(x, y)

idx0 = 0
num_per_run = 100  # Jan 2026
num_per_run = 1000  # Feb 2026
print('len(x)= ',len(x))
print('len(x)/num_per_run= ',len(x)/num_per_run)
N = int(len(x)/num_per_run)
# N = 1
#N = 2
# N = 5
N = 1000
print("N= ",N)
for idx in range(N):
    # print("idx0= ",idx0)
    # plt.plot(x[idx0:idx0+100],y[idx0:idx0+100], color='gray', linewidth=0.5)
    # print("x[idx0:idx0+3]= ",x[idx0:idx0+3])
    # print("y[idx0:idx0+3]= ",y[idx0:idx0+3])
    # plt.plot(x[idx0:idx0+num_per_run],y[idx0:idx0+num_per_run], color='gray', linewidth=0.5)
    plt.plot(x[idx0:idx0+num_per_run],y[idx0:idx0+num_per_run], linewidth=0.5)
    # idx0 += 100
    idx0 += num_per_run+1

# Add labels and title
plt.xlabel('x')
plt.ylabel('y')
#plt.title('Persistent cell migration - N runs')
title = f'{csv_file}, {N} runs'
plt.title(title)

# Display the plot
# plt.xlim(40, 700)
# plt.ylim(25, 75)
plt.show()
