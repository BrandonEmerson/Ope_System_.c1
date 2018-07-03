#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct p{
	int *filas;
	int nfilas;
}parameters;
float **a, **b, **c;
int lm;
int nProcesos;

void crearMatrices(){
	int i;
	a=(float **) malloc(lm*sizeof(float *));
	for(i=0;i<lm;i++){
		a[i]=(float *) malloc(lm*sizeof(float));
	}
	b=(float **) malloc(lm*sizeof(float *));
	for(i=0;i<lm;i++){
		b[i]=(float *) malloc(lm*sizeof(float));
	}
	c=(float **) malloc(lm*sizeof(float *));
	for(i=0;i<lm;i++){
		c[i]=(float *) malloc(lm*sizeof(float));
	}
}

void incializa(){
	int i,j;
	for(i=0;i<lm;i++){
		for(j=0;j<lm;j++){
			a[i][j]=j+1;
			b[i][j]=j+1;
			c[i][j]=0;
		}
	}
}

void impMatriz(float **mat){
	for (int i=0;i<lm;i++){
		for (int j=0;j<lm;j++){
			printf(" %f ", mat[i][j]);
		}
		printf("\n");
	}
}

void asignarFilas(parameters *par){
	int i,j;
	if(!(lm%nProcesos)){
		for(i=0;i<nProcesos;i++){
			par[i].filas=(int *) malloc((lm/nProcesos)*sizeof(int));
			par[i].nfilas=lm/nProcesos;
			for(j=0;j<(lm/nProcesos);j++){
				par[i].filas[j]=j+((lm/nProcesos)*i);
			}
		}
	}
	else{
		for(i=0;i<nProcesos;i++){
			par[i].filas=(int *) malloc(((lm-(lm%nProcesos))/nProcesos)*sizeof(int));
			par[i].nfilas=((lm-(lm%nProcesos))/nProcesos);
			for (j=0;j<(lm-(lm%nProcesos))/nProcesos;j++){
				par[i].filas[j]=(j+((lm-(lm%nProcesos))/nProcesos*i));
			}
		}
		int uf=(lm-(lm%nProcesos));
		i=0;
		int x;
		while(uf<lm){
			par[i].filas=(int *) realloc(par[i].filas,(((lm-(lm%nProcesos))/nProcesos)*sizeof(int)+sizeof(int)));
			par[i].nfilas++;
			x=j;
			par[i].filas[x]=uf;
			i++;
			uf++;
		}
	}
}

void* multiplicar(void *par){
	parameters *p=(parameters*)par; 
	int i,j,k,f;
	int x=0;
	while(x<(p->nfilas)){
		f=p->filas[x];
		printf("Fila %d\n", f);
		for(i=f;i<(f+1);i++){
	    	for(j=0;j<lm;j++){
				c[i][j]=0;
				for (k=0;k<4;k++){
			  		c[i][j]=c[i][j]+a[i][k]*b[k][j];
				}
	  		}
  		}
  		x++;
	}
	printf("Matrices multiplicadas\n");
}

int main(){
	printf("Longitud de la matriz\n");
	scanf("%d", &lm);
	printf("Numero de procesos\n");
	scanf("%d", &nProcesos);
	if(nProcesos<=lm){
		pid_t pid;
		crearMatrices();
		incializa();
		printf("Matriz a\n");
		impMatriz(a);
		printf("Matriz b\n");
		impMatriz(b);
		parameters param[nProcesos];
		asignarFilas(param);
		for(int i=0;i<nProcesos;i++){
			pid=fork();
			switch(pid){
				case -1:
					printf("Error al crear procesos\n");
				break;
				case 0:
					multiplicar((void *)&param[i]);
					impMatriz(c);
					exit(0);
				break;
				default:
					if (i==(nProcesos-1)){
						int x=0;
						while(x<4){
							wait();
							x++;
						}
					}
				break;
			}
		}
	}
	else{
		printf("El numero de nProcesos debe ser menor o igual a la longitud de la matriz\n");
		exit(0);
	}
	return 0;
}
