#!/usr/bin/env python3

#Autotuner/Excersiser for FIR1_1_Tester1

import collections
import string
import platform
import subprocess
import re
import sqlite3
import os.path
import datetime
import csv
import itertools
import threading
import json
import requests

class Parameter:
    """
    Class used to hold information about a parameter included in either 
    a call to the compiler or at runtime.  The paramString is what is 
    actually used when running the compiler or program.  If a parameter 
    only exists (ie. has no assigned value), the value field can be
    blank
    """
    def __init__(self, name=None, paramString=None, value=None):
        self._name = name
        self._paramString = paramString
        self._value = value

    #TODO: Add parameter class

    def name(self, name=None):
        if name is not None:
            self._name = name
        return self._name

    def paramString(self, paramString=None):
        if paramString is not None:
            self._paramString = paramString
        return self._paramString

    def value(self, value=None):
        if value is not None:
            self._value = value
        return self._value

    def addParameterValueEntryIfNotAlready(self, sqlCursor):
        """
        Add a parameter value, and the underlying parameter, to the database if it is not already
        included in the database.  Returns the (parameterID, parameterValueID).
        """

        #TODO: Refactor with define syntax for compilers.  OK for now since virtually all compilers
        #the -D<arg> syntax
        
        #Add Parameter
        parameterID = addIfNotAlready(sqlCursor, 'SELECT ParameterID FROM Parameter WHERE Parameter.Name=?', 
                    'INSERT INTO Parameter (`Name`) VALUES (?)', (self._name,))

        #Add Parameter Value
        #Note, there are 2 value columns: one for numerics, one for text
        if isinstance(self._value, str):
            #String Param
            parameterValueID = addIfNotAlready(sqlCursor, 'SELECT ParameterValueID FROM ParameterValue WHERE ParameterValue.ParameterID=? AND ParameterValue.ValueString=? AND ParameterValue.FullString=?;',
                        'INSERT INTO ParameterValue (`ParameterID`, `ValueString`, `FullString`) VALUES (?, ?, ?);', (parameterID, self._value, self._paramString))
        elif self._value is not None:
            #Numeric Param
            parameterValueID = addIfNotAlready(sqlCursor, 'SELECT ParameterValueID FROM ParameterValue WHERE ParameterValue.ParameterID=? AND ParameterValue.ValueNumeric=? AND ParameterValue.FullString=?;',
                        'INSERT INTO ParameterValue (`ParameterID`, `ValueNumeric`, `FullString`) VALUES (?, ?, ?);', (parameterID, self._value, self._paramString))
        else:
            #Basic Param (no value)
            parameterValueID = addIfNotAlready(sqlCursor, 'SELECT ParameterValueID FROM ParameterValue WHERE ParameterValue.ParameterID=? AND ParameterValue.FullString=?;',
                        'INSERT INTO ParameterValue (`ParameterID`, `FullString`) VALUES (?, ?);', (parameterID, self._paramString))

        return (parameterID, parameterValueID)

class OptionList:
    """
    Contains a list of options.  This set may consist of sublists 
    which are exclusive, depandant

    Note: This should be a set but ordering is potentially 
    important when applying compiler flags.
    """

    def __init__(self, options=None):
        """
        Initilize an option list.
        """
        if options is not None:
            if isinstance(options, collections.Iterable) and isinstance(options, collections.Sized):
                self._options = options
            else:
                self._options = [options]
        else:
            self._options = []

    def options(self, options=None):
        if options is not None:
            self._options = options
        return self._options

    def addOption(self, option):
        self._options.append(option)

    def numberOfFlagCombinations(self):
        """
        Returns the number of compiler options that will be genertated 
        by the compiler flag generator.
        """

        #Special case when options is empty
        if len(self._options)==0:
            return 1 #will return the case of no options

        #Find the number of combinations for each option in the list and multiply
        combinations=1
        for option in self._options:
            combinations *= option.numberOfFlagCombinations()

        return combinations

    def compilerFlagGenerator(self):
        """
        Generates compiler flags for different compiler runs based off 
        of the options included in the option set.  All legal combinations 
        from the perspective of the CompilerOptionSet will be generated.\n\n

        Flags will be included in the order that they are given in the option 
        list.
        """

        def compilerFlagGeneratorHelper(options):
            """
            Helper function to facilitate recursive calls from compilerFlagGenerator
            """

            #Base case when list is empty
            if len(options) == 0:
                yield []
            else:
                l1FlagGenerator = options[0].compilerFlagGenerator()
                for l1Flag in l1FlagGenerator:
                    l2Flags = compilerFlagGeneratorHelper(options[1:]) #The recursive call, slicing the list
                    for l2Flag in l2Flags:

                        #avoid extra spaces when flags are nothing
                        if (l1Flag is not None) and (l2Flag is not None):
                            yield l1Flag + l2Flag
                        elif (l1Flag is not None) and (l2Flag is None):
                            yield l1Flag
                        elif (l1Flag is None) and (l2Flag is not None):
                            yield l2Flag
                        else:
                            yield []

        return compilerFlagGeneratorHelper(self._options)


class ExclusiveOptionList(OptionList):
    """
    Represents a List of options that are mutually exclusive.  
    -O0, -O1, -02, -O3 are mutually exclusive compiler flags for instance.  
    In most cases, the options should be Always options or EnumOptions where 
    alwaysIncluded==True.  This avoids duplicate cases where duplicate blank 
    arguments are included.
    """

    def __init__(self, options=None, alwaysIncluded=False):
        """
        In most cases, the options should be Always options or EnumOptions where 
        alwaysIncluded==True.  alwaysIncluded for ExclusiveOptionList determines 
        if a blank case where none of the options are included should be produced
        """
        super().__init__(options)
        self._alwaysIncluded=alwaysIncluded

    def numberOfFlagCombinations(self):
        """
        Returns the number of compiler options that will be genertated 
        by this exclusive option list.
        """

        #Instead of multiplying the options, we sum them since each option
        #is mutually exclusive

        if self._alwaysIncluded is False:
            combinations = 1
        else:
            combinations = 0

        for option in self._options:
            combinations += option.numberOfFlagCombinations()

        return combinations

    def compilerFlagGenerator(self):
        """
        Generates compiler flags for different compiler runs based off 
        of the options included in the option set.  All legal combinations 
        from the perspective of the CompilerOptionSet will be generated.\n\n

        Flags will be included in the order that they are given in the option 
        list.
        """

        #Since these options are mutually exclusive, the recursive structure
        #used in the superclass is not required; each option is inspected
        #independantly

        if self._alwaysIncluded is False:
            yield []

        for option in self._options:
            caseGenerator = option.compilerFlagGenerator()
            for case in caseGenerator:
                yield case

class DependantOptionList(OptionList):
    """
    Represents a list of options that are dependant on a single parent option.  
    This option is assumed to be a binary option or an always option

    For example, certain options can only be set when another option is set.
    """

    def __init__(self, parent=None, children=None):
        """
        Initilize a dependent option list with a parent and depedant options.  
        Dependent options can also contain exclusive or dependant options
        """
        super().__init__(children)
        if (not isinstance(parent, AlwaysOption)) and (not isinstance(parent, BinaryOption)):
            raise Exception('Parent must either by an AlwaysOption or a BinaryOption')

        self._parent = parent

    def numberOfFlagCombinations(self):
        """
        Returns the number of compiler options that will be genertated 
        by this dependant option list.
        """

        #This is the number of options in the children + 1 for the case
        #when the parent is not included

        combinations = super().numberOfFlagCombinations()
        combinations += self._parent.numberOfFlagCombinations()

        return combinations


    def compilerFlagGenerator(self):
        """
        Generates compiler flags for different compiler runs based off 
        of the options included in the option set.  All legal combinations 
        from the perspective of the CompilerOptionSet will be generated.\n\n

        Flags will be included in the order that they are given in the option 
        list.
        """
        
        parentCombonations = self._parent.compilerFlagGenerator()
        childCombonations = super().compilerFlagGenerator()

        for parentCombonation in parentCombonations:
            for childCombonation in childCombonations:
                if (parentCombonation is not None) and (childCombonation is not None):
                    yield parentCombonation + childCombonation
                elif (parentCombonation is not None) and (childCombonation is None):
                    yield parentCombonation 
                elif (parentCombonation is None) and (childCombonation is not None):
                    yield childCombonation
                else:
                    yield []

