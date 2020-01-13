function vec = importTrackerVector(raw)
%TRIMTRACKERVECTOR Take in a raw tracker vector (as a csv string)
%   Converts to a numeric vector, then trims out any -1s (header and
%   trailer)

vec_untrimmed = str2num(raw);

idx = find(vec_untrimmed ~= -1);

%Check for any gaps
for i = 2:length(idx)
    if idx(i) ~= idx(i-1)+1
        error('Unexpected gap in tracker vector');
    end
end

vec = vec_untrimmed(idx);

end

