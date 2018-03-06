#!/usr/bin/env python3

import platform
import subprocess
import re

def main():

    osType = platform.system()

    if osType == 'Linux':
        lscpu = subprocess.check_output('lscpu').decode('utf-8')
        regexp = re.compile('L3 cache.*:(.*)')
        l3Cache_str = regexp.search(lscpu).group(1).strip()

        #Check for unit
        lastChar = l3Cache_str[len(l3Cache_str)-1]
        if lastChar == 'k' or lastChar == 'K':
            #This is in k units
            l3Cache = int(l3Cache_str[0:len(l3Cache_str)-1])
            l3Cache *= 1024
        elif lastChar == 'm' or lastChar == 'M':
            #This is in m units
            l3Cache = int(l3Cache_str[0:len(l3Cache_str)-1])
            l3Cache *= 1024*1024
        else:
            #no unit
            l3Cache = int(l3Cache_str[0:len(l3Cache_str)-1])

        print(str.format('{}', l3Cache))

if __name__ == "__main__":
    main()