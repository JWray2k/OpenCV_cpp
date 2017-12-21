/* Matlab source - http://pages.cs.wisc.edu/~flynn/cs638_project_files/m_files/energy.m

function f = energy(x)
    global I;
    Z = I;
    [rows, cols, lay] = size(Z);
    Z = im2double(Z);
    H = rgb2hsv(Z);
    r = zeros(rows, cols);
    xc = ceil(cols/2);
    yc = ceil(rows/2);
    for u=1:rows
        for v=1:cols
            r(u,v) = sqrt( abs(v-xc)^2 + abs(u-yc)^2);
        end
    end
    v=vignet(r,x);
    mask = v==0;
    v(mask) = 1;
    
    H(:,:,3) = H(:,:,3) ./ v(:,:);
    
    I=hsv2rgb(H);
    G=rgb2gray(I);
    G=im2uint8(G);    
    
    f = gamma_r(G, 1, 0);
end


*/