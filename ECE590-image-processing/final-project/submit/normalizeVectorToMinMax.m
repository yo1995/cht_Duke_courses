function X = normalizeVectorToMinMax(x, minVal, maxVal)
minX = min(x);
maxX = max(x);
denom = maxX-minX;
[N,M] = size(x);
X = zeros(N,M);
if denom == 0
    for i = 1:length(x)
        X(i) = (maxVal - minVal) / 2 + minVal;
    end
else

    for i = 1:length(x)
        X(i) = (x(i) - minX) * (maxVal - minVal) / denom + minVal;
    end
end
end