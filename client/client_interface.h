#pragma once


#define COMMAND_MAX_LENGTH 64u
const char *prompt = "%s: ";
enum COMMAND_CODES{
	NOOP = 0,
	ERROR_CODE,
	JOIN_GROUP_CHAT,
	EXIT,
	LOGIN,
	GROUPMSG,
	PRIVMSG,
	DELETEMSG,
	EDITMSG,
	LEAVE,
	PERMABAN_USER,
	PERMABAN_GROUP,
	CREATE_GROUP,
	BAN_USER_FROM_GROUP,
	UPLOAD_FILE,
	DOWNLOAD_FILE,
	HELP
};

enum COMMAND_CODES get_command_code_from_str(char *str);

void parse_msg(const char *str, int *recipient, char **payload);