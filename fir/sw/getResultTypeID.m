function resultTypeIDs = getResultTypeID(sqlConn, names)
%getResultTypeID Get a list of specific Result Type ID from the DB
%   A list of parameter names is given and the corresponding IDs are
%   returned
    
    resultTypeIDs = zeros(length(names),1);
    
    for i = 1:length(names)
        result = sqlConn.fetch(['SELECT ResultType.ResultTypeID FROM ResultType WHERE ResultType.Name=''' names{i} '''']);
        resultTypeIDs(i) = result{1};
    end
end

