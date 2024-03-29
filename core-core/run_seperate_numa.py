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
    numa0_unique_topology = filterTopologyByNuma(unique_topology, 0)
    numa1_unique_topology = filterTopologyByNuma(unique_topology, 1)

    print('CPU Topology:')
    printTopology(topology)
    print('\nUnique Cores:')
    printTopology(unique_topology)
    print('\nNUMA 0 Unique Cores:')
    printTopology(numa0_unique_topology)
    print('\nNUMA 1 Unique Cores:')
    printTopology(numa1_unique_topology)

    #Get 2 Unique Cores in Socket 0 (pick sequentially for now)
    cpu_a = (numa0_unique_topology[0])[0]
    cpu_b = (numa1_unique_topology[0])[0]
    cpu_c = (numa0_unique_topology[1])[0]
    cpu_d = (numa1_unique_topology[1])[0]

    cmd = './core-core {} {} {} {}'.format(cpu_a, cpu_b, cpu_c, cpu_d)

    print('\nRunning: {}\n'.format(cmd))

    subprocess.call(cmd, shell=True, executable='/bin/bash')

if __name__ == "__main__":
    main()