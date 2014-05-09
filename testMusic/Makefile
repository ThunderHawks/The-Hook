CPP = g++
OPTS =  -I"../../../include" -L"/usr/lib" ../../../bin/linux-gcc-64/libIrrKlang.so -pthread

all:
	$(CPP) test.cpp sound.cpp -o test $(OPTS)

clean:
	rm test
