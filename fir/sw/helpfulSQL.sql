select Processor.Name as 'Processor Name', BaseISA.Name as 'Base ISA Name', ISAExtension.Name as 'ISA Extension' from 
Processor join BaseISA on Processor.BaseISAID=BaseISA.BaseISAID 
          join ProcessorISAExtension on Processor.ProcessorID=ProcessorISAExtension.ProcessorID 
          join ISAExtension on ProcessorISAExtension.ISAExtensionID=ISAExtension.ISAExtensionID;