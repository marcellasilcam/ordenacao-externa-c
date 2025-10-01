#ifndef STRUCTS_H
#define STRUCTS_H

#define F 20
#define TAMAREA 20

typedef struct TipoRegistro {
    long int numeroIns;
    double nota;
    char estado[3], cidade[50], curso[30];
} TipoRegistro;

typedef struct TipoArea{
    TipoRegistro Vetor[TAMAREA];
    int tam;
} TipoArea;

typedef struct TipoHeap{
    TipoRegistro r;
    int marca;
} TipoHeap;

typedef struct TipoAreaHeap{
    TipoHeap Vetor[TAMAREA];
    int tam;
} TipoAreaHeap;

typedef struct TipoAreaIntercala{
    TipoRegistro Vetor[TAMAREA];
    int tam;
    int fita[TAMAREA];
    int valida[TAMAREA];
} TipoAreaIntercala;

typedef struct v {
    double valor;
    int numeroFita;
} menorValor;

typedef struct f2 {
    int entrada;
    int saida;
} TipoFita;

typedef struct f {
    TipoFita comeco;
    TipoFita fim;
} localizadorFitas;

typedef struct Controle {
    double tempo, tempoPre;
    int transfL, transfE, comp;
    int transfLPre, transfEPre, compPre;
} Controle;
#endif