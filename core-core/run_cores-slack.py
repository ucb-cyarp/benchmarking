#!/usr/bin/env python3

import collections
import string
import platform
import subprocess
import re
import os.path
import datetime
import json
import requests
import argparse

from cpuTopology import *
from slackUtils import *

def main():
    parser = argparse.ArgumentParser(description='Run core core benchmarks on the specified cores')
    parser.add_argument('cpu', metavar='CPU', type=int, nargs='+', help='Logical CPUs of interest')

    args = parser.parse_args()

    cpuList = args.cpu

    if len(cpuList) != 2 and len(cpuList) != 4:
        print('Number of CPUs specified must be 2 or 4')
        return 1

    hostname = platform.node()

    topology = getPlatformTopology()
    unique_topology = getUniqueCPUs(topology)

    print('CPU Topology:')
    printTopology(topology)
    print('\nUnique Cores:')
    printTopology(unique_topology)

    cpuStr = str(cpuList[0])
    for i in range(1, len(cpuList)):
        cpuStr = cpuStr + ' ' + str(cpuList[i])

    cmd = './core-core {}'.format(cpuStr)

    cur_time = datetime.datetime.now()
    print("Starting: {}\n".format(str(cur_time)))
    startStamp = open('start_stamp.txt', 'w')
    startStamp.write("Starting: {}\n".format(str(cur_time)))
    startStamp.close()
    slackStatusPost('*Core-Core Benchmarking Starting*\nHost: ' + hostname + '\n' + 'Time: ' + str(cur_time))

    print('\nRunning: {}\n'.format(cmd))

    subprocess.call('uname -a > uname_a.txt', shell=True, executable='/bin/bash')
    subprocess.call('dmesg | grep NO_HZ > no_hz_dmesg.txt', shell=True, executable='/bin/bash')
    subprocess.call('cat /proc/interrupts > interruptsBefore.txt', shell=True, executable='/bin/bash')
    subprocess.call(cmd, shell=True, executable='/bin/bash')
    subprocess.call('cat /proc/interrupts > interruptsAfter.txt', shell=True, executable='/bin/bash')
    diffInterruptsCmd = '../common/diffInterrupts.py --start interruptsBefore.txt --end interruptsAfter.txt {} | tee interruptsDiff.txt'.format(cpuStr)
    subprocess.call(diffInterruptsCmd, shell=True, executable='/bin/bash')

    cur_time = datetime.datetime.now()
    print("\nFinished: {}\n".format(str(cur_time)))
    endStamp = open('end_stamp.txt', 'w')
    endStamp.write("Finished: {}\n".format(str(cur_time)))
    endStamp.close()
    slackStatusPost('*Core-Core Benchmarking Finishing*\nHost: ' + hostname + '\n' + 'Time: ' + str(cur_time))

if __name__ == "__main__":
    main()