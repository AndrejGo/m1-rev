#!/usr/bin/python3

from abc import abstractproperty
from os import EX_OK
import subprocess
import matplotlib.pyplot as plt
from matplotlib import colors

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
'''.replace('\n', '')

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
'''.replace('\n', '')

        for i in range(start, num_branches, 2):
            assembly += '''
"       add x0, x0, 24 \\n"
"       br x0          \\n"
"       sub x0, x0, 8  \\n"
"       br x0          \\n"
'''.replace('\n', '')

        assembly += '''
"       add x0, x0, 1 \\n"
"       add x0, x0, 1 \\n"
"       add x0, x0, 1 \\n"
"       add x0, x0, 1 \\n"
'''.replace('\n', '')

        return assembly

    # gen
    def gen(self, num_branches, interval=1):

        if interval == 1:
            return self.gen_1_2(num_branches)
        elif interval == 2:

            assembly = f'''
"       adr x0, .       ; Store the current address in x0 \\n"
"       add x0, x0, {(2+interval) * 4}  ;  \\n"
"       br x0           ; Branch to the address in x0 \\n"
'''.replace('\n', '')

            for i in range(interval-1):
                assembly += '''
"       .word 10        ; Dummy instruction \\n"
'''.replace('\n', '')

            for i in range(1, num_branches):
                assembly += f'''
"       add x0, x0, {(1+interval) * 4}                    \\n"
"       br x0                             \\n"
'''.replace('\n', '')
                for i in range(interval-1):
                    assembly += '''
"       .word 10        ; Dummy instruction \\n"
'''.replace('\n', '')

            assembly += '''
"       add x0, x0, 1 \\n"
'''.replace('\n', '')

            return assembly

#"off:   .quad {(2+interval) * 4}    \\n"
#"       ldr x3, off \\n"

        elif interval >= 3:
            assembly = f'''
"       mov x3, {interval}      \\n"
"       add x3, x3, 2           \\n"
"       mov x4, 4               \\n"
"       mul x3, x3, x4           \\n"

"       sub x4, x3, 4           \\n"

"       adr x0, .       ; Store the current address in x0 \\n"
"       add x0, x0, x3  \\n"
"       br x0           ; Branch to the address in x0 \\n"
'''.replace('\n', '')

            for i in range(interval-1):
                assembly += '''
"       .word 10        ; Dummy instruction \\n"
'''.replace('\n', '')

            for i in range(1, num_branches):
                assembly += f'''
"       add x0, x0, x4                    \\n"
"       br x0                             \\n"
'''.replace('\n', '')
                for i in range(interval-1):
                    assembly += '''
"       .word 10        ; Dummy instruction \\n"
'''.replace('\n', '')

            assembly += '''
"       add x0, x0, 1 \\n"
'''.replace('\n', '')

            return assembly

        else:
            print(f'interval {interval} not supported')
            exit(0)

core = "FIRESTORM"

def compile(params):
    cfile = ''
    with open('main.c.template', 'r') as f:
        cfile = f.read()

    cfile = cfile.replace('ASM_CODE', params[1])
    cfile = cfile.replace('CLOBERED', params[2])
    cfile = cfile.replace('LOOP_COUNT', str(params[3]))

    with open('main.c', 'w') as f:
        f.write(cfile)

    try:
        subprocess.run(
            f'gcc -g main.c -D{params[0]} -o main',
            shell=True
        )
    except Exception as e:
        print('compilation of main.c failed', str(e))

#x = [1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7, 7, 2]
#y = [1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7, 7, 31]
#plt.hist2d(x, y, bins=[[1, 2, 3, 4, 5, 6, 7], [2, 4, 8, 16, 32]])
#plt.show()
#exit(0)

loop_count = 1000

branch_num_array = [16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768]
interval_array = [2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048]

#branch_num_array = [16, 32]
#interval_array = [2, 4, 8]

data = []
for _ in interval_array:
    temp = []
    for _ in range(len(branch_num_array)):
        temp.append(0)
    data.append(temp)

