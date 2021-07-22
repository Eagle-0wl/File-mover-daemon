extern void read_file(char audio_types[50][30], char video_types[50][30], char photo_types[50][30], 
                        char document_types[50][30], char directory[1][30],char type_to_watch[4][30]);
extern void recursive_search (const char * dir_name,  char audio_types[50][30], char video_types[50][30], 
                        char photo_types[50][30], char document_types[50][30],char type_to_watch[4][30]);
extern void types(char *temp_string,char type[50][30],char *type_name);
void print_type_extensions(char type[50][30]);
extern void move_file (const char *d_name, char *file_type, char audio_types[50][30]);
extern void log_writer();
