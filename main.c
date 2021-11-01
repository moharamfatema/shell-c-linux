#define _GNU_SOURCE
#include <features.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAXLEN 100
#define ARGLEN 20
#define MAXARGS 10

int main(){
	/*extracting command from user*/
	//setbuf(STDOUT,NULL);
	printf(">> ");
	char command[MAXLEN];
	scanf("%[^\n]s",command);
	printf("command : %s\n",command);
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
		execvp(args[0],newargs);
		printf("done with exec\n");
	}else if(childid < 0){
		perror("fork unsuccessfull\n");
	}else{
		/*parent code*/
		printf("I am the parent\n");
		if(willWait){
			waitpid(childid,&status,0);
		}
	}
	printf("I am still alive!!\n");		
	return 0;
}
