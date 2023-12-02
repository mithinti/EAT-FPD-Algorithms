import matplotlib.pyplot as plt

# Function to read data from a file
def read_data(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
        data = [tuple(map(float, line.split())) for line in lines[:9]]
    # sorted_data = sorted(data, key=lambda x: x[0]) 
    return zip(*data)

# List of file paths
# file_paths = ['Chicago.txt','London.txt','NewYork.txt','Petersburg.txt','Sweden.txt','Switzerland.txt']
#file_paths = ['Sweden.txt','LosAngels.txt','London.txt','Switzerland.txt','Madrid.txt']
# file_paths = ['LosAngels.txt','Petersburg.txt']
file_paths = ['Chicago.txt','NewYork.txt','Paris.txt']
# Plotting each file
for file_path in file_paths:
    time, frequency = read_data(file_path)
    plt.plot(time, frequency, label=file_path, marker='o')
    # print(frequency,"ss", time, "dd"

# Adding labels and legend
plt.xlabel('Time')
plt.ylabel('Frequency')
plt.legend()
plt.title('Line Plot of Frequency over Time')
plt.grid(True)

# Display the plot
plt.show()

