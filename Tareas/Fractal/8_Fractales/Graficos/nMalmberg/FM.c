#include <stdio.h>
#include <stdlib.h>

#include "logo.h"
#include "FM.h"

int nivelRecursionFM;
double longitudFM;
double miX0FM;
double miY0FM;
LOGO *tortugaAuxFM;
double x;


int FMlab(int n, double l, LOGO *tortuga);
int FMint(double l, LOGO *tortuga);

int iniciaFM(int n, double l, double x, double y, LOGO *tortuga){
	
  nivelRecursionFM=n;
  longitudFM=l;
  miX0FM=x;
  miY0FM=y;
  tortugaAuxFM=tortuga;
  
}


int dibujaFM(void){
  inicia(miX0FM, miY0FM, 0, ABAJO, tortugaAuxFM);
  
  if(!x)FMlab(nivelRecursionFM, longitudFM, tortugaAuxFM);

  return 0; 
}



int FMlab(int n, double l, LOGO *tortuga){
	double u;
	int i;
	
	if (n<=0){
		FMint( l, tortuga);
		return 0;
	}  
	
	u=l/3;
	FMlab(n-1, u, tortuga);
	izq(90, tortuga);
	FMlab(n-1, u, tortuga);
	for(i=0;i<3;i++){
		der(90, tortuga);
		FMlab(n-1, u, tortuga);
	}
	for(i=0;i<3;i++){
		izq(90, tortuga);
		FMlab(n-1, u, tortuga);
	}
	der(90, tortuga);
	FMlab(n-1, u, tortuga);
	
	return 0;
}


int FMint(double l, LOGO *tortuga){
	double u,x;
	u=l/5;
	
	if(!x){
		izq(45, tortuga);
	} 
	
	avanza(u*4, tortuga);
	der(90, tortuga);
	avanza(u*3, tortuga);
	der(90, tortuga);
	avanza(u*2, tortuga);
	der(90, tortuga);
	avanza(u, tortuga);
	der(90, tortuga);
	avanza(u, tortuga);
	izq(90, tortuga);
	avanza(u, tortuga);
	izq(90, tortuga);
	avanza(u*2, tortuga);
	izq(90, tortuga);
	avanza(u*3, tortuga);
	izq(90, tortuga);
	avanza(u*4, tortuga);
	izq(90, tortuga);
	avanza(u*4, tortuga);
	der(90, tortuga);
	if(!x){
		der(90, tortuga);
		avanza(u*4, tortuga);
		izq(45, tortuga);
	}
	
	return 0;
}
