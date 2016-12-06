
all : main

main : modules/*.cpp modukes/*.h testbenches/MainTestBench.cpp
	[ -d objdir ] || mkdir objdir
	cd objdir && \
	echo $(PWD) && \
	clang++ -I ../../systemc/include -L ../../systemc/lib-freebsd64 \
		-I .. -Alsystemc ../testbenches/MainTestBench.cpp -lsystemc \
		--std=c++11 -o main

systemc : ../systemc/libsystemc.a ../systemc/libsystemc.la
	
../systemc/libsystemc.a ../systemc/libsystemc.la :
	cd ../../systemc        && \
	[ -d obj ] || mkdir obj && \
	cd obj                  && \
	echo $(PWD)             && \
	../configure            && \
	gmake                   && \
	gmake install

