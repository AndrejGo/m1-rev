#!/usr/bin/python3

import subprocess
import matplotlib.pyplot as plt

# Prepare assembly code
assembly = '''
"       mov x1, 0       ;                                 \\n"
"       adr x0, .       ; Store the current address in x0 \\n"
"       add x0, x0, 16  ; x0 + 4                          \\n"
"       br x0           ; x0 + 8                          \\n"
"       add x1, x1, 1   ; x0 + 12                         \\n"
"       add x1, x1, 2   ; x0 + 16                         \\n"
"       add x1, x1, 4   ; x0 + 20                         \\n"
"       add x1, x1, 8   ; x0 + 24                         \\n"
"       mov %0, x1      ;                                 \\n"
'''.replace('\n', '')

assembly = assembly.replace('"', '\\"')
assembly = assembly.replace('\n', '\\n')

register_list = '"x0", "x1"'
clobered = register_list.replace('"', '\\"')

x = list(map(lambda x : 100*(x+1), range(100)))
y = []
for loop_count in x:

    try:
        #print(f'gcc main.c -DASM_CODE="{assembly}" -DCLOBERED="{clobered}" -DLOOP_COUNT={loop_count} -o main')
        subprocess.run(
            f'gcc main.c -DASM_CODE="{assembly}" -DCLOBERED="{clobered}" -DLOOP_COUNT={loop_count} -o main',
            shell=True
        )
    except Exception as e:
        print('compilation of main.c failed', str(e))

    average = 0
    for j in range(100):
        try:
            result = subprocess.run(
                './main',
                stdout=subprocess.PIPE
            )
            num = int(result.stdout.decode('utf-8').split(' ')[1][:-1])
            average += num
        except:
            print('running ./main returned an error')
        
    average //= 100
    y.append(average)

plt.plot(x, y)
plt.show()