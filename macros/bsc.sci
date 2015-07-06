function [output] = bsc(input, p)
funcprot(0);

[nr , nc ]= size (input)

intermediate = zeros(1, nc);

if (p<0) then
	disp("p must be between 0 and 1");
	abort
end

if (p>1) then
	disp("p must be between 0 and 1");
	abort
end

for i = 1:nc

	select input(1,i)

	case 0
	case 1
	else
		disp("Binary data expected");
		abort
	end
end

for i = 1:nc
	intermediate(1,i) = input(1,i);
end

for i = 1:nc
	x = rand();
	if (x<=p) then
		if (input(1,i)==1) then
			intermediate(1,i) = 0;
		else
			intermediate(1,i) = 1;
		end
	end
end

output = intermediate;

endfunction
