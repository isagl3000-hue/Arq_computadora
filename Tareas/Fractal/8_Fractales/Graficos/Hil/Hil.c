#include <stdio.h>
#include <stdlib.h>

#include "logo.h"
#include "Hil.h"

int nivelRecursionHilbert;
double longitudHilbert;
double miX0Hilbert;
double miY0Hilbert;
LOGO *tortugaAuxHilbert;
int PHilbert;

int HilbertRecursivo(int n, double l, int p, LOGO *tortuga);

int iniciaHilbert(int n, double l, double x, double y, int p, LOGO *tortuga){
	
  nivelRecursionHilbert=n;
  longitudHilbert = l;
  miX0Hilbert=x;
  miY0Hilbert=y;
  tortugaAuxHilbert = tortuga;
  PHilbert=p;
  
}


int dibujaHilbert(void){
  inicia(miX0Hilbert, miY0Hilbert, 0, ABAJO, tortugaAuxHilbert);

  return HilbertRecursivo(nivelRecursionHilbert, longitudHilbert, PHilbert, tortugaAuxHilbert);
}


int HilbertRecursivo(int n, double l, int p, LOGO *tortuga)
{
  
  if (n<=0) return 0;

  
  izq(p*90,tortuga);
  HilbertRecursivo(n-1,l,-p,tortuga);
  avanza(l,tortuga);
  der(p*90,tortuga);
  HilbertRecursivo(n-1,l,p,tortuga);
  avanza(l,tortuga);
  HilbertRecursivo(n-1,l,p,tortuga);
  der(p*90,tortuga);
  avanza(l,tortuga);
  HilbertRecursivo(n-1,l,-p,tortuga);
  izq(p*90,tortuga);
  
  
  return 0;
}


