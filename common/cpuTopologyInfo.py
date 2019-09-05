#!/usr/bin/env python3

import collections
import string
import platform
import subprocess
import re


from cpuTopology import *

def main():
    topology = getPlatformTopology()
    unique_topology = getUniqueCPUs(topology)
    socket0_unique_topology = filterTopologyBySocket(unique_topology, 0)
    numa0_unique_topology = filterTopologyByNuma(unique_topology, 0)
    cache0_unique_topology = filterTopologyByHighestLevelCache(unique_topology, 0)

    print('CPU Topology:')
    printTopology(topology)
    print('\nUnique Cores:')
    printTopology(unique_topology)
    print('\nSocket 0 Unique Cores:')
    printTopology(socket0_unique_topology)
    print('\nNUMA 0 Unique Cores:')
    printTopology(numa0_unique_topology)
    print('\nHighest Level Cache 0 Unique Cores:')
    printTopology(cache0_unique_topology)

if __name__ == "__main__":
    main()