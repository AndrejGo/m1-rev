#!/usr/bin/python3

from os import EX_OK
import subprocess
import matplotlib.pyplot as plt
import time

#plt.gcf().subplots_adjust(left=0.15)

class AsmGen:

    # gen03 generates assembly code with 1 branch per 2 instructions
    def gen_1_2(self, num_branches):
        assembly = ''
        start = 0
        if num_branches % 2 == 0:
            start = 4
            assembly += '''
"       adr x0, .      \\n"
"       add x0, x0, 28 \\n"
"       br x0          \\n"
"       add x0, x0, 32 \\n"
"       br x0          \\n"
"       sub x0, x0, 8  \\n"
"       br x0          \\n"
"       sub x0, x0, 8  \\n"
"       br x0          \\n"
'''#.replace('\n', '')

        else:
            start = 3
            assembly += '''
"       adr x0, .      \\n"
"       add x0, x0, 20 \\n"
"       br x0          \\n"
"       add x0, x0, 24 \\n"
"       br x0          \\n"
"       sub x0, x0, 8  \\n"
"       br x0          \\n"
'''#.replace('\n', '')

        for i in range(start, num_branches, 2):
            assembly += '''
"       add x0, x0, 24 \\n"
"       br x0          \\n"
"       sub x0, x0, 8  \\n"
"       br x0          \\n"
'''#.replace('\n', '')

        assembly += '''
"       add x0, x0, 1 \\n"
"       add x0, x0, 1 \\n"
"       add x0, x0, 1 \\n"
"       add x0, x0, 1 \\n"
'''#.replace('\n', '')

        return assembly

    # gen generates assembly code containing 'num_branches' indirect branches
    # with 'interval' instructions between each branch. 'interval' must be greater
    # or equal to 1
    def gen(self, num_branches, interval=1):

        if interval == 1:
            return self.gen_1_2(num_branches)
        elif interval > 1:

            assembly = f'''
"       adr x0, .       ; Store the current address in x0 \\n"
"       add x0, x0, {(2+interval) * 4}  ;  \\n"
"       br x0           ; Branch to the address in x0 \\n"
'''#.replace('\n', '')

            for i in range(interval-1):
                assembly += '''
"       .word 10        ; Dummy instruction \\n"
'''#.replace('\n', '')

            for i in range(1, num_branches):
                assembly += f'''
"       add x0, x0, {(1+interval) * 4}                    \\n"
"       br x0                             \\n"
'''#.replace('\n', '')
                for i in range(interval-1):
                    assembly += '''
"       .word 10        ; Dummy instruction \\n"
'''#.replace('\n', '')

            assembly += '''
"       add x0, x0, 1 \\n"
'''#.replace('\n', '')

            return assembly

        else:
            print(f'interval {interval} not supported')
            exit(0)


core = "ICESTORM"

loop_count = 1000

x = []
if core == 'ICESTORM':
    x = list(map(lambda x : x * 20, range(30)))
else:
    x = list(map(lambda x : x * 20 + 900, range(100)))

plt.title(f'Number of mispredictions on {core}.')
plt.xlabel('Number of indirect branches in the loop body.')
plt.ylabel('Average number of mispredictions over 100 runs of the loop (with 1000 iterations per loop).', wrap="true")
plt.ticklabel_format(axis="y", style="sci", scilimits=(0,0))
#fig, ax = plt.subplots(4, 4, sharex=True)
#fig.suptitle(f'Number of mispredictions on {core}.')
#fig.text(0.5, 0.04, 'Number of indirect branches in the loop body.', ha='center')
#fig.text(0.04, 0.5, 'Average number of mispredictions.', va='center', rotation='vertical')

colors = [
    '#800000',
    '#9A6324',
    '#469990',
    '#000075',
    '#e6194B',
    '#f58231',
    '#ffe119',
    '#3cb44b',
    '#42d4f4',
    '#4363d8',
    '#f032e6',
    '#a9a9a9',
    '#fabed4',
    '#fffac8',
    '#aaffc3',
    '#dcbeff'
]

intervals = range(49, 65)
for interval in intervals:
    print(f'Interval: {interval}')
    y = []
    #t = []

    for num_branches in x:

        print(f'Num_branches: {num_branches}')

        ag = AsmGen()
        assembly = ag.gen(num_branches, interval)

        #assembly = assembly.replace('"', '\\"')
        #assembly = assembly.replace('\n', '\\n')

        register_list = '"x0"'
        #clobered = register_list.replace('"', '\\"')

        ccode = ''
        # The assembly code got too large to be passed as command line arguments
        with open('main.c.template', 'r') as cfile:
            ccode = cfile.read()

        ccode = ccode.replace('ASM_CODE', assembly)
        ccode = ccode.replace('CLOBERED', register_list)
        ccode = ccode.replace('LOOP_COUNT', str(loop_count))

        with open('main.c', 'w') as cfile:
            cfile.write(ccode)

        try:
            subprocess.run(
                f'gcc -g main.c -D{core} -o main',
                shell=True
            )
        except Exception as e:
            print('compilation of main.c failed', str(e))

        average = 0
        avg_t = 0
        j = 0
        while j < 100:
            result = ''
            try:
                result = subprocess.check_output('./main').decode('utf-8')
                core_id = int(result.split(' ')[0])
                num = int(result.split(' ')[1])
                #time = int(result.split(' ')[2][:-1])
                if num < 1000 * (num_branches+10):
                    if core == "ICESTORM":
                        if core_id == 1:
                            average += num
                            j += 1
                    else:
                        if core_id == 0:
                            average += num
                            j += 1
                    #avg_t += time
            except Exception as e:
                print(f'running ./main returned an error: {str(e)}')
                print(result)
                None
        
        average //= 100
        y.append(average)
        #avg_t //= 100
        #t.append(avg_t)

    plt.plot(x, y, color=colors[interval-49], linewidth=3)
    #x_index = (interval-1) // 4
    #y_index = (interval-1) % 4

    #ax[x_index, y_index].plot(x, y, color=('blue' if core == 'ICESTORM' else 'red'), label=interval)
    #ax[x_index, y_index].ticklabel_format(axis="y", style="sci", scilimits=(0,0))
    #ax[x_index, y_index].legend(loc='upper left')

plt.legend(intervals, loc='upper left', ncol=2)
plt.show()
