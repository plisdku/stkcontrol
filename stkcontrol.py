import ctypes

ctypes.cdll.LoadLibrary("build/libstkcontrol.dylib")
_lib = ctypes.CDLL("build/libstkcontrol.dylib")

_initialize = _lib.initialize
_shutdown = _lib.shutdown
_pushOn = _lib.pushOn
_pushOn.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_float, ctypes.c_float]
_pushOff = _lib.pushOff
_pushOff.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_float, ctypes.c_float]
# pushFreq = _lib.pushFreq
# pushFreq.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_float]
_pushStop = _lib.pushStop
_writeWav = _lib.writeWav


class stk_command(object):

    def __init__(self, in_type, in_id, in_time, in_freq, in_ampl):
        self.type = in_type
        self.id = in_id
        self.time = in_time
        self.freq = in_freq
        self.ampl = in_ampl

_commands = []

def note(id, in_time, duration, freq, onset_ampl=0.5, offset_ampl = 0.5):
    _commands.append(stk_command("on", id, int(in_time), freq, onset_ampl))
    _commands.append(stk_command("off", id, int(in_time+duration), freq, offset_ampl))

def stop(in_time):
    _commands.append(stk_command("stop", 0, in_time, 440.0, 1.0))

def writeWav(fileName):
    sorted_commands = sorted(_commands, key=lambda x: x.time)

    # for ss in sorted_commands:
    #     print ss.time, ss.type, "id", ss.id, "freq", ss.freq, "ampl", ss.ampl

    _initialize()

        # id time freq ampl
    for ss in sorted_commands:
        if ss.type == "on":
            _pushOn(ss.id, ss.time, ss.freq, ss.ampl)
        elif ss.type == "off":
            _pushOff(ss.id, ss.time, ss.freq, ss.ampl)
        elif ss.type == "stop":
            _pushStop(ss.time)

    _writeWav(fileName)

    _shutdown()
