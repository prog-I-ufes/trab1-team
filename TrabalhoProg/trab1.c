//Trabalho de Programação 2 - 2018/2 | Victor David Lima

//Bibliotecas
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

//Função para chegar na linha desejada
void lerArquivo (FILE *arquivo, int linhaDesejada)
{
	rewind(arquivo);

	int linhas = 1;
	char caracter;

	while(linhas != linhaDesejada)
	{
		fscanf(arquivo, "%c", &caracter);

		if (caracter == '\n') {linhas++;}
	}
}

//Função para obter o número de caracteres de uma linha do arquivo
int obterNumeroCaracteres(FILE *arquivo, int linhaDesejada)
{
	rewind(arquivo);

	int linhas = 0;
	int caracteres = 0;
	char caracter;
	
	while (linhas != linhaDesejada)
	{
		fscanf(arquivo, "%c", &caracter);
		
		if (caracter == '\n') {linhas++;}

		if (linhas == linhaDesejada - 1 && caracter != '\n') {caracteres++;}
	}
	
	return caracteres;
}

//Função para obter o número de dados e sua quantidade
void infoArquivo(FILE *arquivo, int *linhas, int *dados)
{
	rewind(arquivo);
	
	char caracter;
        int iniciaDado = 0;
	
	//Usando -1 para desativar a contagem de colunas		
	if (*dados == -1) {
		while (!feof(arquivo))
		{	
			fscanf(arquivo, "%c", &caracter);
		
			if (caracter == '\n') {*linhas = *linhas + 1;}
		}
	}

	else {
		while (!feof(arquivo))
		{
			fscanf(arquivo, "%c", &caracter);
		
			//Conta quantidade de dados por linha
			if (caracter != ' ' && caracter != '\n' && caracter != ',' && iniciaDado != 1) {iniciaDado = 1;}
		
			if (*linhas == 0) {if ((caracter == ',' || caracter == '\n') && iniciaDado == 1) {*dados = *dados + 1; iniciaDado = 0;}}

			if (caracter == '\n') {*linhas = *linhas + 1;}
		}
	}

	*linhas = *linhas - 1;
}

//Função para alocar a matriz do tipo float dinamicamente
float ** alocaMatrizFloat(int *linhas, int *colunas)
{
	float **matriz;

	int i;
	
	matriz = (float **) malloc (*linhas * sizeof(float *));
	
	for (i = 0; i < *linhas; i++)
	{
		matriz[i] = (float *) malloc (*colunas * sizeof(float));
	}

	return matriz;
}

//Função para dar free na matriz float alocada dinamicamente
void freeMatrizFloat(float **matriz, int *linhas)
{
	int i;

	for (i = 0; i < *linhas; i++)
	{
		free(matriz[i]);
	}
	
	free(matriz);
}

//Função para obter as matrizes de treino e teste
float ** obterMatriz(FILE *arquivo, int *linhas, int *colunas, int linhaDesejada)
{
	float **matriz;

	FILE *arquivoAux;
	
	//Obtendo o tamanho do endereço de teste
	int tamanhoString = obterNumeroCaracteres(arquivo, linhaDesejada);

	tamanhoString = tamanhoString + 1;

	//Chegando na linha desejada
	lerArquivo(arquivo, linhaDesejada);
	
	//Obtendo a string do endereço de teste
	char stringComplemento[tamanhoString];
	
	fgets(stringComplemento, tamanhoString, arquivo);
	
	//Acessando o arquivo para obter os valores
	arquivoAux = fopen(stringComplemento, "r");
	
		//Apurar a abertura do arquivo
    		if (arquivoAux == NULL) {printf("Erro na abertura do arquivo auxiliar(obterMatriz).\n"); exit(1);}
	
	//Analisando as propriedades do arquivo
	infoArquivo(arquivoAux, linhas, colunas);
	
	rewind(arquivoAux);

	//Alocando a matriz
	matriz = alocaMatrizFloat(linhas, colunas);
	
	//Preencher a matriz com os valores
	int i, j;

	for (i = 0; i < *linhas; i++)
	{
		for (j = 0; j < *colunas; j++)
		{
			fscanf(arquivoAux, "%f,", &matriz[i][j]);		
		}
	}	

	//Fechando o arquivo
	fclose(arquivoAux);

	return matriz;
}

