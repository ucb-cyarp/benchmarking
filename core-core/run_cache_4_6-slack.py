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

from cpuTopology import *
from slackUtils import *

def main():
    hostname = platform.node()

    topology = getPlatformTopology()
    unique_topology = getUniqueCPUs(topology)

    print('CPU Topology:')
    printTopology(topology)
    print('\nUnique Cores:')
    printTopology(unique_topology)

    cpu_a = 4
    cpu_b = 6

    cmd = './core-core {} {}'.format(cpu_a, cpu_b)

    cur_time = datetime.datetime.now()
    print("Starting: {}\n".format(str(cur_time)))
    slackStatusPost('*Core-Core Benchmarking Starting*\nHost: ' + hostname + '\n' + 'Time: ' + str(cur_time))

    print('\nRunning: {}\n'.format(cmd))

    subprocess.call(cmd, shell=True, executable='/bin/bash')

    cur_time = datetime.datetime.now()
    print("\nFinished: {}\n".format(str(cur_time)))
    slackStatusPost('*Core-Core Benchmarking Finishing*\nHost: ' + hostname + '\n' + 'Time: ' + str(cur_time))

if __name__ == "__main__":
    main()
