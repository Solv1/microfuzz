import binascii
import subprocess as sp
import re
import argparse
import logging

logger = logging.getLogger(__name__)


class BinaryRewrite:
    def __init__(self, path_to_binary, objdump_path, start_address):
        self.path = path_to_binary
        self.objdump = objdump_path
        self.start_address = start_address
        self.target_sig = []
        self.locations = []
        self.raw_binary = []
        self.start_byte = 0
        
    def read_binary(self):
        #Read the raw binary and look the opcode
        
        with open(self.path, 'rb') as fp:

            for binary in iter(lambda: fp.read(1),b''):
                self.raw_binary.append(binascii.hexlify(binary))

        # print(self.raw_binary)
        return self.raw_binary
    
    def find_target_signature(self):
        target_index = 0

        cmd = [self.objdump,'-d',self.path]
        proc = sp.Popen(cmd, stdout=sp.PIPE)
        
        stdout = proc.communicate()[0] 
        output = stdout.decode()
        output_lines = output.splitlines()
        for index, line in enumerate(output_lines):
            if re.search(f'^({self.start_address})', line):
                #Find the first instance of the call and then identify this index
                # print(str(index) + ' : '+ line)
                target_index = index
                break
        # print(output_lines[target_index])
        raw1 = output_lines[target_index+1].split('\t')[1]
        raw2 = output_lines[target_index+2].split('\t')[1]
        raw3 = output_lines[target_index+3].split('\t')[1]
        raw4 = output_lines[target_index+4].split('\t')[1]
    
        self.target_sig.append(bytes((raw1[2:4]),'utf-8'))
        self.target_sig.append(bytes((raw1[:2]), 'utf-8'))
        self.target_sig.append(bytes((raw2[2:4]), 'utf-8'))
        self.target_sig.append(bytes((raw2[:2]), 'utf-8'))
        self.target_sig.append(bytes((raw3[2:4]), 'utf-8'))
        self.target_sig.append(bytes((raw3[:2]), 'utf-8'))
        self.target_sig.append(bytes((raw4[2:4]), 'utf-8'))
        self.target_sig.append(bytes((raw4[:2]), 'utf-8'))
        # print(self.target_sig)
        logger.info(f'INFO: Target signature: {self.target_sig}')

    def find_start_location(self):
        flags = [False,False,False,False,False,False,False,False]
        for count, byte in enumerate(self.raw_binary):
            if byte == self.target_sig[0] and not flags[0]:
                flags[0] = True
                # print(byte)
            elif byte == self.target_sig[1] and flags[0] and not flags[1]:
                flags[1] = True
                # print(byte)
            elif byte == self.target_sig[2] and flags[1] and not flags[2]:
                flags[2] = True
                # print(byte)
            elif byte == self.target_sig[3] and flags[2] and not flags[3]:
                flags[3] = True
                # print(byte)
            elif byte == self.target_sig[4] and flags[3] and not flags[4]:
                flags[4] = True
                # print(byte)
            elif byte == self.target_sig[5] and flags[4] and not flags[5]:
                flags[5] = True
                # print(byte)
            elif byte == self.target_sig[6] and flags[5] and not flags[6]:
                flags[6] = True
                # print(byte)
            elif byte == self.target_sig[7] and flags[6] and not flags[7]:
                flags[7] = True
                # print(byte)
                #This is where everything starts at
                logger.debug(f'DEBUG: Starting byte of target function: byte {count - 7}')
                # print(f'LOG: Starting byte of target function: byte {count - 7}')
                self.start_byte = count - 7
            else:
                flags[:] = [False] * len(flags)
        logger.debug(f'DEBUG: {self.raw_binary[68448]} {self.raw_binary[68449]} {self.raw_binary[68450]} {self.raw_binary[68451]} {self.raw_binary[68452]} {self.raw_binary[68453]} {self.raw_binary[68454]}')
        # print(self.raw_binary[68449])
        # print(self.raw_binary[68450])
        # print(self.raw_binary[68451])
        # print(self.raw_binary[68452])
        # print(self.raw_binary[68453])
        # print(self.raw_binary[68454])

    def rewrite_call(self, target_adds):
        
        for add in target_adds:
            target = int(add,16)
            # print(target)

            #First check if this is before or after the start of the target.
            # print(self.start_address)
            start_add = int(self.start_address, 16)
            # print(start_add)
            offset = target - start_add
            if (offset) < 0:
                offset = -(start_add - target)
            logger.info(f'INFO: Offset for rewritting: {offset}')
            # print(self.raw_binary[self.start_byte + offset-4:self.start_byte + offset+8])
            for i in range(self.start_byte + offset,self.start_byte + offset+4):
                self.raw_binary[i] = b'20'

            # print(self.raw_binary[self.start_byte + offset-4:self.start_byte + offset+8])
        
        with open ('./rewritten.elf', 'wb') as fp:
                for byte in self.raw_binary:
                    fp.write(binascii.unhexlify(byte)) 

def main():
    test = BinaryRewrite('./build/Microfuzz.elf',  'arm-none-eabi-objdump', '20000b60')
    test.read_binary()
    test.find_target_signature()
    test.find_start_location()
    test.rewrite_call('20000b80')
    # test.find_coverage_locations()
    # test.update_binary()


if __name__ == '__main__':
    main()