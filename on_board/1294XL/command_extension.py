import os
import signal
import gdb 

class SignalOnBreakpoint(gdb.Breakpoint):
    def __init__(self, spec, p_pid, sig=signal.SIGUSR1 ):
        super().__init__(spec, internal=False)
        self.signal = sig
        self.ppid = p_pid
    
    def stop(self):
        pid = os.getpid()
        print(f"Sending signal {self.signal} (pid: {self.ppid})")
        os.kill(self.ppid, self.signal)
        return True

class SignalBreakpointCommand(gdb.Command):

    def __init__(self):
        super(SignalBreakpointCommand,self).__init__("coverage-breakpoint", gdb.COMMAND_USER)
    
    def invoke(self, arg, from_tty):
        args = gdb.string_to_argv(arg)
        if len(args) < 3:
            print("Usage: coverage-breakpoint <location> [parent_pid] [signal_to_send]")
            return
        
        location = args[0]
        parent_pid = int(args[1])
        sig = int(args[2])
        SignalOnBreakpoint(location, parent_pid, sig)

SignalBreakpointCommand()