/*
EJercicio 19/Sep/16
Ejemplo de tuberia sin nombre con procesos
*/

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

/*mACRO PARA INDICAR EL TAMANO DE  LA CADENA*/
#define TAM 512 /* 512 para que no se desborde por error de la tubería*/



int main(int argc, char const *argv[])
{ 	
	/*Hacemos el arreglo para 2 tuberias*/
	/*
	con 0 Escribimos
	con 1 Leemos
	*/
	int p1[2]={0,1};
	int p2[2]={0,1};

	/*Arreglo de la cade a desplegar*/
	char cadena [TAM]="yo si voy a resolverlo";
	char cadena1 [TAM]= "JA JA NO TE MINETAS";
	char cadena2 [TAM]="no vas a pasar";
	char cadena3 [TAM]= "claro que si paso";

	/*Iniciamos con PIPE sin nombre*/
	pipe(p1); /*Flujo de lado izquerdo derecho padre*/
	pipe(p2); /*FLujo de lado derecho  izquierdo hijo*/


	/*COmenzamos creando la estructura de los*/
	pid_t pid;
	pid=fork();


	if (pid==-1)
	{
		/* ERROR */
		printf("EROOR AL CREAR PROCESO\n");
	}
	/*Preguntamos por el padre y por el hijo*/
	if (pid==0)
	{
		/* Hijo  */
		printf("	Hijo con pid=[ %d ] Con Padre pid=[ %d ]\n",getpid(), getppid());
		/*tUBERIA HIJO*/
		close ( p2[0]); /* cerramos el lado de lectura del pipe 1*/ 
		close ( p1[1]); /* cerramos el lado de escritura del pipe   2*/ 
		 strcpy( cadena, "Esto llega a traves de la tuberia del hijo" );
		 printf("\n");
    	 write( p2[1], cadena, strlen( cadena ) );
		 read( p1[0], cadena, TAM) ;
    	 printf("	Hijo recibio del Padre = %s \n",cadena );
    	 printf("\n");
    	 close( p1[0] );/*Cerramos la lectura*/


	}
	else{
		/*Padre*/
		printf("Soy padre pid [%d]\n",getpid());
		/*Logica de tuberia*/
		close( p1[0] ); /*Padre to Hijo cerramos el lado de lectura del pipe 1 */
		close( p2[1] ); /* cerramos el lado de escritura del pipe 2 */
		read( p2[0], cadena, TAM );
		printf("Padre recibio del Hijo = %s\n",cadena );
		strcpy( cadena, "HOLA MUNDO\n" );
		write(p1[1], cadena, TAM); 	    
 	    close( p2[0] );



	}



	return 0;
}
