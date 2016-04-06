#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#include "core.h"
#include "functions.h"
#include "profile.h"
#include "login.h"

void login_menu()
{
    char *input, *name, *pass;
    UserValidate *user_validate;
    Profile *user_profile;

    while (1)
    {
        printf("\n\n================================================================================\n\n");
        printf("To log in, please enter your username and password in the format: 'usrename::password'.\n");
        printf("  To go back to the main menu, enter '#'\n");
        printf("  To exit the app, please enter '$'\n");
        printf("\nWhat will it be: ");

        input = getLoginInput();
        if (!strcmp(input, "#"))
        {
            free(input);
            return;
        }

        if (!strcmp(input, "$"))
        {
            free(input);
            close_app();
        }

        name = name_seperator(input);
        if (name == NULL)   // NULL signifies invalid input (check implementation of 'name_seperator')
        {
            free(input);
            free(name);
            printf("\n\n================================================================================\n\n");
            printf("\nSorry, but Your input is invalid\nPlease keep to the format specified!\n\nPress ENTER to try again.");
            getchar();
        }

        else
        {
            user_validate = name_in_use(name);
            free(name);
            if (user_validate == NULL)
            {
                printf("\n\n================================================================================\n\n");
                printf("\nSorry, but the username you entered don't exist in our database.\n\nPlease press ENTER to try again.");
                getchar();
            }

            else
                break;
        }
    }

    pass = pass_seperator(input);
    free(input);
    if (!is_validPass(pass)) // Maybe not neccesary, but prevents someone to login with wrong password because of encryption system
    {
        if (wrong_password_menu(user_validate, "\b"))
            return;
    }

    else if ((encrypt_pass(pass, user_validate->randomNum)) != user_validate->password)
    {
        if (wrong_password_menu(user_validate, "\b"))
            return;
    }

    else
    {
        printf("\n\n================================================================================\n");
        printf("\nHello dear %s!\n", user_validate->username);
    }

    free(pass);
    printf("\nPress ENTER to go to your profile\n");
    getchar();

    user_profile = find_profile(user_validate->username);
    profile_menu(user_profile);
}


/************************ Wrong Password ********************************************/

int wrong_password_menu(UserValidate *user, char *str)
{
    /** the function returns int, in case user wants to go to main menu, we know to return in login_menu **/

    char *input;

    printf("\n\n================================================================================\n\n");
    printf("Oops! We are sorry dear %s, but it looks like the password you entered don't match our data %s. Please try again.\nYou can type the correct password, or:\n", user->username, str);
    printf("-  If you forgot your password and want to set a new one, press '!'\n");
    printf("-  To go back to the main menu, enter '#'\n");
    printf("-  To exit the app, please enter '$'\n");
    printf("\nWhat will it be: ");

    input = getPassword();
    if (input == NULL)
        exit(1);
    if (!strcmp(input, "#"))
    {
        free(input);
        return 1;
    }

    if (!strcmp(input, "$"))
    {
        free(input);
        close_app();
    }

    if (!strcmp(input, "!"))
    {
        free(input);
        if (forgot_password_menu(user))
            return 1;
    }

    else if ((encrypt_pass(input, user->randomNum)) != user->password)
    {
        free(input);
        return wrong_password_menu(user, "again");
    }
    else
    {
        free(input);
        printf("\n\n================================================================================\n\n");
        printf("\nGreat! you've been recognized by our system.\n");
    }
    return 0;
}
/****************************************************************************************************************************/

/****************************** Forgot Password *************************************************/

int forgot_password_menu(UserValidate *user)
{
    /** the function returns int, in case user wants to go to main menu, we know to return in wrong_password_menu **/

    int i=2;
    char *input;

    printf("\n\n================================================================================\n\n");
    printf("To reset your password, please answer our security question.\n");
    printf("'Where did you perform your last air guitar jam?'\n");
    printf("-  To go back to the main menu, enter '#'\n");
    printf("-  To exit the app, please enter '$'\n");
    printf("\nWhat will it be: ");

    input = get_txt();
    if (input[0] == '#')
    {
        free(input);
        return 1;
    }

    if (input[0] == '$')
    {
        free(input);
        close_app();
    }

    while (strcmp(input, user->answer))
    {
        if (i == 0)
        {
            printf("\n\n-------------------------------\n\n");
            printf("\nSorry, but you entered a wrong answer 3 times.\nThe program will close itself.");
            free(input);
            close_app();
        }
        printf("\n\n-------------------------------\n\n");
        if (i == 1)
            printf("\nSorry, the answer is wrong. Please try again.\nNOTE: you have %d attempt left.\n\nYour answer: ", i);
        else
            printf("\nSorry, the answer is wrong. Please try again.\nNOTE: you have %d attempts left.\n\nYour answer: ", i);
        free(input);
        input = get_txt();
        i--;
    }

    reset_password_menu(user);
    return 0;
}
/****************************************************************************************************************************/