//Função para alocar dinamicamente a matriz resultante, com as distâncias e os rótulos em vetores
float *** alocaResultante(int *linhasEntrada, int *linhasComparar)
{
	float ***matriz;
	
	int i, j;
	
	matriz = (float ***) malloc (*linhasEntrada * sizeof(float **));
	
	for (i = 0; i < *linhasEntrada; i++)
	{
		matriz[i] = (float **) malloc (*linhasComparar * sizeof(float *));
	}
	
	for (i = 0; i < *linhasEntrada; i++)
	{
		for (j = 0; j < *linhasComparar; j++)
		{
			matriz[i][j] = (float*) malloc (2 * sizeof(float));
		}
	}
	
	return matriz;
}

//Função para dar free na matriz resultante
void freeResultante(float ***matriz, int *linhas, int *colunas)
{
	int i, j, k;

	for (i = 0; i < *linhas; i++)
	{
		for (j = 0; j < *colunas; j++)
		{			
			free(matriz[i][j]);
		}

		free(matriz[i]);
	}

	free(matriz);
}	

//Função para calcular a distância Euclidiana
void distEuc(float **matrizEntrada, float **matrizComparar, float ***matrizResultante, int *linhasEntrada, int *colunasEntrada, int *linhasComparar)
{
	int i, j, k;
	
	float dist = 0, diferenca;
	
	for (i = 0; i < *linhasEntrada; i++)
	{
		for (j = 0; j < *linhasComparar; j++)
		{
			for (k = 0; k < (*colunasEntrada) - 1; k++)
			{
				//Cálculo da distância
				diferenca = matrizEntrada[i][k] - matrizComparar[j][k];

				dist = (diferenca * diferenca) + dist;
			}
		
			//Inserindo o valor da distância
			matrizResultante[i][j][0] = sqrt(dist);
			
			//Inserindo o valor do rótulo
			matrizResultante[i][j][1] = matrizComparar[j][(*colunasEntrada) - 1];
	
			dist = 0;	
		}
	}
}

//Função para calcular a distância de Minkowsky
void distMink(float **matrizEntrada, float **matrizComparar, float ***matrizResultante, int *linhasEntrada, int *colunasEntrada, int *linhasComparar, float valorRaio)
{
	int i, j, k;

	float dist, valor, valorFinalRaio = 1/valorRaio;

	for (i = 0; i < *linhasEntrada; i++)
	{
		for (j = 0; j < *linhasComparar; j++)
		{
			//Resetando o valor da variável dist, para calcular a distancia novamente	
			dist = 0;

			for (k = 0; k < (*colunasEntrada) - 1; k++)
			{
				//Cálculo do valor absoluto
				if (matrizEntrada[i][k] > matrizComparar[j][k]) {valor = (matrizEntrada[i][k] - matrizComparar[j][k]);}
				else {valor = (matrizComparar[j][k] - matrizEntrada[i][k]);}
	
				//Cálculo da distância
				dist = (pow(valor, valorRaio)) + dist;
			}
		
			//Inserindo o valor da distância
			matrizResultante[i][j][0] = (pow(dist, valorFinalRaio));
			
			//Inserindo o valor do rótulo
			matrizResultante[i][j][1] = matrizComparar[j][(*colunasEntrada) - 1];	
		}
	}
}

