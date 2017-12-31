import stkcontrol

print "Initialize"
stkcontrol.initialize()

stkcontrol.pushOn(0, 10, 440.0, 1.0)
stkcontrol.pushStop(100000)

stkcontrol.writeWav("done.wav")
