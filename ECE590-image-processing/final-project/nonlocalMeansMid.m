function J = nonlocalMeansMid(I,windowSize,sigma)
assert(all(mod(windowSize,2)==1),'window should have odd size')
[N,M] = size(I);
n = windowSize(1); m = windowSize(2);
zIz = padarray(I,(windowSize-1)/2,'replicate');
windows = cell(N*M,1);
for u = 1:N
	for v = 1:M
		windows{u+(v-1)*N} = zIz(u-1+(1:n),v-1+(1:m));
	end
end
dists2 = zeros(N*M,N*M);
for i = 1:N*M
	for j = i:N*M
		dists2(i,j) = sum((windows{i}(:)-windows{j}(:)).^2);
	end
end
dists2 = dists2+dists2';
weights = exp(-0.5*dists2/sigma);
weights = bsxfun(@rdivide,weights,sum(weights,1));
J = reshape(I(:)'*weights,[N,M]);