class EnumOption(OptionList):
    """
    Base case representing an option which can take on many values or which does 
    not exist at all.
    """

    def __init__(self, name=None, values=None, syntax='-{}={}', alwaysIncluded=False):
        """
        Initilize an enum option.  The name of the option, the syntax for the option, 
        the possible values of the option define how the command line option will be 
        printed.  alwaysIncluded==False will also add the case that the option is not 
        included at all.
        """
        super().__init__(name)
        self._syntax=syntax
        self._values=values
        self._alwaysIncluded=alwaysIncluded

    def numberOfFlagCombinations(self):
        """
        Returns the number of cases this option can represent
        """

        combinations = len(self._values)
        if self._alwaysIncluded==False:
            combinations += 1

        return combinations

    def compilerFlagGenerator(self):
        """
        Generates the option combinations based on the values provided.  
        If alwaysIncluded==False, the empty string is returned first.
        """

        if self._alwaysIncluded is False:
            yield []
        for value in self._values:
            yield [Parameter(self._options, self._syntax.format(self._options, value), value)]

    def addOption(self, option):
        raise Exception('Cannot Add to Enum Option.  Can add value with addValue(value)')

    def addValue(self, option):
        self._values.append(option)

class BinaryOption(OptionList):
    """
    Base case representing a simple option which either exists or does not.
    """

    #Don't need to override constructor
    
    def numberOfFlagCombinations(self):
        """
        A binary operator only has 2 options. 2 will be returned by this function
        """

        return 2

    def compilerFlagGenerator(self):
        """
        Generates the case when the flag is not present and then the case when the flag
        is present.
        """

        yield []
        yield [Parameter(self._options, self._options)]

    def addOption(self, option):
        raise Exception('Cannot Add to Binary Option')

class AlwaysOption(OptionList):
    """
    Base case representing a option which is always present
    """

    #Don't need to override constructor
    
    def numberOfFlagCombinations(self):
        """
        An always option only has 1 option, itself. 1 will be returned by this function
        """

        return 1

    def compilerFlagGenerator(self):
        """
        Generates the case when the flag is present
        """

        yield [Parameter(self._options, self._options)]

    def addOption(self, option):
        raise Exception('Cannot Add to Always Option')

def FlagsToFileSafeString(flagString):
    safeString = flagString
    safeString = safeString.replace('=', '')
    safeString = safeString.replace('*', '')
    safeString = safeString.replace('+', '')
    safeString = safeString.replace('/', '')
    safeString = safeString.replace('\\', '')
    safeString = safeString.replace('{', '')
    safeString = safeString.replace('}', '')
    safeString = safeString.replace('<', '')
    safeString = safeString.replace('>', '')
    safeString = safeString.replace('[', '')
    safeString = safeString.replace(']', '')
    safeString = safeString.replace('(', '')
    safeString = safeString.replace(')', '')
    safeString = safeString.replace('?', '')
    safeString = safeString.replace('!', '')
    safeString = safeString.replace('#', '')
    safeString = safeString.replace('$', '')
    safeString = safeString.replace('%', '')
    safeString = safeString.replace('^', '')
    safeString = safeString.replace('&', '')
    safeString = safeString.replace('`', '')
    safeString = safeString.replace('\'', '')
    safeString = safeString.replace('\"', '')
    safeString = safeString.replace('|', '')
    safeString = safeString.replace('~', '')
    safeString = safeString.replace('.', '')
    safeString = safeString.replace(' ', '')
    return safeString

def stringFromParameterList(parameters):
    """
    Convert a list of parameters to a string to be used in compilation or execution.
    """

    return ' '.join(map(lambda param: param.paramString(), parameters))

def addCompilerParametersToDBIfNotAlready(sqlCursor, parameters):
    """
    Add parameters (and prameter values) to the database if they are not already. 
    Returns an array of (ParameterID, ParameterValueID)
    """

    ids = []

    for parameter in parameters:
        id_tuple = parameter.addParameterValueEntryIfNotAlready(sqlCursor)
        ids.append(id_tuple)

    return ids

def addCompilerParametersToDBIfNotAlreadyLinkCompiler(sqlCursor, parameters, compilerID, runID):
    """
    Add parameters (and prameter values) to the database if they are not already. Also 
    add entries linking to the compiler as appropriate (if no such link exists already).
    Also adds an entry to the Config table relating to the specified run.
    Returns an array of (ParameterID, ParameterValueID)
    """

    #Get Phase & Usage Descriptors for config
    phaseID = addIfNotAlready(sqlCursor, 'SELECT PhaseID FROM Phase WHERE Phase.Name=?;',
             'INSERT INTO Phase (`Name`) VALUES (?);', ('Compile',))

    usageID = addIfNotAlready(sqlCursor, 'SELECT UsageID FROM Usage WHERE Usage.Name=?;',
             'INSERT INTO Usage (`Name`) VALUES (?);', ('Compiler',))

    ids = []

    for parameter in parameters:
        id_tuple = parameter.addParameterValueEntryIfNotAlready(sqlCursor)
        parameterID = id_tuple[0]
        parameterValueID = id_tuple[1]

        #Link Parameter to compiler
        addIfNotAlready(sqlCursor, 'SELECT rowid FROM CompilerSupportedParameterValues WHERE CompilerSupportedParameterValues.CompilerID=? AND CompilerSupportedParameterValues.ParameterID=?;',
                        'INSERT INTO CompilerSupportedParameterValues (`CompilerID`, `ParameterID`) VALUES (?, ?);', (compilerID, parameterID))

        #Add Config Entry
        addIfNotAlready(sqlCursor, 'SELECT ConfigID FROM Config WHERE Config.RunID=? AND Config.ParameterValueID=? AND Config.PhaseID=? AND Config.UsageID=?;',
                        'INSERT INTO Config (`RunID`, `ParameterValueID`, `PhaseID`, `UsageID`) VALUES (?, ?, ?, ?);', (runID, parameterValueID, phaseID, usageID))

        ids.append(id_tuple)

    return ids

def addParametersToDBIfNotAlreadyLinkKernelInst(sqlCursor, parameters, kernelInstanceID, runID, phase):
    """
    Add parameters (and prameter values) to the database if they are not already. Also 
    add entries linking to the Kernel Instance as appropriate (if no such link exists already).
    Also adds an entry to the Config table relating to the specified run.
    Phase should be 'Compile' for compile time parameters or 'Run' is used durring runtime
    Returns an array of (ParameterID, ParameterValueID)
    """

    #Get Phase & Usage Descriptors for config
    phaseID = addIfNotAlready(sqlCursor, 'SELECT PhaseID FROM Phase WHERE Phase.Name=?;',
             'INSERT INTO Phase (`Name`) VALUES (?);', (phase,))

    usageID = addIfNotAlready(sqlCursor, 'SELECT UsageID FROM Usage WHERE Usage.Name=?;',
             'INSERT INTO Usage (`Name`) VALUES (?);', ('Kernel',))

    ids = []

    for parameter in parameters:
        id_tuple = parameter.addParameterValueEntryIfNotAlready(sqlCursor)
        parameterID = id_tuple[0]
        parameterValueID = id_tuple[1]

        #Link Parameter to kernel instance
        addIfNotAlready(sqlCursor, 'SELECT rowid FROM KernelInstanceParameters WHERE KernelInstanceParameters.KernelInstanceID=? AND KernelInstanceParameters.ParameterID=?;',
                        'INSERT INTO KernelInstanceParameters (`KernelInstanceID`, `ParameterID`) VALUES (?, ?);', (kernelInstanceID, parameterID))

        #Add Config Entry
        addIfNotAlready(sqlCursor, 'SELECT ConfigID FROM Config WHERE Config.RunID=? AND Config.ParameterValueID=? AND Config.PhaseID=? AND Config.UsageID=?;',
                        'INSERT INTO Config (`RunID`, `ParameterValueID`, `PhaseID`, `UsageID`) VALUES (?, ?, ?, ?);', (runID, parameterValueID, phaseID, usageID))

        ids.append(id_tuple)

    return ids

