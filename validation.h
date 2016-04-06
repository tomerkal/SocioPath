#ifndef NEWACCOUNT_H_INCLUDED
#define NEWACCOUNT_H_INCLUDED


void create_account_menu();
struct validate* create_validation(char *name, int password, int random, char *ans);
void read_validationLine(FILE *fp, char *name, char *pass, char *randstr, char *ans);
void free_validation();


#endif // NEWACCOUNT_H_INCLUDED
