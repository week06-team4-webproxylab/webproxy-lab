CC = gcc
CFLAGS = -g -Wall
LIB = -lpthread

echoclient: echoclient.c csapp.o
	$(CC) $(CFLAGS) -o echoclient echoclient.c csapp.o $(LIB)

echoserver: echoserver.c echo.c csapp.on
	$(CC) $(CFLAGS) -o echoserver echoserver.c echo.c csapp.o $(LIB)

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.cd

clean:
	rm -f *.o

