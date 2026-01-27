#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>


enum {FIN=0,CONST,VAR,NOT,AND,OR,PI, PD,PYC,MAL }; 

int sim; //ultimo simbolo  leido

void error(const char *mensaje){
	std::cerr << "ERROR "<< mensaje << std::
	endl;
	exit(EXIT_FAILURE);
}

int tok(void ){ // analizador lexico
	int c;
	do { c=std::cin.get(); } while (isspace(c));

	if(c=='0'|| c=='1' ) return sim=CONST;
	if (islower(c))return sim=VAR;
	switch (c){
		case '!': return sim=NOT; 
		case '&': return sim=AND; 
		case '|': return sim=OR;  
		case '(': return sim=PI;  
		case ')': return sim=PD;  
		case ';': return sim=PYC; 
		case EOF: return sim=FIN; 
	default: return sim=MAL;
	}

}
void expr(void);
void primario(void){
	if (sim==CONST|| sim==VAR) tok();
	else { // revisar si es expresion
		if (sim==PI){ 
			tok();
			expr();
			if(sim==PD) tok();
			else error("falta cerrar parentesis");
		 }
		else {  if(sim==NOT){ tok(); primario(); }
			else error("Se esperaba una constante, variable,'(', ')' o '!'  ");
		}
	}
}

void termino(void){
	primario();
	while (sim==AND){tok(); primario(); }
}

void expr(void){
	termino();
	while (sim==OR){tok(); termino(); }
}

void linea(void){
	expr();
	if (sim==PYC){ std:: cout << "\n Sintaxis correcta" << std:: endl;}
	else error("Falta ';'");
}

int main(){
	std:: cout << "Ingrese una linea"<< std:: endl;
	tok();
	linea();
	return EXIT_SUCCESS;

}