class Compiler:
    """
    Create a class to represent a given Compiler.  
    Includes information such as the name of the compiler, the 
    """

    def __init__(self, name='gcc', command='g++', envSetup=None, vendor=None, options=None, defineFormat='-D{}={}', compileFormat='-c {}', outputFormat='-o {}', compilerID=None):
        """Initialize a Compiler"""
        self._envSetup = envSetup
        self._name = name
        self._vendor = vendor
        self._command = command
        self._defineFormat = defineFormat
        self._outputFormat = outputFormat
        self._compileFormat = compileFormat
        if options is not None:
            self._options = options
        else:
            self._options = OptionList()
        self._compilerID = compilerID

    #Getters & Setters
    def envSetup(self, envSetup=None):
        if envSetup is not None:
            self._envSetup = envSetup
        return self._envSetup

    def name(self, name=None):
        if name is not None:
            self._name=name
        return self._name

    def vendor(self, vendor=None):
        if vendor is not None:
            self._vendor = vendor
        return self._vendor

    def command(self, command=None):
        if command is not None:
            self._command = command
        return self._command

    def defineFormat(self, defineFormat=None):
        if defineFormat is not None:
            self._defineFormat = defineFormat
        return self._defineFormat

    def compileFormat(self, compileFormat=None):
        if compileFormat is not None:
            self._compileFormat = compileFormat
        return self._compileFormat

    def outputFormat(self, outputFormat=None):
        if outputFormat is not None:
            self._outputFormat = outputFormat
        return self._outputFormat

    def options(self, options=None):
        if options is not None:
            self._options=options
        return self._options

    def compilerID(self, compilerID=None):
        if compilerID is not None:
            self._compilerID = compilerID
        return self._compilerID

    def addCompilerEntryIfNotAlready(self, sqlCursor):
        """
        Add compiler entry if not already in DB.  Returns compilerID of entry regardless
        """
        vendorID = addIfNotAlready(sqlCursor, 'SELECT VendorID FROM Vendor WHERE Vendor.Name=?;', 
                   'INSERT INTO Vendor (`Name`) VALUES (?);', (self.vendor(),))

        compilerID = addIfNotAlready(sqlCursor, 'SELECT CompilerID FROM Compiler WHERE Compiler.Name=? AND Compiler.Command=? AND Compiler.VendorID=?;',
                     'INSERT INTO Compiler (`Name`, `Command`, `VendorID`) VALUES (?, ?, ?);', (self._name, self._command, vendorID))

        return compilerID

class Suite:
    def __init__(self, name=None, description=None,  kernels=[]):
        """
        Name = Name of Kernel Suite
        Instances = List of Kernel Instances
        """
        self._name = name
        self._description = description
        self._kernels = kernels

    def name(self, name=None):
        if name is not None:
            self._name = name
        return self._name

    def description(self, description=None):
        if description is not None:
            self._description = description
        return self._description

    def kernels(self, kernels=None):
        if kernels is not None:
            self._kernels = kernels
        return self._kernels

    def addKernel(self, kernel):
        self._kernels.append(kernel)

    def addSuiteEntryIfNotAlready(self, sqlCursor):
        """
        Add suite entry into DB if it is not already.  Returns suiteID reguardless
        """

        suiteID = addIfNotAlready(sqlCursor, 'SELECT SuiteID FROM Suite WHERE Suite.Name=? AND Suite.Description=?;',
                  'INSERT INTO Suite (`Name`, `Description`) VALUES (?, ?);', (self._name, self._description))

        return suiteID

class Kernel:
    def __init__(self, name=None, description=None, instances=[]):
        """
        Name = Name of Kernel
        Instances = List of Kernel Instances
        """
        self._name = name
        self._description = description
        self._instances = instances

    def name(self, name=None):
        if name is not None:
            self._name = name
        return self._name
    
    def description(self, description=None):
        if description is not None:
            self._description = description
        return self._description

    def instances(self, instances=None):
        if instances is not None:
            self._instances = instances
        return self._instances

    def addInstance(self, instance):
        self._instances.append(instance)

    def addKernelEntryIfNotAlready(self, sqlCursor):
        """
        Add kernel entry to DB if it is not already.  Return KernelID regardless
        """

        kernelID = addIfNotAlready(sqlCursor, 'SELECT KernelID FROM Kernel WHERE Kernel.Name=? AND Kernel.Description=?;',
                  'INSERT INTO Kernel (`Name`, `Description`) VALUES (?, ?);', (self._name, self._description))

        return kernelID

class KernelInstance:
    def __init__(self, kernel=None, description=None, fileList=None, compileOptions=None, runOptions=None, outputFileFormatStr='{}'):
        """
        FileList: List of files to compile
        CompileOptions: OptionList to be exersized at compile time
        RunOptions: OptionList to be exersized at run time
        """
        self._description = description
        self._kernel = None # Link to containing Kernel

        if fileList is not None:
            self._fileList = fileList
        else:
            self._fileList = []

        if compileOptions is not None:
            self._compileOptions = compileOptions
        else:
            self._compileOptions = OptionList()

        if runOptions is not None:
            self._runOptions = runOptions
        else:
            self._runOptions = OptionList()

        self._outputFileFormatStr = outputFileFormatStr

    def fileList(self, fileList=None):
        if fileList is not None:
            self._fileList = fileList
        return self._fileList

    def compileOptions(self, compileOptions=None):
        if compileOptions is not None:
            self._compileOptions = compileOptions
        return self._compileOptions

    def runOptions(self, runOptions=None):
        if runOptions is not None:
            self._runOptions = runOptions
        return self._runOptions
    
    def outputFileFormatStr(self, outputFileFormatStr=None):
        if outputFileFormatStr is not None:
            self._outputFileFormatStr = outputFileFormatStr
        return self._outputFileFormatStr
    
    def addFile(self, file):
        self._fileList.append(file)

    def addKernelImplementationAndFileEntriesIfNotAlready(self, sqlCursor, kernelID):
        """
        Add kernel implementation to the database if it is not already included. 
        Also add kernel files to database if they are not already.
        Returns (KernelImplementationID, [KernelFileID, ...])
        """

        #Create KernelInstance Entry
        kernelImplementationID = addIfNotAlready(sqlCursor, 'SELECT KernelInstanceID FROM KernelInstance WHERE KernelInstance.KernelID=? AND KernelInstance.Description=?;',
                                 'INSERT INTO KernelInstance (`KernelID`, `Description`) VALUES (?, ?);', (kernelID, self._description))

        #Create KernelFileEntries
        kernelFileIDs = []
        
        for file in self._fileList:
            kernelFileID = addIfNotAlready(sqlCursor, 'SELECT KernelFileID FROM KernelFile WHERE KernelFile.KernelInstanceID=? AND KernelFile.Filename=?;',
                           'INSERT INTO KernelFile (`KernelInstanceID`, `Filename`) VALUES (?, ?);', (kernelImplementationID, file))
            kernelFileIDs.append(kernelFileID)

        return (kernelImplementationID, kernelFileIDs)

class Machine:
    """
    Defines the machine this benchmark is being run on
    """

    def __init__(self, friendlyName='', interconnect='', nodeList=[], sqlMachineID=None):
        self._friendlyName = friendlyName
        self._interconnect = interconnect
        self._nodeList = nodeList
        self._sqlMachineID = sqlMachineID

    def friendlyName(self, friendlyName=None):
        if friendlyName is not None:
            self._friendlyName = friendlyName
        return self._friendlyName

    def interconnect(self, interconnect=None):
        if interconnect is not None:
            self._interconnect = interconnect
        return self._interconnect

    def nodeList(self, nodeList=None):
        if nodeList is not None:
            self._nodeList = nodeList
        return self._nodeList

    def sqlMachineID(self, sqlMachineID=None):
        if sqlMachineID is not None:
            self._sqlMachineID = sqlMachineID
        return self._sqlMachineID

    def addNode(self, node):
        self._nodeList.append(node)

    def autoPopulateMachineInfo(self, sqlCursor):
        """
        Auto-popuate machine info and set sqlMachineID
        """
        #TODO: Make this check if entries already exist.  Right now, it simply adds
        #and does not check

        self._sqlMachineID = autoPopulateMachineInfo(sqlCursor, self._friendlyName, self._interconnect)

