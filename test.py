#!python

from assembly import Asm, ScalarVar

a = Asm()

a.add("""
            mov x1, <a>
            mov x2, <b>
            sub x3, x1, x2
            mov <result>, x3
""")
a.output(ScalarVar('unsigned long', 'result', '0'))
a.input(ScalarVar('unsigned long', 'a', '10'))
a.input(ScalarVar('unsigned long', 'b', '6'))

code = a.format()

with open('c/boilerplate.c') as b:
    boilerplate = b.read()
    test_code = boilerplate.replace('//PLACEHOLDER//', code)
    with open('c/test.c', 'w') as t:
        t.write(test_code)


