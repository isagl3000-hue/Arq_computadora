#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "logo.h"
#include "H.h"

int nivelRecursionH;
double longitudH;
double miX0H;
double miY0H;
LOGO *tortugaAuxH;


int rHRecursivo(int n, double l, LOGO *tortuga);
int lHRecursivo(int n, double l, LOGO *tortuga);

int iniciaH(int n, double l, double x, double y, LOGO *tortuga){
	
  nivelRecursionH=n;
  longitudH=l;
  miX0H=x;
  miY0H=y;
  tortugaAuxH=tortuga;
}


int dibujaH(void){
  inicia(miX0H, miY0H, 0, ABAJO, tortugaAuxH);

  return rHRecursivo(nivelRecursionH, longitudH, tortugaAuxH);
}

int rHRecursivo(int n, double l, LOGO *tortuga){
	double u, s;
	if (n<=0){
    avanza(l, tortuga);
    return 0;
    }
    
    u=(l/3);
    s=((l*2*sin(1.0472))/9); 
    
    izq(60, tortuga);
    lHRecursivo(n-1, u, tortuga);
    rHRecursivo(n-1, u, tortuga);
    der(60, tortuga);
    rHRecursivo(n-1, u, tortuga);
    der(60, tortuga);
    rHRecursivo(n-1, u, tortuga);
    der(150, tortuga);
    rHRecursivo(n-1, s, tortuga);
    lHRecursivo(n-1, s, tortuga);
    izq(60, tortuga);
    rHRecursivo(n-1, s, tortuga);
    izq(60, tortuga);
    lHRecursivo(n-1, s, tortuga);
    rHRecursivo(n-1, s, tortuga);
    izq(90, tortuga);
    lHRecursivo(n-1, u, tortuga);
    rHRecursivo(n-1, u, tortuga);
 
    return 0;
	
}

int lHRecursivo(int n, double l, LOGO *tortuga){
	double u, s;
	if (n<=0){
    avanza(l, tortuga);
    return 0;
    }
    
    u=(l/3);
    s=((l*2*sin(1.0472))/9);
    
    lHRecursivo(n-1, u, tortuga);
    rHRecursivo(n-1, u, tortuga);
    der(90, tortuga);
    lHRecursivo(n-1, s, tortuga);
    rHRecursivo(n-1, s, tortuga);
    der(60, tortuga);
    lHRecursivo(n-1, s, tortuga);
    der(60, tortuga);
    rHRecursivo(n-1, s, tortuga);
    lHRecursivo(n-1, s, tortuga);
    izq(150, tortuga);
    lHRecursivo(n-1, u, tortuga);
    izq(60, tortuga);
    lHRecursivo(n-1, u, tortuga);
    izq(60, tortuga);
    lHRecursivo(n-1, u, tortuga);
    rHRecursivo(n-1, u, tortuga);
    der(60, tortuga);
    
    return 0;
	
    
    
}


