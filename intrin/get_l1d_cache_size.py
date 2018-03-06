#!/usr/bin/env python3

import platform
import subprocess
import re

def main():

    osType = platform.system()

    if osType == 'Linux':
        lscpu = subprocess.check_output('lscpu').decode('utf-8')
        regexp = re.compile('L1d cache.*:(.*)')
        l1dCache_str = regexp.search(lscpu).group(1).strip()

        #Check for unit
        lastChar = l1dCache_str[len(l1dCache_str)-1]
        if lastChar == 'k' or lastChar == 'K':
            #This is in k units
            l1dCache = int(l1dCache_str[0:len(l1dCache_str)-1])
            l1dCache *= 1024
        elif lastChar == 'm' or lastChar == 'M':
            #This is in m units
            l1dCache = int(l1dCache_str[0:len(l1dCache_str)-1])
            l1dCache *= 1024*1024
        else:
            #no unit
            l1dCache = int(l1dCache_str[0:len(l1dCache_str)-1])

        print(str.format('{}', l1dCache))

if __name__ == "__main__":
    main()