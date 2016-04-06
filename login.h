#ifndef LOGIN_H_INCLUDED
#define LOGIN_H_INCLUDED

void login_menu();
void reset_password_menu(struct validate *user);
int forgot_password_menu(struct validate *user);
int wrong_password_menu(struct validate *user, char *str);

char* getLoginInput();
char* name_seperator(char *input);
char* pass_seperator(char* input);

#endif // LOGIN_H_INCLUDED
