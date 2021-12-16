#!/usr/bin/python3

import subprocess
import matplotlib.pyplot as plt

count1 = 5
count2 = 15
count3 = 20
count4 = 61

num_bins = 3000

x = [0] * num_bins

repetitions = 10000
i = 0

while i < repetitions:
    result = subprocess.check_output(f'./loop_predictor {count1} {count2} {count3} {count4}', shell=True).decode('utf-8')
    # Check if we got an illegal instruction error
    if "illegal" not in result:
        val = int(result[:-1])
        if val >= num_bins:
            val = num_bins-1
        x[val] += 1
        i += 1

non_zero = -1
max = 0
max_i = 0
for i in range(num_bins):
    if x[i] > max:
        max = x[i]
        max_i = i
    if x[i] != 0 and non_zero == -1:
        non_zero = i

print(f"max: {max_i}, first nonzero: {non_zero}")

plt.bar(
    range(num_bins),
    height=x,
    width=10
)
plt.xlabel('Difference in misprediction counter before and after the program.')
plt.title(f'Frequency of misprediction counts\ncount1: {count1}  count2: {count2}  count3: {count3}  count4: {count4}')
plt.legend()
plt.show()
    