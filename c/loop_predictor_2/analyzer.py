#!/usr/bin/python3

import subprocess
import matplotlib.pyplot as plt

outer = 100
inner = 62

num_bins = 500

ice = [0] * num_bins
fire = [0] * num_bins
times = [0] * num_bins

repetitions = 1000
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

plt.bar(
    range(num_bins),
    height=ice,
    width=2,
    color=(0, 0, 1, 0.5)
)
plt.bar(
    range(num_bins),
    height=fire,
    width=2,
    color=(1, 0, 0, 0.5)
)
#plt.bar(
#    range(num_bins),
#    height=times,
#    width=2,
#    color=(0, 1, 0, 0.5)
#)
plt.xlabel('Difference in misprediction counter before and after the program.')
plt.title(f'Frequency of misprediction counts\nouter: {outer}  inner: {inner}')
plt.legend()
#plt.savefig('Results/result.png')
plt.show()
    
