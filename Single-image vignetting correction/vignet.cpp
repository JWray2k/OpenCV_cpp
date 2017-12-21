/* Matlab source - http://pages.cs.wisc.edu/~flynn/cs638_project_files/m_files/vignet.m

function ret_v = vignet(r, x)
%x(1) = focal length
%x(2) - x(6) parameters for geometric polynomial
ar = 1./((1+(r./x(1)).^2).^2);
gr = 1-x(2)*r-x(3)*(r.^2)-x(4)*(r.^3)-x(5)*(r.^4)-x(6)*(r.^5);
ret_v = ar.*gr;
end

*/