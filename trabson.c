// TRABSON 2018/2 - PROG II; Vidtor Davic && Guana Labriele;

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

void infoArquivo(FILE *arquivo, int *linhas, int *dados)
{
	int iniciaPalavra = 0, linhas = 0, dados = 0;
	
	while (!feof(arquivo))
	{
		char caracter;
		
		fscanf(arquivo, "%c", &caracter);
		
		//Conta quantidade de dados por linha
		if (caracter != ' ' && caracter != '\n') {iniciaPalavra = 1;}
		
		if (caracter == ',' && iniciaPalavra == 1) {dados++;}
		
		if (caracter == '\n' && iniciaPalavra == 1) {dados++; linhas++;}
	}
}

int ** distEuc(float **vetorEntradas, float **vetorComparar, int *linhasEntrada, int *colunasEntrada, int *linhasComparar, int *colunasComparar)
{
	
}

void alocaMatriz(float **matriz, int *linhas, int *colunas)
{
	int i;
	
	*matriz = (int **) malloc (*linhas * sizeof(int *));
	
	for (i = 0; i < *linhas; i++)
	{
		matriz[i] = (int *) malloc (*colunas * sizeof(int));
	}
}	


int main()
{
	FILE *iris, *vowels;
	
	iris = fopen("C:\Users\victo\Desktop\Trab. Prog\iris", "r");
	
	while(!feof(iris))
	{
		
	}
	
}
