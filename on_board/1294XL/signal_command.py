import os
import signal
import gdb 

class SignalOnBreakpoint(gdb.Breakpoint):
    def __init__(self, spec, sig=signal.SIGTRAP):
        super().__init__(spec, internal=False)
        self.signal = sig
    
    def stop(self):
        pid = os.getpid()
        print(f"Sending signal {self.signal} (pid: {pid})")
        os.kill(pid, self.signal)
        return True

class SignalBreakpointCommand(gdb.Command):

    def __init__(self):
        super(SignalBreakpointCommand,self).__init__("signal-breakpoint", gdb.COMMAND_USER)
    
    def invoke(self, arg, from_tty):
        args = gdb.string_to_argv(arg)
        if len(args) < 1:
            print("Usage: signal-breakpoint <location> [signal_number]")
            return
        
        location = args[0]
        sig = int(args[1] if len(args) > 1 else signal.SIGTRAP)
        SignalOnBreakpoint(location, sig)

SignalBreakpointCommand()