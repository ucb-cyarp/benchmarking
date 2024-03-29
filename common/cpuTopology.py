#!/usr/bin/env python3

import collections
import string
import platform
import subprocess
import re
import os.path

def printTopology(topology):
    print('CPU | Socket | NUMA Node | Highest Level Cache | Core')
    for entry in topology:
        print('{:>3d}   {:>6d}   {:>9d}   {:>19d}   {:>4d}'.format(entry[0], entry[1], entry[2], entry[3], entry[4]))

def getPlatformTopology():
    """
    Returns a list of tuples (logical CPU, socket, NUMA node, core)
    """
    osType = platform.system()
    if osType == 'Linux':
        
        lscpu = subprocess.check_output('lscpu -p=CPU,SOCKET,NODE,CORE,CACHE', shell=True).decode('utf-8')

        #Parse the output from lscpu which returns CSV entries with columns : Logical CPU#, Socket, Core
        regexp = re.compile('(\\d*),(\\d*),(\\d*),(\\d*),.*:(\\d*)')

        cpu_entries = re.findall(regexp, lscpu)

        topology = []

        for entry in cpu_entries:
            cpu = int(entry[0])
            socket = int(entry[1])
            numa = int(entry[2])
            core = int(entry[3])
            highestLevelCache = int(entry[4]) #This should be the highest level shared cache (L3 for AMD processors)

            topo_entry = (cpu, socket, numa, highestLevelCache, core)
            topology.append(topo_entry)

        return topology

    return []

def getUniqueCPUs(topology):
    """
    Get a list of unique logical CPUs.  That is remove logical CPUs that share cores with logical CPUs in the list
    """

    unique_topology = []

    topology_set = set()

    for entry in topology:
        #cpu = entry[0]
        socket = entry[1]
        numa = entry[2]
        highestLevelCache = entry[3]
        core = entry[4]
        
        socket_numa_core = (socket, numa, highestLevelCache, core)

        if socket_numa_core not in topology_set:
            unique_topology.append(entry)
            topology_set.add(socket_numa_core)

    return unique_topology

def filterTopologyBySocket(topology, socket_num):
    """
    Filter the topology to only include entries for the specified socket
    """
    filtered_topology = []

    for entry in topology:
        #cpu = entry[0]
        socket = entry[1]
        #numa = entry[2]
        #highestLevelCache = entry[3]
        #core = entry[4]

        if socket == socket_num:
            filtered_topology.append(entry)

    return filtered_topology

def filterTopologyByNuma(topology, numa_num):
    """
    Filter the topology to only include entries for the specified socket
    """
    filtered_topology = []

    for entry in topology:
        #cpu = entry[0]
        #socket = entry[1]
        numa = entry[2]
        #highestLevelCache = entry[3]
        #core = entry[4]

        if numa == numa_num:
            filtered_topology.append(entry)

    return filtered_topology

def filterTopologyByHighestLevelCache(topology, highestLevelCache_num):
    """
    Filter the topology to only include entries for the specified socket
    """
    filtered_topology = []

    for entry in topology:
        #cpu = entry[0]
        #socket = entry[1]
        #numa = entry[2]
        highestLevelCache = entry[3]
        #core = entry[4]

        if highestLevelCache == highestLevelCache_num:
            filtered_topology.append(entry)

    return filtered_topology