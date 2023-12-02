import random
import sys

# random.seed(10)

low = 0
filename = sys.argv[2]
high = int(sys.argv[3])
n    = int(sys.argv[1])

# print(f'Args are : {sys.argv}')
# samples = random.sample(range(low, high), n)
samples = random.sample(range(low, high), n)

# print(f'Random Samples are {samples}')

file = open(filename, 'w')
file.write(str(n) + '\n')
for sample in samples:
	file.write(str(sample) + '\n')


file.close()