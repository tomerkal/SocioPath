#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "profile.h"
#include "functions.h"


void profile_menu(Profile* user_profile)
{
    char *input;

    print_profile(user_profile);

    while (1)
    {
        printf("\n\n================================================================================\n\n");
        printf("What would you like to do next? Choose one of the options, by typing its name:\n");
        printf("\"profile\" - Show your profile\n");
        printf("\"status\" - Update your current status\n");
        printf("\"checkStatus <friend_username>\" - Check the current status of a specific friend, whose username you enter in the place of <friend_username>\n");
        printf("\"checkRequests\" - check incoming friend requests\n");
        printf("\"printFriends\" - print the list of your friends (usernames)\n");
        printf("\"search <username>\" - Search SocioPath network for a username\n");
        printf("\"request <username>\" - Send a friend request to the given <username>\n");
        printf("\"unfriend <friend_username>\" - Unfriend the entered friend\n");
        printf("\"printNetwork\" - Print your social network\n");
        printf("\"#\" - Log out and return to the main menu\n");
        printf("\"$\" - Exit the app\n");
        printf("\nWhat will it be: ");

        input = get_txt();
        if (!strcmp(input, "profile"))
        {
            free(input);
            print_profile(user_profile);
        }

        else if (!strcmp(input, "status"))
        {
            free(input);
            update_status_menu(user_profile);
            print_profile(user_profile);
        }

        else if (!strncmp(input, "checkStatus", 11))
        {
            check_status(user_profile, input+12);
            free(input);
        }

        else if (!strcmp(input, "checkRequests"))
        {
            free(input);
            check_requests(user_profile);
        }

        else if (!strcmp(input, "printFriends"))
        {
            free(input);
            print_friends(user_profile);
        }

        else if (!strncmp(input, "search", 6))
        {
            search_network(input+6);
            free(input);
        }

        else if (!strncmp(input, "request", 7))
        {
            send_request(user_profile, input+8);
            free(input);
        }

        else if (!strncmp(input, "unfriend", 8))
        {
            unfriend(user_profile, input+9);
            free(input);
        }

        else if (!strcmp(input, "printNetwork"))
        {
            free(input);
            print_network(user_profile);
        }

        else if (!strcmp(input, "#"))
        {
            free(input);
            save_profiles();
            save_validations();
            printf("\n\n================================================================================\n\n");
            printf("\nBye bye dear %s, and hope to see you again soon!\n", user_profile->username);
            return;
        }

        else if (!strcmp(input, "$"))
        {
            free(input);
            close_app();
        }

        else
        {
            free(input);
            printf("\n\n================================================================================\n\n");
            printf("\nOops! We didn't understand your request\nPlease choose one of the options as specified!\n\n");
        }
    }
}

/******************************** Print user Profile **********************/

void print_profile(Profile *user_profile)
{
    /** prints 'user_profile's profile **/

    int i=0, counter=0;

    printf("\n\n================================================================================\n\n");
    printf("Hello %s and welcome to your profile! \n\n", user_profile->username);

    if (user_profile->status[0] == '\0')
    {
        printf("You currently have no status. To update your status, choose 'status' in the menu\n\n");
    }
    else
    {
        printf("Your current status is: \"%s\"\n\n", user_profile->status);
    }

    if (user_profile->friends_count == 0)
    {
        printf("You currently have no friends. When you'll have friends in your friends list, you'll be able to see their statuses here...\n");
    }
    else
    {
        for (i=0 ; i<user_profile->friends_count ; i++)
        {
            if (user_profile->friends[i]->status[0] == '\0')
                continue;
            counter++;
            if (counter == 1) // we want to print the header just when at least one friend has a status
                printf("What's on your friends' minds:\n");
            printf("%s: \"%s\"\n", user_profile->friends[i]->username, user_profile->friends[i]->status);
        }

        if (counter == 0)
            printf("Your friends have no statuses right now\nHere would appear their statuses when they'll have...\n");
    }
}
/*************************************************************************************************************/


/********************************* update user's status ************************************/
void update_status_menu(Profile *user_profile)
{
    /** the menu for changing 'user's own status **/

    int checker=1;
    char *input;

    printf("\n\n================================================================================\n\n");
    printf("Hello %s! To update your SocioPath status to share with friends, write it below.\n", user_profile->username);

    while (checker)
    {
        printf("\nYour new status: ");
        input = get_txt();
        if (strlen(input) > 512)
        {
            checker = long_status_menu(user_profile, input);
            free(input);
        }
        else
        {
            strcpy(user_profile->status, input);
            free(input);
            checker = 0;
        }
    }

    printf("\n\n-------------------------------\n\n");
    printf("Your status has been updated!\nPress ENTER to continue");
    getchar();
}

