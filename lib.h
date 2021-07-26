extern void log_writer(char *text);

void rek_mkdir(char *path);

FILE *fopen_mkdir(char *path, char *mode);

extern void move_file (const char *d_name, char *file_type, char types[50][4096],const char *file_location);

extern void recursive_search (const char * dir_name,  char audio_types[50][4096], char video_types[50][4096], 
                    char photo_types[50][4096], char document_types[50][4096],char type_to_watch[4][4096]);

extern void types(char *temp_string,char type[50][4096],char *type_name);

extern void read_file(char audio_types[50][4096], char video_types[50][4096], char photo_types[50][4096], 
                        char document_types[50][4096], char directory[50][4096],char type_to_watch[4][4096]);