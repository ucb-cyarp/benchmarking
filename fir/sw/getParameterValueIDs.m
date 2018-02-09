function parameterValueIDs = getParameterValueIDs(sqlConn, parameterValueSpecArray)
%getParameterValueIDs Get a list of specific ParameterValue IDs from the DB
%   A list of parameter valus is given and the index of each is returned.
%   The input is a cell array with the following columns:
%   Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value

    [rows, cols] = size(parameterValueSpecArray);
    
    parameterValueIDs = zeros(rows,1);
    
    for i = 1:rows
        %There are 3 possible types of parameter values, No Val (presence
        %only), Numeric, and String.  We handle each case seperatly.
        if parameterValueSpecArray{i, 2} == 0
            result = sqlConn.fetch(['SELECT ParameterValue.ParameterValueID FROM ParameterValue JOIN Parameter ON ParameterValue.ParameterID=Parameter.ParameterID WHERE Parameter.Name=''' parameterValueSpecArray{i, 1} ''' AND ParameterValue.ValueNumeric IS NULL AND ParameterValue.ValueString IS NULL']);
            parameterValueIDs(i) = result{1};
        elseif parameterValueSpecArray{i, 2} == 1
            result = sqlConn.fetch(['SELECT ParameterValue.ParameterValueID FROM ParameterValue JOIN Parameter ON ParameterValue.ParameterID=Parameter.ParameterID WHERE Parameter.Name=''' parameterValueSpecArray{i, 1} ''' AND ParameterValue.ValueNumeric=' num2str(parameterValueSpecArray{i, 3}) ' AND ParameterValue.ValueString IS NULL']);
            parameterValueIDs(i) = result{1};
        elseif parameterValueSpecArray{i, 2} == 2
            result = sqlConn.fetch(['SELECT ParameterValue.ParameterValueID FROM ParameterValue JOIN Parameter ON ParameterValue.ParameterID=Parameter.ParameterID WHERE Parameter.Name=''' parameterValueSpecArray{i, 1} ''' AND ParameterValue.ValueNumeric IS NULL AND ParameterValue.ValueString=''' parameterValueSpecArray{i, 3} '''']);
            parameterValueIDs(i) = result{1};
        else
            baseException = MException('GETPARAMETERVALUEIDS:TYPEERR','Type is not 0, 1, or 2');
            throw(baseException);
        end
    end
end

