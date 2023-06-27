# JMCProtocol
Just My Chat Protocol

### The rough plan for now (steps might be changed):
#### Preparation
- [x] Gather information on different algorithms and approaches for creating server-client architecture
  - [x] Event driven architecture
  - [x] Non-blocking sockets
  - [x] Synchronization primitives and their domains
  - [x] Request-Response model
- [x] Decide on a type of a message packet used
  - Check only one: 
    - [x] Packet with a predefined non-text header

#### Abstract mechanisms
- [ ] Plan messsages and their flow through the program <- A list of abstract steps
  - [ ] Text message
    - [x] Sending/receiving
    - [x] Deleting
    - [x] Editing
    - [ ] Private messages (I think of creating the same structure as in a group but without group moderation)
  - [x] Group control
    - [x] Granting group moderation capabilities
    - [x] Connecting/Disconnecting
    - [x] Group creation
  - [x] Group Moderation capablities
    - [x] Deleting user messages
    - [ ] Baning user from the group 
  - [x] Host Moderation capabilities
    - [x] Deleting users messages
    - [ ] Permabanning users
    - [ ] Permabanning groups
  - [x] Authorization
    - [x] Login message
  - [ ] File handling
    - [ ] Uploading a file to a server
    - [ ] Downloading a file from a server

#### Architecture (WIP)
Right now its just a mirror of the specifications I was given with a slight organisation
- [ ] Plan the architecture
  - [ ] Universal
    - [ ] Chat logging
    - [ ] File capabilities
  - [ ] Client side
  - [ ] Server side
    - [ ] Saving/Loading settings
    - [ ] Safe restart
    - [ ] Logging - Event sourcing?
    - [ ] Chats and sessions list indication
