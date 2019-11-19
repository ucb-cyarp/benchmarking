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
    socket0_unique_topology = filterTopologyBySocket(unique_topology, 0)
    socket1_unique_topology = filterTopologyBySocket(unique_topology, 1)

    print('CPU Topology:')
    printTopology(topology)
    print('\nUnique Cores:')
    printTopology(unique_topology)
    print('\nSocket 0 Unique Cores:')
    printTopology(socket0_unique_topology)
    print('\nSocket 1 Unique Cores:')
    printTopology(socket1_unique_topology)

    #Get 2 Unique Cores in Socket 0 (pick sequentially for now)
    cpu_a = (socket0_unique_topology[0])[0]
    cpu_b = (socket1_unique_topology[0])[0]
    cpu_c = (socket0_unique_topology[1])[0]
    cpu_d = (socket1_unique_topology[1])[0]

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