int long_status_menu(Profile *user_profile, char *status)
{
    /** the menu that opens in case of a status longer than 512 characters
    returns 0 in order to finish status menu, returns 1 to continue the loop for input **/

    char *input;

    printf("\n\n================================================================================\n\n");
    printf("The status you entered crosses our length limit.\nPlease choose what to do by entering the corresponding number:\n  1. Cut status to length limit and update.\n  2. Enter a new status.\nYour choice: ");
    while (1)
    {
        input = get_txt();
        if (!strcmp(input, "1"))
        {
            strncpy(user_profile->status, status, 512);
            free(input);
            return 0;
        }
        else if (!strcmp(input, "2"))
        {
            printf("\n\n-------------------------------\n\n");
            free(input);
            return 1;
        }
        else
        {
            free(input);
            printf("\n\n-------------------------------\n\n");
            printf("Ops! We didn't understand your request.\nPlease enter one of the options: 1 or 2: ");
        }
    }
}
/*************************************************************************************************************************************/


/************************* check a user's status ****************************/
void check_status(Profile *user_profile, char *friend_username)
{
    /** to check the status of a specific friend **/

    int i=0;

    printf("\n\n================================================================================\n\n");
    for (i=0 ; i<user_profile->friends_count ; i++)
    {
        if (!strcmp(user_profile->friends[i]->username, friend_username))
        {
            if (user_profile->friends[i]->status[0] == '\0')
                printf("\n%s has no status right now...\n", user_profile->friends[i]->username);
            else
            {
                if (endsWithS(friend_username))
                    printf("\n%s' status is: \"%s\"\n", friend_username, user_profile->friends[i]->status);
                else
                    printf("\n%s's status is: \"%s\"\n", friend_username, user_profile->friends[i]->status);
            }
            return;
        }
    }
    printf("\nSorry, but we couldn't find the friend you were looking for in your friends list.\n");

}
/*************************************************************************************************************************************/


/************************************** check user's friend requests ***********************************/
void check_requests(Profile *user_profile)
{
    /** prints the menu for accepting or refusing friend requests **/

    int i, checker=1;
    char *input;

    printf("\n\n================================================================================\n\n");

    if (user_profile->requests_count == 0)
    {
        printf("Dear %s, you have no friend requests right now...\nCheck in later to see if someone wants to be your friend!\n", user_profile->username);
        return;
    }

    while (checker)
    {
        if (user_profile->requests_count == 1)
            printf("Dear %s, you have 1 friend request!\n\nThe pending request is from the user:\n  - %s\n", user_profile->username, user_profile->requests[0]->username);

        else
        {
            printf("Dear %s, you have %d friend requests!\n\nThe pending requests are from the users:\n", user_profile->username, user_profile->requests_count);
            for (i=0 ; i<user_profile->requests_count ; i++)
            {
                printf("  - %s\n", user_profile->requests[i]->username);
            }
        }

        printf("\n-To approve a request, enter \"approve::<username>\n");
        printf("-To refuse and delete a request, enter \"refuse::<username>\n");
        printf("-To go back to your profile, enter \"&\"\n");
        printf("\nWhat will it be: ");

        input = get_txt();
        if (!strncmp(input, "approve::", 9))
        {
            checker = approve_request(user_profile, input+9, 1);
            free(input);
        }

        else if (!strncmp(input, "refuse::", 8))
        {
            checker = refuse(user_profile, input+8);
            free(input);
        }

        else if (!strcmp(input, "&"))
        {
            free(input);
            return;
        }
        else
        {
            free(input);
            printf("\n\n-------------------------------\n\n");
            printf("Oops! We didn't understand your request.\nPlease enter one of the specified options.\n\n");
        }
    }
}

int approve_request(Profile *user_profile, char *request_username, int flag)
{
    /** To accept a friend request from requests list
        Returns 1 in case of error to let the user try again, or 0 in success
        'flag' determines if we got here from checkRequests menu or request menu (for aesthetic and printing purposes) **/

    int i;

    for (i=0 ; i<user_profile->requests_count ; i++)
    {
        if (!strcmp(user_profile->requests[i]->username, request_username))
        {
            add_friend(user_profile, user_profile->requests[i]);
            add_friend(user_profile->requests[i], user_profile);
            remove_friend_request(user_profile, i);
            if (flag == 1)
                printf("\n\n================================================================================\n\n");
            printf("You and %s are now friends!\n", request_username);
            return 0;
        }
    }

    printf("\n\n-------------------------------\n\n");
    printf("Oops! We didn't find a request from the user you entered.\nPlease try again\n\n");
    return 1;
}

