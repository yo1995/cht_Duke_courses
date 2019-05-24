function J = nonlocalMeansWeightPeak(I,windowSize,sigma)
assert(all(mod(windowSize,2)==1),'window should have odd size')
[N,M] = size(I);
n = windowSize(1);
m = windowSize(2);
zIz = padarray(I,(windowSize-1)/2,'replicate');
J = zeros(N,M);
K = zeros(N,M);
weights = zeros(N,M);
for u1 = 1:N
	for v1 = 1:M
		for u2 = 1:N
			for v2 = 1:M
				win1 = zIz(u1-1+(1:n),v1-1+(1:m));
				win2 = zIz(u2-1+(1:n),v2-1+(1:m));
                % normalize them to have same min/max value
                minW1 = min(win1(:));
                maxW1 = max(win1(:));
                w1 = normalizeVectorToMinMax(win1(:), minW1, maxW1);
                w2 = normalizeVectorToMinMax(win2(:), minW1, maxW1);
				dists2 = sum((w1-w2).^2);
				weights(u2,v2) = exp(-0.5*dists2/sigma);
                K(u2,v2) = normalizeVectorToMinMax(I(u2,v2), minW1, maxW1);  % record the normalized value for each pixel
			end
        end
        
%         if (u1 >= N/2) && (u1 < N/2 + 1) && (v1 >= M/2) && (v1 < M/2 + 1)  % the center point
%             figure();
%             % only plot the ones greater or equal than 80%
%             weights(abs(weights)<quantile(abs(weights(:)),0.90)) = 0;
%             imagesc(weights);
%             axis equal tight;
%             colormap gray;
%             hold on
%             plot(u1,v1, '.r', 'MarkerSize',10);
%             % title(sprintf('%s\nRMSE = %g',titleString,rmse))
%         end
        
% 		J(u1,v1) = sum(sum(weights.*I))/sum(sum(weights));
        J(u1,v1) = sum(sum(weights.*K))/sum(sum(weights));
	end
end