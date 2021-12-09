#!/usr/bin/python3

import subprocess
import matplotlib.pyplot as plt

num_bins = 900
bins = [0] * num_bins

repetitions = 1000
i = 0

while i < repetitions:
    try:
        result = subprocess.check_output(f'./main', shell=True).decode('utf-8')
        # Check if we got an illegal instruction error
        if "illegal" not in result:
            index = int(result.split(' ')[0])

            index -= 100
            if index >= num_bins:
                index = num_bins-1

            if index < 0:
                index = 0

            bins[index] += 1

            i += 1
    except subprocess.CalledProcessError as e:
        print("Exception:", str(e))

xvalues = list(map(lambda x : x, range(num_bins)))

plt.bar(
    range(num_bins),
    height=bins,
    width=4,
    color='red'
)
plt.xlabel(f'Clock count')
plt.title('Frequency of clocks needed to complete the loop')
plt.yscale("log")
plt.xticks([0, 100, 200, 300, 400, 500, 600, 700, 800], [100, 200, 300, 400, 500, 600, 700, 800, 900])
plt.show()