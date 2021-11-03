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
#define MAXLEN 100
#define ARGLEN 20
#define MAXARGS 10

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
  	signal(SIGCHLD, handler);


	/*extracting command from user*/
	while(1){
		//setbuf(STDOUT,NULL);
		printf(">> ");
		char command[MAXLEN];
		scanf("%[^\n]s",command);
		printf("command : %s\n",command);
		setbuf(stdin,NULL);
		if((strcmp(command,"exit")==0) || (strcmp(command,"exit &")==0))
			return 0;
		
		char args[MAXARGS][ARGLEN];
		char *rest = command;

		//char * token = strtok(command," ");
		char * token;
		int noofargs = 0;
		while(token = strtok_r(rest," ",&rest)){
			//add token to args
			//printf("%d %s\n",noofargs,token);
			strcpy(args[noofargs],token);
			//strcat(args[noofargs],"\0");
			noofargs++;
		}
		//*args[noofargs]=NULL;
		

			
		/*wait flag*/
		int8_t willWait=(strcmp(args[noofargs-1],"&")==0)?0:1;
		printf("will wait? %d\n",willWait);
		
		/*new clean arr of args*/
		char newargs[noofargs+1][ARGLEN];
		int k = 0;
		for(k = 0;k<((willWait)?noofargs-1:noofargs-2);k++){
			strcpy(newargs[k],args[k+1]);	
			strcat(newargs[k],"\0");	
			//printf("%d\t%s\n",k,newargs[k]);
			if(k>MAXARGS)
				break;
		}//*newargs[k++]=NULL;
		
		/*printing args[]*/
		printf("Arguments are:\n");
		for (int i = 0; i<k;i++){
			if(newargs[i])
				printf("%d\t%s\n",i,newargs[i]);
			else
				break;
		}
		
		
		//TODO:working except execvp			
		/*executing command*/
		int childid = fork();

		int status;
		printf("childid = %d\n",childid);
		if(childid == 0){
			/*child code*/
			
			printf("I am a child\n");
/*			char *tmparr[2] = malloc(2*sizeof(char));
			if (tmparr == NULL)
			{
				perror;
				return 0;
*/			
			
/*			strcpy(tmparr[0], "ls");
			strcpy(tmparr[1], '\0');
*/
			char* tmp[] = {"ls",NULL};
;			execvp(tmp[0],tmp);
			
			/*
			printf("done with exec\n");
			*/
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
