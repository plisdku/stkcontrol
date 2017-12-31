import ctypes

ctypes.cdll.LoadLibrary("build/libstkcontrol.dylib")
_lib = ctypes.CDLL("build/libstkcontrol.dylib")

initialize = _lib.initialize
pushOn = _lib.pushOn
pushOn.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_float, ctypes.c_float]
pushOff = _lib.pushOff
pushOff.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_float]
pushFreq = _lib.pushFreq
pushFreq.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_float]
pushStop = _lib.pushStop
writeWav = _lib.writeWav

