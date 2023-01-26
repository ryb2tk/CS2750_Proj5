// Renee Brandt
// Project 5: Create a child process using fork. Each process will be responsible for the following:
   // Child process takes numbers from command line arguments and put them into a dynamic array of a large enough size for those numbers. Write these numbers to a file including its PID. Then check to see if the sum of any of the numbers is 19.
   // Parent process will wait for child process to end, then return its PID along with the status of the child process (EXIT_SUCCESS or EXIT_FAILURE).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void help() {
	printf("This program forks a child process that will take in integer arguments. It will output any pairs that sum up to 19 to a file named 'proj5.txt'.\n");
	printf("The parent process will check the status code of the child process (EXIT_SUCCESS if pairs found or EXIT_FAILURE if no pairs found), and write that to the file.\n");
}

int main (int argc, char ** argv) {
char* fileName = "proj5.txt";
FILE * filePtr;
//Create help option using getopt
int option;
while ((option = getopt(argc, argv, "h")) != -1) {
   switch(option){
      case 'h':
         help();
         exit(EXIT_SUCCESS);
      case '?':
         if (isprint (optopt))
            fprintf(stderr, "Unknown option '-%c'.\n", optopt);
         else
            fprintf (stderr, "Unknown option character '\\x%x'.\n", optopt);
         return 1;
      default: //currently does not print when running
         help();
         exit(EXIT_FAILURE);
	}
}

//Fork child process
pid_t childPid = fork();

if (childPid == 0) { //child process
   int * arglist = malloc(argc * sizeof(int));
   int i,j;
   if (arglist == NULL) { //malloc has failed
      printf("Malloc has failed.");
      return (EXIT_FAILURE);
   } else {
      for (i=0; i < argc-1; i++) { //create dynamic array of given integer arguments
         arglist[i]=atoi(argv[i+1]); //skip process name argv[0]
      }
   }
   //open file given for writing and write the numbers to the file
   filePtr = fopen(fileName, "w");
   fprintf(filePtr, "Child %d: ", getpid());
   for (i = 0; i < argc-1; i++) {
      fprintf(filePtr, "%d ", arglist[i]); //print original array in file
   }
   int noPairs = 1; //check if any pairs are found
   fprintf(filePtr, "\nChild %d: ", getpid()); //new line before math portion
   for (i = 0; i < argc-2; i++) { //nested loops
      for (j = 1; j < argc - 1; j++) {
         if (arglist[i] + arglist[j] == 19) {
            fprintf(filePtr, "Pair: %d %d ", arglist[i], arglist[j]);
            noPairs = 0;
         }
      }
   }
fclose(filePtr); //close file
free(arglist); //free dynamically allocated memory
   if (noPairs == 1) { //if no pairs found, change exit status
      exit(1);
   }
} else { //parent process
   int status = 0;
   char * statusString;
   wait(&status);
   if (status == 0) {
      statusString = "EXIT_SUCCESS";
   } else {
      statusString = "EXIT_FAILURE";
   }
   filePtr = fopen(fileName, "a"); //open to append
   fprintf(filePtr, "\nParent %d: %s\n", getpid(), statusString);
   fclose(filePtr);
   printf("Project is done running! Check file 'proj5.txt' for results.\n");
}
return (EXIT_SUCCESS);
}
