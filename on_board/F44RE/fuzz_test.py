
import subprocess 
import logging
import time
import struct
import os
import argparse
import random
import signal
import datetime
#import toml

from pygdbmi.gdbcontroller import GdbController

MAX_SEED_SIZE = 256

LOGGING = True

COVERAGE_INCREASING_LINE = 484 #Change this


def debug_log(f):
    """
    Decorator for log debugging.
    """
    def log(self, *args, **kwargs):
        if LOGGING == True:
            print('LOG: Entering '+f.__name__)
            res = f(self, *args, **kwargs)
            print('LOG: Exiting '+ f.__name__)
        else:
            res = f(self, *args, **kwargs)
        return res
    return log

def timing(f):
    """
    Decorator used for timing functions.
    """
    def timefun(self, *args, **kwargs):
        start = time.time()
        res = f(self, *args, **kwargs)
        end = time.time()
        print(f'Time Elasped: {end-start}s')
        return res
    return timefun


#Need to be able to build a program here as well.
#Init for fuzzing instance

class OCDInstance:

    def __init__(self, cfg_file, core, type):
        self.cfg_file = cfg_file    #Location of OpenOCD config file
        self.type = type            #Hardware vs Emualtor
        self.core = core            #MCU Core 
        self.process = None         #Subprocess object for OCD processs.

    def start_session(self):
        try:
            self.process = subprocess.Popen(["openocd","-f" + self.cfg_file], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL) #Attempts to start openocde instance
            if self.process == None:
                
                raise Exception 
        except Exception:
            print("Error: Failed to open ocd instance.")
            exit(-1)
            
    def end_session(self):
        self.process.terminate()   #Kills running openocd process
        self.process.wait()

        

class GDBInstance:

    #

    def __init__(self):
        #make sure to have the GDB flavor in PATH
        #self.controller = GdbController(['arm-none-eabi-gdb-py', '--interpreter=mi2'])
        try:
            self.controller = GdbController(['arm-none-eabi-gdb-py', '--interpreter=mi2'])
        except Exception:
            print("Error: Failed to open GDB instance.")
            exit(-1)

    def load_program(self, bin_file):
        
        response = self.controller.write("-target-select remote localhost:3333",
                    timeout_sec=5
                    )
        #print(response)
        response = self.controller.write('-interpreter-exec console "source command_extension.py"',
        timeout_sec=10
        )
        # response = self.controller.write('-gdb-set target-async on',
        # timeout_sec=10
        # )
       # print(response)
        #response = self.controller.write('-interpreter-exec console "source command_extension.py"',
        #            timeout_sec=10
        #            )
        #print(response)
        response = self.controller.write("-file-exec-and-symbols " + bin_file , 
                               timeout_sec=5
                    )
        #print(response)
        response = self.controller.write(['-interpreter-exec console "monitor reset halt"'],
                    timeout_sec=5
                    )
        #print(response)
        response = self.controller.write(["-target-download"],
                    timeout_sec=5
                    )
        #print(response)
        # response = self.controller.write(['-interpreter-exec console "monitor reset 0 "'],
                    # timeout_sec=5
                    # )
        #print(response)
        #response = self.controller.write(["-break-insert main"])
        #TODO: Set breakpoints at approaite places here.
        response = self.controller.write(["-exec-continue"])
        #print(response)
        print('Program successfully loaded')
        #TODO: Check the reponse to see if the gdb connection is valid.

    def end_session(self):
        self.controller.exit()
    
    #TODO: Use the toggle breakpoint command.
    def set_breakpoint(self, label):
        #TODO: Check response for validity and for 
        response = self.controller.write(["-break-insert " + label])
        #print(response)

    def set_coverage_breakpoint(self, label, ppid):
        """
        Sets breakpoint using custom python extension for GDB. Generates a SIGTRAP that is handled.
        :param label: string either label or line number to set breakpoint
        :returns: None
        """
        #print(f'-interpreter-exec console "coverage-breakpoint {label} {ppid}"')
        response = self.controller.write(f'-interpreter-exec console "coverage-breakpoint {label} {ppid}"')
        #print(response)

    def remove_breakpoint(self, num):
        response = self.controller.write(["-break-delete " + num ])
    

    #@timing
    def write_memory(self, label:str, data:str):
        """
        @desc: abstraction for memory writing using GDB.

        @parameter: label: linker label to write data to.
                    data: string of hex values to write to memory.
        """
        #TODO: Check response for validity and for 

        # if not len(data) % 2: #Data length has to be an even amount.
        #     data = data + '0'
        
        command = '-data-write-memory-bytes ' + label + ' ' + data
        print(command)
        try:
            response = self.controller.write(command) #TODO: URGENT, check the format that is obtained from this. Should be hex values being set.
        except:
            print(f'LOG: Failed to write memory @{label}')
        print(response)

    def continue_execution(self):
        response = self.controller.write("-exec-continue", timeout_sec = 10)
        print(response)
        return response
    
    def halt_exeuction(self):
        os.kill(self.controller.gdb_process.pid, 2)
        # response = self.controller.write("-exec-interrupt", timeout_sec=30)
        response = self.controller.get_gdb_response(timeout_sec=10, raise_error_on_timeout=False)
        print(response)
        return response

    #@timing
    def read_memory(self, label, amount):
        #response = self.controller.get_gdb_response(timeout_sec=3, raise_error_on_timeout=False)
        # response = self.halt_exeuction()
        # print(response)
        #print(f'-data-read-memory-bytes {label} {amount}')
        try:
            gdb_response = self.controller.write('-data-read-memory-bytes '+ label + ' ' + amount, timeout_sec=10)
        except:
            print(f'LOG: Failed to read memory @{label}')
            return None
            # gdb_response = self.controller.write('-data-read-memory-bytes '+ label + ' ' + amount)
        #print(gdb_response)

        #Parse the dic response for infomation
        #print(gdb_response[0]['payload']['memory'][0]['contents'])
        parsed_response = gdb_response[0]['payload']['memory'][0]['contents']
        

        return parsed_response

    def clear_breakpoint(self):
        response = self.controller.write('-interpreter-exec console "clear"')
        return response
        
        #MUST PARSE RESPONSE HERE TO OBTAIN seed from response.


        
    def check_for_response(self):
        response = self.controller.get_gdb_response(timeout_sec=10, raise_error_on_timeout=False)
        return response


    
    
    #def get_address(self):

