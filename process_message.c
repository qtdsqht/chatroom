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
    }else if (strcmp(cmd, "/list") == 0)
    {
        return LIST;
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
    FILE *fp;
    char buf[MAXLINE];

    if((fp = fopen("/home/qht/CLionProjects/mychat/user_table.txt", "a+")) == NULL)
    {
        perror("error open file /fp");
        exit(1);
    }

    while(fgets(buf, MAXLINE, fp) != NULL)
    {
        if(strstr(buf, username) != NULL)
        {
            fclose(fp);
            return REG_FAILED;
        }
    }
    fputc('\n', fp);
    fputs(username, fp);
    fputc(':', fp);
    fputs(password, fp);
    fclose(fp);
    return REG_SUCCESS;
}

loginstat_t login(const char *username, const char *password, char *online_client)
{
    FILE *fp;
    char buf[MAXLINE];
    char db_username[MAXLINE];
    char db_password[MAXLINE];

    if(online_client[0] != '\0')
    {
        return LOGIN_ISLOGIN;
    }

    if((fp = fopen("/home/qht/CLionProjects/mychat/user_table.txt", "a+")) == NULL)
    {
        perror("error open file /fp");
        exit(1);
    }

    while(fgets(buf, MAXLINE, fp) != NULL)
    {
        strcpy(db_username, strtok(buf, ":"));
        strcpy(db_password, strtok(NULL, "\n"));
        if (strcmp(username, db_username) == 0 && strcmp(password, db_password) == 0)
        {
            strcpy(online_client, username);
            fclose(fp);
            return LOGIN_SUCCESS;
        }else if (strcmp(username, db_username) == 0 && strcmp(password, db_password) != 0)
        {
            fclose(fp);
            return LOGIN_WRONGPW;
        }
    }

    fclose(fp);
    return LOGIN_UNREG;
}