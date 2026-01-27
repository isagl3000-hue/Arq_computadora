#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>


enum {FIN=0,CONST,VAR,NOT,AND,OR,PI, PD,PYC,MAL }; 

int sim,i=0; //ultimo simbolo  leido
char valor; //valor real del simbolo en lecrura
char variables[50];
 // contador de variables

void error(const char *mensaje){
	std::cerr << "ERROR: "<< mensaje << std:: 
	endl;
	exit(EXIT_FAILURE);
}

int tok(void ){ // analizador lexico
	int c,j,b=0;

	do { c=std::cin.get(); }
	while (isspace(c) && c!='\n' );

	valor=(char)c;

	if(c=='0'|| c=='1' ) return sim=CONST;	
	if (islower(c)){ variables[i]=valor; i++;
		return sim=VAR; }
	
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
	if (sim==CONST|| sim==VAR) {
		if(sim==CONST){
			//std::cout <<"\t\t lda #" << valor  << "\n\t\t psh a"<< std::endl;
			std:: cout << "\t\t lda # "<< std:: endl;
			tok();
		}
		else {  //std::cout<<"\t\t lda[bp+@" << valor <<"]\n\t\t psh a "<< std:: endl; 
			std:: cout << "\t\t lda [bp+@"<< valor <<"]"<< std:: endl;
			tok();
		 }
	}
	else { // revisar si es expresion
		if (sim==PI){ 
			tok();
			expr();
			if(sim==PD) tok();
			else error("falta cerrar parentesis");
		 }
		else {  if(sim==NOT){
				tok();
				primario();
				std:: cout<<"\t\t NOT "<< std:: endl;

			 }
			else error("Se esperaba una constante, variable,'(', ')' o '!'  ");
		}
	}
}

void termino(void){
	primario();
	while (sim==AND){
		tok();
		std:: cout <<"\t\t psh a"<< std:: endl; // guarda en la pila lo anterior a &
		primario();
		std:: cout << "\t\t AND\n"<< std:: endl;
	 }
	//if (sim!=AND){ error("Se esperaba '&'");}
}

void expr(void){
	termino();
	while (sim==OR){
		tok();
		std:: cout <<"\t\t psh a"<< std:: endl;
		termino();
		std:: cout << "\t\t OR\n "<< std:: endl;
	}
	//if(sim!=OR){error("Se esperaba '|'"); }
}

void linea(void){
	expr();
	if (sim==PYC){
		std:: cout << "\n\t\t lsp bp \n\t\t pop bp \n\t\t rtn" << std::  endl;
		//return 1;
		 }
	else {
		error("Falta ';'");
		//return 0;
	}
}
int imp_var(const char *cad){
	int i=0,j=0,t;
	char d,aux[50];
	bool letra[26]; // arreglo para saber cuales se usaron;
	
	for(i=0;i<27;i++) letra[i]=false;
	
	for(i=0;cad[i]!='\0'; i++){  //a=97,b=98
		d=cad[i];
		if(!letra[d-'a']){ //si no ha aparecido
			letra[d-'a']=true;
			aux[j++]=d;
		}
	}
	aux[j]='\0';

	//  cout << "\n \t\t\t\t AUX:------"<< aux << endl;
	for(i=0;aux[i]!='\0';i++){
	std:: cout << "\n\t\t lda #'"<< aux[i] << "'" << std:: endl;
	std:: cout << "\t\t irq rd_chr \n\t\t sta [@"<< aux[i] << "]" << std:: endl;
	}
	return 0;
}

int main(){
	int j,k;

	std::cout << "Ingrese una linea"<< std:: endl;
	tok();
	std:: cout << "Código: \n\n \t\t *=0x100\n@linea: \n\t\t psh bp \n \t\t lbp sp \n "<< std:: endl;
	linea();

	std:: cout << "\n@main:\n\t\t psh bp \n\t\t lbp sp " << std:: endl;
	
	j= imp_var(variables);
	std:: cout << "\n\t\t jsr @linea\n\t\t lsp bp \n\t\t pop bp \n\t\t rtn "<< std:: endl;
	std:: cout << "@pila: \n\t\t end @main, @pila " << std:: endl;
	// std:: cout << "Variables:"<< variables << std:: endl;
	return EXIT_SUCCESS;
}
