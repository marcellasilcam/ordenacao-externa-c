#ifndef INTERCALACAOVARIOSCAMINHOS_H
#define INTERCALACAOVARIOSCAMINHOS_H

#include "Structs.h"

void Intercalacao_Varios_Caminhos(long int quantidade, Controle *control);
void Intercalacao_Varios_Caminhos_Selecao(long int quantidade, Controle *control);
char** CriaNomeFitas();
int CriaBlocosSelecao(FILE *arq, FILE **fitas, int tam, int *blocos, Controle *control);
void HeapSort(TipoHeap *v, int n, Controle *control);
void HeapConstroi(TipoHeap *v, int n, Controle *control);
void HeapRefaz(TipoHeap *v, int esq, int dir, Controle *control);
int Compara(TipoHeap valor1, TipoHeap valor2);
int Intercala0_19(FILE **fitas, int entradaI, int saida, int entradaF, int blocosTotais, Controle *control);
int Intercala20_39(FILE **fitas, int entradaI, int saida, int entradaF, int blocosTotais, Controle *control);

void DesalocaNomeFitas (char** nomeFitas);
void CriaBlocos(FILE* arqBin, FILE* fitas[2*F], int tamanhoArq, Controle *control, int *fitaFInal);
void OrdenaMemInterna(TipoRegistro* registro, Controle* control);
void Intercala(FILE* fitas[2*F], int* tamBloco, int tamanhoArq, localizadorFitas* localizador, int *fitaFinal, Controle* control);


#endif