#!/usr/bin/env python3

#Autotuner/Excersiser for FIR1_1_Tester1

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
            self._options = options
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
                yield ""
            else:
                l1FlagGenerator = options[0].compilerFlagGenerator()
                for l1Flag in l1FlagGenerator:
                    l2Flags = compilerFlagGeneratorHelper(options[1:]) #The recursive call, slicing the list
                    for l2Flag in l2Flags:
                        yield l1Flag + ' ' + l2Flag

        return compilerFlagGeneratorHelper(self._options)


class ExclusiveOptionList(OptionList):
    """
    Represents a List of options that are mutually exclusive.  
    -O0, -O1, -02, -O3 are mutually exclusive compiler flags for instance.
    """

    #Don't need to override constructor

    def numberOfFlagCombinations(self):
        """
        Returns the number of compiler options that will be genertated 
        by this exclusive option list.
        """

        #Instead of multiplying the options, we sum them since each option
        #is mutually exclusive
        combinations=0
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
        for option in self._options:
            for case in option:
                yield case

class DependantOptionList(OptionList):
    """
    Represents a list of options that are dependant on a single parent option.  
    This option is assumed to be a simple string and will be treated as a 
    binary option.

    For example, certain options can only be set when another option is set.
    """

    def __init__(self, parent=None, children=None):
        """
        Initilize a dependent option list with a parent and depedant options.  
        Dependent options can also contain exclusive or dependant options
        """
        super().__init__(children)
        self._parent = parent

    def numberOfFlagCombinations(self):
        """
        Returns the number of compiler options that will be genertated 
        by this dependant option list.
        """

        #This is the number of options in the children + 1 for the case
        #when the parent is not included

        combinations = super().numberOfFlagCombinations()
        combinations += 1

        return combinations


    def compilerFlagGenerator(self):
        """
        Generates compiler flags for different compiler runs based off 
        of the options included in the option set.  All legal combinations 
        from the perspective of the CompilerOptionSet will be generated.\n\n

        Flags will be included in the order that they are given in the option 
        list.
        """

        yield ""
        childCombonations = super().compilerFlagGenerator()

        for childCombonation in childCombonations:
            yield self._parent + " " + childCombonation 

class EnumOption(OptionList):
    """
    Base case representing an option which can take on many values or which does 
    not exist at all.
    """

    def __init__(self, name=None, syntax='-{}={}', values=None, alwaysIncluded=False):
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
            yield ""
        for value in self._values:
            yield self._syntax.format(self._options, value)

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

        yield ""
        yield self._options

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

        yield self._options

    def addOption(self, option):
        raise Exception('Cannot Add to Always Option')

class Compiler:
    """
    Create a class to represent a given Compiler.  
    Includes information such as the name of the compiler, the 
    """

    def __init__(self, name='g++', envSetup=None, options=None, defineFormat='-D{}={}'):
        """Initialize a Compiler"""
        self._envSetup = envSetup
        self._name = name
        self._defineFormat = defineFormat
        if options is not None:
            self._options = options
        else:
            self._options = OptionList()

    #Getters & Setters
    def envSetup(self, envSetup=None):
        if envSetup is not None:
            self._envSetup = envSetup
        return envSetup

    def name(self, name=None):
        if name is not None:
            self._name=name
        return self._name

    def defineFormat(self, defineFormat=None):
        if defineFormat is not None:
            self._defineFormat = defineFormat
        return defineFormat

    def options(self, options=None):
        if options is not None:
            self._options=options
        return self._options


# class Program:
#     #Source file name(s)
#     #Flags for each source file (OptionList)
#     #Linker flags

#     #Running arg format


def main():
    options = OptionList()
    options.addOption(AlwaysOption('-Wall'))

    flagGenerator = options.compilerFlagGenerator()

    for flags in flagGenerator:
        print(flags)


if __name__ == "__main__":
    main()