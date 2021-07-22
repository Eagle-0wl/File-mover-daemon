#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern void types(char *temp_string,char type[50][30],char *type_name){
char ch;
int type_calc=0;
for (int i=strlen(type_name)+1; i<=strlen(temp_string);i++){
    
    if (temp_string[i]!=',')
    {
        ch=temp_string[i];
        strncat(type[type_calc], &ch, 1);
    }

    /*else*/ if (temp_string[i]==',' ||temp_string[i]=='\n')
    {    
        type_calc++;   
    }
    }
    type_calc=0;
}

void print_type_extensions(char type[50][30]){
      for (int i=0;i<50; i++)
    {
        if(strlen(type[i]) != 0)
        {
            printf("%s\n", type[i]);
        }
                    
    }
}

extern void read_file()
{
    char *filename = "Config.txt";
    FILE *file;    
    char s;                 //variable used to get char from file
    file=fopen(filename,"r");               //opening file for read operation

    char *video="video_types";
    char *audio="audio_types";
    char *photo="photo_types";
    char *document="document_types";
    char *dirname="dir_to_watch";
    char *types_name="types_to_watch";

    char audio_types[50][30];               //50 audio types, with 30 char lenght
    char video_types[50][30]; 
    char photo_types[50][30]; 
    char document_types[50][30]; 
    char directory[1][30];
    char type_to_watch[4][30]; 


    memset(audio_types, 0, sizeof(audio_types));
    memset(video_types, 0, sizeof(video_types));
    memset(photo_types, 0, sizeof(photo_types));
    memset(document_types, 0, sizeof(document_types));
    memset(directory, 0, sizeof(directory));
    memset(type_to_watch, 0, sizeof(type_to_watch));

    char temp_string[50];

    while((s=fgetc(file))!=EOF)         //loop reads file character by character till end
    {
        if (s != '\n')
        {
            if (s != ' ' && s != '\n')
            {
                strncat(temp_string, &s, 1);
            }
        }
        else 
        {
            if (strstr(temp_string,audio))
            {
                types (temp_string,audio_types,audio);
            }
            else if (strstr(temp_string,video))
            {
                types (temp_string,video_types,video);
            }
            else if (strstr(temp_string,photo))
            {
                types (temp_string,photo_types,photo);
            }
            else if (strstr(temp_string,document))
            {
                types (temp_string,document_types,document);
            }
            else if (strstr(temp_string,dirname))
            {
                types (temp_string,directory,dirname);
            }
            else if (strstr(temp_string,types_name))
            {
                types (temp_string,type_to_watch,types_name);
            }

            memset(temp_string, 0, sizeof temp_string);
        }
        
    }
    print_type_extensions(audio_types);
    print_type_extensions(video_types);
    print_type_extensions(photo_types);
    print_type_extensions(document_types);
    print_type_extensions(directory);
    print_type_extensions(type_to_watch);
    //printf ("%s", directory[1]);
  
    fclose(file);                           //close file
}