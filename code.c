#include <stdio.h>
#include <lib.h>
#include <string.h>
int main(void) {

   char audio_types[50][30];               //50 audio types of 30 char lenght
   char video_types[50][30];               //50 video types of 30 char lenght
   char photo_types[50][30];               //50 photo types of 30 char lenght
   char document_types[50][30];            //50 document types of 30 char lenght
   char directory[1][30];                  //directory
   char type_to_watch[4][30];                

   memset(audio_types, 0, sizeof(audio_types));
   memset(video_types, 0, sizeof(video_types));
   memset(photo_types, 0, sizeof(photo_types));
   memset(document_types, 0, sizeof(document_types));
   memset(directory, 0, sizeof(directory));
   memset(type_to_watch, 0, sizeof(type_to_watch));
   
   log_writer("Log file created/opened");
   read_file(audio_types,video_types,photo_types,document_types,directory,type_to_watch);

   //while(1){
   //sleep(30);
   
   recursive_search(directory[0],audio_types,video_types,photo_types,document_types,type_to_watch);

   //}
   return 0;
}