int refuse(Profile *user_profile, char *request_username)
{
    /** To refuse a friend request from requests list
        Returns 1 in case of error to let the user try again, or 0 in success **/

    int i;

    for (i=0 ; i<user_profile->requests_count ; i++)
    {
        if (!strcmp(user_profile->requests[i]->username, request_username))
        {
            remove_friend_request(user_profile, i);
            printf("\n\n================================================================================\n\n");
            printf("\nThe friend request from %s has been successfully deleted!\n", request_username);
            return 0;
        }
    }

    printf("\n\n-------------------------------\n\n");
    printf("Oops! We didn't find a request from the user you entered.\nPlease try again\n\n");
    return 1;

}
/*************************************************************************************************************************************/

/************************* print user's friends **********************************/

void print_friends(Profile *user)
{
    /** prints the user's friends list, alphabetically **/

    int i;

    printf("\n\n================================================================================\n\n");
    if (user->friends_count == 0)
    {
        printf("\nYou currently have no friends.\nA list of your friends will be available here after you add more friends to your friends list.\n");
        return;
    }

    printf("\nYour current friends are:\n\n");
    for (i=0 ; i<user->friends_count ; i++)
    {
        printf(" -  %s\n", user->friends[i]->username);
    }
}
/********************************************************************************************************************************/

/************************** search the network for username **************************************************/

void search_network(char *query)
{
    /** Search the SocioPath network for a username **/

    int results_num=0, i;
    char *query_low;
    char *username_low;
    char **results = (char**)malloc(sizeof(char*));
    Profile *temp;

    printf("\n\n================================================================================\n\n");
    if (query[0] == '\0' || !(strcmp(query, " ")))
    {   // if the user leaves search field empty
        printf("We didn't quite get what you are searching for\nPlease try again and type a name to search for...");
        return;
    }

    query_low = to_lower_string(query+1); // we want the search to don't be case sensitive
    for (temp=head_profile ; temp!=NULL ; temp=temp->next)
    {
        username_low = to_lower_string(temp->username); // we want the search to don't be case sensitive
        if (is_partof(username_low, query_low)) // we want the search query to be even a part of the result
        {
            /** we add the search to 'results' by the same concept of friends/requests lists
                if it's empty, we just add and increase the count, else realloc the list by one slot **/
            if (results_num == 0)
            {
                results[0] = temp->username;
                results_num++;
            }

            else
            {
                results = (char**)realloc(results, (++results_num)*sizeof(char*));
                results[results_num-1] = temp->username;
            }
        }
        free(username_low);
    }

    if (results_num == 0)
    {
        printf("The search for \"%s\" has found no results in our system!\n", query+1);
    }

    else
    {
        printf("The search for \"%s\" has found the following results in our system:\n\n", query+1);
        for (i=0 ; i<results_num ; i++)
        {
            printf(" -  %s\n", results[i]);
        }
    }

    free(results);
    free(query_low);
}
/*************************************************************************************************************/


/*************************************** send friend request **************************************************************/

void send_request(Profile *user, char *request_name)
{
    /** Send a friend request to user 'request_name' from 'user' **/

    Profile *to_send = find_profile(request_name);

    printf("\n\n================================================================================\n\n");

    if (!strcmp(user->username, request_name)) // user can't add himself
    {
        printf("\nSorry, but you can't be friends with yourself (at least in the SocioPath network.....)\n");
        return;
    }

    if (to_send == NULL) // if user doesn't exist
    {
        printf("\nSorry, but the username you are looking for does not exist in our system.\n");
        return;
    }

    if (is_inList(to_send->requests, to_send->requests_count, user->username))
    {
        printf("\nLooks like you already sent a friend request to %s...\nPlease wait until the user accepts it.\n", request_name);
        return;
    }

    if (is_inList(user->friends, user->friends_count, request_name))
    {
        printf("\nLooks like you and %s are already friends...\n", request_name);
        return;
    }

    if (is_inList(user->requests, user->requests_count, request_name))
    {
        printf("\nLooks like %s also sent you a friend request lately...\n", request_name);
        approve_request(user, request_name, 0);
        return;
    }

    add_request(to_send, user);

    printf("\nYour friend request to \"%s\" was successfully sent!\n", request_name);
}
/*************************************************************************************************************************/

/**************************************** unfriend ********************************************************************/

