import re
from sre_constants import CATEGORY

class ScalarVar:
    ctype = ''
    name = ''
    val = ''
    format = ''
    formatdict = {
        'int': '%d',
        'unsigned int': '%u',
        'long': '%ld',
        'unsigned long': '%lu',
    }

    def __init__(self, ctype, name, val, format=''):
        self.ctype = ctype
        self.name = name
        self.val = val
        if format == '':
            self.format = self.formatdict[ctype]
        else:
            self.format = format

    def cinit(self):
        return self.ctype + ' ' + self.name + ' = ' + self.val + ';\n'

    def cprintf(self, mode):
        if mode == 'in':
            return 'printf("IN  | ' + self.name + ': ' + self.format + '\\n", ' + self.name + ');\n'
        else:
            return 'printf("OUT | ' + self.name + ': ' + self.format + '\\n", ' + self.name + ');\n'

class Asm:
    asm = ''
    clobbers = []
    inputs = []
    outputs = []
    varcount = 0

    def __init__(self):
        self.asm = ''
        self.clobbers = []
        self.inputs = []
        self.outputs = []
        self.varcount = 0

    # Assembly is a string representing the assembly instructions
    # that we want to insert into our C code.
    def add(self, assembly):
        lines = assembly.split('\n')
        for line in lines:
            if len(line) == 0:
                continue
            self.asm += '\t"' + line + ' \\n\\t"\n'
            elements = re.split(',?\s+', line)
            for i in range(len(elements)):
                el = elements[i]
                if re.match('x\d*', el) and not el in self.clobbers:
                    self.clobbers.append(el)
        return self

    def output(self, output):
        if isinstance(output, ScalarVar):
            self.asm = self.asm.replace('<' + output.name + '>', '%{}'.format(self.varcount))
            self.varcount += 1
            self.outputs.append(output)
        return self

    def input(self, input):
        if isinstance(input, ScalarVar):
            self.asm = self.asm.replace('<' + input.name + '>', '%{}'.format(self.varcount))
            self.varcount += 1
            self.inputs.append(input)
        return self

    def format(self):
        code = ''
        for i in self.inputs:
            code += i.cinit()
        for o in self.outputs:
            code += o.cinit()
        
        code += '__asm__ volatile (\n{}\t: {}\n\t: {}\n\t: {}\n);\n'.format(
            self.asm,
            ','.join(map(lambda x : '"=r"(' + x.name + ')', self.outputs)),
            ','.join(map(lambda x : '"r"(' + x.name + ')', self.inputs)),
            ','.join(map(lambda x : '"' + x + '"', self.clobbers))
        )

        for i in self.inputs:
            code += i.cprintf('in')
        for o in self.outputs:
            code += o.cprintf('out')
        
        return code