#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(){
	
    char line[512];  		//get command line
    char* argv[100];       //user commands and parameters
	 char* option;          //to store FG or BG option
    char* path = "/bin/";    //command location
    char programPath[25];      //full command path

	 while(1){
   	printf("--SHELL--214677322>> ");           //print shell prompt
		fgets(line, 512, stdin); 				//get command and put it in line
   	int length = strlen(line);
	 
		if (line[length - 1] == '\n'){
    		line[length - 1] = '\0';
   	}

   	char *token;                  //split command into separate strings
   	token = strtok(line, " ");
   	option = token;			//keep 'FG' or 'BG' for later checking
   	token = strtok(NULL, " ");
   	int i = 0;
   	for(; token != NULL; i++) {   //filling argv with command and parameters
   		argv[i] = token;      
      	token = strtok(NULL, " ");
      }
   	
    	argv[i] = NULL;                     //set last value to NULL for execvp

    	strcpy(programPath, path);           //add /bin to file path
    	strcat(programPath, argv[0]);            //add command to path

    	for(i = 0; i < strlen(programPath); i++){    //delete newline
      	if(programPath[i] == '\n'){      
         	programPath[i] = '\0';
        	}
    	}
    	
    	if(*option == 'F') {               //for FG processes
    		int pid = fork();              //fork child
    		if(pid == 0){               //Child process
				printf("...working on request...");
				printf("\n...output is ready. Display it now [Y/N]>>");
				char answer = getchar();
				if(answer == 'Y') {
					if(execvp(programPath,argv) < 0){
        				fprintf(stderr, "Child process could not do execute\n");
        			}
				}       		
    		}else{                    //Parent
        		wait(NULL);
    		}
    	}else if(*option == 'B') {    //for BG processes
    		int pid = fork();              //fork child
    		if(pid == 0){               //Child
				printf("...request submitted, returning prompt...\n--SHELL--214677322>> ");
				printf("\n...output for '%s %s %s' is ready. Display it now [Y/N]>> ", line, argv[0], argv[1]);
				char response = getchar();
				if(response == 'Y') {
					if(execvp(programPath,argv) < 0){
        				fprintf(stderr, "Child process could not do execute\n");
        			}
				}       		
    		}else{                    //Parent
        		wait(NULL);
    		}
    	}else {    //Incorrect input
    		printf("Incorrect request. Please enter 'FG' for foreground and 'BG' for background.");
    	}
	} 
}