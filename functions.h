#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

char* get_txt();
int is_validName(char *name);
int is_validAnswer(char *ans);
char* getPassword();
int is_validPass(char *pass);
int encrypt_pass(char *pass, int randNum);
void print_encrypt(FILE *fp, int pass);
void txtFromFile(FILE* fp, char *dest, char ender);
int binary2int(char *str);
char* to_lower_string(char *str);
int is_partof(char *str, char *part);
void skip_line(FILE *fp);
struct validate* name_in_use(char *name);
int endsWithS(char *str);


#endif // FUNCTIONS_H_INCLUDED
