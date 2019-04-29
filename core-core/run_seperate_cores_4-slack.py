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
    numa0_unique_topology = filterTopologyByNuma(unique_topology, 0)

    print('CPU Topology:')
    printTopology(topology)
    print('\nUnique Cores:')
    printTopology(unique_topology)
    print('\nNUMA 0 Unique Cores:')
    printTopology(numa0_unique_topology)

    #Get 2 Unique Cores in Socket 0 (pick sequentially for now)
    cpu_a = (numa0_unique_topology[0])[0]
    cpu_b = (numa0_unique_topology[1])[0]
    cpu_c = (numa0_unique_topology[2])[0]
    cpu_d = (numa0_unique_topology[3])[0]

    cmd = './core-core {} {} {} {}'.format(cpu_a, cpu_b, cpu_c, cpu_d)

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