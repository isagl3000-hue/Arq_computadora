#include <stdio.h>
#include <stdlib.h>

#include "logo.h"
#include "X.h"

int nivelRecursionX;
double longitudX;
double miX0X;
double miY0X;
LOGO *tortugaAuxX;
int PX=-1;
double x;

int Xlab(int n, double l, LOGO *tortuga);
int Xint(double l, LOGO *tortuga);

int iniciaX(int n, double l, double x, double y, LOGO *tortuga){
	
  nivelRecursionX=n;
  longitudX = l;
  miX0X=x;
  miY0X=y;
  tortugaAuxX = tortuga;
  
}


int dibujaX(void){
  inicia(miX0X, miY0X, 0, ABAJO, tortugaAuxX);

  return Xlab(nivelRecursionX, longitudX, tortugaAuxX);
}



int Xlab(int n, double l, LOGO *tortuga){
	double u;
	int i;
	
	if (n<=0){
		Xint( l, tortuga);
		return 0;
	}  
	
	u=l/3;
	Xlab(n-1, u, tortuga);
	izq(90, tortuga);
	Xlab(n-1, u, tortuga);
	for(i=0;i<3;i++){
		der(90, tortuga);
		Xlab(n-1, u, tortuga);
	}
	for(i=0;i<3;i++){
		izq(90, tortuga);
		Xlab(n-1, u, tortuga);
	}
	der(90, tortuga);
	Xlab(n-1, u, tortuga);
	
	return 0;
}


int Xint(double l, LOGO *tortuga){
	double u;
	PX=-PX;
	u=l/5;
	
	if(x){
		PX=1;
		izq(45, tortuga);
	} 
	
	avanza(u*4, tortuga);
	der(PX*90, tortuga);
	avanza(u*3, tortuga);
	der(PX*90, tortuga);
	avanza(u*2, tortuga);
	der(PX*90, tortuga);
	avanza(u, tortuga);
	der(PX*90, tortuga);
	avanza(u, tortuga);
	izq(PX*90, tortuga);
	avanza(u, tortuga);
	izq(PX*90, tortuga);
	avanza(u*2, tortuga);
	izq(PX*90, tortuga);
	avanza(u*3, tortuga);
	izq(PX*90, tortuga);
	avanza(u*4, tortuga);
	izq(PX*90, tortuga);
	avanza(u*4, tortuga);
	der(PX*90, tortuga);
	if(x){
		der(90, tortuga);
		avanza(u*4, tortuga);
		izq(45, tortuga);
	}
	
	return 0;
}

