/* Matlab source - http://pages.cs.wisc.edu/~flynn/cs638_project_files/m_files/gamma_r.m
function skew = gamma_r( G, lambda, disp )
[rows, cols] = size(G);
xc = ceil(cols/2);
yc = ceil(rows/2);

r=zeros(rows,cols,2);

for x=1:cols
r(:,x,1)= xc - x;
end
for y=1:rows
r(y,:,2)= yc - y;
end
len = sqrt(r(:,:,1).^2 + r(:,:,2).^2);
ind = len==0;

[fx,fy]=gradient(double(G));
f(:,:,1)=fx;
f(:,:,2)=fy;

fprojr = dot(f, r, 3)./len;
fprojr(ind) = 0;

RG = reshape(fprojr, 1, rows*cols);
[count,loc] = hist(RG, -100:.5:100);

total = sum(sum(count));
prob=count./total;
[m, i]=min(abs(loc));

if(disp==1)
hold on;
plot(log(1+abs(loc(1:i))), prob(1:i),'r'); % Negative values in red
plot(log(1+loc(i:end)), prob(i:end), 'b'); % Positive in blue
hold off;
end

start_pos = min(find(loc>0)); %#ok<MXFND>

A1 = sum(count(start_pos-1:end));
A2 = sum(count(1:start_pos-1));

h_plus = count(start_pos-1:end)./A1;
h_minus = count(start_pos-1:-1:1)./A2;

lg =log(h_plus./h_minus);
mask = isfinite(lg);
mask_ind = mask==0;
lg(mask_ind) = 0;

KL_div = sum(h_plus .* lg);
nrm = abs(A1-A2)^(1/4);
skew = lambda * KL_div + (1-lambda)*nrm;
end 
*/