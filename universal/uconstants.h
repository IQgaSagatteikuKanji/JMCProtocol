#pragma once



// Command codes
enum MESSAGE_TYPES{
    NULL_MSG = 0,
    //Client commands
    LOGIN, //must be sent first, if not user is kicked from server
    
    PRIVMSG,
    GROUPMSG,    
    DELETEMSG_FROM_GROUP,
    DELETEMSG_FROM_PRIVATE,
    EDITMSG_IN_GROUP,
    EDITMSG_IN_PRIVATE,
    JOIN,
    LEAVE,

    PERMABAN_USER,
    PERMABAN_GROUP,
    CREATE_GROUP,
    BAN_USER_FROM_GROUP,

    UPLOAD_FILE,
    DOWNLOAD_FILE,

    //Server-only respones
    ACK,
    NACK
};