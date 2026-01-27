#include <stdio.h>
#include <stdlib.h>

#include "logo.h"
#include "N.h"

int nivelRecursionN;
double longitudN;
double miX0N;
double miY0N;
LOGO *tortugaAuxN;


int NRecursivo(int n, double l, LOGO *tortuga);

int iniciaN(int n, double l, double x, double y, LOGO *tortuga){
	
  nivelRecursionN=n;
  longitudN=l;
  miX0N=x;
  miY0N=y;
  tortugaAuxN=tortuga;
  
  return 0;
}


int dibujaN(void){
  inicia(miX0N, miY0N, 0, ABAJO, tortugaAuxN);

  return NRecursivo(nivelRecursionN, longitudN, tortugaAuxN);
}


int NRecursivo(int n, double l, LOGO *tortuga){
  int i;
  double l2;
  
  if(n<=0)return 0;
  
  l2=l/2.0;
  for(i=0; i<3; i++){
  	NRecursivo(n-1, l2, tortuga);
  	avanza(l, tortuga);
  	der(120, tortuga);
  } 
  return 0;
}
