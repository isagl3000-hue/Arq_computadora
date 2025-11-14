#include <iostream>
#include <cstdlib>

enum { SUM, SUB, MUL, DIV, EXP, PIZ, PDR, PYC, NUM, MAL };
int sim;

void error(const char *men)
{
	std::cerr << "ERROR - " << men << std:: endl;
	exit(EXIT_FAILURE);
}


int tok(void)
{
	int c;
	do {	
		c= std::cin.get();
	} while (isspace(c));

	double x;
	switch (c) {
		case '+':  return sim= SUM;
		case '-':  return sim= SUB;
		case '*':  return sim= MUL;
		case '/':  return sim= DIV;
		case '^':  return sim= EXP;
		case '(':  return sim= PIZ;
		case ')':  return sim= PDR;
		case '0':  
		case '1': 
		case '2': 
		case '3': 
		case '4': 
		case '5':  
		case '6': 
		case '7': 
		case '8': 
		case '9': std::cin.putback(c);
				  std::cin >> x;
				  return sim= NUM;
	}
	return MAL;
}

void expr(void);


void prim(void)
{
	if (sim==NUM)
		tok();
	else
	if (sim==PIZ) {
		tok();
		expr();
		if (sim==PDR)
			tok();
		else
			error("falta ')'");
	}
}

void factor(void)
{
	prim();
	while(sim==EXP) {
		tok();
		factor();
	}
}

void term(void)
{
	factor();
	while (sim==MUL || sim==DIV) {
		tok();
		factor();
	}
}

void expr(void)
{
	if (sim==SUM) {
		tok();
		term();
	}
	else
	if (sim==SUB) {
		tok();
		term();
	}
	else
		term();

	while (sim==SUM || sim==SUB) {
		tok();
		term();
	}
}

void linea()
{
	expr();
	if (sim==PYC)
		;
	else
		error("Falsta ';'");
}

int main(int argc, char *argv[])
{
	tok();

	return EXIT_SUCCESS;
}