class Fuzzer:

    @debug_log
    def __init__(self, seed_dir, results_dir, board_config, device_side_bin, local_pool_size, max_seed_size):
        
        #Checks to see if directories exist if not make them.
        try:
            os.mkdir(seed_dir)
            #TODO: There needs to be initialization if there is no seeds present here.
        except FileExistsError:
            pass
        try: 
            os.mkdir(results_dir)
        except FileExistsError:
            pass

        self.seed_dir = seed_dir
        self.results_dir = results_dir
        self.board_config = board_config
            
        self.ocd = OCDInstance(self.board_config, "Cortex-M4", "Hardware") 
        self.ocd.start_session()
        time.sleep(10) #Wait a couple seconds for ocd to get configured.

        self.gdb = GDBInstance()
        #Load the device side fuzzer
        self.gdb.load_program(device_side_bin)

        self.local_pool_size = local_pool_size
        self.max_seed_size = max_seed_size

        self.pid = os.getpid() #So that GDB can communicate back instances of a crash or coverage..

        #-----Fuzzing Context------
        self.iterations = 0
        self.start_time = 0
        self.current_time = 0
        self.isCoverage = False



    
    @debug_log
    def check_seed_size(self):
        """
        Look at current global pool and see if there are seeds that are over the size that can be loaded onto the board. For now cut them into seperate seeds.
        """
        new_seeds = []
        global_seeds = os.listdir(self.seed_dir)
        global_pool_size = len(global_seeds)

        for seed in global_seeds:#Check each of these seeds to see if they are the appriate length.
            with open(self.seed_dir + str(seed),"r") as file:
                content = file.read()
                if len(content) > self.max_seed_size:
                    new_seeds.extend([content[i:i + self.max_seed_size] for i in range(0, len(content),self.max_seed_size)]) #Split the seed into max sized chunks
                else:
                    continue
        for new_seed in new_seeds:
            with open(self.seed_dir + str(global_pool_size+1), "w") as file:
                file.write(new_seed)
            global_pool_size+=1

    @timing
    @debug_log
    def write_local_pool(self, seeds, isSetup):
        """
        Load a local pool onto target. list of seeds is a tuple of (<seed_content>, <size>)
        """

        # if not isSetup:
            # self.gdb.halt_exeuction()
        #TODO: Maybe have a toml file for breakpoint configurations etc...
        local_pool_label = "dequeue_seed"
        self.gdb.set_breakpoint(local_pool_label)
        self.gdb.continue_execution()

        #TODO: There is a bug here with writing to memory.
        
        #Command to write -data-write-memory-bytes poolPtr.localPool[0].testCase+(offset) + '<hex_value>'
        #Command to read -data-read-memory poolPtr.localPool[0].testCase x 1 1 <seed size>
        for i in range(0, len(seeds)):
            self.gdb.write_memory('&g_localPool[+'+ str(i)+ '].testCase', ''.join(str(seeds[i][0]))) #TWrites seed content 
            self.gdb.write_memory('&g_localPool[+'+ str(i)+ '].size', str(seeds[i][1])) #Writes size of seed

        self.gdb.clear_breakpoint()
        #self.gdb.remove_breakpoint('3')

    @timing
    @debug_log
    def pull_current_testcase(self) -> list:
        """
        Pull the current testcase off of a device when there is a coverage increasing case or a crash.
        """
        # self.gdb.halt_exeuction()
        test_case = None
        test_case_label = '&g_testCaseBuffer.'

        test_case_size = int(self.gdb.read_memory(test_case_label+'size','1'), 16)
        test_case = self.gdb.read_memory(test_case_label+'testCase', str(test_case_size))
        
        #Loading and reading seed values are all in hex
        # self.gdb.continue_execution()
        return test_case

    @debug_log
    def seed_selection(self) -> list:
        selected_seeds = []

        global_seeds = os.listdir(self.seed_dir)
        global_pool_size = len(global_seeds)

        if global_pool_size <= int(self.local_pool_size): #Don't need to randomly select just load seeds one by one.
            for seed in global_seeds:

                with open(self.seed_dir + str(seed), "r") as file:
                    content  = file.read()
                    content_len = len(content)
                    selected_seeds.append((content, content_len))

                    # selected_seeds.append(file.read()) #Read the entire seed into memory to be loaded onto the board.
        else:
            for x in range(0,self.local_pool_size):
                rnd_seed = random.randint(0,global_pool_size-1) #Pick a random seed

                with open(self.seed_dir+str(global_seeds[rnd_seed]),"r") as file:
                    content  = file.read()
                    content_len = len(content)
                    selected_seeds.append((content, content_len))
                    # selected_seeds.append(file.read()) #Read the entire seed into memory to be loaded onto the board.
                global_pool_size-=1 #Make sure there are no repeat seeds being loaded here.
                global_seeds.pop(rnd_seed)
        return selected_seeds

    @debug_log
    def find_coverage_calls(self):
        #TODO: Implement binary anaysis.
        pass
    
    def _reverse_endianness(self, data):

        two_string = [(data[i:i+2]) for i in range(0, len(data), 2)] #Break up the string into byte sized chunks
        new_string = two_string[::-1] #Reverse the bytes
        reversed = "".join(new_string)

        return reversed

    @debug_log
    def init_global_state(self):
        """
        Read the binary and find calls with their offsets. Information on ASM is found in config file (ideally)
        """
        #TODO: Do this intialization stuff.
        random_bytes = os.urandom(4)
        random_number = int.from_bytes(random_bytes)
        print(random_bytes, random_number )
        self.gdb.write_memory('&g_randomSeed', str(random_number))
        
        

        little_endian_pointer = self.gdb.read_memory('&g_sutStartPtr', '4')
        function_pointer = self._reverse_endianness(little_endian_pointer)
        print(function_pointer)


    @debug_log
    def check_iterations(self):
        """
        Reads memory from board to store number of global iterations.
        """
        # self.gdb.halt_exeuction()
        
        board_its = self.gdb.read_memory('&g_iterations', '4')
        print(board_its)
        if (board_its == None):
            return
        reversed_hex = bytes.fromhex(board_its)[::-1].hex()
        print(reversed_hex)
        integer = (int(reversed_hex, 16))
        print(integer)
        self.iterations += integer
        self.current_time = time.time()
        elpased_time = self.current_time - self.start_time
        self.gdb.write_memory('&g_iterations', '00000000')
        print(f'Current execution speed: {self.iterations/elpased_time} tc/sec')
        
        # self.gdb.continue_execution()

    @timing
    @debug_log
    def handle_coverage_increasing(self):
        """
        Stores coverage increasing test case in the global seed pool. 
        """
        seed = self.pull_current_testcase()

        global_seeds = os.listdir(self.seed_dir)
        global_pool_size = len(global_seeds)

        print(f'New seed: {seed}')

        self.check_iterations()
        print(f'Iterations: {self.iterations}')

        self.current_time = time.time()

        elpased_time = self.current_time - self.start_time

        print(f'Current execution speed: {self.iterations/elpased_time} tc/sec')

        seeds = self.seed_selection()
        self.write_local_pool(seeds, False)

        
        now = datetime.datetime.now()
        print(f'Current time {now}.')

        with open(self.seed_dir + str(global_pool_size + 1), 'w') as fp:
            fp.write(str(seed))

    @timing
    @debug_log
    def signal_handler(self,signum, frame):
        
        #global isCoverage
        self.isCoverage = True
        #isCoverage = True
        signame = signal.Signals(signum).name
        print(f'Signal handler called with signal {signame} ({signum})')


    @debug_log
    def init_signal_handler(self):
        self.sig_handler = signal.signal(signal.SIGUSR1, self.signal_handler)
    
    @debug_log
    def init_campaign(self):
        #set breakpoints, load seed pool, start tracking the global coverage state.
        #The target device needs to be halted while entering this function. 
        now = datetime.datetime.now()
        print(f'Starting time {now}.')

        self.init_signal_handler()

        #The labels for the breakpoints should also be in the configuration file in the future. Just hard coded for now.
        self.gdb.set_breakpoint('bubble_coverage') #Set this breakpoint at a certian line nuber? Instead of a label??
        #self.gdb.set_coverage_breakpoint(str(COVERAGE_INCREASING_LINE), self.pid) #Want to switch this to a specific label but I know that this isn't the best way to do it.
        self.gdb.set_breakpoint('HardFault_Handler') #This may change with more robust error tracking.
        
        self.init_global_state()


        self.check_seed_size()
        seeds = self.seed_selection()
        self.write_local_pool(seeds, True)

        self.start_time = time.time()

    @debug_log
    def execute_campaign(self):
        """
        The main loop of the fuzzing experiance. 
        """

        self.gdb.continue_execution()
    
        while(True):
            time.sleep(10)
            self.gdb.halt_exeuction()
            self.check_iterations()
            self.gdb.continue_execution()

            if self.isCoverage:
                self.handle_coverage_increasing()
                self.isCoverage = False
                self.gdb.continue_execution()

    @debug_log
    def pause_campaign(self):
        pass
    
    @debug_log
    def shutdown(self):
        """
        Closes OCD and GDB sessions. Along with anyother clean up nessacary. 
        """
        
        self.check_iterations()
        print(f'Iterations: {self.iterations}')

        self.current_time = time.time()

        elpased_time = self.current_time - self.start_time

        print(f'Current execution speed: {self.iterations/elpased_time} tc/sec')

        self.gdb.end_session()
        self.ocd.end_session()


