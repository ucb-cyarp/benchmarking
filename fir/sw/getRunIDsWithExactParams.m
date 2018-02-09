function runIDs = getRunIDsWithExactParams(sqlConn, mustHaveParamValues, atLeastOneOfParams, orderByID, kernelInstanceID, compilerID, machineID)
%getRunsWithExactParams Gets the RunIDs of Runs which have a set of "must
%have parameter values".  The Runs must also have at least one parameter
%value for each of the parameters given in "atLeastOneOfParams".  The run
%can have no other associated parameter values.  The runs must also 
%correspond to a given kernelInstanceID, a given compilerID, and a given 
%machineID.  Orders by the value of the ParameterValue  

query = {};
ind = 0;
for i = 1:length(mustHaveParamValues)
    %Construct Queries for Runs with the must have parameter values
    query{ind+i} = ['SELECT Run.RunID AS RID FROM Run JOIN Config ON Run.RunID=Config.RunID WHERE Config.ParameterValueID=' num2str(mustHaveParamValues(i)) ' AND Run.CompilerID=' num2str(compilerID) ' AND Run.KernelInstanceID=' num2str(kernelInstanceID) ' AND Run.MachineID=' num2str(machineID)];
end

ind = ind + length(mustHaveParamValues);

for i = 1:length(atLeastOneOfParams)
    %Construct Queries for Runs with the must have parameter values
    query{ind+i} = ['SELECT Run.RunID AS RID FROM Run JOIN Config ON Run.RunID=Config.RunID JOIN ParameterValue on Config.ParameterValueID=ParameterValue.ParameterValueID WHERE ParameterValue.ParameterID=' num2str(atLeastOneOfParams(i)) ' AND Run.CompilerID=' num2str(compilerID) ' AND Run.KernelInstanceID=' num2str(kernelInstanceID) ' AND Run.MachineID=' num2str(machineID)];
end

ind = ind + length(atLeastOneOfParams);

%Check Parameter Count
query{ind+1} = ['SELECT Run.RunID AS RID FROM Run JOIN Config ON Run.RunID=Config.RunID WHERE Run.CompilerID=' num2str(compilerID) ' AND Run.KernelInstanceID=' num2str(kernelInstanceID) ' AND Run.MachineID=' num2str(machineID) ' GROUP BY Run.RunID HAVING COUNT(Config.ParameterValueID)=' num2str(ind)];
ind = ind+1;

queryStr = 'SELECT RID FROM (';
%Form the Union Query
for i = 1:(ind-1)
    queryStr = [queryStr query{i} ' INTERSECT '];
end

%Tag the last query on without INTERSECT at the end
queryStr = [queryStr query{ind}];

queryStr = [queryStr, ') JOIN Config ON RID=Config.RunID JOIN ParameterValue ON Config.ParameterValueID=ParameterValue.ParameterValueID WHERE ParameterValue.ParameterID=' num2str(orderByID) ' ORDER BY ParameterValue.ValueNumeric ASC'];
runIDs = sqlConn.fetch(queryStr);

end

