#include <stdio.h>
#include <stdlib.h>

#include "logo.h"
#include "G.h"

int nivelRecursionG;
double longitudG;
double miX0G;
double miY0G;
LOGO *tortugaAuxG;


int rG(int n, double l, LOGO *tortuga);
int lG(int n, double l, LOGO *tortuga);

int iniciaG(int n, double l, double x, double y, LOGO *tortuga){
	
  nivelRecursionG=n;
  longitudG = l;
  miX0G=x;
  miY0G=y;
  tortugaAuxG = tortuga;
  
}


int dibujaG(void){
	int i;
	//double d=longitudSpk/sqrt(2);
	
  	inicia(miX0G, miY0G, 0, ABAJO, tortugaAuxG);

  return rG(nivelRecursionG, longitudG, tortugaAuxG);
}


int rG(int n, double l, LOGO *tortuga)
{
  
  double u=l/(sqrt(7));
  
  if (n<=0){
    avanza(l, tortuga);
    return 0;
  }
  
  der(75, tortuga);
  lG(n-1, u, tortuga);
  izq(60, tortuga);
  rG(n-1, u, tortuga);
  rG(n-1, u, tortuga);
  izq(120, tortuga);
  rG(n-1, u, tortuga);
  izq(60, tortuga);
  lG(n-1, u, tortuga);
  der(120, tortuga);
  lG(n-1, u, tortuga);
  der(60, tortuga);
  rG(n-1, u, tortuga);
  izq(15, tortuga);
  
  return 0;
}

int lG(int n, double l, LOGO *tortuga)
{
  double u=l/(sqrt(7));
  
  if (n<=0){
    avanza(l, tortuga);
    return 0;
  }
  
  der(15, tortuga);
  lG(n-1, u, tortuga);
  izq(60, tortuga);
  rG(n-1, u, tortuga);
  izq(120, tortuga);
  rG(n-1, u, tortuga);
  der(60, tortuga);
  lG(n-1, u, tortuga);
  der(120, tortuga);
  lG(n-1, u, tortuga);
  lG(n-1, u, tortuga);
  der(60, tortuga);
  rG(n-1, u, tortuga);
  izq(75, tortuga);

  return 0;
}







