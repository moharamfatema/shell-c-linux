#define _GNU_SOURCE
#include <features.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#define MAXLEN 4096
#define ARGLEN 128
#define MAXARGS 32

FILE * fptr ;

void handler(int sig){
	fprintf(fptr,"Child process terminated\n");
}

int main(){
	
	 
	/*create log file*/
	fptr = fopen("logs.txt","w");

	/*child signal handler*/
  	signal(SIGCHLD,handler);

	/*initializing vars*/
	char command[MAXLEN];
	char * token;char * rest;
	char* args[MAXARGS];
	int noofargs = 0;
	int status;
	int childid;

	/*extracting command from user*/
	while(1){
		setbuf(stdout,NULL);
		printf("SHELL>> ");
		scanf("%[^\n]s",command);
		setbuf(stdin,NULL);
		if((strcmp(command,"exit")==0) || (strcmp(command,"exit &")==0))
			exit(0);
		
		rest = command;
		noofargs = 0;
		while(token = strtok_r(rest," ",&rest)){
			//add token to args
			//printf("%d %s\n",noofargs,token);
			args[noofargs] = token;
			noofargs++;
		}
		args[noofargs]=NULL;
					
		/*wait flag*/
		int8_t willWait=(strcmp(args[noofargs-1],"&")==0)?0:1;
		//printf("will wait? %d\n",willWait);
		if (!willWait){
			args[noofargs-1] = NULL;
		}else{
			//printf("----------WAITING FOR CHILD------------\n");
		}
		
		/*printing args[]*//*
		printf("Arguments are:\n");
		for (int i = 0; i<noofargs;i++){
			if(args[i])
				printf("%d\t%s\n",i,args[i]);
			else
				break;
		}*/
		
		/*executing command*/
		childid = fork();

		//printf("childid = %d\n",childid);
		if(childid == 0){
			/*child code*/
			//printf("----------------CHILD PROCESS--------------\n");
			int8_t flag = execvp(args[0],&args[0]);
			//INCASE OF ERROR
			if (flag)
			{
				perror("Execution unsuccessful ");
				return 0;
			}
			
		}else if(childid < 0){
			perror("Fork unsuccessfull");
		}else{
			/*parent code*/
			if(willWait){
				waitpid(childid,&status,0);
			}

		}
		//printf(childid>0?((willWait)?"-----------------BACK TO PARENT----------------\n":"\n"):"\n");		
	}
	fclose(fptr);
	return 0;
}
