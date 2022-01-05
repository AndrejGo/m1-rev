#!/usr/bin/python3

import subprocess
import matplotlib
import matplotlib.pyplot as plt

matplotlib.rc('xtick', labelsize=16) 
matplotlib.rc('ytick', labelsize=16) 

loop_count_base = 1000

num_bins = 70

#core = "FIRESTORM"

fig, ax = plt.subplots(2, 1, sharex=True)
fig.suptitle("Average number of mispredictions", fontsize=20)
fig.subplots_adjust(bottom=0.15, left=0.14, top=0.90)

bins_with_extra_branches = num_bins * [0]

for core in ["ICESTORM", "FIRESTORM"]:

    bins = num_bins * [0]

    for modulus in range(1, num_bins+1):

        print(modulus)
        loop_count = loop_count_base * modulus
        extra_branches = 0

        if(extra_branches == 0):
            try:
                subprocess.run(
                    f'clang -O0 main.c \
                    -D{core} \
                    -DLOOP_COUNT="{loop_count}" \
                    -DMODULUS="{modulus}" \
                    -o main',
                    shell=True
                )
            except Exception as e:
                print('compilation of main.c failed', str(e))
        else:
            try:
                subprocess.run(
                    f'clang -O0 main.c \
                    -D{core} \
                    -DLOOP_COUNT="{loop_count}" \
                    -DMODULUS="{modulus}" \
                    -DEXTRA_INSTRUCTIONS \
                    -o main',
                    shell=True
                )
            except Exception as e:
                print('compilation of main.c failed', str(e))

        repetitions = 100
        avg = 0

        i = 0
        while i < repetitions:
            try:
                result = subprocess.run(
                    './main',
                    stdout=subprocess.PIPE
                ).stdout.decode('utf-8')

                profiled_core = int(result.split(' ')[0])
                index = int(result.split(' ')[1][:-1])

                if core == "ICESTORM":
                    if profiled_core == 1:
                        avg += index
                        i += 1
                else:
                    if profiled_core == 0:
                        avg += index
                        i += 1

            except:
                print('running ./main returned an error')

        avg //= repetitions
        if extra_branches == 0:
            bins[modulus-1] = avg
        else:
            bins_with_extra_branches[modulus-1] = avg

    if core == 'ICESTORM':
        ax[0].plot(range(1, num_bins+1), bins, color='#0000ff', linewidth=3)
    else:
        ax[1].plot(range(1, num_bins+1), bins, color='#ff0000', linewidth=3)

dark = '#0000ff'
light = '#5485ff'
if core == "FIRESTORM":
    dark = '#ff0000'
    light = '#ff5454'

#plt.plot(range(1, num_bins+1), bins, color=dark, linewidth=3)
#plt.plot(range(1, num_bins+1), bins_with_extra_branches, color=light, linewidth=3, linestyle=':')
#plt.title("Average number of mispredictions", fontsize=20)
#plt.xlabel("Length of pattern (m)", fontsize=18)
#plt.xticks(fontsize=16)
#plt.yticks(fontsize=16)
ax[1].set_xlabel("Length of pattern (m)", fontsize=18)

plt.show()