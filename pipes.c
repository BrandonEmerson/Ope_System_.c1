#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int main(){
	pid_t pid;
	int p1[2]={0,1};
	int p2[2]={0,1};
	char cadena[TAM];
	pipe(p1);
	pipe(p2);
	pid=fork();
	
	switch(pid){
		case -1;
	     	printf("Error \n");
	    break;
	    case 0;
	         printf("Proceso hijo pid: %d\n", getpid());
	         close(p1[1]);
	         close(p2[0]);
	         strcpy(cadena, "esto llega a traves de la tuberia\n");
	         write(p2[1],cadena, strlen(cadena)));
	         read(p1[0],cadena,TAM);
	         printf
	         close
		
	}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
