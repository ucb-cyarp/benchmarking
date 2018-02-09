function parameterIDs = getParameterIDs(sqlConn, parameterNames)
%getParameterIDs Get a list of specific Parameter IDs from the DB
%   A list of parameter names is given and the corresponding IDs are
%   returned
    
    parameterIDs = zeros(length(parameterNames),1);
    
    for i = 1:length(parameterNames)
        result = sqlConn.fetch(['SELECT Parameter.ParameterID FROM Parameter WHERE Parameter.Name=''' parameterNames{i} '''']);
        parameterIDs(i) = result{1};
    end
end

