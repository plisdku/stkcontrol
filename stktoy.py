import numpy as np
import stkcontrol

for ii in xrange(20):
    stkcontrol.note(2, ii*10000, 22100, 50 * (1 + ii/20.0), 0.5 + 0.4*np.sin(2*np.pi*ii/4.2))
    stkcontrol.note(1, (ii+0.5)*10000, 22100, 50 * (1 + ii/20.0), 0.5 + 0.4*np.cos(2*np.pi*ii/4.2))

stkcontrol.stop(400000)

stkcontrol.writeWav("out.wav")


