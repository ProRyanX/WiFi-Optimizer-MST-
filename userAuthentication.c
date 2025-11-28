#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "userAuthentication.h"

// ------------------- PASSWORD VALIDATION -------------------
int isValidPassword(char *pass) {
    int len = strlen(pass);
    int upper = 0, lower = 0, digit = 0, special = 0, space = 0;

    if (len < 8) return 0;

    for (int i = 0; i < len; i++) {
        if (isupper(pass[i])) upper = 1;
        else if (islower(pass[i])) lower = 1;
        else if (isdigit(pass[i])) digit = 1;
        else if (strchr("@#$%!*", pass[i])) special = 1;
    }

    if(strcspn(pass," ") == -1) space = 1; 

    return upper && lower && digit && special;
}

// ------------------- CHECK IF USER EXISTS -------------------
int userExists(char *username) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) return 0;

    User temp;
    while (fscanf(fp, "%s %s", temp.username, temp.password) != EOF) {
        if (strcmp(temp.username, username) == 0) {
            fclose(fp);
            return 1; // username already taken
        }
    }

    fclose(fp);
    return 0;
}

// ------------------- SIGNUP -------------------
void signup() {
    User newUser;

    printf("\n--- SIGNUP ---\n");
    do{
        printf("Enter username (must not contain space): ");
        fgets(newUser.username,sizeof(newUser.username),stdin);
        newUser.username[strcspn(newUser.username,"\n")] = '\0';
        if(strcspn(newUser.username," ") != strlen(newUser.username)){
            printf("Invalid Username!\n\n");
            continue;
        }
        break;
    }while(1);

    if (userExists(newUser.username)) {
        printf("Username already exists! Try another.\n");
        return;
    }

    do{
        printf("Enter password: ");
        scanf("%s", newUser.password);

        if (!isValidPassword(newUser.password)) {
            printf("Password must contain:\n");
            printf("1.) Minimum 8 chars\n");
            printf("2.) 1 uppercase, 1 lowercase\n");
            printf("3.) 1 digit\n");
            printf("4.) 1 special (@#$%!* )\n");
            printf("5.) No space character.\n\n");
            continue;
        }
        break;
    }while(1);

    FILE *fp = fopen(FILE_NAME, "a");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "%s %s\n", newUser.username, newUser.password);
    fclose(fp);

    printf("Signup successful!\n");
}

// ------------------- LOGIN -------------------
int login() {
    char username[50], password[50];

    printf("\n--- LOGIN ---\n");
    printf("Enter username: ");
    fgets(username,50,stdin);
    printf("Enter password: ");
    fgets(password,50,stdin);

    username[strcspn(username,"\n")] = '\0';
    password[strcspn(password,"\n")] = '\0';

    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) {
        printf("No users found! Please signup first.\n");
        return 0;
    }

    User temp;
    while (fscanf(fp, "%s %s", temp.username, temp.password) != EOF) {
        if (strcmp(temp.username, username) == 0 && strcmp(temp.password, password) == 0){
            printf("Login successful! Welcome, %s\n", username);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    printf("Invalid username or password.\n");
    return 0;
}

// ------------------- MAIN MENU -------------------
int mainPage() {
    int choice,ch;

    while (1) {
        printf("\n==== USER AUTHENTICATION SYSTEM ====\n");
        printf("1. Signup\n");
        printf("2. Login\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if(choice == 1)
            signup();
        else if(choice == 2){
            ch = login();
            break;
        }
        else if(choice == 0){
            printf("Exiting...\n");
            exit(0);
        }
        else
            printf("Invalid choice! Try again.\n");
    }
    
    if(ch) return 1;
    else return 0;

    return 0;
}