/****************************** Reset Password ************************************************/

void reset_password_menu(UserValidate *user)
{
    int checker=1;
    char *pass;

    printf("\n\n================================================================================\n\n");
    printf("Your answer was confirmed! Please choose a new password.\n\nYour new password: ", user->username);

    pass = getPassword();
    while (!is_validPass(pass))
    {
        free(pass);
        printf("\n\n-------------------------------\n\n");
        printf("\nSorry, but the password must be consisted from English letters and numbers only, be 8 characters long, and must include at least 1 uppercase letter, 1 lowercase letter and 1 number.\n\nPlease try again: ");
        pass = getPassword();
    }

    user->password = encrypt_pass(pass, user->randomNum);
    free(pass);
    printf("\n\n-------------------------------\n");
    printf("\nYour new password has been saved!\n");
    printf("\n-------------------------------\n\n");

}
/****************************************************************************************************************************/

/********************************General Functions **********************************************************/

char* getLoginInput()
{
    /** special get function to get from user the login menu input
        returns a pointer to start of buffer                **/

    int i = 0, capacity=10, counter=0, wtp=0;
    char c, *buffer = (char*) malloc(capacity * sizeof(char));

    do {
        c = getch();

        if (counter == 2)
            wtp = 1; // Means we count 2 ':' in a row and now writing to password
        if (buffer[i-1] == ':')
            counter = 1; // in case we go back with backspace, the counter will reset, but we reach a ':'
        if (c == ':')
            counter++;
        else
            counter = 0; // we want to reset the counter if they are not in a row
        if( isprint(c) )
        {
            if (i == (capacity - 1))
            {
                buffer = (char*) realloc(buffer, capacity * 2 * sizeof(char));
                if (buffer == NULL)
                    return NULL;
                capacity *= 2;
            }
            buffer[i] = c;
            i++;
            if (wtp)
                printf("%c", '*');
            else
                printf("%c", c);
        }
        else if( c == 8 && i )
        {
            buffer[i] = '\0';
            i--;
            printf("%s", "\b \b");
            if (buffer[i] == ':')
                wtp = 0; // if we deleted a ':' means we no longer write to password
        }
    } while( c != 13 );

    buffer[i] = '\0';
    return buffer;
}

char* name_seperator(char *input)
{
    /** takes the special input and "sucks" from it only the name **/

    int i=0, counter=0, capacity = 10;
    char *temp = (char*)malloc(capacity*sizeof(char));

    while (input[i] != '\0') // runs on all the input (to cover all cases)
    {
        if (input[i] == ':')
        {
            counter++;
            if (input[i+1] != ':')
                return NULL; // NULL signifies for us an invalid input
            break;
        }
        if (i == capacity-1)
        {
            temp = (char*)realloc(temp, capacity*2*sizeof(char));
            if (temp == NULL)
                return NULL;
            capacity *= 2;
        }
        temp[i] = input[i];
        i++;

    }
    if (counter == 0)
        return NULL; //invalid input
    temp[i] = '\0';
    return temp;
}

char* pass_seperator(char* input)
{
    /** takes the special input and "sucks" from it only the password **/

    int i=0, j=0, wt_pass=0, capacity = 10;
    char *temp = (char*)malloc(capacity*sizeof(char));

    while (input[i] != '\0')
    {
        if (input[i] == ':')
        {
            i += 2;
            wt_pass = 1;
        }

        if (wt_pass)
        {
            if (j == capacity-1)
            {
                temp = (char*)realloc(temp, capacity*2*sizeof(char));
                if (temp == NULL)
                    return NULL;
                capacity *= 2;
            }
            temp[j] = input[i];
            j++;
        }
        i++;
    }
    temp[j] = '\0';

    return temp;
}
