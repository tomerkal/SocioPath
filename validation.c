#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "core.h"
#include "profile.h"
#include "validation.h"
#include "functions.h"

void create_account_menu()
{
    int checker = 1, randnum=0, encrypted_password=0;
    char *name, *pass, *val, *ans;

    printf("\n\n================================================================================\n\n");
    printf("To start your account, please choose a username\n");
    printf("\nYour username please: ");
    while (checker)
    {
        name = get_txt();
        printf("\n\n-------------------------------\n\n");
        if (!is_validName(name))
        {
            free(name);
            printf("\nSorry, but the username must be consisted from English letters or spaces only.\n\nPlease try again: ");
        }
        else if (name_in_use(name) != NULL)
        {
            free(name);
            printf("\nSorry, but the username you chose already exists in our system.\n\nPlease try again: ");
        }
        else
           checker = 0; // name can be in use
    }

    printf("\nWelcome %s! Please choose a password.\n\nYour password: ", name);
    checker = 1;
    while (checker)
    {
        pass = getPassword();
        printf("\n\n-------------------------------\n\n");
        if (!is_validPass(pass))
        {
            free(pass);
            printf("\n\nSorry, but the password must be consisted from English letters and numbers only, be 8 characters long, and must include at least 1 uppercase letter, 1 lowercase letter and 1 number.\n\nPlease try again: ");
        }

        else
        {
            printf("\nTo continue, please validate your password, by reentering it below.\n\nYour password: ");
            val = getPassword();
            printf("\n\n-------------------------------\n\n");
            if (strcmp(pass, val))
            {
                free(pass);
                free(val);
                printf("\nSorry, your inputs don't match.\n\nPlease choose a password again: ");
            }
            else
                checker = 0;
        }
    }
    free(val);

    printf("\nGreat! Password confirmed!\n");
    printf("\nPlease answer our security question:\n'Where did you perform your last air guitar jam?'\n\nYour answer: ");
    ans = get_txt();
    printf("\n\n-------------------------------\n\n");
    while (!is_validAnswer(ans))
    {
        free(ans);
        printf("\nSorry, your answer must be consisted from English letters, numbers, and spaces only.\n\nPlease try again: ");
        ans = get_txt();
        printf("\n\n-------------------------------\n\n");
    }

    printf("\nCongratulations %s! Your new account has been created. Press ENTER to go to your new profile!", name);
    getchar();

    srand(time(NULL));
    randnum = rand();

    encrypted_password = encrypt_pass(pass, randnum);
    free(pass);

    head_validate = create_validation(name, encrypted_password, randnum, ans);
    free(ans);

    head_profile = create_profile(name);
    free(name);

    profile_menu(head_profile);
}

UserValidate* create_validation(char *name, int password, int random, char *ans)
{
    /** creates a new user validation structure and returns its pointer
        the previous head becomes the new user's next, and the new one is the new head **/

    UserValidate *new_user;

    new_user = (UserValidate*)malloc(sizeof(UserValidate));

    strcpy(new_user->username, name);
    new_user->password = password;
    new_user->randomNum = random;
    strcpy(new_user->answer, ans);
    new_user->next = head_validate;

    return new_user;
}

void free_validation()
{
    /** free mallocated validation database **/

    UserValidate *temp=head_validate, *to_free;

    while (temp != NULL)
    {
        to_free = temp;
        temp = temp->next;
        free(to_free);
    }
}

void read_validationLine(FILE *fp, char *name, char *pass, char *randstr, char *ans)
{
    /** reads a single line (a single user) from validate.txt **/

    txtFromFile(fp, name, '_');

    fseek(fp, 2, 1);

    txtFromFile(fp, pass, '_');

    fseek(fp, 2, 1);

    txtFromFile(fp, randstr, '_');

    fseek(fp, 2, 1);

    txtFromFile(fp, ans, '\n');
}
