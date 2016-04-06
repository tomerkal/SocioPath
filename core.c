#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "validation.h"
#include "profile.h"
#include "login.h"
#include "core.h"


void main_menu()
{
    char *input;

    while (1)
    {
        printf("\n\n================================================================================\n\n");
        printf("Hello dear Guest and welcome to 'SocioPath - Socialize in an alternative path'!\n");
        printf("What would you like to do next?\n");
        printf("1.\tLog in\n2.\tCreate a new account\n3.\tExit the app\n");
        printf("\nYour choice please: ");

        input = get_txt();
        if (input == NULL)
            exit(1);
        if (!strcmp(input, "1"))
        {
            free(input);
            login_menu();
        }

        else if (!strcmp(input, "2"))
        {
            free(input);
            create_account_menu();
        }

        else if (!strcmp(input, "3"))
        {
            free(input);
            close_app();
        }

        else
        {
            free(input);
            printf("\n\n================================================================================\n\n");
            printf("\nOops! We didn't understand your request\nPlease choose one of the options from 1-3!");
        }
    }
}

void save_validations()
{
    /** Save/copy the validation database to file **/

    FILE* fp;
    UserValidate *temp;

    fp = fopen("validation.txt", "w");
    for (temp=head_validate ; temp!=NULL ; temp=temp->next)
    {
        fprintf(fp, "%s_$_", temp->username);
        print_encrypt(fp, temp->password);
        fprintf(fp, "_$_");
        fprintf(fp, "%d_$_", temp->randomNum);
        fprintf(fp, "%s\n", temp->answer);
    }
    fclose(fp);
}

void save_profiles()
{
    /** Save/copy the profile database to file **/

    int i;
    FILE* fp;
    Profile *temp;

    fp = fopen("profile.txt", "w");
    for (temp=head_profile ; temp!=NULL ; temp=temp->next)
    {
        fprintf(fp, "Username_%s\n", temp->username);
        fprintf(fp, "Status_%s\n", temp->status);

        fprintf(fp, "Friends_");
        for (i=0 ; i<temp->friends_count ; i++)
        {
            fprintf(fp, "%s", temp->friends[i]->username);
            if (i != temp->friends_count-1)
                fprintf(fp, "_$_");
        }
        fprintf(fp, "\n");

        fprintf(fp, "Pending_");
        for (i=0 ; i<temp->requests_count ; i++)
        {
            fprintf(fp, "%s", temp->requests[i]->username);
            if (i != temp->requests_count-1)
                fprintf(fp, "_$_");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void close_app()
{
    /** Safely exit the application, saving all data **/

        save_validations();
        free_validation();

/*********************************/

        save_profiles();
        free_profiles();


    printf("\n\n================================================================================\n\n");
    printf("\nThanks for using SocioPath, hope to see you again soon! %c%c%c\n", 1,1,1);
    printf("\n\n================================================================================\n\n");
    printf("\npress ENTER to exit");
    getchar();
    exit(0);
}


void open_app()
{
    /** Read all data files and copy them to run-time databases **/

    int end=0, randnum=0, encrypted=0, i=0;
    char name[51], ans[101], pass[32], randstr[11], status[513];
    FILE* fp;
    Profile *temp;

/******READ VALIDATION.TXT **************/

    fp = fopen("validation.txt", "r+");
    if (fp == NULL)
        return;
    fseek(fp, 0, 2);
    end = ftell(fp); // my own EOF
    rewind(fp);

    while (ftell(fp) < end)
    {
        read_validationLine(fp, name, pass, randstr, ans);
        randnum = atoi(randstr);
        encrypted = binary2int(pass);
        head_validate = create_validation(name, encrypted, randnum, ans);
        head_profile = create_profile(name); // we already create the profiles for easier work reading profiles.txt
    }
    fclose(fp);

/****  READ PROFILE.TXT. ***********/

    fp = fopen("profile.txt", "r");
    if (fp == NULL)
        return;
    fseek(fp, 0, 2);
    end = ftell(fp);
    rewind(fp);

    while (ftell(fp) < end)
    {
        fseek(fp, 9, 1); // skip Username_
        txtFromFile(fp, name, '\n');
        temp = find_profile(name);
        fseek(fp, 7, 1); // skip Status_
        txtFromFile(fp, status, '\n');
        strcpy(temp->status, status);
        fseek(fp, 8, 1); // skip Friends_
        read_friends_line(fp, temp);
        fseek(fp, 8, 1); // skip Pending_
        read_requests_line(fp, temp);
    }
    fclose(fp);
}