def flagTester():
    options = OptionList()
    options.addOption(AlwaysOption('-Wall'))
    options.addOption(BinaryOption('-g'))
    options.addOption(ExclusiveOptionList([AlwaysOption('-O0'), AlwaysOption('-O1'), AlwaysOption('-O2'), DependantOptionList(AlwaysOption('-O3'), [BinaryOption('--vectorize'), BinaryOption('--threadme')])]))
    options.addOption(DependantOptionList(AlwaysOption('--superflag'), ExclusiveOptionList([AlwaysOption('--superchoice1'), AlwaysOption('--superchoice2')])))
    options.addOption(EnumOption('march', ['sse', 'sse2', 'avx', 'fma'], '--{}={}'))
    options.addOption(EnumOption('UnrollFactor', range(0,6,2), '--{}={}'))

    flagGenerator = options.compilerFlagGenerator()

    for flags in flagGenerator:
        flagString = stringFromParameterList(flags)
        print(flagString)
        # print(FlagsToFileSafeString(flagString))

def createSqlTables(sqlCursor):
    """Create SQL Tables"""

    tableCreateCmds=[
    """
    CREATE TABLE `Suite` (
        `SuiteID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT,
        `Description`	TEXT
    );""", 
    """
    CREATE TABLE `Kernel` (
        `KernelID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT,
        `Description`	TEXT
    );""",
    """
    CREATE TABLE `KernelSuite` (
        `SuiteID`	INTEGER NOT NULL,
        `KernelID`	INTEGER NOT NULL,
        PRIMARY KEY(`SuiteID`,`KernelID`)
    );""", 
    """
    CREATE TABLE `KernelInstance` (
        `KernelInstanceID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `KernelID`	INTEGER NOT NULL,
        `Description`	INTEGER
    );""", 
    """
    CREATE TABLE `KernelFile` (
        `KernelFileID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `KernelInstanceID`	INTEGER NOT NULL,
        `Filename`	TEXT
    );""", 
    """
    CREATE TABLE `Parameter` (
        `ParameterID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT
    );""",
    """
    CREATE TABLE `Class` (
        `ClassID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT
    );""",
    """
    CREATE TABLE `ParameterClass` (
        `ParameterID`	INTEGER NOT NULL,
        `ClassID`	INTEGER NOT NULL,
        PRIMARY KEY(`ParameterID`,`ClassID`)
    );""",
    """
    CREATE TABLE `ParameterValue` (
        `ParameterValueID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `ParameterID`	INTEGER NOT NULL,
        `ValueNumeric`	NUMERIC,
        `ValueString`	TEXT,
        `FullString`	TEXT
    );""",
    """
    CREATE TABLE `Vendor` (
        `VendorID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT
    );""",
    """
    CREATE TABLE `Compiler` (
        `CompilerID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT,
        `Command`	TEXT,
        `VendorID`	INTEGER
    );""",
    """
    CREATE TABLE `Machine` (
        `MachineID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT,
        `Interconnect`	TEXT
    );""",
    """
    CREATE TABLE `Status` (
        `StatusID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT
    );""",
    """
    CREATE TABLE `Run` (
        `RunID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `KernelInstanceID`	INTEGER NOT NULL,
        `MachineID`	INTEGER NOT NULL,
        `DateTimeStart`	TEXT,
        `DateTimeStop`	TEXT,
        `StatusID`	INTEGER,
        `Notes`	TEXT,
        `CompilerID`	INTEGER NOT NULL,
        `CommandExecuted`	TEXT,
        `RawResult`	BLOB,
        `RawResultFormat`	TEXT
    );""",
    """
    CREATE TABLE `Phase` (
        `PhaseID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT
    );""",
    """
    CREATE TABLE `Usage` (
        `UsageID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	BLOB
    );""",
    """
    CREATE TABLE `Config` (
        `ConfigID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `RunID`	INTEGER NOT NULL,
        `ParameterValueID`	INTEGER NOT NULL,
        `PhaseID`	INTEGER NOT NULL,
        `UsageID`	INTEGER NOT NULL
    );""",
    """
    CREATE TABLE `Trial` (
        `TrialID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `RunID`	INTEGER NOT NULL,
        `TrialNumber`	INTEGER NOT NULL
    );""",
    """
    CREATE TABLE `ResultType` (
        `ResultTypeID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT,
        `IsSummary`	INTEGER NOT NULL
    );""",
    """
    CREATE TABLE `Result` (
        `ResultID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `TrialID`	INTEGER NOT NULL,
        `ResultTypeID`	INTEGER NOT NULL,
        `Unit`	TEXT,
        `Value`	NUMERIC
    );""",
    """
    CREATE TABLE `OS` (
        `OSID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT,
        `Distribution`	TEXT,
        `Version`	TEXT,
        `VendorID`	INTEGER
    );""",
    """
    CREATE TABLE `Node` (
        `NodeID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `MachineID`	INTEGER NOT NULL,
        `Hostname`	TEXT,
        `OSID`	INTEGER NOT NULL,
        `Hypervisor`	INTEGER,
        `Numa`	INTEGER
    );""",
    """
    CREATE TABLE `BaseISA` (
        `BaseISAID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT
    );""",
    """
    CREATE TABLE `ISAExtension` (
        `ISAExtensionID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT,
        `BaseISAID`	INTEGER NOT NULL
    );""",
    """
    CREATE TABLE `Processor` (
        `ProcessorID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT,
        `Vendor`	INTEGER NOT NULL,
        `BaseISAID`	INTEGER NOT NULL,
        `Cores`	INTEGER
    );""",
    """
    CREATE TABLE `Cache` (
        `CacheID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `ProcessorID`	INTEGER NOT NULL,
        `Level`	TEXT,
        `Size`	Text
    );""",
    """
    CREATE TABLE `ProcessorISAExtension` (
        `ProcessorID`	INTEGER NOT NULL,
        `ISAExtensionID`	INTEGER NOT NULL,
        PRIMARY KEY(`ProcessorID`,`ISAExtensionID`)
    );""",
    """
    CREATE TABLE `MemoryType` (
        `MemoryTypeID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `Name`	TEXT,
        `SpeedMHz`	NUMERIC
    );""",
    """
    CREATE TABLE `Socket` (
        `SocketID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `ProcessorID`	INTEGER NOT NULL,
        `Memory`	TEXT,
        `MemoryTypeID`	INTEGER
    );""",
    """
    CREATE TABLE `NodeSocket` (
        `NodeSocketID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `NodeID`	INTEGER NOT NULL,
        `SocketID`	INTEGER NOT NULL
    );""",
    """
    CREATE TABLE `KernelInstanceParameters` (
        `KernelInstanceID`	INTEGER NOT NULL,
        `ParameterID`	INTEGER NOT NULL,
        PRIMARY KEY(`KernelInstanceID`,`ParameterID`)
    );""",
    """
    CREATE TABLE `CompilerSupportedParameterValues` (
        `CompilerID`	INTEGER NOT NULL,
        `ParameterID`	INTEGER NOT NULL,
        PRIMARY KEY(`CompilerID`,`ParameterID`)
    );
    """,
    """CREATE INDEX `ConfigInd` ON `Config` ( `RunID`, `ParameterValueID`, `PhaseID`, `UsageID` );""",
    """CREATE INDEX `RunInd` ON `Run` ( `KernelInstanceID`, `MachineID`, `CompilerID` );""",
    """CREATE INDEX `ParameterValueInd` ON `ParameterValue` ( `ParameterID` );"""]

    for cmd in tableCreateCmds:
        #print(cmd)
        sqlCursor.execute(cmd)

