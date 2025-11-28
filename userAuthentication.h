#ifndef USERAUTHENTICATION_H
#define USERAUTHENTICATION_H

// File name
#define FILE_NAME "users.txt"

// User details structure
typedef struct {
    char username[50];
    char password[50];
} User;

// User AUthentication Functions
int isValidPassword(char *pass);
int userExists(char *username);
void signup();
int login();
int mainPage();

#endif