//Função para calcular a distância de Chebyshev
void distCheby(float **matrizEntrada, float **matrizComparar, float ***matrizResultante, int *linhasEntrada, int *colunasEntrada, int *linhasComparar)
{
	int i, j, k;

	float maiorValor, valor;

	for (i = 0; i < *linhasEntrada; i++)
	{
		for (j = 0; j < *linhasComparar; j++)
		{
			maiorValor = -1;	

			for (k = 0; k < (*colunasEntrada - 1); k++)
			{
				//Cálculo do valor absoluto
				if (matrizEntrada[i][k] > matrizComparar[j][k]) {valor = (matrizEntrada[i][k] - matrizComparar[j][k]);}
				else {valor = (matrizComparar[j][k] - matrizEntrada[i][k]);}

				if (valor > maiorValor) {maiorValor = valor;}
			}
		
			//Inserindo o valor da maior distância
			matrizResultante[i][j][0] = maiorValor;
			
			//Inserindo o valor do rótulo
			matrizResultante[i][j][1] = matrizComparar[j][(*colunasEntrada) - 1];
		}
	}
}

//Função para ordenar a matriz resultante
void ordenaResultante(float ***matrizResultante, int *linhasEntrada, int *linhasComparar)
{
	int i, j, k, sinal, troca;

	float distancia, rotulo;

	//Bubble Sort
	for (i = 0; i < *linhasEntrada; i++)
	{
		//Detectar se o vetor já está ordenado, para otimizar a ordenação
		sinal = 0;

		for (k = (*linhasComparar) - 1; k > 0 && sinal == 0; k--)
		{
			troca = 0;

			for (j = 0; j < k; j++)
			{ 
				if (matrizResultante[i][j][0] > matrizResultante[i][j + 1][0]) {
				 troca = 1;
				 distancia = matrizResultante[i][j][0]; 
				 rotulo = matrizResultante[i][j][1]; 
				 matrizResultante[i][j][0] = matrizResultante[i][j + 1][0]; 
				 matrizResultante[i][j][1] = matrizResultante[i][j + 1][1];
				 matrizResultante[i][j + 1][0] = distancia;
				 matrizResultante[i][j + 1][1] = rotulo;
				}
			}

			if (troca == 0) {sinal = 1;}		
		}
	}
} 

//Função para alocar matriz de inteiros
int ** alocaMatrizInt(int *linhas, int *colunas)
{
	int **matriz, i;
	
	matriz = (int **) malloc (*linhas * sizeof(int *));
	
	for (i = 0; i < *linhas; i++)
	{
		matriz[i] = (int *) malloc (*colunas * sizeof(int));
	}
	
	return matriz;
}

//Função para dar free na matriz de inteiros alocada dinamicamente
void freeMatrizInt(int **matriz, int *linhas)
{
	int i;

	for (i = 0; i < *linhas; i++)
	{
		free(matriz[i]);
	}
	
	free(matriz);
}

//Função para retirar os 'K' valores mais próximos
int ** retirarProximos(int *K, float ***matrizResultante, int *linhasEntrada)
{
	int **matrizFinal;

	matrizFinal = alocaMatrizInt(linhasEntrada, K);

	int i, j, k;
 
	for (i = 0; i < *linhasEntrada; i++)
	{
		for (j = 0; j < *K; j++)
		{
			matrizFinal[i][j] = matrizResultante[i][j][1];
		}
	}

	return matrizFinal;	
}

//Função para o critério de desempate e escolha do rótulo
int * escolhaRotulo (int **matrizFinal, int *K, int *linhasEntrada)
{
	int *vetor, i, j, k;

	//Alocando um vetor para os valores de K de cada amostra do teste
	vetor = (int *) malloc (*linhasEntrada * sizeof(int));

	//Decidindo o valor de K
		//Vetor com duas posições, uma para o nº de repetições e outra para a posição do valor
	int decisao[2]; 

	int nRepeticoes;

	for (i = 0; i < *linhasEntrada; i++)
	{
		decisao[0] = -1;

		for (j = 0; j < *K; j++)
		{
			nRepeticoes = 0;

			for (k = j + 1; k < *K; k++)
			{
				if (matrizFinal[i][j] == matrizFinal[i][k]) {nRepeticoes++;}
			}

			if (nRepeticoes > decisao[0]) {decisao[0] = nRepeticoes; decisao[1] = j;}
		}

		vetor[i] = matrizFinal[i][decisao[1]];
	}

	return vetor;
}

