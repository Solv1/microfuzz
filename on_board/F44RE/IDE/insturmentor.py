
#TODO LIST
#====================================================================
#
#====================================================================

import os 
import sys
import re
from treelib import Node, Tree
from itertools import islice
import argparse



#====================================================================
trampoline = ["\tbl _coverage_trace\n"]
#extern_identifier = #TODO: Find out if I need this.
#====================================================================


# Argument Handling
#===================================================================
parser = argparse.ArgumentParser(
    prog='Insturmentor.py',
    description='Performs assembly time insturmentation on a file.'
)
parser.add_argument('-f','--filename',dest='filename',action='store',
    help='Filename for insturmention'
)
#===================================================================
class Insturmentor():

    def __init__(self, asm_file_path, new_file_name):

        self.asm_file_path = asm_file_path
        self.new_file_name = new_file_name

        self.instructions = None

        self.branch_instructions = None
        self.trampoline = None
        self.label_key = None

        self.branch_locations = []
        self.label_locations = []            
            

    def read_asm_file(self):
        try:
            with open(self.asm_file_path, 'r') as f:
                self.instructions = f.readlines()
        except FileNotFoundError:
            print('Error: Assembly file not found, exiting :(.')
            exit(-1)
    
    def _strip_branch_labels(self, instruction):
        pass

    def _find_branches(self):
        pass

class ArmM4_Insturmentor(Insturmentor):

    def __init__(self, asm_file_path, new_file_name):
        super().__init__(asm_file_path, new_file_name)
        self.branch_instructions = [
            '\tbeq\t', '\tbeq.W\t',
            '\tbne\t', '\tbne.W\t',
            '\tbcs\t', '\tbcs.W\t',
            '\tbcc\t', '\tbcc.W\t',
            '\tbmi\t', '\tbmi.W\t',
            '\tbpl\t', '\tbpl.W\t',
            '\tbvs\t', '\tbvs.W\t',
            '\tbvc\t', '\tbvc.W\t',
            '\tbhi\t', '\tbhi.W\t',
            '\tbls\t', '\tbls.W\t',
            '\tbge\t', '\tbge.W\t',
            '\tblt\t', '\tblt.W\t',
            '\tbgt\t', '\tbgt.W\t',
            '\tble\t', '\tble.W\t',
            '\tbal\t', '\tbal.W\t',
            '\tb\t',   '\tb.W\t',
            '\tcbz\t','\tcbnz\t'
                                    ]
        self.trampoline = '\tbl\t_coverage_trace\t @Tracing Function\n'
        self.label_key = '.+:$' #Regex for labels <label_name>: <-- always ends with colon.           
        
        self.block_locations = []
        self.jump_tables = []

        self.cfg = Tree() #TODO: Implement a CFG for shits and giggles.
    
    def _strip_branch_labels(self, instruction) -> list:
        split_ins = instruction.split('\t')
        if len(split_ins) == 3:
            print(split_ins[2])

        elif len(split_ins) == 4:
            print(split_ins[3])

    def _strip_offsets(self, instruction):
        byte_removal = instruction.split('.byte')
        offsets = byte_removal.split(',')


    def _find_jump_tables(self):
        table_name = None
        for line in self.instructions:
            if re.search(self.label_key, line): #At each label check to see if there is a table present following.
                table_name = line
            #TODO: Change the pulled offsets into hex values I can work with.
            elif table_name:
                if re.search('(\t)*(.byte).+', line):   #TBB Instructions --> Byte sized jumps
                    byte_removal = line.split('.byte')[1]
                    offsets = byte_removal.split(',')
                    self.jump_tables.append({table_name.replace('\n',''):[offset.replace('\n','') for offset in offsets]})
                    print(self.jump_tables)
                elif re.search('(\t)*(.hword).+', line):    #TBH Instructions --> Half-word sized jumps
                    hword_removal = line.split('.hword')[1]
                    offsets = hword_removal.split(',')
                    self.jump_tables.append({table_name.replace('\n',''):[offset.replace('\n','') for offset in offsets]})
                    print(self.jump_tables)
                table_name = None

    #TODO: Ask Matt about dynamic instrumentation techniques if nesscary?
    # def _find_load_pc(self, index, instr):
    #     #Find register and search lines reversed from that index for the value.
    #     instr_removed = instr.split('ldr')[1]
    #     register = instr_removed.split(',')[0]
    #     print(instr)
    #     print(register)



    # def _find_move_pc(self,index, instr):
    #     #Find register and search lines reversed from that index for the value.
    #     instr_removed = instr.split('mov')[1]
    #     register = instr_removed.split(',')[1]       
    #     # print(instr)
    #     # print(register)
    #     for line in reversed(self.instructions[0:index]):
    #         print(line)

    def _find_leaders(self):
        for index, line in enumerate(self.instructions):
            loc = None

            if any(ins in line for ins in self.branch_instructions):
                #print(str(index) + ' ' + line)
                loc = index 
            # elif re.search('[\t| ]*(ldr)[\t| ]*(pc),.*', line): #These two statments are for indriect jumps.
            #     loc = self._find_load_pc(index, line)
            # elif re.search('[\t| ]*mov[\t| ]*pc,.*', line):
            #     loc = self._find_move_pc(index, line)
            elif re.search('^IT[T|E]*', line): #Finds If Then Blocks specific to ARM architectures.
                loc = index - 1  #Have to insturement before The IT block 
            elif ((re.search(self.label_key, line)) and not ([dic for dic in self.jump_tables if next(iter(dic)) in line])): #Hacky, makes sure not to insturment the jump tables we find.
                #print(str(index) + ' ' + line)
                loc = index 

            if loc:
                self.block_locations.append(loc)                 
        print(self.block_locations)
    
    def _write_insturmented_code(self):
        with open('./'+self.new_file_name, 'w') as f:
            f.writelines(self.new_instructions)

    def _add_insturmentation(self):
        """
        Have to add trampoline after branches/labels and before IT blocks.
        """
        self.new_instructions = []

        for index, line in enumerate(self.instructions):
            if ((index in self.branch_locations) or (index in self.label_locations)):
                self.new_instructions.append(line)
                self.new_instructions.append(self.trampoline)
            else:
                self.new_instructions.append(line)

        for line in self.new_instructions:
            print(line)




def main():
    ment_test = ArmM4_Insturmentor('./test_branch.s', 'new_branch.s')
    ment_test.read_asm_file()
    #print(ment_test.instructions)
    ment_test._find_jump_tables()
    ment_test._find_leaders()
    ment_test._add_insturmentation()
    ment_test._write_insturmented_code()
    ment_test.cfg.show()
    #ment_test._add_insturmentation()


if __name__ =='__main__':
    main()








