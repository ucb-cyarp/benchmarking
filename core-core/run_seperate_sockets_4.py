#!/usr/bin/env python3

import collections
import string
import platform
import subprocess
import re
import os.path

from cpuTopology import *
from slackUtils import *

def main():
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

    print('\nRunning: {}\n'.format(cmd))

    subprocess.call(cmd, shell=True, executable='/bin/bash')

if __name__ == "__main__":
    main()