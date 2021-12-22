#!/usr/bin/python3

from os import EX_OK
import subprocess
import matplotlib.pyplot as plt

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
        elif interval > 1:

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

        else:
            print(f'interval {interval} not supported')
            exit(0)

    # gen01 generates assembly code with 1 branch per 3 instructions
    def gen01(self, num_branches):
        assembly = '''
"       adr x0, .       ; Store the current address in x0 \\n"
"       add x0, x0, 16  ; The target is 4 instruction after the above one \\n"
"       br x0           ; Branch to the address in x0 \\n"
"       sub x0, x0, 1   ; Dummy instruction \\n"
'''.replace('\n', '')

        for i in range(1, num_branches):
            assembly += '''
"       add x0, x0, 12                    \\n"
"       br x0                             \\n"
"       sub x0, x0, 1 ; dummy instruction \\n"
'''.replace('\n', '')

        assembly += '''
"       add x0, x0, 1 \\n"
'''.replace('\n', '')

        return assembly

    # gen02 generates assembly code with 2 branches per 5 instructions
    def gen02(self, num_branches):
        assembly = '''
"       adr x0, .       ; Store the current address in x0 \\n"
"       adr x1, .       ; Store the current address in x0 \\n"

"       add x0, x0, 28  ; Change x0 to point to the next instruction that adds to x0 \\n"
"       add x1, x1, 20  ; Change x1 so that it points to the next br x0 instruction  \\n"
"       add x1, x1, 20  ;                                                            \\n"

"       br x1           ; Jump to the next br x0 instruction    \\n"
"       br x0           ; This instruction will not be executed \\n"
'''.replace('\n', '')

        for i in range((num_branches-2)//2):
            assembly += '''
"       add x0, x0, 20 ; Change x0 to point to the next add x0 instruction. \\n"
"       add x1, x1, 10 ; Change x1 to point to the next br x1 instruction   \\n"
"       add x1, x1, 10 ;                                                    \\n"

"       br x1          ; Branch to the next br x0 instruction \\n"
"       br x0          ; We come here from the previous br x1 instruction. Branch to the above add x0. \\n"
'''.replace('\n', '')

        assembly += '''
"       add x0, x0, 20 ; Dummy instruction, never executed. \\n"
"       add x1, x1, 10 ; Dummy instruction, never executed. \\n"
"       add x1, x1, 10 ; Dummy instruction, never executed. \\n"
    
"       br x1          ; Dummy instruction, never executed. \\n"
"       add x0, x0, 28 ; We come here from the last br x1 instruction. Change x0 to point to the last instuction in this block. \\n"
"       br x0          ; \\n"

"       add x0, x0, 20 ; Dummy instruction, never executed. \\n"

"       add x0, x0, 20 ; Dummy instruction, just acts as a target for the last branch. \\n"
'''.replace('\n', '')

        return assembly

    # gen03 generates assembly code with 1 branch per 2 instructions
    def gen03(self, num_branches):
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

    # This is a bad chunk of assembly, since all branches have the same target
    # gen04 generates assembly code with 1 branch per 1 instruction
    def gen04(self, num_branches):
        assembly = '''
"       adr x0, .       \\n"
"       adr x1, .       \\n"

"       add x0, x0, 16  \\n"
"       add x1, x1, 20  \\n"
 
"       add x1, x1, 4  ; x0 always points here \\n"
"       br x1                                 \\n"

"       add x0, x0, 1  ; dummy instruction, never executed \\n"
'''.replace('\n', '')

        for i in range(num_branches):
            assembly += '''
"       br x0          \\n"
'''.replace('\n', '')

        assembly += '''
"       add x0, x0, 1       \\n"
'''.replace('\n', '')

        return assembly



core = "ICESTORM"

loop_count = 1000

x = list(map(lambda x : x + 450, range(2)))
t = []
y = []
for num_branches in x:

    print(num_branches)

    ag = AsmGen()
    assembly = ag.gen(10, 3)

    print(assembly.replace('""', '\n'))
    exit(0)

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