def main():
    
    parser = argparse.ArgumentParser(
        prog='Microfuzz',
        description='Fuzzer for varrious Microcontrollers',
        epilog='By Santiago Garcia'
    )
    parser.add_argument('-b', 
        help='Location on the fuzzing binary.', 
        required=True
    )
    parser.add_argument('-s', 
        default='./fuzzer/seeds', 
        help='Seed directory or one to be created.', 
        required=True
    )
    parser.add_argument('-c',
        help='Openocd config file.',
        required=True    
    )
    parser.add_argument('-r',
        default='./fuzzer/results', 
        help='Results directory or one to be created.', 
        required=True
    )
    parser.add_argument('-l',
        default= 15,
        help= '',
        required=False,
    )
    args = parser.parse_args()
    #print(args)
    
    try:
        fuzz = Fuzzer(args.s,args.r,args.c, args.b, args.l, 256)
        fuzz.init_campaign()
        fuzz.execute_campaign()
    # ocd = OCDInstance(args.c, "Cortex-M4", "Hardware")
    # ocd.start_session()
    # time.sleep(3) #Wait a few seconds to start the GDB instance.
    # gdb = GDBInstance()
    # gdb.load_program(args.b)
    # seeds = [[34,54,23,45,32,43,12,34,54],[34,53,12,69,45,65,43]]
    # gdb.write_local_pool(seeds)
    except KeyboardInterrupt:
    #     print(e)
        fuzz.shutdown()
    #     # gdb.end_session()
    #     # ocd.end_session()

    #     exit(0)

if __name__ == "__main__":
    main()

