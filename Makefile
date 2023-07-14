GCC=gcc
FLAGS=-c
UNIVERSAL_LIBS=-I./universal
SERVER_LIBS=-I./server
SERVER_FUNCTIONALITY_LIBS=-I./server/handlers -I./server/handlers/Users -I./server/handlers/Chats -I./server/handlers/Data_structures 
ALL_LIBS=$(UNIVERSAL_LIBS) $(SERVER_FUNCTIONALITY_LIBS) $(SERVER_LIBS)

BARE_SERVER=logger.o XDR_representation.o threads_proxy.o socket_proxy.o packet_net_transmission.o packet_format.o internet_address.o event_handler_entry.o events.o server_control.o server_main.o 
SERVER_FUNCTIONALITY=server_state.o server_responses.o moderation_handler.o connection_handler.o chatting_handler.o id_collection.o private_chat.o pc_collection.o group_chat.o gc_collection.o chat_entry.o double_linked_list.o user.o user_collection.o

all: client server

server: $(BARE_SERVER) $(SERVER_FUNCTIONALITY)
	$(GCC) -pthread $^ -o server_app

client: logger.o XDR_representation.o socket_proxy.o packet_net_transmission.o packet_format.o internet_address.o client_main.o client_interface.o
	$(GCC) $^ -o client_app


private_chat.o: ./server/handlers/Chats/private_chat.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^

pc_collection.o: ./server/handlers/Chats/pc_collection.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^

group_chat.o: ./server/handlers/Chats/group_chat.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^

gc_collection.o: ./server/handlers/Chats/gc_collection.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^

chat_entry.o: ./server/handlers/Chats/chat_entry.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^

double_linked_list.o: ./server/handlers/Data_structures/double_linked_list.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^

user.o: ./server/handlers/Users/user.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^

user_collection.o: ./server/handlers/Users/user_collection.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^

id_collection.o: ./server/handlers/Users/id_collection.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^

chatting_handler.o: ./server/handlers/chatting_handler.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^

connection_handler.o: ./server/handlers/connection_handler.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^

moderation_handler.o: ./server/handlers/moderation_handler.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^

server_responses.o: ./server/handlers/server_responses.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^

server_state.o: ./server/handlers/server_state.c
	$(CC) $(FLAGS) $(ALL_LIBS) $^


client_interface.o: ./client/client_interface.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^

client_main.o: ./client/client_main.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^


server_main.o: ./server/server_main.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^

server_control.o: ./server/server_control.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^

events.o: ./server/events.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^

event_handler_entry.o: ./server/event_handler_entry.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $(SERVER_FUNCTIONALITY_LIBS) $(SERVER_LIBS) $^


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

