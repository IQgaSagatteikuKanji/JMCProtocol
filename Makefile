GCC=gcc
FLAGS=-c -g

ARCHIVATION_LIBS=-I./lib_LZMA_SDK
UNIVERSAL_LIBS=-I./universal
SERVER_LIBS=-I./server
SERVER_FUNCTIONALITY_LIBS=-I./server/handlers -I./server/handlers/Users -I./server/handlers/Chats -I./server/handlers/Data_structures 
ALL_LIBS=$(UNIVERSAL_LIBS) $(SERVER_FUNCTIONALITY_LIBS) $(SERVER_LIBS)

UNIVERSAL_OBJECTS=logger.o XDR_representation.o socket_proxy.o packet_net_transmission.o packet_format.o internet_address.o  threads_proxy.o
CLIENT_NCURSES_OBJECTS=client_globals.o client_chat_entry.o client_chat_messages.o client_command_handler.o client_console.o client_context.o client_interface.o client_main.o client_messages_renderer.o
ARCHIVER_OBJECTS=LzmaDec.o LzmaEnc.o LzFind.o CpuArch.o archivation_proxy.o

BARE_SERVER=$(ARCHIVER_OBJECTS) $(UNIVERSAL_OBJECTS) event_handler_entry.o events.o server_control.o server_main.o 
SERVER_FUNCTIONALITY=server_state.o server_responses.o moderation_handler.o connection_handler.o chatting_handler.o id_collection.o private_chat.o pc_collection.o group_chat.o gc_collection.o chat_entry.o double_linked_list.o user.o user_collection.o


all: client server

server: $(BARE_SERVER) $(SERVER_FUNCTIONALITY) $(ARCHIVER_OBJECTS)
	$(GCC) -pthread $^ -o server_app

client: $(ARCHIVER_OBJECTS) $(UNIVERSAL_OBJECTS) $(CLIENT_NCURSES_OBJECTS)
	$(GCC) -pthread $^ -o client_app -lncurses 


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

archivation_proxy.o: ./universal/archivation_proxy.c
	$(CC) $(FLAGS) -D_7ZIP_ST $(ARCHIVATION_LIBS) $(UNIVERSAL_LIBS) $^

LzmaDec.o: ./lib_LZMA_SDK/LzmaDec.c
	$(CC) $(FLAGS) -D_7ZIP_ST $(UNIVERSAL_LIBS) $^

LzmaEnc.o: ./lib_LZMA_SDK/LzmaEnc.c
	$(CC) $(FLAGS) -D_7ZIP_ST $(UNIVERSAL_LIBS) $^

LzFind.o: ./lib_LZMA_SDK/LzFind.c
	$(CC) $(FLAGS) -D_7ZIP_ST $(UNIVERSAL_LIBS) $^

CpuArch.o: ./lib_LZMA_SDK/CpuArch.c
	$(CC) $(FLAGS) -D_7ZIP_ST $(UNIVERSAL_LIBS) $^


client_chat_entry.o: ./client/client_chat_entry.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^ 

client_chat_messages.o: ./client/client_chat_messages.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^ 

client_command_handler.o: ./client/client_command_handler.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^ 

client_console.o: ./client/client_console.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^ 

client_context.o: ./client/client_context.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^ 

client_interface.o: ./client/client_interface.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^ 

client_main.o: ./client/client_main.c
	$(CC) $(FLAGS) -pthread $(UNIVERSAL_LIBS) $^  

client_messages_renderer.o: ./client/client_messages_renderer.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^ 

client_globals.o: ./client/client_globals.c
	$(CC) $(FLAGS) $(UNIVERSAL_LIBS) $^ 

clean: 
	rm *.o

