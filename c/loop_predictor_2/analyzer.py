#!/usr/bin/python3

import subprocess
import matplotlib.pyplot as plt

outer = 100
inner = 62

num_bins = 500

ice = [0] * num_bins
fire = [0] * num_bins
times = [0] * num_bins

repetitions = 50000
i = 0

while i < repetitions:
    try:
        result = subprocess.check_output(f'./loop_predictor {outer} {inner}', shell=True).decode('utf-8')
        # Check if we got an illegal instruction error
        if "illegal" not in result:
            if "not" not in result:
                core = int(result.split(' ')[0])
                val = int(result.split(' ')[1])
                time = int(result.split(' ')[2][:-1])

                if time > num_bins - 1:
                    time = num_bins - 1
                times[time] += 1

                if val >= num_bins:
                    val = num_bins-1

                if core == 0:
                    ice[val] += 1
                else:
                    fire[val] += 1
                
                i += 1
    except BaseException as e:
        print(str(e))
        i += 1

fig, ax = plt.subplots(2)
fig.suptitle(f'Frequency of misprediction counts\nouter: {outer}  inner: {inner}\n4 stressed cores.')
fig.subplots_adjust(left=0.1, top=0.8, right=0.9, bottom=0.1, hspace=0.6)

ax[0].bar(
    range(num_bins),
    height=ice,
    width=2,
    color=(0, 0, 1)
)
ax[0].set_title('Icestorm')
ax[0].set_xlabel('Difference in misprediction counter before and after the program.')

ax[1].bar(
    range(num_bins),
    height=fire,
    width=2,
    color=(1, 0, 0)
)
ax[1].set_title('Firestorm')
ax[1].set_xlabel('Difference in misprediction counter before and after the program.')

plt.show()
    
