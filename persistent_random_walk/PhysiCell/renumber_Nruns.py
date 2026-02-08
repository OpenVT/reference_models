import pandas as pd

# Read the CSV file
# df = pd.read_csv('save_2runs.csv')
# df = pd.read_csv('save.csv')

csv_file = 'model004_physicell_dtmech_1.csv'
df = pd.read_csv(csv_file)

# time,id,com_1,com_2
# 0,0,50,50
# 10,0,49.9868,50.0027

time = df['time']
ids = df['id']
print("len(ids) = ",len(ids))
x = df['com_1']
y = df['com_2']

N = 3
N = 1000
print("N= ",N)
kdx = 0
if True:
  for idx in range(N):
    for jdx in range(N+1):
        # ids[kdx] = idx
        print(f'{time[kdx]},{idx},{x[kdx]},{y[kdx]}')
        kdx += 1

# df = pd.DataFrame({'time': time, 'id': ids, 'com_1':x, 'com_2:':y })

# df.to_csv("new.csv", index=False)
