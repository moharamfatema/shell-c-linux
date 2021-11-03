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
pid_t childpid;

void handler(int sig){
	if(childpid >0)
		fprintf(fptr,"Process %d terminated\n",childpid);
	
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


	/*extracting command from user*/
	while(1){
		setbuf(stdout,NULL);
		printf("SHELL>> ");
		scanf("%[^\n]s",command);
		printf("command : %s\n",command);
		setbuf(stdin,NULL);
		if((strcmp(command,"exit")==0) || (strcmp(command,"exit &")==0))
			return 0;
		
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
		printf("will wait? %d\n",willWait);
		if (!willWait){
			args[noofargs-1] = NULL;
		}
		/*new clean arr of args*/
		/*char newargs[noofargs+1][ARGLEN];
		int k = 0;
		for(k = 0;k<((willWait)?noofargs-1:noofargs-2);k++){
			strcpy(newargs[k],args[k+1]);	
			strcat(newargs[k],"\0");	
			//printf("%d\t%s\n",k,newargs[k]);
			if(k>MAXARGS)
				break;
		}//*newargs[k++]=NULL;*/
		
		/*printing args[]*/
		printf("Arguments are:\n");
		for (int i = 0; i<noofargs;i++){
			if(args[i])
				printf("%d\t%s\n",i,args[i]);
			else
				break;
		}
		
		
		//TODO:working except execvp			
		/*executing command*/
		int childid = fork();

		printf("childid = %d\n",childid);
		if(childid == 0){
			/*child code*/
			
			printf("I am a child\n");

			//char* tmp[] = {"ls",NULL};
			execvp(args[0],&args[0]);

			return 0;
		}else if(childid < 0){
			perror("fork unsuccessfull\n");
		}else{
			/*parent code*/
			childpid = childid;

			printf("I am the parent\n");
			if(willWait){
				waitpid(childid,&status,0);
			}

		}
		printf(childid>0?"I am the parent and am still alive!!\n":"I should not print\n");		
	}
	fclose(fptr);
	return 0;
}
