CC_FLAGS ?= -O2

build:
	gcc $(CC_FLAGS) -c ../hausy.c
	gcc $(CC_FLAGS) alphex.c hausy.o -o alphex
	gcc $(CC_FLAGS) create_timings.c hausy.o -o create_timings 

clean:
	rm -f hausy.o
	rm -f alphex
	rm -f create_timings

