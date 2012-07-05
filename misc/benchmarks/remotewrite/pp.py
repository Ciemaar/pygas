import string
import random
import time

from pygas import *

NUM_TRIALS = 100

class ObjectManager(object):
    def __init__(self):
        self.val = ""

def main():
    om = ObjectManager() if MYTHREAD is 1 else None
    om_proxy = share(om, from_thread=1)

    for msg_size in [2**x for x in range(16)]:
        if MYTHREAD == 0:
            arr  = str(MYTHREAD)*msg_size
            timer = SplitTimer("%d " % msg_size)
            for trial in range(NUM_TRIALS):
                with timer:
                    om_proxy.val = arr
            print timer.report()
        barrier()
        if MYTHREAD == 1:
            assert om.val == '0'*msg_size
        barrier()

if __name__ == '__main__':
    assert THREADS is 2
    main()