x = []
y = []
for a in range(len(branch_num_array)):
    num_branches = branch_num_array[a]
    for b in range(len(interval_array)):
        interval = interval_array[b]

        if(num_branches * interval >= 262144):
            data[len(interval_array) - b-1][a] = 0
            continue

        print(f'{num_branches} x {interval}')

        ag = AsmGen()
        assembly = ag.gen(num_branches, interval)
        #assembly = assembly.replace('"', '\\"')
        #assembly = assembly.replace('\n', '\\n')
        assembly = assembly.replace('""', '"\n"')

        register_list = '"x0", "x1", "x3", "x4"'
        #clobered = register_list.replace('"', '\\"')

        compile((core, assembly, register_list, loop_count))

        """
        try:
            #print(f'gcc main.c -DASM_CODE="{assembly}" -DCLOBERED="{clobered}" -DLOOP_COUNT={loop_count} -o main')
            subprocess.run(
                f'gcc -g main.c -D{core} -DASM_CODE="{assembly}" -DCLOBERED="{clobered}" -DLOOP_COUNT={loop_count} -o main',
                shell=True
            )
        except Exception as e:
            print('compilation of main.c failed', str(e))
        """

        average = 0
        j = 0
        while j < 100:
            try:
                result = subprocess.run(
                    './main',
                    stdout=subprocess.PIPE
                )
                core_id = int(result.stdout.decode('utf-8').split(' ')[0])
                num = int(result.stdout.decode('utf-8').split(' ')[1])
                time = int(result.stdout.decode('utf-8').split(' ')[2][:-1])
                if num < 1000 * (num_branches+10):
                    if core == "ICESTORM":
                        if core_id == 1:
                            average += num
                            j += 1
                    else:
                        if core_id == 0:
                            average += num
                            j += 1
            except Exception as e:
                print(f'({num_branches} {interval}) running ./main returned an error: {str(e)}\n{result.stdout.decode()}')

        average //= 100
        data[len(interval_array) - b-1][a] = average
        #for i in range(average):
        #    x.append(num_branches)
        #    y.append(interval)

#branch_num_array.append(branch_num_array[-1] * 2)
#interval_array.append(interval_array[-1] * 2)

print(data)
#plt.xscale('log', base=2)
#plt.yscale('log', base=2)
#plt.hist2d(x, y, bins=[branch_num_array, interval_array], cmap=plt.cm.jet)
plt.imshow(data, cmap='magma', norm=colors.LogNorm())
#plt.imshow(data, cmap='cividis')
#plt.colorbar()
plt.colorbar(pad=0.03)
plt.title('Average number of mispredictions', fontsize=20)
plt.xlabel('log₂(number of branches) - 4', fontsize=18)
plt.ylabel('Address interval', fontsize=18)
plt.xticks(range(len(branch_num_array)), fontsize=16)
plt.yticks(range(len(interval_array)), interval_array[::-1], fontsize=16)
plt.show()
exit(0)

loop_count = 1000

x = list(map(lambda x : x + 450, range(2)))
t = []
y = []

for num_branches in x:

    print(num_branches)

    ag = AsmGen()
    assembly = ag.gen(10, 3)

    assembly = assembly.replace('"', '\\"')
    assembly = assembly.replace('\n', '\\n')

    register_list = '"x0", "x1"'
    clobered = register_list.replace('"', '\\"')

    try:
        #print(f'gcc main.c -DASM_CODE="{assembly}" -DCLOBERED="{clobered}" -DLOOP_COUNT={loop_count} -o main')
        subprocess.run(
            f'gcc -g main.c -D{core} -DASM_CODE="{assembly}" -DCLOBERED="{clobered}" -DLOOP_COUNT={loop_count} -o main',
            shell=True
        )
    except Exception as e:
        print('compilation of main.c failed', str(e))

    average = 0
    avg_t = 0
    j = 0
    while j < 100:
        try:
            result = subprocess.run(
                './main',
                stdout=subprocess.PIPE
            )
            core_id = int(result.stdout.decode('utf-8').split(' ')[0])
            num = int(result.stdout.decode('utf-8').split(' ')[1])
            time = int(result.stdout.decode('utf-8').split(' ')[2][:-1])
            if num < 1000 * (num_branches+10):
                if core == "ICESTORM":
                    if core_id == 1:
                        average += num
                        j += 1
                else:
                    if core_id == 0:
                        average += num
                        j += 1
                avg_t += time
        except Exception as e:
            print(f'running ./main returned an error: {str(e)}')
            None
        
    average //= 100
    y.append(average)
    avg_t //= 100
    t.append(avg_t)

plt.title(f'Number of mispredictions on {core}.')
plt.plot(x, y, color=('blue' if core == 'ICESTORM' else 'red'))
#plt.plot(x, t, color='#214d00')
plt.xlabel('Number of indirect branches in the loop body.')
plt.ylabel('Average number of mispredictions over 100 runs of the loop (with 1000 iterations per loop).', wrap="true")
plt.ticklabel_format(axis="y", style="sci", scilimits=(0,0))
plt.show()
