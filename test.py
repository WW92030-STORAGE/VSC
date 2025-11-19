import os
import sys
import subprocess

# This runs main.cpp multiple times and returns the average runtime

def exec(cmdnargs):
    thing = subprocess.run(cmdnargs, capture_output=True, text=True)
    stt = thing.stdout

    if (str(thing.stderr) != ""):
         print("ERROR: " + thing.stderr)

    return stt

def runTest():
    result = exec("./main").split()
    return float(result[-10]) # -10 for us, -6 for ms, -2 for sec

NUM_TESTS = 32
NUM_WARMUP = 4


if __name__ == "__main__":
    # Compile the program
    exec("g++ main.cpp -o main -O3 -w -fcompare-debug-second".split())

    # Do a few warmups to do stuff
    for i in range(NUM_WARMUP):
        runTest()

    print("BEGIN FIELD TESTS")
    
    res = 0.0
    for i in range(NUM_TESTS):
        test = runTest()
        res += test
        print(test)
    
    res = res * (1.0 / NUM_TESTS)
    
    print("AVERAGE TIME ACROSS " + str(NUM_TESTS) + " TESTS = " + str(res) + " uS | " + str(res / 1000.0) + " MS | " + str(res / 1000000.0) + " S")

    # 9304927.0 BVHStanford 128x128 1 depth SAH
    # 20171709.375 BVHStanford 128x128 1 depth no SAH