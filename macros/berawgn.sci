function [matrix] = berawgn(EbNo,scheme)
funcprot(0);

if (string(scheme) == "bpsk") then
	[nr, nc]= size (EbNo)
	matrix = zeros(1, nc);
	
	for i = 1 : nc
		var = sqrt(EbNo(1,i));
		errorfunc = erfc(var);
		matrix(1,i) = 1/2*errorfunc; 
	end
end

endfunction
