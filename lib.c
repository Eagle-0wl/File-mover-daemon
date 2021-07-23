#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
extern void log_writer()
{

}
extern void move_file (const char *d_name, char *file_type, char types[50][30],const char *file_location)
{
    char add_dot[30]=".";
    char old_location[4096];        //max path length for linux is 4096 characters
    char new_location[527];         //for new location by specifications max path length can be 527 characters
    
    for (int i=0;i<50; i++)
    {
        memset(old_location, 0, sizeof(old_location));
        memset(new_location,0 ,sizeof(new_location));

        if(strlen(types[i]) != 0){
            strncat(add_dot, types[i], strlen (types[i]));
            
            if (strstr(d_name,add_dot))
            {   
                strcpy(old_location,file_location);
                strcat(old_location,"/");
                strcat(old_location,d_name);
                strcpy(new_location,"/home/");
                strcat(new_location,getenv("LOGNAME"));
                strcat(new_location,"/"); 
                strcat(new_location,file_type);
                DIR *d;
    
                // Open the directory specified by "dir_name".
                d = opendir (new_location);

                // Check if it was opened. 
                if (! d) 
                {
                    printf ("Cannot open directory");
                    exit (EXIT_FAILURE);
                }   
                closedir (d);

                strcat(new_location,"/");
                strcat(new_location,d_name); 

                if( access( new_location, F_OK ) == 0 ) 
                {
                    printf ("File already exist abort");
                }
                else{

                    if(rename(old_location,new_location))
                    {
                        printf("Move failed"); 
                    }
                    else 
                    {
                    //printf("Move successful");
                    }
                }

                                        
            }
            memset(add_dot, 0, sizeof(add_dot));
            strcpy(add_dot,".");
        }
    }
}

extern void recursive_search (const char * dir_name,  char audio_types[50][30], char video_types[50][30], 
                        char photo_types[50][30], char document_types[50][30],char type_to_watch[4][30])
{
    DIR * d;
    
    // Open the directory specified by "dir_name".
    d = opendir (dir_name);

    // Check if it was opened. 
    if (! d) 
    {
        printf ("Cannot open directory");
        exit (EXIT_FAILURE);
    }
    while (1) 
    {
        struct dirent * entry;
        const char * d_name;

        // "Readdir" gets subsequent entries from "d". 
        entry = readdir (d);
        if (! entry) 
        {
            //There are no more entries in this directory, so break out of the while loop. 
            break;
        }

        d_name = entry->d_name;

        move_file(d_name,"Music",audio_types,dir_name);
        move_file(d_name,"Videos",video_types,dir_name);
        move_file(d_name,"Pictures",photo_types,dir_name);
        move_file(d_name,"Documents",document_types,dir_name);
   
        if (entry->d_type != DT_DIR) 
        {   
            char filedir[]="";
            strcat(filedir, dir_name);
            strcat(filedir, "/");
            strcat(filedir, d_name);
	    }
   
        if (entry->d_type & DT_DIR) 
        {
            // Check that the directory is not "d" or d's parent.     
            if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) 
            {
                int path_length;
                char path[PATH_MAX];
    
                path_length = snprintf (path, PATH_MAX,"%s/%s", dir_name, d_name);
                if (path_length >= PATH_MAX) 
                {
                    printf ("Path length has got too long.\n");
                    exit (EXIT_FAILURE);
                }
                // Recursively call "list_dir" with the new path. 
                //list_dir (path);
                recursive_search (path,audio_types,video_types,photo_types,document_types,type_to_watch);
            }
	    }
    }   
    //After going through all the entries, close the directory. 
    if (closedir (d)) 
    {
        printf ("Could not close");
        exit (EXIT_FAILURE);
    }
}

extern void types(char *temp_string,char type[50][30],char *type_name){
char ch;
int type_calc=0;
for (int i=strlen(type_name)+1; i<=strlen(temp_string);i++){
    
    if (temp_string[i]!=',')
    {
        ch=temp_string[i];
        strncat(type[type_calc], &ch, 1);
    }

    else if (temp_string[i]==',' ||temp_string[i]=='\n')
    {    
        type_calc++;   
    }
    }
    type_calc=0;
}
extern void read_file(char audio_types[50][30], char video_types[50][30], char photo_types[50][30], 
                        char document_types[50][30], char directory[1][30],char type_to_watch[4][30])
{
    char *filename = "Config.txt";
    FILE *file;    
    char s;                                 //variable used to get char from file
    file=fopen(filename,"r");               //opening file for read operation
    if (file == NULL) 
        {   
            printf("Error! Could not open file\n"); 
            exit(-1);
        } 
    char *video="video_types";
    char *audio="audio_types";
    char *photo="photo_types";
    char *document="document_types";
    char *dirname="dir_to_watch";
    char *types_name="types_to_watch";

    char temp_string[50];
    memset(temp_string, 0, sizeof temp_string);

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
                types(temp_string,type_to_watch,types_name); 
            }

            memset(temp_string, 0, sizeof temp_string);
        }
        
    }
            if (strlen(type_to_watch[0]) == 0 && strlen(directory[0]) == 0)
            {
                //printf ("Somethings wrong with config file!\n");
                fclose(file);                           //close file
                exit(-1);
            }
             else 
            {
                //printf ("Config file read correctly!\n");
            } 
    fclose(file);
}