def autoPopulateMachineInfo(sqlCursor, machineFriendlyName=None, interconnect=None):
    """
    Autopopulate machine info assuming entry does not already exist in DB.  Returns machine ID
    """

    #TODO: Check if already in table
    #TODO: Extend for OS appart from linux

    #Create Machine
    #TODO: Interconnect left blank for now
    sqlCursor.execute('INSERT INTO Machine (`Name`, `Interconnect`) VALUES (?, ?);', (machineFriendlyName, interconnect))
    sqlCursor.execute('SELECT MachineID FROM Machine WHERE Machine.rowid=?;', (sqlCursor.lastrowid,))
    machineID = sqlCursor.fetchone()[0]

    osType = platform.system()

    if osType == 'Linux':
        #Get Dist Information
        distName = (platform.dist())[0]
        distVersion = (platform.dist())[1]

        #Create OS Entry (For Now, Will not populate vendor)
        sqlCursor.execute('INSERT INTO OS (`Name`, `Distribution`, `Version`) VALUES (?, ?, ?);', (osType, distName, distVersion))
        sqlCursor.execute('SELECT OSID FROM OS WHERE OS.rowid=?;', (sqlCursor.lastrowid,))
        OSID = sqlCursor.fetchone()[0]

        #Find processor info
        #TODO: for now, will assume sockets are identicle if there are multiple sockets.
        #TODO: in vm, multiple cores are presented as multiple processors.  For now, will just diplay hyporvisor string
        lscpu = subprocess.check_output('lscpu').decode('utf-8')
        
        #Create baseISA entry
        regexp = re.compile('Architecture.*:(.*)')
        baseISA = regexp.search(lscpu).group(1).strip()
        sqlCursor.execute('INSERT INTO BaseISA (`Name`) VALUES (?);', (baseISA,))
        sqlCursor.execute('SELECT BaseISAID FROM BaseISA WHERE BaseISA.rowid=?;', (sqlCursor.lastrowid,))
        baseISAID = sqlCursor.fetchone()[0]

        #Find Hypervisor
        #Regex help from https://stackoverflow.com/questions/6633678/finding-words-after-keyword-in-python
        regexp = re.compile('Hypervisor.*:(.*)')
        hypervisor = regexp.search(lscpu)
        if hypervisor is not None:
            hypervisorStr = hypervisor.group(1).strip()

        #Find Core and Socket Info
        regexp = re.compile('Socket[(]s[)].*:(.*)')
        sockets = int(regexp.search(lscpu).group(1).strip())

        regexp = re.compile('Core[(]s[)] per.*:(.*)')
        cores = int(regexp.search(lscpu).group(1).strip())

        regexp = re.compile('NUMA node[(]s[)].*:(.*)')
        numa = int(regexp.search(lscpu).group(1).strip())

        #Find Processor Info
        cpuinfo = subprocess.check_output('cat /proc/cpuinfo', shell=True).decode('utf-8')
        
        #TODO: Only looks at 1st processor for now, extend to look at all processors
        #Find Vendor
        regexp = re.compile('vendor_id.*:(.*)')
        vendor = regexp.search(cpuinfo).group(1).strip()
        sqlCursor.execute('INSERT INTO Vendor (`Name`) VALUES (?);', (vendor,))
        sqlCursor.execute('SELECT VendorID FROM Vendor WHERE Vendor.rowid=?;', (sqlCursor.lastrowid,))
        vendorID = sqlCursor.fetchone()[0]

        #CPU Name
        regexp = re.compile('model name.*:(.*)')
        cpuName = regexp.search(cpuinfo).group(1).strip()

        #Create Processor Entry
        sqlCursor.execute('INSERT INTO Processor (`Name`, `Vendor`, `BaseISAID`, `Cores`) VALUES (?, ?, ?, ?);', (cpuName, vendorID, baseISAID, cores))
        sqlCursor.execute('SELECT ProcessorID FROM Processor WHERE Processor.rowid=?;', (sqlCursor.lastrowid,))
        processorID = sqlCursor.fetchone()[0]

        #Find ISA Extension Info
        regexp = re.compile('flags.*:(.*)')
        extensions = regexp.search(cpuinfo).group(1).strip().split(' ')

        for extension in extensions:
            sqlCursor.execute('INSERT INTO ISAExtension (`Name`, `BaseISAID`) VALUES (?, ?);', (extension, baseISAID))
            sqlCursor.execute('SELECT ISAExtensionID FROM ISAExtension WHERE ISAExtension.rowid=?;', (sqlCursor.lastrowid,))
            extensionID = sqlCursor.fetchone()[0]
            sqlCursor.execute('INSERT INTO ProcessorISAExtension (`ProcessorID`, `ISAExtensionID`) VALUES (?, ?);', (processorID, extensionID))

        #Find Cache Info
        cacheStrs = re.findall('.*cache.*:.*', lscpu)
        for cacheStr in cacheStrs:
            splitStr = cacheStr.split(':')
            cacheName = splitStr[0].strip()
            cacheSize = splitStr[1].strip()
            sqlCursor.execute('INSERT INTO Cache (`ProcessorID`, `Level`, `Size`) VALUES (?, ?, ?);', (processorID, cacheName, cacheSize))

        #Find Memory
        meminfo = subprocess.check_output('cat /proc/meminfo', shell=True).decode('utf-8')
        regexp = re.compile('MemTotal.*:(.*)')
        memory = regexp.search(meminfo).group(1).strip()

        #TODO: Find Memory Type, leave blank for now
        #Create Socket
        sqlCursor.execute('INSERT INTO Socket (`ProcessorID`, `Memory`) VALUES (?, ?);', (processorID, memory))
        sqlCursor.execute('SELECT SocketID FROM Socket WHERE Socket.rowid=?;', (sqlCursor.lastrowid,))
        socketID = sqlCursor.fetchone()[0]
        
        #Create Node
        hostname = platform.node()

        sqlCursor.execute('INSERT INTO Node (`MachineID`, `Hostname`, `OSID`, `Hypervisor`, `Numa`) VALUES (?, ?, ?, ?, ?);', (machineID, hostname, OSID, hypervisorStr, numa))
        sqlCursor.execute('SELECT NodeID FROM Node WHERE Node.rowid=?;', (sqlCursor.lastrowid,))
        nodeID = sqlCursor.fetchone()[0]

        #Link Node and Socket
        #TODO: for now, will assume sockets are identicle if there are multiple sockets.
        for i in range(0, sockets):
            sqlCursor.execute('INSERT INTO NodeSocket (`NodeID`, `SocketID`) VALUES (?, ?);', (nodeID, socketID))

    return machineID

def addIfNotAlready(sqlCursor, idSearchStr, addStr, addTuple):
    """
    Add a parameter to a table if it is not included already.  Returns the ID regardless
    """

    #TODO: Optomize to use last index

    sqlCursor.execute(idSearchStr, addTuple)
    id_row = sqlCursor.fetchone()

    if id_row is None:
        sqlCursor.execute(addStr, addTuple)
        sqlCursor.execute(idSearchStr, addTuple)
        id_val = sqlCursor.fetchone()[0]
    else:
        id_val = id_row[0]

    return id_val

def sqlTester():
    conn = sqlite3.connect('test.db')
    sqlCursor = conn.cursor()

    createSqlTables(sqlCursor)
    conn.commit()

    autoPopulateMachineInfo(sqlCursor, 'Chris Macbook Pro', None)
    conn.commit()

def tester():
    flagTester() #Testing Flags

    sqlTester() #TestingSQL

