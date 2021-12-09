#!/usr/bin/python3

import sys

if len(sys.argv) < 2:
    print('Usage: ./jumper.py <even-number>')
    sys.exit(0)

num = int(sys.argv[1])

# Pad the number to an even one
if num % 2 == 1:
    num += 1

for i in range(num):
    label = f'l{i}:'.ljust(8)
    if i == 0:
        # Jump 2 forward
        print(f'"{label} b l{i+2: <8}\\n" \\')
    elif i == num - 1:
        # Exit
        print(f'"{label}\\n"')
    elif i % 2 == 0:
        # Jump 1 back
        print(f'"{label} b l{i-1: <8}\\n" \\')
    else:
        if i + 2 == num - 1:
            # Jump only 2 forward to finish
            print(f'"{label} b l{i+2: <8}\\n" \\')
        else:
            # Jump 3 forward
            print(f'"{label} b l{i+3: <8}\\n" \\')