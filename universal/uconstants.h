#pragma once



// Command codes
enum MESSAGE_TYPES{
    NULL_MSG = 0,
    GREETING,//delete it later
    PRIVMSG,
    GROUPMSG,
    LOGIN, //must be sent first, if not user is kicked from server
    DELETEMSG,
    EDITMSG,
    JOIN,
    LEAVE,
    PERMABAN_USER,
    PERMABAN_GROUP,
    CREATE_GROUP,
    BAN_USER_FROM_GROUP,
    UPLOAD_FILE,
    DOWNLOAD_FILE,
    ACK,
    NACK
};