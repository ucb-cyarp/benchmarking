#!/usr/bin/env python3

import collections
import string
import platform
import subprocess
import re
import os.path

def printTopology(topology):
    print('CPU | Socket | Core')
    for entry in topology:
        print('{:>3d}   {:>6d}   {:>4d}'.format(entry[0], entry[1], entry[2]))

def getPlatformTopology():
    """
    Returns a list of tuples (logical CPU, socket, core)
    """
    osType = platform.system()
    if osType == 'Linux':
        
        lscpu = subprocess.check_output('lscpu -p=CPU,SOCKET,CORE', shell=True).decode('utf-8')

        #Parse the output from lscpu which returns CSV entries with columns : Logical CPU#, Socket, Core
        regexp = re.compile('(\\d*),(\\d*),(\\d*)')

        cpu_entries = re.findall(regexp, lscpu)

        topology = []

        for entry in cpu_entries:
            cpu = int(entry[0])
            socket = int(entry[1])
            core = int(entry[2])

            topo_entry = (cpu, socket, core)
            topology.append(topo_entry)

        return topology

def getUniqueCPUs(topology):
    """
    Get a list of unique logical CPUs.  That is remove logical CPUs that share cores with logical CPUs in the list
    """

    unique_topology = []

    topology_set = set()

    for entry in topology:
        #cpu = entry[0]
        socket = entry[1]
        core = entry[2]

        socket_core = (socket, core)

        if socket_core not in topology_set:
            unique_topology.append(entry)
            topology_set.add(socket_core)

    return unique_topology

def filterTopologyBySocket(topology, socket_num):
    """
    Filter the topology to only include entries for the specified socket
    """
    filtered_topology = []

    for entry in topology:
        #cpu = entry[0]
        socket = entry[1]
        #core = entry[2]

        if socket == socket_num:
            filtered_topology.append(entry)

    return filtered_topology

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