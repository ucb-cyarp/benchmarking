#!/usr/bin/env python3

#Computes the difference between 2 dumps of /proc/interrupts for a user provided list of CPUs
#Filters out rows where there was no difference

import argparse

def getColums(header, cpuList):
    tokenizedHeader = header.split()
    startCPUCols = []

    for cpu in cpuList:
        cpuLbl = 'CPU' + str(cpu)
        ind = -1
        try:
            ind = tokenizedHeader.index(cpuLbl)
        except ValueError:
            raise RuntimeError(cpuLbl + ' not found')
        
        startCPUCols.append(ind+1) #The 0 colum (IRQ#) does not have a header
    
    numCPUs = len(tokenizedHeader)

    return startCPUCols

def parseRows(rows, cpuCols):
    irqIDOrder = []
    irqCounts = {}

    for row in rows:
        tolkenizedRow = row.split()

        if len(tolkenizedRow) > 0:
            irqIDRaw = tolkenizedRow[0]
            irqID = irqIDRaw.split(':')[0]

            irqIDOrder.append(irqID)

            counts = []

            for cpuCol in cpuCols:
                if cpuCol < len(tolkenizedRow):
                    counts.append(int(tolkenizedRow[cpuCol]))
                else:
                    counts.append(0)

            irqCounts[irqID] = counts

    return (irqCounts, irqIDOrder)

def diffCounts(irqCountsBefore, irqCountsAfter):
    irqCountDiffs = {}

    if len(irqCountsBefore) != len(irqCountsAfter):
        raise RuntimeError('Softirq Discriptors in Before and After are not the same')

    for irqID in irqCountsBefore.keys():
        if irqID not in irqCountsAfter:
            raise RuntimeError('Softirq Discriptors in Before and After are not the same')

    for irqID in irqCountsBefore.keys():
        irqCountBefore = irqCountsBefore[irqID]
        irqCountAfter = irqCountsAfter[irqID]

        if len(irqCountBefore) != len(irqCountAfter):
            raise RuntimeError('Softirq Discriptors in Before and After are not the same')

        countDiff = []
        for i in range(0, len(irqCountBefore)):
            countDiff.append(irqCountAfter[i] - irqCountBefore[i])
        
        irqCountDiffs[irqID] = countDiff
    
    return irqCountDiffs

def printTable(irqCountDiffs, cpuList, irqIDOrder):
    irqIDOrderFiltered = irqIDOrder

    irqIDHeaderLabel = 'softirq Name'
    cpuFormat = 'CPU{}'
    seperator = '    '

    #Remove entries with 0 difference
    for irqID in irqCountDiffs.keys():
        diffs = irqCountDiffs[irqID]

        rm = True

        for diff in diffs:
            rm = rm and (diff == 0)

        if rm:
            irqIDOrderFiltered.remove(irqID)
    
    #Get Table Column Widths
    widthIrqID = len(irqIDHeaderLabel)
    diffWidths = []

    for irqID in irqIDOrderFiltered:
        if len(irqID)>widthIrqID:
            widthIrqID = len(irqID)
        
        for i in range(0, len(cpuList)):
            if i >= len(diffWidths):
                diffWidths.append(len(cpuFormat.format(cpuList[i])))

            entryLen = len(str(irqCountDiffs[irqID][i]))
            if entryLen > diffWidths[i]:
                diffWidths[i] = entryLen

    #Print the table

    #Print the header
    formatStr = '{:<'+str(widthIrqID)+'}'
    print(formatStr.format(irqIDHeaderLabel) , end='')

    for i in range(0, len(cpuList)):
        cpuStr = cpuFormat.format(cpuList[i])
        formatStr = seperator+'{:>'+str(diffWidths[i])+'}'
        print(formatStr.format(cpuStr) , end='')

    print('');

    #Print Rows
    for irqID in irqIDOrderFiltered:
        formatStr = '{:<'+str(widthIrqID)+'}'
        print(formatStr.format(irqID) , end='')

        for i in range(0, len(cpuList)):
            formatStr = seperator+'{:>'+str(diffWidths[i])+'}'
            print(formatStr.format(irqCountDiffs[irqID][i]) , end='')

        print('')

def run():
    #Parse CLI Arguments for Config File Location
    parser = argparse.ArgumentParser(description='Compute the difference between 2 dumps of /proc/softirqs')
    parser.add_argument('--start', type=str, required=True, help='The dump of /proc/softirqs at the beginning of the period of interest')
    parser.add_argument('--end', type=str, required=True, help='The dump of /proc/softirqs at the end of the period of interest')
    parser.add_argument('cpu', metavar='CPU', type=int, nargs='+', help='Logical CPUs of interest')

    args = parser.parse_args()

    cpuList = args.cpu

    start = open(args.start, 'r')
    end = open(args.end, 'r')

    #Find the CPU Column from the header

    #Can use the standard str.split() method without arguments.  It treats adjacent whitespace as one separator
    startLines = start.readlines()
    endLines = end.readlines()

    # /proc/interrups is formatted in the following way: interruptNum: CPUX  CPUY ... detail1 [detail2] [detail3]

    #We are interested in colum 1, the CPU colums, and the detail colums (if they exist)
    startHeader = startLines[0]
    startRows = startLines[1:(len(startLines)-1)]

    endHeader = endLines[0]
    endRows = endLines[1:(len(endLines)-1)]

    startCPUCols = getColums(startHeader, cpuList)
    endCPUCols = getColums(endHeader, cpuList)

    startIrqCounts, startIrqIDOrder = parseRows(startRows, startCPUCols)
    endIrqCounts, endIrqIDOrder = parseRows(endRows, endCPUCols)

    irqCountDiffs = diffCounts(startIrqCounts, endIrqCounts)

    #Print entries in order and filter ones that did not change
    printTable(irqCountDiffs, cpuList, startIrqIDOrder)


if __name__ == '__main__':
    run()