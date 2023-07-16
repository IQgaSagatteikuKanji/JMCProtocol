#include "client_interface.h"

#include <stdlib.h>
#include <string.h>

const char help[] = "help";
const char download_file[] = "download";
const char upload_file[] = "upload";
const char ban_user_from_group[] = "kick";
const char create_group[] = "create";
const char permaban_group[] = "bangroup";
const char permaban_user[] = "banuser";
const char leave_group[] = "leave";
const char edit_message[] = "edit";
const char delete_message[] = "delete"; 
const char private_message[] = "privmsg";
const char group_message[] = "msg";
const char login_prompt[] = "login";
const char join_group_chat[] = "join";
const char exit_app[] = "exit";


enum COMMAND_CODES get_command_code_from_str(char *str){
	if(!strncmp(join_group_chat, str, sizeof(join_group_chat) - 1)) return CC_JOIN_GROUP_CHAT;
	if(!strncmp(exit_app, str, sizeof(exit_app) - 1)) return CC_EXIT;
	if(!strncmp(login_prompt, str, sizeof(login_prompt) - 1)) return CC_LOGIN;
	if(!strncmp(group_message, str, sizeof(group_message) - 1)) return CC_GROUPMSG;

	if(!strncmp(private_message, str, sizeof(private_message) - 1)) return CC_PRIVMSG;
	if(!strncmp(delete_message, str, sizeof(delete_message) - 1)) return CC_DELETEMSG;
	if(!strncmp(edit_message, str, sizeof(edit_message) - 1)) return CC_EDITMSG;
	if(!strncmp(leave_group, str, sizeof(leave_group) - 1)) return CC_LEAVE;

	if(!strncmp(permaban_user, str, sizeof(permaban_user) - 1)) return CC_PERMABAN_USER;
	if(!strncmp(permaban_group, str, sizeof(permaban_group) - 1)) return CC_PERMABAN_GROUP;
	if(!strncmp(create_group, str, sizeof(create_group) - 1)) return CC_CREATE_GROUP;
	if(!strncmp(ban_user_from_group, str, sizeof(ban_user_from_group) - 1)) return CC_BAN_USER_FROM_GROUP;

	if(!strncmp(upload_file, str, sizeof(upload_file) - 1)) return CC_UPLOAD_FILE;
	if(!strncmp(download_file, str, sizeof(download_file) - 1)) return CC_DOWNLOAD_FILE;
	if(!strncmp(help, str, sizeof(help) - 1)) return CC_HELP;

	return CC_ERROR_CODE; 
}


void parse_msg(const char *str, int *recipient, char **payload){
    int strlength = strlen(str);
    int recipos = strchr(str, ' ') - str;
    int paypos = strchr(str + recipos + 1, ' ') - str;

    if(recipient != NULL){
		int recilength;
		if(paypos > 0){
			recilength = paypos - recipos;
		}
        recilength = strlength - recipos;
        char *reci = calloc(sizeof(char), recilength + 1);
        strncpy(reci, str + recipos, recilength);
        *recipient = atoi(reci);
        free(reci); 
    }

    if(payload != NULL && *payload == NULL){
        int paylength = strlength - paypos;
        *payload = calloc(sizeof(char), paylength);
        strncpy(*payload, str + paypos + 1, paylength);
    }

}