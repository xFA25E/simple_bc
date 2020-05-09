run: simple_bc
	var="((3 + 4) * 2 - 1 + 6 / (2 * 1)) / 3 + ((92 - 31) / (6 * 3 + 12)) * (2 * 5) / 5 * 81 * (9 + 19 + 1)"; \
	echo "$${var}" | bc; \
	echo "$${var}" | ./simple_bc;

simple_bc: simple_bc.c
	musl-gcc -static simple_bc.c -O3 -Wall -Wextra -pedantic -o simple_bc
