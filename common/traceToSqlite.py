#!/bin/python3

import sqlite3
import argparse
import re
import os

class Trace:
    def __init__(self, process='', cpu=-1, flags='', time=-1, event='', eventDetails=''):
        self.process = process
        self.cpu = cpu
        self.flags = flags
        self.time = time
        self.event = event
        self.eventDetails = eventDetails

def createTable(cursor):
    cursor.execute('''
        CREATE TABLE trace
        (process TEXT, cpu INT, flags TEXT, time REAL, event TEXT, event_details TEXT)
    ''')

def parseLines(traceLines):
    parsedTraces = []
    regex = re.compile('\\s*(\\S*)\\s+\\[(\\d*)\\]\\s+(\\S*)\\s+([0-9,.]*):\\s+([^:]*): (.*)')

    for line in traceLines:
        m = regex.match(line)

        trace = Trace(process=m.group(1), cpu=int(m.group(2)), flags=m.group(3), time=float(m.group(4)), event=m.group(5), eventDetails=m.group(6))
        parsedTraces.append(trace)

    return parsedTraces

def insertIntoSql(cursor, parsedTraces):
    for trace in parsedTraces:
        cursor.execute('INSERT INTO trace VALUES (?,?,?,?,?,?)', (trace.process, trace.cpu, trace.flags, trace.time, trace.event, trace.eventDetails))

    return

def run():
    #Parse CLI Arguments for Config File Location
    parser = argparse.ArgumentParser(description='Converts kernel logs to sqlite')
    parser.add_argument('-o', type=str, required=False, default='', help='Specify the output filename')
    parser.add_argument('src', type=str, help='Logical CPUs of interest')

    args = parser.parse_args()

    srcFilePath = args.src

    #Validate
    if not os.path.isfile(srcFilePath):
        print(srcFilePath + ' is not a file')
        exit(1)

    if args.o == '':
        outFilePath = os.path.relpath(srcFilePath)
        outFilePath, outFileExt = os.path.splitext(outFilePath)
        outFilePath = outFilePath + '.db'
    else:
        outFilePath = args.o

    srcFile = open(srcFilePath, 'r')
    traceLines = srcFile.readlines()
    srcFile.close()

    parsedTraces = parseLines(traceLines)
    
    conn = sqlite3.connect(outFilePath)
    cursor = conn.cursor()

    createTable(cursor)
    insertIntoSql(cursor, parsedTraces)

    conn.commit()
    conn.close()

if __name__ == '__main__':
    run()

