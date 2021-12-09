#!/usr/bin/python3

import subprocess
import matplotlib.pyplot as plt

outer = 100
inner = 62

count1 = 10
count2 = 20
count3 = 62

num_bins = 500

x1 = [0] * num_bins
x2 = [0] * num_bins

repetitions = 10000
i = 0

while i < repetitions:
    result = subprocess.check_output(f'./loop_predictor {outer} {inner} {count1} {count2} {count3}', shell=True).decode('utf-8')
    # Check if we got an illegal instruction error
    if "illegal" not in result:
        val1 = int(result.split('\n')[0])
        val2 = int(result.split('\n')[1])
        if val1 >= num_bins:
            val1 = num_bins-1
        if val2 >= num_bins:
            val2 = num_bins-1
        x1[val1] += 1
        x2[val2] += 1
        i += 1

plt.bar(
    range(num_bins),
    height=x1,
    width=1,
    color=(1, 0, 0, 0.7)
)
plt.bar(
    range(num_bins),
    height=x2,
    width=1,
    color=(0, 0, 1, 0.7)
)
plt.xlabel('Difference in misprediction counter before and after the program.')
plt.title(f'Frequency of misprediction counts\n{outer}/{inner}  {count1}/{count2}/{count3}')
plt.legend(["Single nested", "Double nested"])
plt.show()
    