# This is literally a thing that tests how long it takes to build this crap

import time, subprocess

def exec(cmdnargs):
    thing = subprocess.run(cmdnargs, capture_output=True, text=True)
    stt = thing.stdout

    if (str(thing.stderr) != ""):
         print("ERROR: " + thing.stderr)

    return stt

if __name__ == "__main__":
    # Compile the program

    S = time.time_ns()
    exec("make clean all".split())

    E = time.time_ns()

    print("ELAPSED", (E - S) * 0.000001, "ms")