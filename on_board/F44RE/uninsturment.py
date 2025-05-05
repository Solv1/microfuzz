import subprocess as sp
import re
import os

class Uninsturmentor():

    def __init__(self, file):
        self.file = file

class ArmM4Uninsturmentor(Uninsturmentor):

    def __init__(self, file, intial_address):
        super().__init__(file)
        self.intial_address = int(intial_address, 16)
        self.address_bytecode = None
        self.offset_dic = {}

    def find_calls(self):
        function_name = '<_coverage_trace>'


        cmd = ['arm-none-eabi-objdump','-d',self.file]
        proc = sp.Popen(cmd, stdout=sp.PIPE)
        
        stdout = proc.communicate()[0] 
        output = stdout.decode()
        matches = [line for line in output.splitlines() if re.search(function_name, line)]
        
        matches.pop(0)
        #print(matches)

        tab_removed = [elm.replace('\t',' ') for elm in matches]
        #print(tab_removed)

        extra_removed = [elm.split('bl')[0].strip() for elm in tab_removed]
        #print(extra_removed)

        address_bytecode = [tuple(elm.split(':')) for elm in extra_removed]
        #print(address_bytecode)
        self.address_bytecode = address_bytecode

    def calc_offsets(self):

        for pair in self.address_bytecode:
            offset = int(pair[0], 16) - self.intial_address
            self.offset_dic[offset] = pair[1].strip()

        print(self.offset_dic)

    def overwrite_calls(self, overwrite_offsets):
        pass



def main():
    test = ArmM4Uninsturmentor('./microfuzz.elf', '20000601')
    test.find_calls()
    test.calc_offsets()


if __name__ == '__main__':
    main()