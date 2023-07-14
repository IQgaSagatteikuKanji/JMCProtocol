#pragma once



#define COMMAND_MAX_LENGTH 64u
enum COMMAND_CODES{
	CC_NOOP = 0,
	CC_ERROR_CODE,
	CC_JOIN_GROUP_CHAT,
	CC_EXIT,
	CC_LOGIN,
	CC_GROUPMSG,
	CC_PRIVMSG,
	CC_DELETEMSG,
	CC_EDITMSG,
	CC_LEAVE,
	CC_PERMABAN_USER,
	CC_PERMABAN_GROUP,
	CC_CREATE_GROUP,
	CC_BAN_USER_FROM_GROUP,
	CC_UPLOAD_FILE,
	CC_DOWNLOAD_FILE,
	CC_HELP
};

enum COMMAND_CODES get_command_code_from_str(char *str);

void parse_msg(const char *str, int *recipient, char **payload);