void unfriend(Profile *user, char *name)
{
    /** remove a friend from friends list **/

    printf("\n\n================================================================================\n\n");
    if (!is_inList(user->friends, user->friends_count, name))
    {   // if the requested name is not even in friends list...
        printf("\nSorry, but the user you are trying to unfriend is not in your friends list...\n");
        return;
    }
    // If we got here, the user is in the friends list so we want to delete mutual friendship:
    remove_friend(user, name);
    remove_friend(find_profile(name), user->username);

    printf("\nThe user \"%s\" is no longer in your friends list\n", name);
}

/****************************************************************************************************************************/

/************************************* print user's network *****************************************************************/

void print_network(Profile *user)
{
    /** Prints the printNetwork function of profile menu **/

    /** We create a "helper" list 'network' and start off with user
        Then, we run over the list until its end, while expanding it by adding friends that are not there already
        Every iteration we count the size of the next, and save it as stop index
        The loop will end alone when there are no more users to add because they already appear **/

    int iteration=1, i=0, k, size=1, cur_run=1, next_run=0;
    Profile **network = (Profile**)malloc(sizeof(Profile*));

    printf("\n\n================================================================================\n\n");
    if (user->friends_count == 0)
    {   // if the user have no friends there is no network at all...
        printf("Dear %s, your social network is currently empty because you have no friends...\nWhen you'll have friends in your friends list, here will appear your full socila network\n", user->username);
        return;
    }
    printf("Dear %s, your social network is:\n\n", user->username);
    printf("You: %s\n", user->username);


    network[0] = user;

    while (i < size)
    {
        if (i == cur_run)
        {
            printf("\b\b \n");  // if we reached the end of 'cur_run', we delete the ', ' from last use and go down a line
            iteration++;        // we move to next iteration
            cur_run += next_run;// our next iteration's stop index
            next_run = 0;       // reset the counter for next iteration
        }

        for (k=0 ; k<network[i]->friends_count ; k++)
        {
            if (!is_inNetwork(network, size, network[i]->friends[k]))
            {   // if current friend is not already in 'network', we want to add
                network = (Profile**)realloc(network, (++size)*sizeof(Profile*));
                network[size-1] = network[i]->friends[k];
                next_run++; // we will want to check this friend in next iteration
                if (next_run == 1)// we want to print the header only if there is at least one user!
                {
                    if (iteration == 1)
                        printf("Your friends: ");
                    else if (iteration == 2)
                        printf("Friends of your friends: ");
                    else if (iteration == 3)
                        printf("3rd circle friends: ");
                    else
                        printf("%dth circle friends: ", iteration);
                }

                printf("%s, ", network[i]->friends[k]->username);
            }
        }

        i++;
    }
}


int is_inNetwork(Profile **network, int size, Profile *user)
{
    /** Checks if 'user' is already in 'network'
        Returns 1 if it is, 0 otherwise             **/

    int i;

    for (i=0 ; i<size ; i++)
    {
        if (network[i] == user)
            return 1;
    }

    return 0;
}

/*****************************************************************************************************************************/

/************************************************* GENERAL PROFILE FUNCTIONS *********************************************/

void add_request(Profile *user, Profile *to_add)
{
    /** Pretty clear, adds the user 'to_add' to 'user's requests list **/

    if (user->requests_count == 0)
    {   /** If there are no requests, we know there is one empty slot.
            We fill it and increase the count                **/
        user->requests[0] = to_add;
        user->requests_count++;
    }

    else
    {
        user->requests = (Profile**)realloc(user->requests, (++user->requests_count)*sizeof(Profile*));
        user->requests[user->requests_count-1] = to_add;
    }
}

void add_friend(Profile *user, Profile *to_add)
{
    /** Pretty clear, adds the user 'to_add' to 'user's friends list **/

    int i;
    char *to_add_low;
    char *cur_low;

    if (user->friends_count == 0)
    {   /** If there are no friends, we know there is one empty slot.
            We fill it and increase the count                **/
        user->friends[0] = to_add;
        user->friends_count++;
    }

    else
    {
        // We know the list is full, we want to add a slot, and increase the count
        user->friends = (Profile**)realloc(user->friends, (++user->friends_count)*sizeof(Profile*));
        to_add_low = to_lower_string(to_add->username);
        for (i=0 ; i<(user->friends_count-1) ; i++)
        {
            cur_low = to_lower_string(user->friends[i]->username);
            if ((strcmp(cur_low, to_add_low)) > 0)
            {   // we want to add the user in the alphabetic order (for printFriends function)
                free(cur_low);
                make_space(user, i);
                user->friends[i] = to_add;
                return;
            }
            free(cur_low);
        }
        free(to_add_low);
        user->friends[i] = to_add;
    }
}

