CC_FLAGS ?= -O2

build:
	gcc $(CC_FLAGS) -c ../hausy.c
	gcc $(CC_FLAGS) alphex2int.c hausy.o -o alphex2int
	gcc $(CC_FLAGS) int2alphex.c hausy.o -o int2alphex
	gcc $(CC_FLAGS) create_timings.c hausy.o -o create_timings 

clean:
	rm -f hausy.o
	rm -f alphex2int
	rm -f int2alphex
	rm -f create_timings

