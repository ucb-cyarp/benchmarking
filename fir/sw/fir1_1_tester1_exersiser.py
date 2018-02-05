#!/usr/bin/env python3

#Autotuner/Excersiser for FIR1_1_Tester1

import collections
import string

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

    def __init__(self, name='g++', envSetup=None, options=None, defineFormat='-D{}={}', outputFormat='-o {}'):
        """Initialize a Compiler"""
        self._envSetup = envSetup
        self._name = name
        self._defineFormat = defineFormat
        self._outputFormat = outputFormat
        if options is not None:
            self._options = options
        else:
            self._options = OptionList()

    #Getters & Setters
    def envSetup(self, envSetup=None):
        if envSetup is not None:
            self._envSetup = envSetup
        return self._envSetup

    def name(self, name=None):
        if name is not None:
            self._name=name
        return self._name

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

class Suite:
    def __init__(self, name=None, kernels=None):
        """
        Name = Name of Kernel Suite
        Instances = List of Kernel Instances
        """
        self._name = name
        self._instances = instances

    def instances(self, instances=None):
        if instances is not None:
            self._instances = instances
        return self._instances

    def addInstance(self, instance):
        self._instances.append(instance)

class Kernel:
    def __init__(self, name=None, instances=None):
        """
        Name = Name of Kernel
        Instances = List of Kernel Instances
        """
        self._name = name
        self._instances = instances

    def instances(self, instances=None):
        if instances is not None:
            self._instances = instances
        return self._instances

    def addInstance(self, instance):
        self._instances.append(instance)

class KernelInstance:
    def __init__(self, fileList=None, compileOptions=None, runOptions=None, outputFileFormatStr='{}'):
        """
        FileList: List of files to compile
        CompileOptions: OptionList to be exersized at compile time
        RunOptions: OptionList to be exersized at run time
        """
        if fileList is not None:
            self._fileList = fileList
        else:
            self._fileList = []

        if compileOptions is not None:
            self._compileOptions = compileOptions
        else:
            self.compileOptions = OptionList()

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
    
    def addFile(self, file):
        self._fileList.append(file)

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

def main():
    flagTester()


if __name__ == "__main__":
    main()