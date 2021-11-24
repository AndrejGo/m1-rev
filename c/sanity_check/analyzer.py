#!/usr/bin/python3

import subprocess
import matplotlib.pyplot as plt

x = []
y = []

loop_count = 100
for i in range(100):
    sum = 0
    for j in range(10):
        result = subprocess.check_output(f'./sanity_check {loop_count}', shell=True).decode('utf-8')
        sum += int(result[:-1])
    x.append(loop_count)
    y.append(sum // 10)
    loop_count += 100

# plotting points as a scatter plot
plt.scatter(
    x,
    y,
    #s=30
    label="Average time of 10 runs",
    color= "red",
    marker= "."
)

plt.xlabel('Loop count')
plt.ylabel('Difference in PMC0 value after and before the loop')
plt.title('Linear relationship of loop count and PMC0 value')
plt.legend()
plt.show()