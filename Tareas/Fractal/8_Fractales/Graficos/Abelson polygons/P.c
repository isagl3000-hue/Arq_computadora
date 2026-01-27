#include <stdio.h>
#include <stdlib.h>

#include "logo.h"
#include "P.h"

// int nivelRecursionP;
int girototalP;
int anguloP=0;
double longitudP;
double miX0P;
double miY0P;
LOGO *tortugaAuxP;
double limiteP;



int P(int a, double l, int girototalP, LOGO *tortuga);
int Pstep(int a, double l, LOGO *tortuga);

int iniciaP(int a, double l, double x, double y, double lmt, LOGO *tortuga){
	
  // nivelRecursionP=n;
  anguloP = a;
  longitudP = l;
  miX0P=x;
  miY0P=y;
  tortugaAuxP = tortuga;
  limiteP=lmt;
  
}


int dibujaP(void){
	//double d=longitudSpk/sqrt(2);
	
  	inicia(miX0P, miY0P, 0, ABAJO, tortugaAuxP);
  	
  return P(anguloP, longitudP, girototalP, tortugaAuxP);
}


int P(int a, double l, int girototalP, LOGO *tortuga){
	
	if(l<=limiteP) return 0;
	
	Pstep(a, l, tortuga);
	girototalP=girototalP+a;
	if ((girototalP%360)!= 0) P(a, l, girototalP, tortuga);
	return 0;
}


int Pstep(int a, double l, LOGO *tortuga){
	
	avanza(l, tortuga);
	P(-a, l/2, 0, tortuga);
	der(a, tortuga);
	
	return 0;
}