#Specifics for Run
class gcc(Compiler):
    """
    Represents the GCC compiler
    """

    def __init__(self):
        compilerOptions = OptionList()
        compilerOptions.addOption(AlwaysOption('-std=c++11'))
        #TODO: Omitted debugger optomization
        #compilerOptions.addOption(ExclusiveOptionList([AlwaysOption('-O0'), AlwaysOption('-O1'), AlwaysOption('-O2'), AlwaysOption('-O3'), AlwaysOption('-Os'), AlwaysOption('-Ofast')], True))
        #compilerOptions.addOption(ExclusiveOptionList([AlwaysOption('-O1'), AlwaysOption('-O2'), AlwaysOption('-O3'), AlwaysOption('-Os'), AlwaysOption('-Ofast')]))
        compilerOptions.addOption(ExclusiveOptionList([AlwaysOption('-O2'), AlwaysOption('-O3'), AlwaysOption('-Ofast')]))
        #compilerOptions.addOption(ExclusiveOptionList([AlwaysOption('-Ofast')]))
        #TODO: Only targeting Intel ISA Extensions for now.  This is actually a non-exhaustive list.  See the g++ man page
        #compilerOptions.addOption(EnumOption('march', ['core2', 'corei7', 'corei7-avx', 'core-avx-i', 'core-avx2', 'native'], '-{}={}'))
        #TODO: For Now, let's just try the default x86_64 and native
        #compilerOptions.addOption(EnumOption('march', ['native'], '-{}={}'))
        #TODO: For Now, let's just try native
        compilerOptions.addOption(EnumOption('march', ['native'], '-{}={}', True))

        super().__init__(name='gcc', command='g++', envSetup=None, vendor='GNU', options=compilerOptions, defineFormat='-D{}={}', compileFormat='-c {}', outputFormat='-o {}')   

def CompileRunCfgs(compilerList, suiteList, sqlConnection, sqlCursor, machineID):
    """
    Generator for compile and run configuration
    """

    for suite in suiteList:
        #Create entry for suite in DB
        suiteID = suite.addSuiteEntryIfNotAlready(sqlCursor)
        sqlConnection.commit()

        kernels = suite.kernels()

        for kernel in kernels:
            #Create entry for kernel in DB
            kernelID = kernel.addKernelEntryIfNotAlready(sqlCursor)

            #Link Suite and Kernel (this is all to all but should potentially be changed)
            #TODO: Evaluate all to all
            addIfNotAlready(sqlCursor, 'SELECT rowid FROM KernelSuite WHERE KernelSuite.SuiteID=? AND KernelSuite.KernelID=?;', 
                   'INSERT INTO KernelSuite (`SuiteID`, `KernelID`) VALUES (?, ?);', (suiteID, kernelID))

            kernelInstances = kernel.instances()

            for kernelInstance in kernelInstances:
                #Add Kernel Instance to Database
                (kernelInstanceID, kernelFileIDx) = kernelInstance.addKernelImplementationAndFileEntriesIfNotAlready(sqlCursor, kernelID)

                #Now, let's go through all of the provided compilers
                for compiler in compilerList:
                    #Add to database if not already in it (will not add duplicate)
                    compilerID = compiler.addCompilerEntryIfNotAlready(sqlCursor)

                    #Now, let's go through the different sets of kernel options
                    compilerOptions = compiler.options()
                    compilerFlagGenerator = compilerOptions.compilerFlagGenerator()

                    #At this point, we need a run entry for each set of flags used
                    #We create one run entry for each run and this is one of the few
                    #cases where duplicates are not checked.
                    #The creation of the run entry needs to be placed in the inner
                    #loop of all flag itterations
                    for compilerFlags in compilerFlagGenerator:
                        kernelInstanceCompilerOptions = kernelInstance.compileOptions()
                        kernelInstanceCompilerFlagGenerator = kernelInstanceCompilerOptions.compilerFlagGenerator()

                        for kernelInstanceCompilerFlags in kernelInstanceCompilerFlagGenerator:
                            yield (kernelInstance, kernelInstanceID, kernelFileIDx, compiler, compilerID, compilerFlags, kernelInstanceCompilerFlags)
                                
def compileInstance(compiler, kernelInstance, compilerFlags, kernelInstanceCompilerFlags, linkname, suffix):
    """
    Compile a kernel instance.  Returns the command used to compile and the exit code of the compile
    """

    #Construct the shell command to compile
    cmd = ''

    compilerFlagString = stringFromParameterList(compilerFlags)
    kernelInstanceCompilerFlagString = stringFromParameterList(kernelInstanceCompilerFlags)

    compilerSetup = compiler.envSetup()
    if compilerSetup is not None:
        cmd += compilerSetup + '; '

    cmd += 'cd build; '

    kernelInstFiles = kernelInstance.fileList()
    kernelCompiledFilenames = []

    for kernelFile in kernelInstFiles:
        
        #Get the filename without extension
        #TODO: Check for complex paths (ex. ../filename.cpp)
        outputFileName = kernelFile.split('.')[0] + '-' + suffix + '.o'
        kernelCompiledFilenames.append(outputFileName)

        cmd += compiler.command() + ' ' + compilerFlagString + ' ' + kernelInstanceCompilerFlagString + ' ' + str.format(compiler.compileFormat(), '') + ' ' + str.format(compiler.outputFormat(), outputFileName) + ' ../' + kernelFile +'; '
    
    #Add command to link
    linkedFilename = linkname + '-' + suffix
    cmd += compiler.command() + ' ' + compilerFlagString + ' ' + kernelInstanceCompilerFlagString + ' ' + str.format(compiler.outputFormat(), linkedFilename) + ' '
    for compiledFile in kernelCompiledFilenames:
        cmd += compiledFile + ' '
    
    #cmd += '; '

    #Execute Command
    print('Building: ' + cmd)
    exitCode = subprocess.call(cmd, shell=True)

    return (cmd, exitCode)

def compileInstanceThread(compiler, kernelInstance, compilerFlags, kernelInstanceCompilerFlags, linkname, suffix, resultArray, threadID):
    """
    Calls compileInstance and stores result in pre-allocated array.  Results are (cmd, exitCode)
    """
    resultArray[threadID] = compileInstance(compiler, kernelInstance, compilerFlags, kernelInstanceCompilerFlags, linkname, suffix)


def runInstanceAndParseOutput(sqlCursor, kernelInstanceID, machineID, compilerID, compiler, compilerFlags, kernelInstanceCompilerFlags, kernelRuntimeFlags, linkedFilename, compilerCmd, compilerExitCode):
    #Create Run Entry (will update some table values later)
    sqlCursor.execute('INSERT INTO Run (`KernelInstanceID`, `MachineID`, `CompilerID`) VALUES (?, ?, ?);', (kernelInstanceID, machineID, compilerID))
    sqlCursor.execute('SELECT RunID FROM Run WHERE Run.rowid=?;', (sqlCursor.lastrowid,))
    runID = sqlCursor.fetchone()[0]

    #Add flags and flag values to database
    addCompilerParametersToDBIfNotAlreadyLinkCompiler(sqlCursor, compilerFlags, compilerID, runID)
    addParametersToDBIfNotAlreadyLinkKernelInst(sqlCursor, kernelInstanceCompilerFlags, kernelInstanceID, runID, 'Compile')
    addParametersToDBIfNotAlreadyLinkKernelInst(sqlCursor, kernelRuntimeFlags, kernelInstanceID, runID, 'Run')

    kernelInstanceRuntimeFlagString = stringFromParameterList(kernelRuntimeFlags)

    if(compilerExitCode == 0):
        #Add command to run
        rptFilename = 'rpt'

        cmd = ''

        #TODO: May not be nessisary except for cases when shared libraries are needed
        compilerSetup = compiler.envSetup()
        if compilerSetup is not None:
            cmd += compilerSetup + '; '
        
        cmd = 'cd ./build; '

        #TODO: Refactor for report file not first argument
        cmd += './' + linkedFilename + ' ' + rptFilename + ' ' + kernelInstanceRuntimeFlagString + ';'

        print('Running: ' + cmd)

        #Start Timer
        startTime = datetime.datetime.now()

        exitCode = subprocess.call(cmd, shell=True)

        #Stop Timer
        stopTime = datetime.datetime.now()

        if exitCode != 0:
            status = 'Run Error'
        else:
            status = 'Success'
            
            #Parse Results
            with open('./build/'+rptFilename, newline='') as csvfile:
                csvReader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)

                headers = next(csvReader)
                trial = 1
                for row in csvReader:
                    trialID = addIfNotAlready(sqlCursor, 'SELECT TrialID FROM Trial WHERE Trial.RunID=? AND Trial.TrialNumber=?;',
                                'INSERT INTO Trial (`RunID`, `TrialNumber`) VALUES (?, ?);', (runID, trial))

                    #Now get row data and enter into database
                    #Use the header as the type
                    for col in range(0, len(row)):
                        resultTypeID = addIfNotAlready(sqlCursor, 'SELECT ResultTypeID FROM ResultType WHERE ResultType.Name=? AND ResultType.IsSummary=?;',
                                        'INSERT INTO ResultType (`Name`, `IsSummary`) VALUES (?, ?);', (headers[col], 0))

                        #TODO: Add unit
                        addIfNotAlready(sqlCursor, 'SELECT ResultID FROM Result WHERE Result.TrialID=? AND Result.ResultTypeID=? AND Result.Value=?;',
                        'INSERT INTO Result (`TrialID`, `ResultTypeID`, `Value`) VALUES (?, ?, ?);', (trialID, resultTypeID, row[col]))                                                               
                    trial += 1
    else:
        status = 'Build Error'

    #Done Parsing
    print(status)

    statusID = addIfNotAlready(sqlCursor, 'SELECT StatusID FROM Status WHERE Status.Name=?;',
                'INSERT INTO Status (`Name`) VALUES (?);', (status,))
    
    #Update Run Entry
    sqlCursor.execute('UPDATE Run SET DateTimeStart=?, DateTimeStop=?, StatusID=?, CommandExecuted=? WHERE Run.RunID=?;', (startTime, stopTime, statusID, compilerCmd + '; ' + cmd , runID))

    #Clean Report
    subprocess.call(str.format('rm -f ./build/{}', rptFilename), shell=True)


