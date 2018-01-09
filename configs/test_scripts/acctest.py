import m5
from m5.objects import *

root = Root(full_system = False)

root.acc = BasicAcc()

m5.instantiate()

print "Beginning to run the simulation"

exit_event = m5.simulate()

print "Exiting at tick %d because %s" % (m5.curTick(), exit_event.getCause())