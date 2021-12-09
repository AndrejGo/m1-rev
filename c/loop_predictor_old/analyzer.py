#!/usr/bin/python3

import subprocess
import matplotlib.pyplot as plt

x = [0]

for i in range(1000):
    result = subprocess.check_output(f'./bmis', shell=True).decode('utf-8')
    val = int(result[:-1])
    if val not in [0, 1]:
        val = 2
    x[val] += 1

plt.bar(
    ['0', '1', 'other'],
    height=x
)
plt.xlabel('Difference of PMC5 before and after the program')
plt.ylabel('Frequency')
plt.title('PMC5 with event number 0xcb')
plt.legend()
plt.show()
    