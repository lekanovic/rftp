#ifndef __FTP_MSG_H__
#define __FTP_MSG_H__

#define LOG_IN_OK		"230 Welcome to server\r\n"
#define WELCOME_MSG		"220 Raddes ftp server beta v1.0\r\n"
#define LOGIN_MSG		"331 Anonymous access allowed\r\n"
#define SYSTEM_TYPE		"215 LINUX-2.6\r\n"
#define OPEN_ASCII_MODE		"150 Opening ASCII mode data connection\r\n"
#define TYPE_ASCII_MODE		"200 Opening ASCII mode data connection\r\n"
#define OPEN_BINARY_MODE	"150 Opening binary mode data connection\r\n"
#define TYPE_BINARY_MODE	"200 Opening binary mode data connection\r\n"
#define TYPE_EBCDIC_MODE	"200 Opening EBCDIC mode data connection\r\n"
#define WORKING_DIR_CHANGED	"200 Working directory changed\r\n"
#define TRANSFER_COMPLETE	"226 Transfer complete\r\n"
#define GOODBYE			"221 Goodbye\r\n"
#define PORT_CMD_OK		"200 PORT command successful\r\n"
#define START_STOR_CMD		"150 File start to download\r\n"
#define ENTER_PASSV_MODE	"227 Entering Passive Mode\r\n"
#define FILE_UNAVAILABLE	"550 Requested action not taken. File unavailable\r\n"
#define FILE_ACTION_OK		"250 Requested file action okay, completed\r\n"
#define AUTHEN_FAILED		"530 Authentication failed.\r\n"
#define INVALID_USER_NAME	"530 Invalid user name\r\n"
#define NEED_PASSWD		"331 User name okay, need password.\r\n"

#endif
