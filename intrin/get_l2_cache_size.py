#!/usr/bin/env python3

import platform
import subprocess
import re

def main():

    osType = platform.system()

    if osType == 'Linux':
        lscpu = subprocess.check_output('lscpu').decode('utf-8')
        regexp = re.compile('L2 cache.*:(.*)')
        l2Cache_str = regexp.search(lscpu).group(1).strip()

        #Check for unit
        lastChar = l2Cache_str[len(l2Cache_str)-1]
        if lastChar == 'k' or lastChar == 'K':
            #This is in k units
            l2Cache = int(l2Cache_str[0:len(l2Cache_str)-1])
            l2Cache *= 1024
        elif lastChar == 'm' or lastChar == 'M':
            #This is in m units
            l2Cache = int(l2Cache_str[0:len(l2Cache_str)-1])
            l2Cache *= 1024*1024
        else:
            #no unit
            l2Cache = int(l2Cache_str[0:len(l2Cache_str)-1])

        print(str.format('{}', l2Cache))

if __name__ == "__main__":
    main()