void remove_friend(Profile *user, char *name)
{
    /** Removes a friend from list,
        "closes" the gap created,
        and changes the size of list (by realloc) to be the new list size **/
    int i, j;

    for (i=0 ; i<user->friends_count ; i++)
    {
        if (!strcmp(user->friends[i]->username, name))
        {
            for (j=i+1 ; j<user->friends_count ; j++)
            {
                user->friends[i++] = user->friends[j];
            }
            user->friends[i] = NULL;
            if (user->friends_count == 1) // if there was one slot, we just want to leave it empty
                user->friends_count = 0;
            else
                user->friends = (Profile**)realloc(user->friends, (--user->friends_count)*sizeof(Profile*));
        }
    }
}

void remove_friend_request(Profile *user, int index)
{
    /** Removes a friend request from list,
        "closes" the gap created,
        and changes the size of list (by realloc) to be the new list size **/

    int i=index,j=0;

    for (j=i+1 ; j<user->requests_count ; j++)
    {
        user->requests[i++] = user->requests[j];
    }
    user->requests[i] = NULL;
    if (user->requests_count == 1) // if there was one slot, we just want to leave it empty
        user->requests_count = 0;
    else
        user->requests = (Profile**)realloc(user->requests, (--user->requests_count)*sizeof(Profile*));

}

Profile* create_profile(char *name)
{
    /** creates a new profile at head of profiles list
        previous head becomes last and the pointer to new heas is returned **/

    Profile* new_profile;

    new_profile = (Profile*)malloc(sizeof(Profile));

    strcpy(new_profile->username, name);

    *new_profile->status = '\0';

    new_profile->friends = (Profile**)malloc(sizeof(Profile*));
    new_profile->friends_count = 0; // the new list has one empty slot

    new_profile->requests = (Profile**)malloc(sizeof(Profile*));
    new_profile->requests_count = 0; // the new list has one empty slot

    new_profile->next = head_profile;

    return new_profile;
}

void free_profiles()
{
    /** At the end of the program we want to free all allocated memory, this is executed before exit **/

    Profile *next=head_profile, *to_free;

    while (next != NULL)
    {
        to_free = next;
        next = next->next;
        free(to_free->friends); // free friends list
        free(to_free->requests); // free requests list
        free(to_free);  // free the profile itself
    }
}

Profile* find_profile(char *name)
{
    /** Returns a pointer to profile related with 'name', NULL if profile not found **/

    Profile *temp;

    temp = head_profile;
    while (temp != NULL)
    {
        if (!strcmp(temp->username, name))
            return temp;
        temp = temp->next;
    }
    return NULL;

}

void read_friends_line(FILE *fp, Profile *user)
{
    /** Read the Friends line in Profile.txt and saves it to 'user' **/

    int i=0;
    char c;
    char name[51];

    while ((c=getc(fp)) != '\n')
    {
        if (c == '_')
        {   // means it's end of name
            name[i] = '\0';
            add_friend(user, find_profile(name));
            fseek(fp, 2, 1); // Skip the '$_' separators
            i = 0;
        }

        else
            name[i++] = c;
    }

    if (i != 0) // Means it's the last name
    {
        name[i] = '\0';
        add_friend(user, find_profile(name));
    }
}

void read_requests_line(FILE *fp, Profile *user)
{
    /** Read the Pending line in Profile.txt and saves it to 'user' **/

    int i=0;
    char c;
    char name[51];

    while ((c=getc(fp)) != '\n')
    {
        if (c == '_')
        {   // means it's end of name
            name[i] = '\0';
            add_request(user, find_profile(name));
            fseek(fp, 2, 1); // Skip the '$_' separators
            i = 0;
        }

        else
        {
            name[i] = c;
            i++;
        }
    }

    if (i != 0) // Means it's the last name
    {
        name[i] = '\0';
        add_request(user, find_profile(name));
    }
}

int is_inList(Profile **list, int size, char *name)
{
    /** checks if user 'name' is in 'list' (a user's Requests or Friends list)
        Returns 1 if it is, 0 otherwise                             **/

    int i;

    for (i=0 ; i<size ; i++)
    {
        if (!strcmp(list[i]->username, name))
            return 1;
    }

    return 0;
}

void make_space(Profile *user, int space_index)
{
    /** Creates an empty slot in the friends list of 'user' at 'space_index' **/

    int i;

    for (i=user->friends_count-1 ; i>space_index ; i--)
    {
        user->friends[i] = user->friends[i-1];// moves up every slot until 'space_index' left empty
    }
}
