function [groups, arr] = group_by(grouping_vec,data_vec)
%group_by Groups data into a 2d array.  There is assumed to be an equal
%number of elements in each group.  Otherwise, erronious zeros will be
%included

group_index = containers.Map;
group_count = containers.Map;

groups = [];
arr = [];

for i = 1:length(grouping_vec)
    %check if group is in maps
    if(isKey(grouping_vec(i), group_index))
        %insert into array
        index = group_index(grouping_vec(i));
        current_count = group_count(grouping_vec(i));
        arr(current_count+1, index) = data_vec(i);
        group_count(grouping_vec(i)) = current_count + 1;
    else
        %not in maps, add it
        index = length(groups)+1;
        groups(index) = grouping_vec(i);
        group_index(grouping_vec(i)) = index;
        group_count(grouping_vec(i)) = 1;
        
        %add to array
        arr(1, index) = data_vec(i);
        %already set count
    end
end

end

