# JMCProtocol
Just My Chat Protocol

### Usage
#### Client
client_app [-i IPv4] [-p port] [-u user_id]
  -i Defines ip address of the server
  -p Defines port of the server
  -u Defines user_id that will be used to identify your account


After the program is run, it overtakes the terminal and provides command line where you can type commands.
I haven't given it the ability to dynamically configure the amount of messages to be shown yet, so Im asking you to open it in a way that there can fit 5 messages on the screen. 

Before doing anything with the server it is required that you perform the login by typing `login` in the console. The status of your login will be printed in the chatting part of the screen.

##### Commands
- `privmsg user_id content`
Sends message if the user is online and exists.

- `create`
Creates a new group. While server does respomd with the id of a new group, my client doesn't have a way to store previously sent messages yet, so its unable to get it. Please look into the server console to get the id of the newly created group.

- `join group_id`
Joins existing group

- `msg group_id content`
Sends message if the group exists

- `leave group_id`
Leaves group

- `delete group_id message_id`
Deletes the message from the group

Other command handlers are fully implemented and working, but are not yet available to clients, because clients dont have a way to correctly show server responses or don't have command parsers ready for them yet.


#### Server
server_app [-i IPv4] [-p port]
  -i Defines ip address of the server
  -p Defines port of the server


### The description
All the server logic is done, the only thing i need is a packet queue for client to remember the packages it has sent, sadly I don't think i have enough time to implement it. So you'll have to look into a server console to get the group id for now, even though server does send it in target field.


### TODO
- Logging
I didn't have enough time to implement logging statements, but they are simple in the sense they are added to the handlers on both sides. I'll try to do them on monday.

- Files
They are also straightforward. One packet will be sent with a description of a file, then if the other side accepts it, it will be sent over the same socket, but will have 2 opcodes: UPLOAD_FILES_EXPECT_MORE and UPLOAD_FILES_FINISHED for server. The receiver will just append it to the end of the file.

- Recovery
It's tightly related to the Logging. We can log different changes to the state and recover by reading the file, it requires a lot of code.
