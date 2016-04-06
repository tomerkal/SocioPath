#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <math.h>

#include "functions.h"
#include "core.h"

char* get_txt()
{
    /** Reads input from user, and returns a pointer to start of read buffer **/

   int i = 0, capacity = 10;
   char c, *buffer = (char*) malloc(capacity * sizeof(char));

   if (buffer == NULL)
      return NULL;

   for (c = getchar(); c != '\n'; c = getchar())
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
   }
   buffer[i] = '\0';

   return buffer;
}


int is_validName(char *name)
{
    /** A "boolean" function that checks if a name is valid by the definitions
        of the exercise and returns 1 if it is, 0 otherwise                     **/
    int i=0;

    if (name[0] == '\0')
        return 0;
    while (name[i] != '\0')
    {
        if (!isalpha(name[i]) && !(name[i]==' '))
            return 0;
        i++;
    }
    return 1;
}

int is_validAnswer(char *ans)
{
    /** A "boolean" function that checks if an answer is valid by the definitions
        of the exercise and returns 1 if it is, 0 otherwise                     **/

    int i=0;

    if (ans[0] == '\0')
        return 0;
    if (strlen(ans) > 100)
        return 0;
    while (ans[i] != '\0')
    {
        if (!isalnum(ans[i]) && !(ans[i]==' '))
            return 0;
        i++;
    }
    return 1;
}


char* getPassword()
{
    /** Reads an input from the user and prints * instead of the characters
        returns a pointer to the start of read buffer                       **/

    int i = 0, capacity=10;
    char c, *buffer = (char*) malloc(capacity * sizeof(char));

    do {
        c = getch();

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
            printf("%c", '*');
        }
        else if( c == 8 && i )
        {
            buffer[i] = '\0';
            i--;
            printf("%s", "\b \b");
        }
    } while( c != 13 );

    buffer[i] = '\0';
    return buffer;

}

int is_validPass(char *pass)
{
    /** A "boolean" function that checks if a password is valid by the definitions
        of the exercise and returns 1 if it is, 0 otherwise                     **/

    int i=0, dig=0, low=0, up=0;

    if (strlen(pass) != 8)
        return 0;

    for (i=0 ; i<8 ; i++)
    {
        if (isdigit(pass[i]))
            dig++;
        if (islower(pass[i]))
            low++;
        if (isupper(pass[i]))
            up++;
    }

    if (dig>=1 && low>=1 && up>=1)
        return 1;
    return 0;

}

int encrypt_pass(char *pass, int randNum)
{
    /** encrypts an 8 digit password with the random number
        Returns an int that saves the 32-bit encryption     **/

    int i=0;
    unsigned char c=0xff;

    for (i=0 ; i<8 ; i++)
    {
        if (pass[i] == '\0')
            return -1;
        c &= pass[i];
    }

    if ((c&0x1) == 0)
        c = c<<4;
    else
        c = c>>6;

    return c^randNum;
}

void print_encrypt(FILE *fp, int pass)
{
    /** prints the encrypted int to a 32 digit binary number **/

    int i=0;
	unsigned int temp=0;
	unsigned int mask = 0x80000000;

	for (i=0 ; i<32 ; i++)
	{
		temp = mask&pass;           // we use a mask on the most left digit
		temp >>= (31-i);            // we move to the right to print a 0 or a 1
		fprintf(fp, "%d", temp);
		mask >>= 1;                 // and move the msak to the next number
	}
}


void txtFromFile(FILE* fp, char *dest, char ender)
{
    /** Reads a text from file int specific destination
        Gets a specific ender, to know when to stop. and always stops at end of file **/

    int i=0;
    char c;

    while ((c=getc(fp))!= ender && c != EOF)
    {
        dest[i] = c;
        i++;
    }
    dest[i] = '\0';
}


int binary2int(char *str)
{
    /** Turns a binary 32-bit string into an int (reads file for encrypted password) **/

    int i=0, res=0;

    for (i=0 ; i<32 ; i++)
        res += (str[i]-'0')*(int)pow(2, 31-i);

    return res;
}


char* to_lower_string(char *str)
{
    /** Turns a string to all-lowercase letters (for search)
        Returns a pointer to start of buffer
        !!! MALLOCATED !!!                                  **/

    int i=0;
    char *lower = (char*)malloc(strlen(str)+1);

    while (str[i] != '\0')
    {
        lower[i++] = tolower(str[i]);
    }
    lower[i] = '\0';

    return lower;
}

int is_partof(char *str, char *part)
{
    /** Returns 1 if 'part' is part of 'str', 0 otherwise **/

    unsigned int i=0;

    for (i=0 ; i<=(strlen(str)-strlen(part)) ; i++)
    {
        if (!strncmp(str+i, part, strlen(part)))
            return 1;
    }
    return 0;
}


void skip_line(FILE *fp)
{
    /** Skips a whole line when reading a file **/

    char c;

    while (c=getc(fp) != '\n');
}

UserValidate* name_in_use(char *name)
{
    /** Looks for a Validate entry by name and returns its pointer, NULL otherwise **/

    UserValidate *temp;

    temp = head_validate;
    while (temp != NULL)
    {
        if (!strcmp(temp->username, name))
            return temp;
        temp = temp->next;
    }
    return NULL;
}


int endsWithS(char *str)
{
    /** Returns 1 if string ends with 's', 0 otherwise **/

    int len=strlen(str);

    if (str[len-1] == 's')
        return 1;
    return 0;
}
