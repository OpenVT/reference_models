import matplotlib.pyplot as plt
import pandas as pd

# Read the CSV file
df = pd.read_csv('run0.csv')

# Extract x and y values
x = df['com_1']
y = df['com_2']

# Create the plot
plt.plot(x, y)

# Add labels and title
plt.xlabel('x')
plt.ylabel('y')
plt.title('Persistent cell migration')

# Display the plot
plt.show()
