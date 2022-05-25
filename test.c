//
// Created by qht on 22-5-24.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "chatmacro.h"

loginstat_t login(const char *username, const char *password, char *online_client)
{
    FILE *fp;
    char buf[MAXLINE];
    char db_username[MAXLINE];
    char db_password[MAXLINE];

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

int main()
{
    char username[MAXLINE];
    char password[MAXLINE];
    char online[MAXLINE];

    scanf("%s", username);
    scanf("%s", password);

    printf("%d\n",login(username, password, online));
    printf("%s", online);
    return 0;
}