//Função para obter a matriz de confusão
int ** matrizConfusao(int *vetorKValores, float **matrizTeste, int *linhasTeste, int *colunasTeste, int *dimensoesConf, float *acuracia)
{
	int quantidadeRotulos, i, j, k; 
	
	*dimensoesConf = -1;

	*acuracia = 0;

	//Obtendo a quantidade de rotulos
	for (i = 0; i < *linhasTeste; i++)
	{
		if (matrizTeste[i][(*colunasTeste) - 1] > *dimensoesConf) {*dimensoesConf = matrizTeste[i][(*colunasTeste) - 1];}
	}

	int **matrizConf;

	//Alocando a matriz de confusão
	matrizConf = alocaMatrizInt(dimensoesConf, dimensoesConf);

	//Obtendo a matriz de confusão
	int acertos, erros;

	int recebeValor;

	for (i = 0; i < *dimensoesConf; i++)
	{
		for (j = 0; j < *dimensoesConf; j++)
		{
			acertos = 0;

			erros = 0;

			for (k = 0; k < *linhasTeste; k++)
			{
				if (i == j) {if (matrizTeste[k][(*colunasTeste) - 1] == vetorKValores[k] && vetorKValores[k] == (i+1)) {acertos++;}}
				else if (matrizTeste[k][(*colunasTeste) - 1] == (i+1) && vetorKValores[k] == (j+1) && vetorKValores[k] != matrizTeste[k][(*colunasTeste) - 1]) {erros++;}
			}
			
			if (i == j) {matrizConf[i][j] = acertos; *acuracia = acertos + *acuracia;}
			else {matrizConf[i][j] = erros;}
		}
	}	

	*acuracia = ((*acuracia)/(*linhasTeste));

	return matrizConf;
}

//Função para criar os arquivos "resposta" e printar os dados nele
void criarArquivo(char *endereco, int numero, float acuracia, int **matrizConf, int dimensoesConf, int *vetorKValores, int *linhasTeste)
{
	FILE *arquivoAux;

	//Transformando o numero em uma string
	int tamanho = 0, auxiliar = numero;

	while (auxiliar > 0)
	{
		auxiliar = auxiliar/10;
		tamanho++;
	}

	char num[tamanho], complemento[] = ".txt";

	sprintf(num, "%i", numero);

	char local[strlen(endereco) + strlen(complemento) + tamanho];

	strcpy(local, endereco);

	strcat(local, num);

	strcat(local, complemento);

	//Criando o arquivo para escrita
	arquivoAux = fopen(local,"w");

	//Apurar a abertura do arquivo
	if (arquivoAux == NULL) {printf("Erro na abertura do arquivo auxiliar(criarArquivo).\n"); exit(1);}

	//Printando a acurácia
	fprintf(arquivoAux, "%.2f\n\n", acuracia);

	//Printando a matriz de confusão
	int i, j;

	for (i = 0; i < dimensoesConf; i++)
	{
		for (j = 0; j < dimensoesConf; j++)
		{
			fprintf(arquivoAux, "%i ", matrizConf[i][j]);
		}

		fprintf(arquivoAux, "\n");
	}

	fprintf(arquivoAux, "\n");

	//Printando os valores de K encontrados	
	for (i = 0; i < *linhasTeste; i++)
	{
		fprintf(arquivoAux, "%i\n", vetorKValores[i] - 1);
	}

	//Fechando o arquivo
	fclose(arquivoAux);
}

