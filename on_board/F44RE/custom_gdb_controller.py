import subprocess 
import logging
import time
import asyncio
import os
import argparse
import random
import signal

# from pygdbmi.gdbcontroller import GdbController

logger = logging.getLogger(__name__)
MAX_SEED_SIZE = 256
INCOV = 1
CRASH = 2

COVERAGE_INCREASING_LINE = 466

#Need to be able to build a program here as well.
#Init for fuzzing instance

class OCDController:

    def __init__(self, cfg_file, core, type):
        self.cfg_file = cfg_file    #Location of OpenOCD config file
        self.type = type            #Hardware vs Emualtor
        self.core = core            #MCU Core 
        self.process = None         #Subprocess object for OCD processs.

    def start_session(self):
        try:
            self.process = subprocess.Popen(["openocd","-f" + self.cfg_file]) #Attempts to start openocde instance
            if self.process == None:
                
                raise Exception 
        except Exception:
            logging.error('Failed to start the openOCD instance.')
            exit(-1)
            
    def end_session(self):
        self.process.terminate()   #Kills running openocd process
        self.process.wait()

        logging.info('Closed openocd instance.')

class GDBController:
    def __init__(self, bin_file):
        self.bin_file = bin_file
        self.process = None
    
    def interrupt_handler(self, signal):
        print("Signal Recieved:", signal)

    def start_session(self):
        self.process = subprocess.Popen(["arm-none-eabi-gdb", "--interpreter=mi2"],stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
        # self.process = await asyncio.create_subprocess_exec("/home/santiago/Research/microfuzz/toolchains/arm/gcc/bin/arm-none-eabi-gdb --interpreter=mi2",
        #     stdout=asyncio.subprocess.PIPE,
        #     stdin=asyncio.subprocess.PIPE,
        #     )
        signal.signal(signal.SIGTRAP, self.interrupt_handler)

    def execute_command(self, command):
        # self.process.stdout.flush()
        self.process.stdin.write(command)
        # self.process.stdin.write("-target-select extended-remote localhost:3333")
        self.process.stdin.flush()
        while True:
            output = self.process.stdout.readline()
            if self.process.poll() is not None:
                break
            print(output)
    

def main():
    ocd = OCDController('./openocd_debug.cfg','ARM-CortextM4', 'Hardware')
    ocd.start_session()
    time.sleep(3)
    gdb = GDBController('./microfuzz.elf')
    gdb.start_session()
    gdb.execute_command('-target-select extended-remote localhost:3333')
    time.sleep(3)
    gdb.execute_command("-file-exec-and-symbols microfuzz.elf" )
    while(1):
        i = 30

if __name__ == "__main__":
    main()
