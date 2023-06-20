# JMCProtocol
Just My Chat Protocol

### The rough plan for now (steps might be changed):
#### Preparation
- [ ] Gather information on different protocols that implement Internet Chats
  - [ ] IRC - rfc 1459
- [ ] Decide on a type of a message packet used
  - Check only one: 
    - [ ] Full text protocol
    - [ ] Packet with a predefined non-text header

#### Abstract mechanisms
- [ ] Plan messsages and their flow through the program <- A list of abstract steps
  - [ ] Text message
    - [ ] Sending/receiving
    - [ ] Deleting
    - [ ] Editing
  - [ ] Group control
    - [ ] Connecting/Disconnecting
    - [ ] Group creation
  - [ ] Group Moderation capablities
    - [ ] Creating group moderators <- only for group creators or ?equals
    - [ ] Deleting user messages
    - [ ] Baning user from the group 
  - [ ] Host Moderation capabilities
    - [ ] Deleting users messages
    - [ ] ?Granting serverwide moderation capabilities
    - [ ] Permabanning users
    - [ ] Permabanning groups
  - [ ] Authorization
  - [ ] File links
    - [ ] Uploading a file to a server
    - [ ] Downloading a file from a server
  - [ ] Keep alive

#### Architecture (WIP)
Right now its just a mirroring of the specifications I was given with a slight organisation
- [ ] Plan the architecture
  - [ ] Universal
    - [ ] Chat logging
    - [ ] File capabilities
  - [ ] Client side
  - [ ] Server side
    - [ ] Saving/Loading settings
    - [ ] Safe restart
    - [ ] Logging
    - [ ] Chats and sessions list indication
