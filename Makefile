C=$(CROSS_COMPILE)gcc -Wall -g
CPP=$(CROSS_COMPILE)g++ -Wall -g

GFLAGS=`pkg-config --libs --cflags gtk+-2.0 gmodule-2.0`
OFLAGS=`pkg-config --libs --cflags opencv`

rebuild:remove libserial.o soccer.o gafuso.o robot  

build:libserial.o soccer.o gafuso.o  robot

robot:robot.cpp
	$(CPP) robot.cpp libserial.o soccer.o gafuso.o -o robot -lpthread $(OFLAGS)

gafuso.o: gafuso.c
	$(CPP) -c gafuso.c $(OFLAGS)

libserial.o: libserial.c
	$(CPP) -c libserial.c

soccer.o: soccer.c
	$(CPP) -c soccer.c
remove:
	rm libserial.o
	rm soccer.o	
	rm gafuso.o
	rm robot
