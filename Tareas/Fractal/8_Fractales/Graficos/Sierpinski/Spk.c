#include <stdio.h>
#include <stdlib.h>

#include "logo.h"
#include "Spk.h"

int nivelRecursionSpk;
double longitudSpk;
double miX0Spk;
double miY0Spk;
LOGO *tortugaAuxSpk;
//double d=longitudSpk/sqrt(2);

int SpkRecursivo(int n, double l, LOGO *tortuga);

int iniciaSpk(int n, double l, double x, double y, LOGO *tortuga){
	
  nivelRecursionSpk=n;
  longitudSpk = l;
  miX0Spk=x;
  miY0Spk=y;
  tortugaAuxSpk = tortuga;
  
}


int dibujaSpk(void){
	int i;
	//double d=longitudSpk/sqrt(2);
	
  	inicia(miX0Spk, miY0Spk, 0, ABAJO, tortugaAuxSpk);
  
  for (i=0; i<2; i++){
  	SpkRecursivo(nivelRecursionSpk, longitudSpk, tortugaAuxSpk);
  	der(90,tortugaAuxSpk);
	avanza(longitudSpk,tortugaAuxSpk);
  	der(90,tortugaAuxSpk);
  	
  }

  return 0;
}


int SpkRecursivo(int n, double l, LOGO *tortuga)
{
  //double d=l/sqrt(2);
  if (n<=0){
    avanza(l, tortuga);
    return 0;
	}
	

  SpkRecursivo(n-1, l,tortuga);
  izq(45,tortuga);
  avanza(l*sqrt(2),tortuga);
  izq(45,tortuga);
  SpkRecursivo(n-1, l,tortuga);
  der(90, tortuga);
  avanza(l,tortuga);
  der(90, tortuga);
  SpkRecursivo(n-1, l,tortuga);
  izq(45,tortuga);
  avanza(l*sqrt(2),tortuga);
  izq(45,tortuga);
  SpkRecursivo(n-1, l,tortuga);
  
  
  
  return 0;
}
