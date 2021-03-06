#!/usr/bin/python3

import subprocess
import matplotlib
import matplotlib.pyplot as plt

matplotlib.rc('xtick', labelsize=16) 
matplotlib.rc('ytick', labelsize=16)
fig, ax = plt.subplots(3, 1, sharex=True)
fig.suptitle("Average number of mispredictions", fontsize=20)
fig.subplots_adjust(bottom=0.15, left=0.15, top=0.8, hspace=0.8)
#fig.tight_layout()

start = 1
upper_limit = 110

core = 'FIRESTORM'

for loop_type in ['for', 'while', 'do_while']:

    results = []

    # For different iteration numbers of the internal loop
    for inner in range(start, upper_limit+1):

        print(inner)

        # Compile the program with this number
        try:
            subprocess.check_output(f'clang -g -O0 -DINNER_COUNT="{inner}" main_{loop_type}.c -o main', shell=True)
        except Exception as e:
            print("Compilation failed")
            exit()

        # Run the program 100 times and calculate the average number of mispredictions
        avg = 0
        num_repetitions = 10
        i = 0
        while(i < num_repetitions):
            try:
                result = subprocess.check_output(f'./main', shell=True).decode('utf-8')
                # Check if we got an illegal instruction error
                if "illegal" not in result:
                    coreid = int(result.split(' ')[0])
                    val = int(result.split(' ')[1][:-1])
                    if val >= 1000 * (2 + inner):
                        print(val)
                        continue
                    if core == "ICESTORM":
                        if coreid == 0:
                            avg += val
                            i += 1
                    else:
                        if coreid == 1:
                            avg += val
                            i += 1
            except Exception as e:
                print(str(e))
        avg //= num_repetitions
        results.append(avg)

    if loop_type == 'for':
        ax[0].plot(range(start, upper_limit+1), results, color='#ff0000', linewidth=3)
    elif loop_type == 'while':
        ax[1].plot(range(start, upper_limit+1), results, color='#ff0000', linewidth=3)
    else:
        ax[2].plot(range(start, upper_limit+1), results, color='#ff0000', linewidth=3)

dark = '#0000ff'
light = '#5485ff'
if core == "FIRESTORM":
    dark = '#ff0000'
    light = '#ff5454'

ax[0].set_title('for loops', size=18, style='italic')
ax[1].set_title('while loops', size=18, style='italic')
ax[2].set_title('do-while loops', size=18, style='italic')
ax[2].set_xlabel("COUNT", fontsize=18)

#plt.subplots_adjust(bottom=0.15, left=0.14, top=0.82)
#plt.plot(range(start, upper_limit+1), results, color=dark, linewidth=3)
#plt.title(f"Average number of mispredictions,\nfor loops.", fontsize=20)
#plt.xlabel("COUNT", fontsize=18)
#plt.xticks(fontsize=16)
#plt.yticks(fontsize=16)
plt.show()

"""
fig, ax = plt.subplots(2)
fig.suptitle(f'Frequency of misprediction counts\nouter: {outer}  inner: {inner}\n4 stressed cores.')
fig.subplots_adjust(left=0.1, top=0.8, right=0.9, bottom=0.1, hspace=0.6)

ax[0].bar(
    range(num_bins),
    height=ice,
    width=4,
    color=(0, 0, 1)
)
ax[0].set_title('Icestorm')
ax[0].set_xlabel('Difference in misprediction counter before and after the program.')

ax[1].bar(
    range(num_bins),
    height=fire,
    width=4,
    color=(1, 0, 0)
)
ax[1].set_title('Firestorm')
ax[1].set_xlabel('Difference in misprediction counter before and after the program.')

plt.show()
"""


