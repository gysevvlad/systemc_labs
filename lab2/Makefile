
all : main
	

controller :
	[ -d objdir ] || mkdir objdir
	cd objdir && \
	echo $(PWD) && \
	clang++ -I ../../systemc/include -L ../../systemc/lib-freebsd64 \
		-I .. -Alsystemc ../testbenches/PmodOledControllerTestBench.cpp \
		-lsystemc --std=c++11 -o controller


spi :
	[ -d objdir ] || mkdir objdir
	cd objdir && \
	echo $(PWD) && \
	clang++ -I ../../systemc/include -L ../../systemc/lib-freebsd64 \
		-I .. -Alsystemc ../testbenches/SpiTestBench.cpp -lsystemc \
		--std=c++11 -o spi


main : modules/*.cpp modules/*.h testbenches/MainTestBench.cpp
	[ -d objdir ] || mkdir objdir
	cd objdir && \
	echo $(PWD) && \
	clang++ -w -I ../../systemc/include -L ../../systemc/lib-freebsd64 \
		-I .. -Alsystemc ../testbenches/MainTestBench.cpp -lsystemc \
		--std=c++11 -o main ../modules/*.cpp

systemc : ../systemc/libsystemc.a ../systemc/libsystemc.la
	
../systemc/libsystemc.a ../systemc/libsystemc.la :
	cd ../../systemc        && \
	[ -d obj ] || mkdir obj && \
	cd obj                  && \
	echo $(PWD)             && \
	../configure            && \
	gmake                   && \
	gmake install

