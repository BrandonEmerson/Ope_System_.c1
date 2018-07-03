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
int nHilos;

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
	if(!(lm%nHilos)){
		for(i=0;i<nHilos;i++){
			par[i].filas=(int *) malloc((lm/nHilos)*sizeof(int));
			par[i].nfilas=lm/nHilos;
			for(j=0;j<(lm/nHilos);j++){
				par[i].filas[j]=j+((lm/nHilos)*i);
			}
		}
	}
	else{
		for(i=0;i<nHilos;i++){
			par[i].filas=(int *) malloc(((lm-(lm%nHilos))/nHilos)*sizeof(int));
			par[i].nfilas=((lm-(lm%nHilos))/nHilos);
			for (j=0;j<(lm-(lm%nHilos))/nHilos;j++){
				par[i].filas[j]=(j+((lm-(lm%nHilos))/nHilos*i));
			}
		}
		int uf=(lm-(lm%nHilos));
		i=0;
		int x;
		while(uf<lm){
			par[i].filas=(int *) realloc(par[i].filas,(((lm-(lm%nHilos))/nHilos)*sizeof(int)+sizeof(int)));
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
	pthread_exit(NULL);
}

int main(){
	printf("Longitud de la matriz\n");
	scanf("%d", &lm);
	printf("Numero de hilos\n");
	scanf("%d", &nHilos);
	if(nHilos<=lm){
		pthread_t th[nHilos];
		parameters par[nHilos];
		crearMatrices();
		incializa();
		printf("Matriz a\n");
		impMatriz(a);
		printf("Matriz b\n");
		impMatriz(b);
		parameters param[nHilos];
		asignarFilas(param);
		for (int i=0;i<nHilos;i++){
			int create=pthread_create(&th[i], NULL, &multiplicar, (void *)&param[i]);
			printf("Hilo %d\n",i);
			if(!create)
				printf("Hilo creado\n");
		}

		for (int j=0;j<nHilos;j++){
			pthread_join(th[j],NULL);	
			printf("Hilo: %d\n", j);
		}
		printf("Matriz c\n");
		impMatriz(c);
	}
	else{
		printf("El numero de hilos debe ser menor o igual a la longitud de la matriz\n");
		exit(0);
	}
	
	return 0;
}
