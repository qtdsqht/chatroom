//
// Created by qht on 22-5-25.
//

#ifndef MYCHAT_CHATMACRO_H
#define MYCHAT_CHATMACRO_H

#define MAXLINE 50
#define MAXARG 10

#define order_t int
#define REG 1
#define LOGIN 2
#define LIST 3
#define WHISPER 4
#define ALL 5

#define regstat_t int
#define REG_SUCCESS 1
#define REG_FAILED 0

#define loginstat_t int
#define LOGIN_SUCCESS 1
#define LOGIN_UNREG 0
#define LOGIN_WRONGPW -1
#define LOGIN_ISLOGIN -2

#endif //MYCHAT_CHATMACRO_H
