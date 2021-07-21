#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern void read_file()
{
    char *filename = "Config.txt";
    FILE *file;    
    char s;                 //variable used to get char from file
    file=fopen(filename,"r");               //opening file for read operation

    while((s=fgetc(file))!=EOF)         //loop reads file character by character till end
    {
        if (s!=' '){
           printf ("%c",s);
            
        }
        
    }   
    fclose(file);                           //close file
}
