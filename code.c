#include <stdio.h>
#include <lib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
int main(void) {

   pid_t pid, sid;                // Our process ID and Session ID
   
   pid = fork();                  //Fork off the parent process
   if (pid < 0) 
   {
      exit(EXIT_FAILURE);
   }
   if (pid > 0) 
   {                              //If we got a good PID, then we can exit the parent process.
      exit(EXIT_SUCCESS);
   }

   umask(0);                      //Change the file mode mask
                
   log_writer("Log file created/opened");     
                
   
   sid = setsid();               // Create a new SID for the child process
   if (sid < 0) 
   {
      log_writer("Failedto create new SID");
      exit(EXIT_FAILURE);
   }
         
   
   if ((chdir("/")) < 0)         // Change the current working directory
   {
      log_writer("Failed to change working directory to root");
      exit(EXIT_FAILURE);
   }
        
   //Close out the standard file descriptors 
   close(STDIN_FILENO);
   close(STDOUT_FILENO);
   close(STDERR_FILENO);
   
   /* create two dimensional string arrays for storing data from config file */     
   char audio_types[50][4096];               //50 audio types of 4096 char lenght
   char video_types[50][4096];               //50 video types of 4096 char lenght
   char photo_types[50][4096];               //50 photo types of 4096 char lenght
   char document_types[50][4096];            //50 document types of 4096 char lenght
   char directory[50][4096];                  //directory
   char type_to_watch[4][4096];                

   char audio_loc[1][4096];
   char video_loc[1][4096];
   char photo_loc[1][4096];
   char document_loc[1][4096];
   /* initialize arrays as NULL*/
   memset(audio_types, 0, sizeof(audio_types));
   memset(video_types, 0, sizeof(video_types));
   memset(photo_types, 0, sizeof(photo_types));
   memset(document_types, 0, sizeof(document_types));
   memset(directory, 0, sizeof(directory));
   memset(type_to_watch, 0, sizeof(type_to_watch));

   memset(audio_loc, 0, sizeof(audio_loc));
   memset(video_loc, 0, sizeof(video_loc));
   memset(photo_loc, 0, sizeof(photo_loc));
   memset(document_loc, 0, sizeof(document_loc));


   
   read_file(audio_types,video_types,photo_types,document_types,directory,type_to_watch, audio_loc, video_loc, photo_loc, document_loc); //read config file
   
   /* infinite while loop to execute code every 30 seconds */
   while(1){

      for (int i=0;i<50;i++)        //goes through all directories
      {
         if(strlen(directory[i]) != 0)
         {
            recursive_search(directory[i],audio_types,video_types,photo_types,document_types,type_to_watch,audio_loc, video_loc ,photo_loc,document_loc);
         }
      }
      log_writer("Cycle complete");
      sleep(30);
   }
   exit(EXIT_SUCCESS);
}
