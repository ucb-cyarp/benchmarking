#!/usr/bin/env python3

#Autotuner/Excersiser for FIR1_1_Tester1

import collections
import string
import platform
import subprocess
import re
import sqlite3

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

def StringFromParameterList(parameters):
    return ' '.join(map(lambda param: param.paramString(), parameters))

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
                   'INSERT INTO Vendor (`Name`) VALUES (?);', (self.vendor,))

        compilerID = addIfNotAlready(sqlCursor, 'SELECT CompilerID FROM Compiler WHERE Compiler.Name=? AND Compiler.Command=? AND Compiler.VendorID=?;',
                     'INSERT INTO Compiler (`Name`, `Command`, `VendorID`) VALUES (?, ?, ?);', (self._name, self._command, vendorID))

        return compilerID

class Suite:
    def __init__(self, name=None, description=None,  kernels=None):
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
            self._kerneles = kernels
        return self._kerneles

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
    def __init__(self, name=None, description=None, instances=None):
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
                           'INSERT INTO KernelInstance (`KernelInstanceID`, `Filename`) VALUES (?, ?);', (kernelImplementationID, file))
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
        flagString = StringFromParameterList(flags)
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
        `ParameterName`	TEXT
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
    CREATE TABLE `Run` (
        `RunID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `KernelInstanceID`	INTEGER NOT NULL,
        `MachineID`	INTEGER NOT NULL,
        `DateTimeStart`	TEXT,
        `DataTimeStop`	TEXT,
        `Status`	TEXT,
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
        `Phase`	INTEGER NOT NULL,
        `ParamUsageID`	INTEGER NOT NULL
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
        `ParametersID`	INTEGER NOT NULL,
        PRIMARY KEY(`CompilerID`,`ParametersID`)
    );
    """]

    for cmd in tableCreateCmds:
        #print(cmd)
        sqlCursor.execute(cmd)

def autoPopulateMachineInfo(sqlCursor, machineFriendlyName=None, interconnect=None):
    """
    Autopopulate machine info assuming entry does not already exist in DB
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
    Add a parameter to a table if it is not included already.  Returns the ID
    """

    sqlCursor.execute(idSearchStr, addTuple)
    id_row = sqlCursor.fetchone()

    if id_row is None:
        sqlCursor.execute(addStr, addTuple)
        sqlCursor.execute(idSearchStr, addTuple)
        id_val = sqlCursor.fetchone()[0]
    else:
        id_val = id_row[0]

    return id_val

def addParameterValue(sqlCursor, compilerID, paramName, paramValue, paramFullString):
    """
    Add a parameter value, and the underlying parameter, to the database if it is not already
    included in the database.  Returns the (parameterID, parameterValueID).
    """

    #Add Parameter
    parameterID = addIfNotAlready(sqlCursor, 'SELECT ParameterID FROM Parameter WHERE Parmeter.Name=?', 
                  'INSERT INTO Parameter (`Name`) VALUES (?)', (paramName,))

    #Add Parameter Value
    #Note, there are 2 value columns: one for numerics, one for text
    if isinstance(paramValue, str):
        #String Param
        parameterValueID = addIfNotAlready(sqlCursor, 'SELECT ParameterValueID FROM ParameterValue WHERE ParmeterValue.ParameterID=? AND ParameterValue.ValueString=? AND ParameterValue.FullString=?;',
                      'INSERT INTO ParameterValue (`ParmeterValue`, `ValueString`, `FullString`) VALUES (?, ?, ?);', (parameterID, paramValue, paramFullString))
    else:
        #Numeric Param
        parameterValueID = addIfNotAlready(sqlCursor, 'SELECT ParameterValueID FROM ParameterValue WHERE ParmeterValue.ParameterID=? AND ParameterValue.ValueNumeric=? AND ParameterValue.FullString=?;',
                      'INSERT INTO ParameterValue (`ParmeterValue`, `ValueNumeric`, `FullString`) VALUES (?, ?, ?);', (parameterID, paramValue, paramFullString))

    return (parameterID, parameterValueID)

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
        compilerOptions.addOption(ExclusiveOptionList([AlwaysOption('-O0'), AlwaysOption('-O1'), AlwaysOption('-O2'), AlwaysOption('-O3'), AlwaysOption('-Os'), AlwaysOption('-Ofast')]))
        #TODO: Only targeting Intel ISA Extensions for now.  This is actually a non-exhaustive list.  See the g++ man page
        compilerOptions.addOption(EnumOption('march', ['i686', 'pentium4', 'core2', 'corei7', 'corei7-avx', 'core-avx-i', 'core-avx2', 'native'], '--{}={}'))

        super().__init__(name='gcc', command='g++', envSetup=None, vendor='GNU', options=compilerOptions, defineFormat='-D{}={}', compileFormat='-c {}', outputFormat='-o {}')
    

def main():
    #Select Compilers to Use
    compilers = [gcc()]




if __name__ == "__main__":
    main()