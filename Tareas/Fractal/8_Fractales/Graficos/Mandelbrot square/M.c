#include <stdio.h>
#include <stdlib.h>

#include "logo.h"
#include "M.h"

int nivelRecursionM;
double longitudM;
double miX0M;
double miY0M;
LOGO *tortugaAuxM;


int MRecursivo(int n, double l, LOGO *tortuga);

int iniciaM(int n, double l, double x, double y, LOGO *tortuga){
	
  nivelRecursionM=n;
  longitudM = l;
  miX0M=x;
  miY0M=y;
  tortugaAuxM = tortuga;
  
}


int dibujaM(void){
	int i;
	//double d=longitudSpk/sqrt(2);
	
  	inicia(miX0M, miY0M, 0, ABAJO, tortugaAuxM);
  	
  	for (i=0; i<4; i++){
    MRecursivo(nivelRecursionM, longitudM, tortugaAuxM);
    der(90, tortugaAuxM);
  }

  return 0;
}


int MRecursivo(int n, double l, LOGO *tortuga)
{
  
  double u=l/4;
  
  if (n<=0){
    avanza(l, tortuga);
    return 0;
  }
  
  MRecursivo(n-1, u, tortuga);
  izq(90, tortuga);
  MRecursivo(n-1, u, tortuga);
  der(90, tortuga);
  MRecursivo(n-1, u, tortuga);
  der(90, tortuga);
  MRecursivo(n-1, u, tortuga);
  MRecursivo(n-1, u, tortuga);
  izq(90, tortuga);
  MRecursivo(n-1, u, tortuga);
  izq(90, tortuga);
  MRecursivo(n-1, u, tortuga);
  der(90, tortuga);
  MRecursivo(n-1, u, tortuga);
  
  
  
  return 0;
}

