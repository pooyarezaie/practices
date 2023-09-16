all: counter create_location
counter: tcounter.o pcounter.o main.cpp
	g++ -o counter tcounter.o pcounter.o main.cpp

tcounter.o: tcounter.cpp tcounter.hpp
	g++ -c tcounter.cpp

pcounter.o: pcounter.cpp pcounter.hpp
	g++ -c pcounter.cpp

create_location:
	mkdir -p /tmp/pipe;
