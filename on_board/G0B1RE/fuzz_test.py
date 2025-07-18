
import subprocess 
import logging
import time
import struct
import os
import argparse
import random
import signal
import datetime
import binary_rewrite as bw
#import toml

from pygdbmi.gdbcontroller import GdbController

MAX_SEED_SIZE = 256
MAX_COVERAGE_POINTS = '800' #Set as 200 32-bit pointers on target

LOGGING = False

logger = logging.getLogger(__name__)
logging.basicConfig(filename='fuzz.log', encoding='utf-8', level=logging.DEBUG)
logger.debug(f'--------------------------------')
logger.debug(f'LOG: Starting Microfuzz Debug Log')

def debug_log(f):
    """
    Decorator for log debugging.
    """
    def log(self, *args, **kwargs):
        if LOGGING == True:
            logger.debug('DEBUG: Entering '+f.__name__)
            # print('LOG: Entering '+f.__name__)
            res = f(self, *args, **kwargs)
            logger.debug('LOG: Exiting '+ f.__name__)
            # print('LOG: Exiting '+ f.__name__)
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
        logger.debug(f'LOG: Time Elasped: {end-start}s')
        # print(f'Time Elasped: {end-start}s')
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
        except Exception as e:
            logger.error(f'Error: {e} -> failed to open OCD instance.')
            # print("Error: Failed to open ocd instance.")
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
        except Exception as e :
            logger.error(f'ERROR: {e} -> failed to open GDB instance.')
            # print("Error: Failed to open GDB instance.")
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
        logger.info(f'INFO: Program successfully loaded')
        #TODO: Check the reponse to see if the gdb connection is valid.

    def end_session(self):
        self.controller.exit()
    
    #TODO: Use the toggle breakpoint command.
    def set_breakpoint(self, label):
        #TODO: Check response for validity and for 
        response = self.controller.write(["-break-insert " + label])
        #print(response)

    def set_alerting_breakpoint(self, label, ppid, signal_num):
        """
        Sets breakpoint using custom python extension for GDB. Generates a SIGTRAP that is handled.
        :param label: string either label or line number to set breakpoint
        :returns: None
        """
        logger.debug(f'DEBUG: -interpreter-exec console "coverage-breakpoint {label} {ppid}, {signal_num}"')
        response = self.controller.write(f'-interpreter-exec console "coverage-breakpoint {label} {ppid} {signal_num}"')
        #print(response)

    def remove_breakpoint(self, num):
        response = self.controller.write(["-break-delete " + num ])
    
    # def create_variable(self, label:str, var:str):
    #     """
    #         @desc creates gdb variables as an abstraction to acutal variables on target
    #         @parameter: label: what you want the variable to be called in gdb
    #                     var: the label of the variable you want to track
    #     """
    #     try:
    #         response = self.controller.write(f'-var-create {label} * {var}')
    #     except Exception as e:
    #         logger.error(f'Error: {e} -> failed to create virtual variable in GDB.')
    #         # print("LOG: Failed to create virtual variable in GDB.")
    
    # def read_variable(self, label:str):
    #     """
    #         @desc reads a GDB variable 
    #         @parameter: label: name of the GDB variable created.
    #     """
    #     try:
    #         response = self.controller.write(f'-var-update *')
    #     except Exception as e:
    #         logger.error(f'Error: {e} -> failed to read GDB variable.')
    #         # print("LOG: Failed to update GDB variables.")
    #     try:
    #         response = self.controller.write(f'-var-evaluate-expression {label}')
    #         # print(response)
    #     except:
    #         print("LOG: Failed to read the GDB variable.")

    # def set_variable(self, label:str, val:str):
    #     """
    #         @desc sets a GDB variable -> this trickles down to the acutal variables
    #         on a target assocated with GDB variable
    #         @parameter: label: name of the GDB variable created
    #                     val: value to be written.
    #     """
    #     try:
    #         response = self.controller.write(f'-var-assign {label} {val}')
    #     except:
    #         print("LOG: Failed to set GDB variable")

    
    #@timing
    def write_memory(self, label:str, data:str):
        """
        @desc: abstraction for memory writing using GDB.

        @parameter: label: label to write data to.
                    data: string of hex values to write to memory.
        """
        extra = ''
        #TODO: Check response for validity and for 
        logger.debug(f'DEBUG: You are in write_memory.')
        # print('You are in write_memory')
        logger.debug(f'DEBUG: Data to be written {data}')
        # print(data)
        if not ((len(data) % 2) == 0): 
            # print(len(data))
            extra = '0'
        if len(data) == 0:
             # print(f'LOG: Empty write attempted')
             extra = '00'
        
        command = '-data-write-memory-bytes ' + label + ' ' + data + extra  
        logger.debug(f'DEBUG: {command}')
        try:
            response = self.controller.write(command) #TODO: URGENT, check the format that is obtained from this. Should be hex values being set.
            logger.debug(f'DEBUG: {response}')
            # print(response)
        except Exception as e:
            logger.error(f'ERROR: {e} -> failed to write memory @{label}')
            # print(f'LOG: Failed to write memory @{label}')
        

    def continue_execution(self):

        response = self.controller.write('-interpreter-exec console "continue"', timeout_sec = 10)
        # response = self.controller.write("-exec-continue --all", timeout_sec = 10)
        logger.debug('DEBUG: You are in continue_execution')
        logger.debug(f'DEBUG: {response}')
        # print("LOG: You are in continue_execution.")
        # print(response)
        return response
    
    def halt_exeuction(self):
        local_pool_label = "dequeue_seed"
        self.set_breakpoint(local_pool_label)
        response = self.continue_execution()
        logger.debug(f'DEBUG: {response}')
        # print(response)
        logger.debug('DEBUG: You are in halt_execution.')
        # print("LOG: You are in halt_execution.")
        response = self.clear_breakpoint()
        # print(response)
        logger.debug(f'DEBUG: {response}')
        return response

    #@timing
    def read_memory(self, label, amount):
        #response = self.controller.get_gdb_response(timeout_sec=3, raise_error_on_timeout=False)
        # response = self.halt_exeuction()
        # print(response)
        gdb_response = None
        logger.info(f'INFO: -data-read-memory-bytes {label} {amount}')
        # print(f'-data-read-memory-bytes {label} {amount}')
        try:
            gdb_response = self.controller.write('-data-read-memory-bytes '+ label + ' ' + str(amount), timeout_sec=30)
        except Exception as e:
            logger.error(f'ERROR: Failed to read memory @{label} -> {e}')
            # print(f'LOG: Failed to read memory @{label}')
            # print(gdb_response)
            return None
            # gdb_response = self.controller.write('-data-read-memory-bytes '+ label + ' ' + amount)
        

        #Parse the dic response for infomation
        parsed_response = None

        if (type(gdb_response) == list):
            for i, item in enumerate(gdb_response):
                rep_type = gdb_response[i]['type']
                if rep_type == 'result':
                    if gdb_response[i]['payload'] == None:
                        # print("You are here")
                        continue
                    else:
                        logger.info(f'INFO: GDB Response: {gdb_response}')
                        # print("---------GDB RESPONSE----------")
                        # print(gdb_response)
                        parsed_response = gdb_response[i]['payload']['memory'][0]['contents']
        else:
            parsed_response = gdb_response[0]['payload']['memory'][0]['contents']
        
        

        return parsed_response

    def clear_breakpoint(self):
        response = self.controller.write('-interpreter-exec console "clear"')
        return response
        
        #MUST PARSE RESPONSE HERE TO OBTAIN seed from response.
    
    def reset_board(self, bin_file):
        response = self.controller.write(['-interpreter-exec console "monitor reset halt"'],
                    timeout_sec=5
                    )
        response = self.controller.write("-file-exec-and-symbols " + bin_file , 
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
        self.crash_dir = results_dir+'crashes'
        self.coverage_dir = results_dir + 'lists'
            
        self.ocd = OCDInstance(self.board_config, "Cortex-M4", "Hardware") 
        self.ocd.start_session()
        time.sleep(10) #Wait a couple seconds for ocd to get configured.

        self.gdb = GDBInstance()
        #Load the device side fuzzer
        self.gdb.load_program(device_side_bin)

        self.local_pool_size = int(local_pool_size)
        self.max_seed_size = int(255)

        self.pid = os.getpid() #So that GDB can communicate back instances of a crash or coverage..

        #-----Fuzzing Context------
        self.iterations = 0
        self.start_time = 0
        self.current_time = 0
        self.isCoverage = False
        self.isCrash = False
        self.start_pointer = ''
        self.binary_handler = None
        self.target_arch = 'ARM'



    
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
            # if not isSetup:
            #     test_case = self.gdb.read_memory('&g_localPool['+ str(i)+ '].testCase','255')
            #     print(f'DEBUG: Testcase: {test_case}')
            #     size = self.gdb.read_memory('&g_localPool['+ str(i)+ '].size', '4')
            # print(f'DEBUG: Testcase size: {size}')


            
            test = seeds[i][1]
            if test < 10:
                test = 10
            logger.debug(f'DEBUG: Testcase size {test}')
            # Convert integer back to bytes (little-endian)
            little_endian_bytes = test.to_bytes(test, 'little')

            # Convert bytes to hex string (optional)
            little_endian_hex = little_endian_bytes.hex()


            self.gdb.write_memory('&g_localPool[+'+ str(i)+ '].testCase', ''.join(str(seeds[i][0]))) #TWrites seed content
            logger.debug(f"DEBUG: Testcase size: {str(seeds[i][1])}, {str(little_endian_hex)}"  ) 
            # self.gdb.write_memory('&g_localPool[+'+ str(i)+ '].size', str(little_endian_hex)) #Writes size of seed
            self.gdb.write_memory('&g_localPool[+'+ str(i)+ '].size', '25') #Writes size of seed
            test_case = self.gdb.read_memory('&g_localPool['+ str(i)+ '].testCase','255')
            logger.debug(f'DEBUG: Testcase: {test_case}')
            size = self.gdb.read_memory('&g_localPool['+ str(i)+ '].size', '4')


        num_seeds = (hex(len(seeds)))[2:]
        if len(num_seeds) == 1:
            num_seeds = '0'+ num_seeds
        #make sure to let the fuzzer know how many seeds to work with.
        self.gdb.write_memory('&g_nSeeds', num_seeds)

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

        # test_case_size = int(self.gdb.read_memory(test_case_label+'size','1'), 16)
        test_case = self.gdb.read_memory(test_case_label+'testCase', MAX_SEED_SIZE)
        
        #Loading and reading seed values are all in hex
        # self.gdb.continue_execution()
        return test_case

    @debug_log
    def seed_selection(self) -> list:
        selected_seeds = []

        global_seeds = os.listdir(self.seed_dir)
        # print(global_seeds)
        # print(self.seed_dir)
        global_pool_size = len(global_seeds)
        # print(len(global_seeds))

        if global_pool_size <= int(self.local_pool_size): #Don't need to randomly select just load seeds one by one.
            for seed in global_seeds:

                with open(self.seed_dir + str(seed), "r") as file:
                    # content  = file.read()
                    # content_len = len(content)
                    # selected_seeds.append((content, content_len))
                    content  = str(file.read())
                    # print(content)
                    content = content[0:self.max_seed_size]
                    content_len = self.max_seed_size
                    selected_seeds.append((content, content_len))
# 
                    # selected_seeds.append(file.read()) #Read the entire seed into memory to be loaded onto the board.
        else:
            logger.info(f'INFO: Size of local pool {self.local_pool_size}')
            for x in range(0,self.local_pool_size):
                rnd_seed = random.randint(0,global_pool_size-1) #Pick a random seed

                with open(self.seed_dir+str(global_seeds[rnd_seed]),"r") as file:
                    content  = str(file.read())
                    split_size = content.split('0')
                    content_len = len(split_size[0])
                    selected_seeds.append((split_size[0], content_len))
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
        logger.debug(f'DEBUG: Random numbers chosen {random_bytes}, {random_number}')
        # print(random_bytes, random_number )
        self.gdb.write_memory('&g_randomSeed', str(random_number))
        
        

        little_endian_pointer = self.gdb.read_memory('&g_sutStartPtr', '4')
        # print(little_endian_pointer)
        self.start_pointer = self._reverse_endianness(little_endian_pointer)
        if self.target_arch == 'ARM':
            #If this is ARM we have to remove the LSB to find the real address... thanks Thumb Mode
            dummy_pointer = hex(int(self.start_pointer, 16) - 1)[2:10]
            self.start_pointer = dummy_pointer
        logger.info(f'INFO: Start of target program: {self.start_pointer}')


    @debug_log
    def check_iterations(self):
        """
        Reads memory from board to store number of global iterations.
        """
        response = None
        # self.gdb.halt_exeuction()
        
        board_its = self.gdb.read_memory('&g_iterations', '4')
        logger.info(f'INFO: Current board iterations {board_its}')
        if (board_its == None):
            return
        reversed_hex = bytes.fromhex(board_its)[::-1].hex()
        # print(reversed_hex)
        integer = (int(reversed_hex, 16))
        # print(integer)
        self.iterations += integer
        self.current_time = time.time()
        elpased_time = self.current_time - self.start_time
        command = '-data-write-memory-bytes ' + '&g_iterations 00 4' 
        # print(command)
        try:
            response = self.gdb.controller.write(command) #TODO: URGENT, check the format that is obtained from this. Should be hex values being set.
        except Exception as e:
            logger.error(f'ERROR: Failed to write memory @g_iterations -> {e}')
            #TODO: Handle this case with some magic.
            logger.error(f'ERROR: GDB response:{response}')
            # print(response)
        logger.info(f'INFO: Current execution speed: {self.iterations/elpased_time} tc/sec')
        print(f'INFO: Current execution speed: {self.iterations/elpased_time} tc/sec')

    def pull_coverage_list(self):

        cov_list = []
        little_e_cov = []
        raw_data = self.gdb.read_memory('&g_coverageList', MAX_COVERAGE_POINTS)
        
        logger.info(f'INFO: Size of cov-list: {len(raw_data)}')
        for index in range(0, len(raw_data),8):
            address = raw_data[index:index+8]
            if (int(address, 16)) == 0:
                break
            else:
                little_e_cov.append(address)

        if not little_e_cov:
            return None
        for add in little_e_cov:
            cov_list.append(add[6:8] + add[4:6]+ add[2:4] + add[0:2])
        logger.info(f'INFO: Cov List {cov_list}')
        # print(cov_list)

        lists = len(os.listdir(self.coverage_dir))
        with open(self.coverage_dir + f'/cov-{lists+1}', 'w+') as fp:
            for cov in cov_list:
                fp.write(cov)

        return cov_list
        
    def handle_crash(self):
        crashing_testcase = self.pull_current_testcase()

        crash_area = os.listdir(self.crash_dir)
        crashes = len(crash_area)

        self.current_time = time.time()
        with open(str(self.crash_dir)+'/'+str(self.current_time), 'w+') as fp:
            fp.write(crashing_testcase)
        
        self.check_iterations()
        logger.info(f'INFO: Iterations: {self.iterations}')
        # print(f'Iterations: {self.iterations}')

        

        elpased_time = self.current_time - self.start_time
        logger.info(f'INFO: Current execution speed: {self.iterations/elpased_time} tc/sec')
        # print(f'Current execution speed: {self.iterations/elpased_time} tc/sec')

        self.gdb.reset_board('./rewritten.elf')

        self.init_global_state()

        seeds = self.seed_selection()
        self.write_local_pool(seeds, True)
        
        return

    @timing
    @debug_log
    def handle_coverage_increasing(self):
        """
        Stores coverage increasing test case in the global seed pool. 
        """
        seed = self.pull_current_testcase()

        global_seeds = os.listdir(self.seed_dir)
        global_pool_size = len(global_seeds)

        with open(str(self.seed_dir)+str(global_pool_size), 'w+') as fp:
            fp.write(seed)

        # print(f'New seed: {seed}')

        self.check_iterations()
        logger.info(f'INFO: Iterations: {self.iterations}')
        # print(f'Iterations: {self.iterations}')

        self.current_time = time.time()

        elpased_time = self.current_time - self.start_time
        logger.info(f'INFO: Current execution speed: {self.iterations/elpased_time} tc/sec')
        # print(f'Current execution speed: {self.iterations/elpased_time} tc/sec')

        cov_list = self.pull_coverage_list()
        self.binary_handler.rewrite_call(cov_list)

        seeds = self.seed_selection()
        self.write_local_pool(seeds, False)

        
        now = datetime.datetime.now()
        logger.info(f'INFO: Current time {now}')
        # print(f'Current time {now}.')

        with open(self.seed_dir + str(global_pool_size + 1), 'w') as fp:
            fp.write(str(seed))

    @timing
    @debug_log
    def covarage_handler(self,signum, frame):
        
        #global isCoverage
        self.isCoverage = True
        #isCoverage = True
        self.gdb.isHalted = True
        signame = signal.Signals(signum).name
        logger.debug(f'DEBUG: Coverage handler called with signal {signame} ({signum})')
        # print(f'Coverage handler called with signal {signame} ({signum})')

    def crash_handler(self, signum, frame):
        self.isCrash = True
        self.gdb.isHalted = True
        signame = signal.Signals(signum).name
        logger.debug(f'DEBUG: Crash handler called with signal {signame} ({signum})')
        # print(f'Crash handler called with signal {signame} ({signum})')

    @debug_log
    def init_signal_handler(self):
        self.cov_handler = signal.signal(signal.SIGUSR1, self.covarage_handler)
        self.cra_handler = signal.signal(signal.SIGUSR2, self.crash_handler)
    
    @debug_log
    def init_campaign(self):
        #set breakpoints, load seed pool, start tracking the global coverage state.
        #The target device needs to be halted while entering this function. 
        now = datetime.datetime.now()
        logger.info(f'INFO: Starting time {now}.')
        # print(f'Starting time {now}.')

        self.init_signal_handler()

        #The labels for the breakpoints should also be in the configuration file in the future. Just hard coded for now.
        # self.gdb.set_breakpoint('bubble_coverage') #Set this breakpoint at a certian line nuber? Instead of a label??
        self.gdb.set_alerting_breakpoint('bubble_coverage', self.pid, signal.SIGUSR1) #Want to switch this to a specific label but I know that this isn't the best way to do it.
        self.gdb.set_alerting_breakpoint('crash_handler', self.pid, signal.SIGUSR2) #This may change with more robust error tracking.
        
        self.init_global_state()
        # print(self.start_pointer)
        logger.info(f'INFO: Starting Pointer {self.start_pointer}')
        self.binary_handler = bw.BinaryRewrite('./Microfuzz.elf',  'arm-none-eabi-objdump', self.start_pointer)
        self.binary_handler.read_binary()
        self.binary_handler.find_target_signature()
        self.binary_handler.find_start_location()
        #self.check_seed_size()
        seeds = self.seed_selection()
        # print(seeds)
        self.write_local_pool(seeds, True)

        self.start_time = time.time()

    @debug_log
    def execute_campaign(self):
        """
        The main loop of the fuzzing experiance. 
        """
        start_time = time.clock_gettime(time.CLOCK_BOOTTIME)
        self.gdb.continue_execution()
    
        while(True):
            
            if self.isCoverage:
                self.handle_coverage_increasing()
                self.isCoverage = False
                self.gdb.continue_execution()

            if self.isCrash:
                self.handle_crash()
                self.isCrash = False
                self.gdb.continue_execution()

            check_time = time.clock_gettime(time.CLOCK_BOOTTIME)
            if ((check_time - start_time) > 60) and not self.isCoverage and not self.isCrash:
                os.kill(self.gdb.controller.gdb_process.pid, signal.SIGINT)
                self.gdb.halt_exeuction()
                self.check_iterations()
                start_time = time.clock_gettime(time.CLOCK_BOOTTIME)

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
        logger.info(f'INFO: Iterations: {self.iterations}')
        # print(f'Iterations: {self.iterations}')

        self.current_time = time.time()

        elpased_time = self.current_time - self.start_time

        logger.info(f'INFO: Current execution speed: {self.iterations/elpased_time} tc/sec')
        # print(f'Current execution speed: {self.iterations/elpased_time} tc/sec')

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
    except KeyboardInterrupt:
    #     print(e)
        fuzz.shutdown()
    #     # gdb.end_session()
    #     # ocd.end_session()

    #     exit(0)

if __name__ == "__main__":
    main()

