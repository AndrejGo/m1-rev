#!/usr/bin/python3

import subprocess
import matplotlib.pyplot as plt

num_bins = 4000
bins = [0] * num_bins

repetitions = 10000
i = 0

for core in ['firestorm', 'icestorm']:

    #if core == 'icestorm':
        #subprocess.run('clang -DICESTORM main.c -o main', shell=True)
        #s = input('Launch the occupier and run again.')
    #else:
        #subprocess.run('clang -DFIRESTORM main.c -o main', shell=True)

    while i < repetitions:
        try:
            result = subprocess.check_output(f'./main', shell=True).decode('utf-8')
            # Check if we got an illegal instruction error
            if "illegal" not in result:
                index = int(result.split(' ')[0])

                index -= 1000

                if index >= num_bins:
                    index = num_bins-1

                bins[index] += 1

                i += 1
        except subprocess.CalledProcessError as e:
            print("Exception:", str(e))
            print(i)

xvalues = list(map(lambda x : x + 1000, range(num_bins)))

plt.bar(
    xvalues,
    height=bins,
    width=30,
    color='#007480'
)
plt.xlabel(f'Clock count', fontsize=18)
plt.xticks([1000, 2000, 3000, 4000, 5000], [1000, 2000, 3000, 4000, 5000], fontsize=16)
plt.yticks(fontsize=16)
plt.title('Frequency of clock counts needed\nto complete the loop', fontsize=20)
plt.yscale("log")
#plt.xticks([0, 100, 200, 300, 400, 500, 600, 700, 800], [100, 200, 300, 400, 500, 600, 700, 800, 900])
plt.show()