def runExperiment(compilerList, suiteList, sqlConnection, sqlCursor, machineID, threadCount, reportFrequency, reportPrefix):
    """
    Runs a set of experements defined by the provided kernel suites and compilers
    """

    #Clean build dir first
    subprocess.call('rm -f ./build/*', shell=True)

    cfgGen = CompileRunCfgs(compilerList, suiteList, sqlConnection, sqlCursor, machineID) #yields (kernelInstance, kernelInstanceID, kernelFileIDx, compiler, compilerID, compilerFlags, kernelInstanceCompilerFlags)

    #Run through all of the compiler configs, pulling N at a time
    doneProcessing = False

    runCount = 0

    while (doneProcessing == False):
        compilerCfgs = []
        threads = []
        buildResults = [None] * threadCount # preallocate array to avoid issues with thread concurrency
        executableNames = []

        #Pull N at a time, break out of the loop if we reach the end of the configs
        for i in range(0, threadCount):
            try:
                buildCfg = next(cfgGen)
            except StopIteration:
                doneProcessing = True
                break

            compilerCfgs.append(buildCfg)

            #Unpack cfg
            buildKernelInstance = buildCfg[0]
            #buildKernelInstanceID = buildCfg[1]
            #buildKernelFileIDx = buildCfg[2]
            buildCompiler = buildCfg[3]
            #buildCompilerID = buildCfg[4]
            buildCompilerFlags = buildCfg[5]
            buildKernelInstanceCompilerFlags = buildCfg[6]

            executableNames.append('executable-' + str(i))

            #Create build thread
            buildThread = threading.Thread(target = compileInstanceThread, args=(buildCompiler, buildKernelInstance, buildCompilerFlags, buildKernelInstanceCompilerFlags, 'executable', str(i), buildResults, i), name = 'BuildThread-' + str(i))
            threads.append(buildThread)
            buildThread.start()

        #Wait for threads to exit (the array is only as large as the number of threads spawned)
        for thread in threads:
            thread.join()

        #For each of the compiler configs generated, conduct the run(s) - serially to avoid interaction between instances
        for i in range(0, len(compilerCfgs)):
            runCfg = compilerCfgs[i] 
            runKernelInstance = runCfg[0]
            runKernelInstanceID = runCfg[1]
            #runKernelFileIDx = runCfg[2]
            runCompiler = runCfg[3]
            runCompilerID = runCfg[4]
            runCompilerFlags = runCfg[5]
            runKernelInstanceCompilerFlags = runCfg[6]

            buildResult = buildResults[i]
            buildCmd = buildResult[0]
            buildExitCode = buildResult[1]

            executableName = executableNames[i]
            
            kernelRuntimeOptions = runKernelInstance.runOptions()
            kernelRuntimeFlagGenerator = kernelRuntimeOptions.compilerFlagGenerator()

            for kernelRuntimeFlags in kernelRuntimeFlagGenerator:
                #Run the kernel
                runInstanceAndParseOutput(sqlCursor, runKernelInstanceID, machineID, runCompilerID, runCompiler, runCompilerFlags, runKernelInstanceCompilerFlags, kernelRuntimeFlags, executableName, buildCmd, buildExitCode)

                #Commit to DB
                sqlConnection.commit()

        #Clean build dir
        subprocess.call('rm -f ./build/*', shell=True)

        #Report
        runCount += len(compilerCfgs)
        if runCount % reportFrequency < threadCount and len(compilerCfgs)>0:
            msgTime = datetime.datetime.now()
            slackStatusPost(reportPrefix + 'Time: ' + str(msgTime) + '\n`' + buildCmd + '`')

def slackStatusPost(message):
    """
    Post a status update to slack
    """

    #Based on work on https://gist.github.com/devStepsize/b1b795309a217d24566dcc0ad136f784

    try:
        webhook_url = os.environ["SLACK_API_URL"]

        slack_data = {'text': message}

        response = requests.post(
            webhook_url, data=json.dumps(slack_data),
            headers={'Content-Type': 'application/json'}
        )

        if response.status_code != 200:
            print('Could not post slack message, code ' + str(response.status_code) + ' ... continuing')
    except KeyError:
        print('Could not read slack API url')

