CC = gcc
CFLAGS = -Wall -Wextra -pthread
OBJS = echo_server.o connection_handler.o request.o response.o router.o header.o

TARGET = http_server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

echo_server.o: echo_server.c connection_handler.h
	$(CC) $(CFLAGS) -c echo_server.c

connection_handler.o: connection_handler.c connection_handler.h request.h response.h router.h
	$(CC) $(CFLAGS) -c connection_handler.c

request.o: request.c request.h
	$(CC) $(CFLAGS) -c request.c

response.o: response.c response.h
	$(CC) $(CFLAGS) -c response.c

router.o: router.c router.h request.h response.h
	$(CC) $(CFLAGS) -c router.c

header.o: header.c header.h
	$(CC) $(CFLAGS) -c header.c

clean:
	rm -f $(TARGET) $(OBJS)