//Função para realizar os cálculos conforme requisitado no config e escrever os resultados nos arquivos "resposta"
void realizarOperacoes(FILE *arquivo, float **matrizTeste, int *linhasTeste, int *colunasTeste, float **matrizTreino, int *linhasTreino, int *colunasTreino, float ***matrizResultante, char *endereco)
{
	int **matrizFinal, *vetorKValores, i, K, linhasConfig, **matrizConf, dimensoesConf, anularContagemColunas = -1;

	char caracter;

	float raio, acuracia;

	//Obter a quantidade de linhas do config
	infoArquivo(arquivo, &linhasConfig, &anularContagemColunas);

	//Chegando até a quarta linha, para receber as instruções
	lerArquivo(arquivo, 4);

	for (i = 1; i <= linhasConfig - 3; i++)
	{
		//Obtendo os dados
		fscanf(arquivo, "%i, %c,", &K, &caracter);

		//Verificando a distância desejada e calculando-a
		if (caracter == 'M') {fscanf(arquivo, "%f", &raio); distMink(matrizTeste, matrizTreino, matrizResultante, linhasTeste, colunasTeste, linhasTreino, raio);}

		else if (caracter == 'E') {distEuc(matrizTeste, matrizTreino, matrizResultante, linhasTeste, colunasTeste, linhasTreino);}

		     else {distCheby(matrizTeste, matrizTreino, matrizResultante, linhasTeste, colunasTeste, linhasTreino);}

		//Ordenando a matriz resultante
		ordenaResultante(matrizResultante, linhasTeste, linhasTreino);

		//Obtendo a matriz com os rótulos			
		matrizFinal = retirarProximos(&K, matrizResultante, linhasTeste);

		//Escolhendo os K rótulos mais próximos
		vetorKValores = escolhaRotulo(matrizFinal, &K, linhasTeste);	

		//Obtendo a matriz de confusão
		matrizConf = matrizConfusao(vetorKValores, matrizTeste, linhasTeste, colunasTeste, &dimensoesConf, &acuracia);

		//Criando os arquivos
		criarArquivo(endereco, i, acuracia, matrizConf, dimensoesConf, vetorKValores, linhasTeste);

		//Dando free nas matrizes e vetores usados
		freeMatrizInt(matrizFinal, linhasTeste);

		free(vetorKValores);	

		freeMatrizInt(matrizConf, &dimensoesConf);
	}
}

//Função principal
int main()
{
	float **matrizTeste, **matrizTreino, ***matrizResultante; 

	int linhasTeste = 0, colunasTeste = 0, linhasTreino = 0, colunasTreino = 0;

	FILE *arquivo1;

	//Abrir o config
	arquivo1 = fopen("config.txt", "r");

	//Apurar a abertura do arquivo
    	if (arquivo1 == NULL) {printf("Erro na abertura do arquivo(Main).\n"); exit(1);}

	//Obtendo a matriz de teste
	matrizTeste = obterMatriz(arquivo1, &linhasTeste, &colunasTeste, 2);

	//Obtendo a matriz de treino
	matrizTreino = obterMatriz(arquivo1, &linhasTreino, &colunasTreino, 1);

	//Alocando a resultante
	matrizResultante = alocaResultante(&linhasTeste, &linhasTreino);

	//Obter o endereço para a criação dos arquivos na função realizarOperacoes
		int tamanhoEndereco; 

		tamanhoEndereco = (obterNumeroCaracteres(arquivo1, 3) + 1);

		char result[] = "predicao_";

		char endereco[tamanhoEndereco + strlen(result)];

			//Chegando até a terceira linha
		lerArquivo(arquivo1, 3);

			//Obtendo o endereco pelo config
		fgets(endereco, tamanhoEndereco, arquivo1);

		strcat(endereco, result);

	//Fazendo as operações desejadas
	realizarOperacoes(arquivo1, matrizTeste, &linhasTeste, &colunasTeste, matrizTreino, &linhasTreino, &colunasTreino, matrizResultante, endereco);

	//Dando free nas matrizes
	freeMatrizFloat(matrizTeste, &linhasTeste);

	freeMatrizFloat(matrizTreino, &linhasTreino);

	freeResultante(matrizResultante, &linhasTeste, &linhasTreino);

	//Fechando o arquivo
	fclose(arquivo1);
}

