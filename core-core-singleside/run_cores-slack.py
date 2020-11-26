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
    parser = argparse.ArgumentParser(description='Run core core single side benchmark on the specified cores')
    parser.add_argument('cpu', metavar='CPU', type=int, nargs=2, help='Logical CPUs of interest')

    args = parser.parse_args()

    cpuList = args.cpu

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

    cmd = './core-core-singleside {}'.format(cpuStr)

    cur_time = datetime.datetime.now()
    print("Starting: {}\n".format(str(cur_time)))
    startStamp = open('start_stamp.txt', 'w')
    startStamp.write("Starting: {}\n".format(str(cur_time)))
    startStamp.close()
    slackStatusPost('*Core-Core Single Sided Benchmarking Starting*\nHost: ' + hostname + '\n' + 'Time: ' + str(cur_time))

    print('\nRunning: {}\n'.format(cmd))

    subprocess.call('uname -a > uname_a.txt', shell=True, executable='/bin/bash')
    subprocess.call('cat /proc/cmdline > kernel_boot_cmdline.txt', shell=True, executable='/bin/bash')
    subprocess.call('dmesg | grep NO_HZ > no_hz_dmesg.txt', shell=True, executable='/bin/bash')
    subprocess.call('dmesg | grep "RCU" > rcu_dmesg.txt', shell=True, executable='/bin/bash')
    subprocess.call('cat /proc/interrupts > interruptsBefore.txt', shell=True, executable='/bin/bash')
    subprocess.call('cat /proc/softirqs > softirqsBefore.txt', shell=True, executable='/bin/bash')
    subprocess.call('git log -1 > gitLastCommitDetailed.txt', shell=True, executable='/bin/bash')
    subprocess.call('git log -1 --format="%H"  > gitLastCommit.txt', shell=True, executable='/bin/bash')
    subprocess.call('git status -b > gitStatus.txt', shell=True, executable='/bin/bash')
    subprocess.call('git diff > gitDiff.patch', shell=True, executable='/bin/bash')
    rtnCode = subprocess.call(cmd, shell=True, executable='/bin/bash')
    subprocess.call('cat /proc/interrupts > interruptsAfter.txt', shell=True, executable='/bin/bash')
    subprocess.call('cat /proc/softirqs > softirqsAfter.txt', shell=True, executable='/bin/bash')
    diffInterruptsCmd = '../common/diffInterrupts.py --start interruptsBefore.txt --end interruptsAfter.txt {} | tee interruptsDiff.txt'.format(cpuStr)
    subprocess.call(diffInterruptsCmd, shell=True, executable='/bin/bash')
    diffSoftirqsCmd = '../common/diffSoftirqs.py --start softirqsBefore.txt --end softirqsAfter.txt {} | tee softirqsDiff.txt'.format(cpuStr)
    subprocess.call(diffSoftirqsCmd, shell=True, executable='/bin/bash')

    rtnCodeFile = open('return_code.txt', 'w')
    rtnCodeFile.write("Return Code: {}\n".format(str(rtnCode)))
    rtnCodeFile.close()

    cur_time = datetime.datetime.now()
    if rtnCode == 0:
        print("\nFinished: {}\n".format(str(cur_time)))
    else:
        print("\nErrored: {}\n".format(str(cur_time)))
    endStamp = open('end_stamp.txt', 'w')
    endStamp.write("Finished: {}\n".format(str(cur_time)))
    endStamp.close()
    print("\nCompressing Results\n")
    subprocess.call('gzip -f *.csv', shell=True, executable='/bin/bash')
    cur_time = datetime.datetime.now()
    print("\nFinished Compressing: {}\n".format(str(cur_time)))
    if rtnCode == 0:
        slackStatusPost('*Core-Core Single Sided Benchmarking Finishing :white_check_mark:*\nHost: ' + hostname + '\n' + 'Time: ' + str(cur_time))
    else:
        slackStatusPost('*Core-Core Single Sided Benchmarking Errored :x:*\nHost: ' + hostname + '\n' + 'Time: ' + str(cur_time))

if __name__ == "__main__":
    main()
