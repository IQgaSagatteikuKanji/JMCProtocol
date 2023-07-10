GCC=gcc
FLAGS=-c
UNIVERSAL_LIBS=-I./universal

all: 
	echo "It will not yet compile because its not connected"

server: logger.o XDR_representation.o threads_proxy.o socket_proxy.o packet_net_transmission.o packet_format.o internet_address.o event_handler_entry.o events.o server_control.o server_main.o 
	$(GCC) -pthread $^ -o server_app

client: logger.o XDR_representation.o socket_proxy.o packet_net_transmission.o packet_format.o internet_address.o client_main.o
	$(GCC) $^ -o client_app


client_main.o: ./client/client_main.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^

server_main.o: ./server/server_main.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^

server_control.o: ./server/server_control.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^

events.o: ./server/events.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^

event_handler_entry.o: ./server/event_handler_entry.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^


logger.o: ./universal/logger.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^

internet_address.o: ./universal/internet_address.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^

packet_format.o: ./universal/packet_format.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^

packet_net_transmission.o: ./universal/packet_net_transmission.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^


threads_proxy.o: ./universal/threads_proxy.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) -pthread $^

XDR_representation.o: ./universal/XDR_representation.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^

socket_proxy.o: ./universal/socket_proxy.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^

clean: 
	rm *.o

