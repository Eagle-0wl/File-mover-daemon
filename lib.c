#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

extern void log_writer(char *text)
{
    FILE *file;
    char *file_name="/home/darius/Desktop/File_mover_daemon/log_file.txt";
    if ((file = fopen(file_name, "a")))
    {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(file,"%d-%02d-%02d %02d:%02d:%02d %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, text); //Time stamp with every line of logs
        fclose(file);
    }
    else
    {
        printf("Error! Failed to open log file");
        exit(EXIT_FAILURE);
    }

}

void rek_mkdir(char *path) {
    char *sep = strrchr(path, '/');
    if(sep != NULL) {                       //if previous folder exist create new folder
        *sep = 0;
        rek_mkdir(path);
        *sep = '/';
    }
    if(!mkdir(path, 0777))                  //if folder created log to file
    {   
        char string[4110]="Path created: "; 
        strcat(string, path);
        log_writer(string);
    }
}

FILE *fopen_mkdir(char *path, char *mode) {
    char *sep = strrchr(path, '/');          //gets last folder name
    if(sep) {                                //checks if it go name correctly
        char *path0 = strdup(path);          //pointer to a new string
        path0[ sep - path ] = 0;             
        rek_mkdir(path0);                    //sends path to recursive directory making function   
        free(path0);                         //freeing pointer
    }
    return fopen(path,mode);
}

extern void move_file (const char *d_name, char *file_type, char types[50][4096],const char *file_location)
{
    
    char add_dot[4096]=".";
    char old_location[4096];                //max path length for linux is 4096 characters
    char new_location[527];                 //for new location by specifications max path length can be 527 characters
    
    for (int i=0;i<50; i++)
    {
        /* initialize arrays as NULL */
        memset(old_location, 0, sizeof(old_location));    
        memset(new_location,0 ,sizeof(new_location));

        if(strlen(types[i]) != 0)           //checks if extentions for that file type are awailable
        { 
            strncat(add_dot, types[i], strlen (types[i]));      //add dot before extension, so program only checks for file extensions
            
            if (strstr(d_name,add_dot))
            {                                                   /*creating path where to move the file*/
                strcpy(old_location,file_location);
                strcat(old_location,"/");
                strcat(old_location,d_name);
                strcpy(new_location,"/home/");
                strcat(new_location,getenv("LOGNAME"));         //getting user name
                strcat(new_location,"/"); 
                strcat(new_location,file_type);                 //setting location based on file type


                DIR *d;
                d = opendir (new_location);                     //opening directory
                
                if (! d)                                        // Check if it was opened. 
                {
                    log_writer("Error! Cannot open directory");

                    if (fopen_mkdir(new_location,"w")){         //try to create directory recursivly
                        log_writer("Error! Failed to created path");   
                        exit(EXIT_FAILURE);                               
                    }
                }   
                closedir (d);                                   //clode directory
                                                                /*add file name to path*/
                strcat(new_location,"/");                       
                strcat(new_location,d_name); 

                if( access( new_location, F_OK ) == 0 )         //check if there is a file in a directory
                {
                    log_writer("Error! File already exist abort");
                }
                else{

                    if(rename(old_location,new_location))           //try to move file
                    {
                        log_writer("Error! Move failed"); 
                    }
                    else                                            //log to file successful move
                    { 
                        char string[4895]="Moved: ";    // old_location+new_location+text+file name = 4895 characters max
                        strcat(string, d_name);
                        strcat(string, " from ");
                        strcat(string, old_location);
                        strcat(string, " to ");
                        strcat(string, new_location);
                        log_writer(string);
                        memset(string,0,sizeof(string)); 
                    }
                }                                 
            }
            memset(add_dot, 0, sizeof(add_dot));//initializing array to null
            strcpy(add_dot,".");
        }
    }
}

extern void recursive_search (const char * dir_name,  char audio_types[50][4096], char video_types[50][4096], 
                        char photo_types[50][4096], char document_types[50][4096],char type_to_watch[4][4096])
{
    DIR * d;
    // Open the directory specified by "dir_name".
    d = opendir (dir_name);

    // Check if it was opened. 
    if (! d) 
    {
        log_writer("Error! Cannot open directory");
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
        /* calling move function to move file */
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
                    log_writer ("Path length has got too long.");
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
        log_writer ("Error!  Failed to close");
        exit (EXIT_FAILURE);
    }
}

extern void types(char *temp_string,char type[50][4096],char *type_name)
{
    char ch;
    int type_calc=0;
    for (int i=strlen(type_name)+1; i<=strlen(temp_string);i++)         //starts at end of type_name+1 goes through the whole line symbol by symbol 
    {        
    
        if (temp_string[i]!=',')                                        //if comma found 
        {
            ch=temp_string[i];                                          //get char from array
            strncat(type[type_calc], &ch, 1);                           //save that chat to type array
        }

        else if (temp_string[i]==',' ||temp_string[i]=='\n')            //if another comma found or end of line reached increment the counter
        {    
            type_calc++;                                                //incriment counter
        }
    }
    type_calc=0;
}

extern void read_file(char audio_types[50][4096], char video_types[50][4096], char photo_types[50][4096], 
                        char document_types[50][4096], char directory[50][4096],char type_to_watch[4][4096])
{
    char *filename = "/home/darius/Desktop/File_mover_daemon/Config.txt";   //config file location
    FILE *file;    
    char s;                                 //variable used to get char from file
    file=fopen(filename,"r");               //opening file for read operation
    if (file == NULL)                       //check if file opend
        {   
            log_writer("Error! Could not open file"); 
            exit(EXIT_FAILURE);
        } 
    /*creating char arrays with types*/
    char *video="video_types";
    char *audio="audio_types";
    char *photo="photo_types";
    char *document="document_types";
    char *dirname="dir_to_watch";
    char *types_name="types_to_watch";

    //creating and initializing temporary string used for reading file
    char temp_string[4096];
    memset(temp_string, 0, sizeof temp_string);
    
    while((s=fgetc(file))!=EOF)         //loop reads file character by character till end of file
    {
        if (s != '\n')                  //check if its not end of file
        {
            if (s != ' ' && s != '\n')  //read every character except spaces and end of line characters
            {
               strncat(temp_string, &s, 1);
            }
        }
        else                            //if file end reached
        {   
            //checks for file type and calls function which assings file extensions to that file type 
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

            memset(temp_string, 0, sizeof temp_string);         //clearing temporary string*/ 
        }
        
    }
            if (strlen(type_to_watch[0]) == 0 && strlen(directory[0]) == 0)     //checks if config file read
            {
                log_writer ("Somethings wrong with config file!");  
                fclose(file);                           //close file
                exit(EXIT_FAILURE);
            }
             else 
            {
                log_writer("Config file read correctly!");
            }
    fclose(file);
}