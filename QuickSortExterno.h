#ifndef QUICKSORTEXTERNO_H
#define QUICKSORTEXTERNO_H

#include "Structs.h"

void Quick_sort_externo(long int quantidade, Controle *control);
void QuickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, long int Dir, Controle *control);
void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, bool *OndeLer, Controle *control);
void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, bool *OndeLer, Controle *control);
void InserirArea(TipoArea *Area, TipoRegistro *UltLido, int *NRArea, Controle *control);
void EscreveMax(FILE **ArqLEs, TipoRegistro R, int *Es, Controle *control);
void EscreveMin(FILE **ArqEi, TipoRegistro R, int *Ei, Controle *control);
void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea);
void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea);
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, long int *i, long int *j, Controle *control);
void InsereItem(TipoRegistro UltLido, TipoArea *Area, Controle *control);
void RetiraUltimo(TipoArea *Area, TipoRegistro *R);
void RetiraPrimeiro(TipoArea *Area, TipoRegistro *R);
int ObterNumCelOCupadas(TipoArea *Area);
void FAVazia(TipoArea *Area);

#endif