//
// Created by qht on 22-5-24.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "chatmacro.h"

int split_msg(const char *str, char argv[][MAXLINE])
{
    char tmp[MAXLINE];
    int argc = 0;
    int i = 0;

    while(*str == ' ')
    {
        str++;            //remove front blank
    }

    while (*str != '\n')
    {
        if (*str != ' ')
        {
            tmp[i++] = *str;
        } else if (*(str - 1) != ' ')
        {
            tmp[i] = '\0';
            strcpy(argv[argc++], tmp);      //skip blank
            i = 0;
        }
        str++;
    }

    tmp[i] = '\0';
    strcpy(argv[argc++], tmp);
    return argc;
}

order_t process_cmd(const char *cmd)
{
    if(strcmp(cmd, "/register") == 0)
    {
        return REG;
    }else if (strcmp(cmd, "/login") == 0)
    {
        return LOGIN;
    }else if (strcmp(cmd, "/online") == 0)
    {
        return ONLINE;
    }else if (strcmp(cmd, "/whisper") == 0)
    {
        return WHISPER;
    }else
    {
        return ALL;
    }
}


regstat_t reg(const char *username, const char *password)
{
    FILE *user_table;
    char buf[MAXLINE];

    if((user_table = fopen("/home/qht/CLionProjects/mychat/user_table.txt", "a+")) == NULL)
    {
        perror("error open file /user_table");
        exit(1);
    }

    while(fgets(buf, MAXLINE, user_table) != NULL)
    {
        if(strstr(buf, username) != NULL)
        {
            fclose(user_table);
            return REG_FAILED;
        }
    }
    fputc('\n', user_table);
    fputs(username,user_table);
    fputc(':', user_table);
    fputs(password, user_table);
    fclose(user_table);
    return REG_SUCCESS;
}
