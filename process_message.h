//
// Created by qht on 22-5-25.
//

#ifndef MYCHAT_PROCESS_MESSAGE_H
#define MYCHAT_PROCESS_MESSAGE_H
#include "chatmacro.h"

int split_msg(const char *str, char argv[][MAXLINE]);

order_t process_cmd(const char *cmd);

regstat_t reg(const char *username, const char *password);

#endif //MYCHAT_PROCESS_MESSAGE_H
