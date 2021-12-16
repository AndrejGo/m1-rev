#!/usr/bin/python3

import subprocess
import matplotlib.pyplot as plt

loop_count = 10000

num_bins = 500

fig, ax = plt.subplots(5, 2)
fig.suptitle(f'Frequency of misprediction counts for different patterns\nof internal branches. Pattern is 11111...0\nOn Firestorm')
fig.subplots_adjust(left=0.1, top=0.8, right=0.9, bottom=0.1, hspace=0.6)

#ax[0].set_title('Icestorm')
#ax[0].set_xlabel('Difference in misprediction counter before and after the program.')

#ax[1].set_title('Firestorm')
#ax[1].set_xlabel('Difference in misprediction counter before and after the program.')

alpha = 0.5
colors = {
    50: (1, 0, 0, alpha),
    51: (1, 0.5, 0, alpha),
    52: (1, 1, 0, alpha),
    53: (0, 1, 0, alpha),
    54: (0, 1, 0.5, alpha),
    55: (0, 1, 1, alpha),
    56: (0, 0.78, 1, alpha),
    57: (0, 0.78, 1, alpha),
    58: (0.58, 0, 1, alpha),
    59: (1, 0, 1, alpha)
}

subplot_indices = {
    50: (0, 0),
    51: (1, 0),
    52: (2, 0),
    53: (3, 0),
    54: (4, 0),
    55: (0, 1),
    56: (1, 1),
    57: (2, 1),
    58: (3, 1),
    59: (4, 1)
}

moduli = colors.keys()

for modulus in moduli:

    ice_bins = [0] * num_bins
    fire_bins = [0] * num_bins

    try:
        subprocess.run(
            f'clang -O0 main.c -DLOOP_COUNT="{loop_count}" -DMODULUS="{modulus}" -o main',
            shell=True
        )
    except Exception as e:
        print('compilation of main.c failed', str(e))

    repetitions = 10000
    for i in range(repetitions):

        try:
            result = subprocess.run(
                './main',
                stdout=subprocess.PIPE
            ).stdout.decode('utf-8')

            core = int(result.split(' ')[0])
            index = int(result.split(' ')[1][:-1])

            if index >= num_bins:
                index = num_bins - 1

            if core == 0:
                # Firestorm
                fire_bins[index] += 1
            else:
                # Icestorm
                ice_bins[index] += 1

        except:
            print('running ./main returned an error')

    ax[subplot_indices[modulus]].bar(
        range(num_bins),
        height=fire_bins,
        width=2,
        color=colors[modulus]
    )

    labels = [modulus]
    handles = [plt.Rectangle((0,0),1,1, color=colors[modulus])]
    ax[subplot_indices[modulus]].legend(handles, labels)

plt.show()