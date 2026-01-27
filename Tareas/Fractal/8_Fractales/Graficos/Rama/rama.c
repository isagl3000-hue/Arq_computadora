/*
 * Veronica Bola~nos
 * Materia: Programaci'on I
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "logo.h"
#include "rama.h"

int nivelRecursionRama;
double longitudRama;
double miX0Rama;
double miY0Rama;
int miGRama;
LOGO *tortugaAuxRama;

int RamaRecursiva(int n, double l, LOGO *tortuga);

int iniciaRama(int n, double l, int g, double x, double y, LOGO *tortuga)
{
  nivelRecursionRama=n;
  longitudRama = l;
  miX0Rama=x;
  miY0Rama=y;
  miGRama=g;
  tortugaAuxRama = tortuga;
  
  return 0;
}

int dibujaRama(void){
  inicia(miX0Rama, miY0Rama, miGRama, ABAJO, tortugaAuxRama);

  return RamaRecursiva(nivelRecursionRama, longitudRama, tortugaAuxRama);
}


int RamaRecursiva(int n, double l, LOGO *tortuga)
{
  double l4;
  if (n<=0){
    avanza(l, tortuga);
    return 0;
  }
  l4=l/4.0;
  avanza(l4, tortuga);
  der(60, tortuga);
  RamaRecursiva(n-1, l4, tortuga);
  izq(60, tortuga);
  der(15, tortuga);
  avanza(l4, tortuga);
  izq(60, tortuga);
  RamaRecursiva(n-1, l4, tortuga);
  der(60, tortuga);
  der(15, tortuga);
  avanza(l4, tortuga);
  der(60, tortuga);
  RamaRecursiva(n-1, l4, tortuga);
  izq(60, tortuga);
  izq(45, tortuga);
  avanza(l4, tortuga);

  retrocede(l4, tortuga);
  der(45, tortuga);
  retrocede(l4, tortuga);
  izq(15, tortuga);
  retrocede(l4, tortuga);
  izq(15, tortuga);
  retrocede(l4, tortuga);
  
  return 0;
}
