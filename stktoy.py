import numpy as np
import stkcontrol


# print "Initialize"
# stkcontrol.initialize()

# stkcontrol.pushOn(0, 0, 440.0, 0.25)
# stkcontrol.pushOn(1, 10000, 440.0, 0.5)
# stkcontrol.pushOn(1, 20000, 440.0, 0.75)
# stkcontrol.pushOn(1, 30000, 440.0, 1.0)
# stkcontrol.pushStop(100000)

# stkcontrol.writeWav("done.wav")


# stkcontrol.note(0, 0, 44100, 440.0)
# stkcontrol.note(0, 5000, 22050, 550.0)
# stkcontrol.note(0, 50000, 44100, 660.0)
# stkcontrol.note(0, 50000, 44100, 770.0)
# stkcontrol.note(0, 20000, 10000, 880.0)


for ii in xrange(20):
    stkcontrol.note(2, ii*10000, 22100, 440.0 * (1 + ii/20.0), 0.5 + 0.4*np.sin(2*np.pi*ii/4.2))
    stkcontrol.note(1, (ii+0.5)*10000, 22100, 440.0 * (1 + ii/20.0), 0.5 + 0.4*np.cos(2*np.pi*ii/4.2))

stkcontrol.stop(400000)

stkcontrol.writeWav("out.wav")