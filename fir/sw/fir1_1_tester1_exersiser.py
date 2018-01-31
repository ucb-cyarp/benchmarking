#!/usr/bin/env python3

#Autotuner/Excersiser for FIR1_1_Tester1

class OptionList:
    """
    Contains a list of options.  This set may consist of sublists 
    which are exclusive, depandant

    Note: This should be used as a set but ordering is potentially 
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

    def numberOfFlagCombinations(self):
        """
        Returns the number of compiler options that will be genertated 
        by the compiler flag generator.
        """

    def compilerFlagGenerator(self):
        """
        Generates compiler flags for different compiler runs based off 
        of the options included in the option set.  All legal combinations 
        from the perspective of the CompilerOptionSet will be generated.\n\n

        Flags will be included in the order that they are given in the option 
        list.
        """

class ExclusiveOptionList(OptionList):
    """
    Represents a List of options that are mutually exclusive.  
    -O0, -O1, -02, -O3 are mutually exclusive compiler flags for instance.
    """

    def numberOfFlagCombinations(self):
        """
        Returns the number of compiler options that will be genertated 
        by this exclusive option list.
        """

class DependantOptionList(OptionList):
    """
    Represents a list of options that are dependant on a parent option.  
    For example, certain options can only be set when another option is set.
    """

    def __init__(self, parent=None, children=None):
        """
        Initilize a dependent option list with a parent and depedant options.  
        Dependent options can also contain exclusive or dependant options
        """
        super().__init__(children)
        self._parent = parent


#Contain option and bool to state if active or not?
#Can also be accomplished with chain of for loops
#ie, for each of my options
    #for each of my neighbors options
        #for each of their neighbor's neighbor's options

class Option:
    """
    Base Class representing a single option.
    """

#This may actually be done with a generator of generators?\

class EnumOption(Option):
    """
    Base Class representing an option which can take on many values or which does 
    not exist at all.
    """
#include format string, option, list

class BinaryOption(Option):
    """
    Base Class representing a simple option which either exists or does not.
    """

class AlwaysOption(Option):
    """
    Base Class representing a option which is always present
    """

class Compiler:
    """
    Create a class to represent a given Compiler.  
    Includes information such as the name of the compiler, the 
    """

    def __init__(self, name='g++', env_setup=None, options=None, define_format='-D{}={}'):
        """Initialize a Compiler"""
        self._env_setup = env_setup
        self._name = name
        self._define_format = define_format
        if options is not None:
            self._options = options
        else:
            self._options = OptionList()

class Program:
    #Source file name(s)
    #Flags for each source file (OptionList)
    #Linker flags

    #Running arg format