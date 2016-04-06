#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED


void main_menu();
void close_app();
void save_validations();
void save_profiles();
void open_app();


typedef struct validate // mallocated
{
    char username[51];
    int password;
    int randomNum;
    char answer[101];
    struct validate *next;

}UserValidate;


UserValidate *head_validate;


typedef struct profile  // mallocated
{
    char username[51];
    char status[513];
    struct profile** friends; // mallocated
    int friends_count;
    struct profile** requests; // mallocated
    int requests_count;
    struct profile* next;
}Profile;


Profile *head_profile;


#endif // CORE_H_INCLUDED