def main():

    #*****Setup*****
    buildThreads = 4
    reportFrequency = 5000
    #Create Suites
    firSuite = Suite('FIR', 'Testing feed forward system performance using FIR filters')

    #=======Create FIR Kernel========
    firSingleKernel = Kernel('FIR - Single', 'A single FIR filter operating in a single thread')
    firSuite.addKernel(firSingleKernel)

    #TODO remove format option from enum option
    #Create Naive FIR Kernel Instance

    firTrials = 5
    firStimLen = 50000

    #+++++++FIR Naive++++++++++++++++

    firNaiveCompileOptions = OptionList()
    naiveRangeIterator = itertools.chain(range(1, 2, 1), range(2, 31, 2))

    naiveRangeArray = []
    for naiveRangeVal in naiveRangeIterator:
        naiveRangeArray.append(naiveRangeVal)

    #So itterator is not lost
    #TODO: Investigate reseting iterator
    firNaiveOrderRange = naiveRangeArray
    firNaiveBlockRange = naiveRangeArray
    #Static Options
    firNaiveCompileOptions.addOption(EnumOption('PRINT_TITLE', [0], '-D{}={}', True))
    firNaiveCompileOptions.addOption(EnumOption('PRINT_TRIALS', [0], '-D{}={}', True))
    firNaiveCompileOptions.addOption(EnumOption('PRINT_STATS', [0], '-D{}={}', True))
    firNaiveCompileOptions.addOption(EnumOption('WRITE_CSV', [1], '-D{}={}', True))
    firNaiveCompileOptions.addOption(EnumOption('TRIALS', [firTrials], '-D{}={}', True))
    firNaiveCompileOptions.addOption(EnumOption('STIM_LEN', [firStimLen], '-D{}={}', True))
    #Dynamic Options
    firNaiveCompileOptions.addOption(EnumOption('DATATYPE', ['int16_t', 'int32_t', 'float', 'double'], '-D{}={}', True))
    #firNaiveCompileOptions.addOption(EnumOption('DATATYPE', ['double'], '-D{}={}', True))

    firNaiveCompileOptions.addOption(EnumOption('COEF_LEN', firNaiveOrderRange, '-D{}={}', True))
    firNaiveCompileOptions.addOption(EnumOption('IO_LEN', firNaiveBlockRange, '-D{}={}', True))

    firRunOptions = OptionList()

    firSingleKernelNaive = KernelInstance(firSingleKernel, 'Naive implementation with blocked input and output, shift register state, no explicit vectorization or unrolling.', ['fir1_1_tester1.cpp'], firNaiveCompileOptions, firRunOptions)
    firSingleKernel.addInstance(firSingleKernelNaive)

    #+++++++FIR Circular Buffer++++++++++++++++
    firCircularCompileOptions = firNaiveCompileOptions
    firSingleKernelCircular = KernelInstance(firSingleKernel, 'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ['fir1_2_tester1.cpp'], firCircularCompileOptions, firRunOptions)
    firSingleKernel.addInstance(firSingleKernelCircular)

    #+++++++FIR Circular Buffer (No Mod)++++++++++++++++
    firCircularNoModCompileOptions = firNaiveCompileOptions
    firSingleKernelCircularNoMod = KernelInstance(firSingleKernel, 'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ['fir1_2_2_tester1.cpp'], firCircularNoModCompileOptions, firRunOptions)
    firSingleKernel.addInstance(firSingleKernelCircularNoMod)

    #+++++++FIR Circular Buffer - Reversed ++++++++++++++++
    firCircularRevCompileOptions = firNaiveCompileOptions
    firSingleKernelCircularRev = KernelInstance(firSingleKernel, 'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ['fir1_2b_tester1.cpp'], firCircularRevCompileOptions, firRunOptions)
    firSingleKernel.addInstance(firSingleKernelCircularRev)

    #+++++++FIR Circular Buffer (No Mod) - Reversed ++++++++++++++++
    firCircularNoModRevCompileOptions = firNaiveCompileOptions
    firSingleKernelCircularNoModRev = KernelInstance(firSingleKernel, 'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ['fir1_2_2b_tester1.cpp'], firCircularNoModRevCompileOptions, firRunOptions)
    firSingleKernel.addInstance(firSingleKernelCircularNoModRev)

    #+++++++FIR Naive Unroll 2++++++++++++++++
    firUnroll2CompileOptions = OptionList()
    unroll2RangeIterator = range(2, 31, 2)

    unroll2RangeArray = []
    for unroll2RangeVal in unroll2RangeIterator:
        unroll2RangeArray.append(unroll2RangeVal)

    #So itterator is not lost
    #TODO: Investigate reseting iterator
    firUnroll2OrderRange = unroll2RangeArray
    firUnroll2BlockRange = unroll2RangeArray
    #Static Options
    firUnroll2CompileOptions.addOption(EnumOption('PRINT_TITLE', [0], '-D{}={}', True))
    firUnroll2CompileOptions.addOption(EnumOption('PRINT_TRIALS', [0], '-D{}={}', True))
    firUnroll2CompileOptions.addOption(EnumOption('PRINT_STATS', [0], '-D{}={}', True))
    firUnroll2CompileOptions.addOption(EnumOption('WRITE_CSV', [1], '-D{}={}', True))
    firUnroll2CompileOptions.addOption(EnumOption('TRIALS', [firTrials], '-D{}={}', True))
    firUnroll2CompileOptions.addOption(EnumOption('STIM_LEN', [firStimLen], '-D{}={}', True))
    #Dynamic Options
    firUnroll2CompileOptions.addOption(EnumOption('DATATYPE', ['int16_t', 'int32_t', 'float', 'double'], '-D{}={}', True))
    #firNaiveCompileOptions.addOption(EnumOption('DATATYPE', ['double'], '-D{}={}', True))

    firUnroll2CompileOptions.addOption(EnumOption('COEF_LEN', firUnroll2OrderRange, '-D{}={}', True))
    firUnroll2CompileOptions.addOption(EnumOption('IO_LEN', firUnroll2BlockRange, '-D{}={}', True))

    firSingleKernelUnroll2 = KernelInstance(firSingleKernel, 'Naive implementation with blocked input and output, shift register state, manually unrolled by 2.', ['fir1_3_tester1.cpp'], firUnroll2CompileOptions, firRunOptions)
    firSingleKernel.addInstance(firSingleKernelUnroll2)

    #+++++++FIR Naive Unroll 4++++++++++++++++
    firUnroll4CompileOptions = OptionList()
    unroll4RangeIterator = range(4, 31, 4)

    unroll4RangeArray = []
    for unroll4RangeVal in unroll4RangeIterator:
        unroll4RangeArray.append(unroll4RangeVal)

    #So itterator is not lost
    #TODO: Investigate reseting iterator
    firUnroll4OrderRange = unroll4RangeArray
    firUnroll4BlockRange = unroll4RangeArray
    #Static Options
    firUnroll4CompileOptions.addOption(EnumOption('PRINT_TITLE', [0], '-D{}={}', True))
    firUnroll4CompileOptions.addOption(EnumOption('PRINT_TRIALS', [0], '-D{}={}', True))
    firUnroll4CompileOptions.addOption(EnumOption('PRINT_STATS', [0], '-D{}={}', True))
    firUnroll4CompileOptions.addOption(EnumOption('WRITE_CSV', [1], '-D{}={}', True))
    firUnroll4CompileOptions.addOption(EnumOption('TRIALS', [firTrials], '-D{}={}', True))
    firUnroll4CompileOptions.addOption(EnumOption('STIM_LEN', [firStimLen], '-D{}={}', True))
    #Dynamic Options
    #firUnroll4CompileOptions.addOption(EnumOption('DATATYPE', ['int8_t', 'int16_t', 'int32_t', 'int64_t', 'float', 'double'], '-D{}={}', True))
    firUnroll4CompileOptions.addOption(EnumOption('DATATYPE', ['int16_t', 'int32_t', 'float', 'double'], '-D{}={}', True))
    #firNaiveCompileOptions.addOption(EnumOption('DATATYPE', ['double'], '-D{}={}', True))

    firUnroll4CompileOptions.addOption(EnumOption('COEF_LEN', firUnroll4OrderRange, '-D{}={}', True))
    firUnroll4CompileOptions.addOption(EnumOption('IO_LEN', firUnroll4BlockRange, '-D{}={}', True))

    firSingleKernelUnroll4 = KernelInstance(firSingleKernel, 'Naive implementation with blocked input and output, shift register state, manually unrolled by 4.', ['fir1_3_4_tester1.cpp'], firUnroll4CompileOptions, firRunOptions)
    firSingleKernel.addInstance(firSingleKernelUnroll4)

    #------------END FIR-----------------------

    #TODO: Better handle output file as an option, right now is assumed to be the first option (before other options specified)
    #TODO: Cleanup output file format str in KernelInstance

    #Select Tests to Run
    compilers = [gcc()]
    suites = [firSuite]

    #*****GO*****
    #TODO: Make these arguments
    sqlFilePath = './results.db'
    machineDescription = 'Chris Workstation VM'

    if os.path.isfile(sqlFilePath):
        print('Report file already exists - Aborting')
        return
    
    #Create tables
    conn = sqlite3.connect(sqlFilePath, detect_types=sqlite3.PARSE_DECLTYPES)
    sqlCursor = conn.cursor()

    createSqlTables(sqlCursor)
    conn.commit()

    machineID = autoPopulateMachineInfo(sqlCursor, machineDescription, None)
    conn.commit()

    #Run the Experement
    exeStartTime = datetime.datetime.now()
    hostname = platform.node()
    slackStatusPost('*Starting Benchmarking*\nHost: ' + hostname + ' (' + machineDescription + ')\nTime: ' + str(exeStartTime))

    runExperiment(compilers, suites, conn, sqlCursor, machineID, buildThreads, reportFrequency, '*Benchmarking Update*\nHost: ' + hostname + ' (' + machineDescription + ')\n')

    exeEndTime = datetime.datetime.now()
    slackStatusPost('*Finished Benchmarking*\nHost: ' + hostname + ' (' + machineDescription + ')\nTime: ' + str(exeEndTime))

    conn.commit()
    conn.close()

if __name__ == "__main__":
    main()