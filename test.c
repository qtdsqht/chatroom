//
// Created by qht on 22-5-24.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "chatmacro.h"

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


int main()
{
    char username[MAXLINE];
    char password[MAXLINE];

    scanf("%s", username);
    scanf("%s", password);

    printf("%d",reg(username, password));
    return 0;
}