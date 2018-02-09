Select Run.RunID, Parameter.ParameterID, ParameterValue.ParameterValueID, Parameter.Name, ParameterValue.FullString
From Run join Config on Run.RunID=Config.RunID
join ParameterValue on Config.ParameterValueID=ParameterValue.ParameterValueID
join Parameter on ParameterValue.ParameterID=Parameter.ParameterID
where Run.CompilerID=1 AND Run.KernelInstanceID=1 AND Run.MachineID=1
ORDER BY Run.RunID;