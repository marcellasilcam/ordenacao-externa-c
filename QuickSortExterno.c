#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#include "QuickSortExterno.h"
#include "Structs.h"

void Quick_sort_externo(long int quantidade, Controle *control){

    FILE *ArqLi;
    FILE *ArqEi;
    FILE *ArqLEs;

    ArqLi = fopen("PROVAO.bin", "r+b");
    if(ArqLi == NULL){
        printf("Arquivo nao pode ser aberto\n");
        exit(1);
    }
    ArqEi = fopen("PROVAO.bin", "r+b");
    if(ArqEi == NULL){
        printf("Arquivo nao pode ser aberto\n");
        exit(1);
    }
    ArqLEs = fopen("PROVAO.bin", "r+b");
    if(ArqLEs == NULL){
        printf("Arquivo nao pode ser aberto\n");
        exit(1);
    }
    fflush(ArqLi);
    fflush(ArqEi);
    fflush(ArqLEs);

    double tempo = 0;

    clock_t begin = clock();

    QuickSortExterno(&ArqLi, &ArqEi, &ArqLEs, 1, quantidade, control);

    clock_t end = clock(); //termino do calculo do clock de inserção

    tempo += (double)(end - begin) / CLOCKS_PER_SEC;

    control->tempo = tempo;

    fclose(ArqEi);
    fclose(ArqLEs);
    fclose(ArqLi);
}


void QuickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, long int Dir, Controle *control){
    
    long int i, j;
    TipoArea Area;

    if(Dir - Esq < 1) return;

    FAVazia(&Area);

    Particao(ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j, control);

    fflush(*ArqLi);
    fflush(*ArqEi);
    fflush(*ArqLEs);

    if(i-Esq < Dir-j){
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i, control);
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir, control);
    }
    else{
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir, control);
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i, control);
    }
}

void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, long int *i, long int *j, Controle *control){
    int Ls, Es, Li, Ei, NRArea;
    double Linf, Lsup;
    Ls = Dir;
    Es = Dir;
    Li = Esq;
    Ei = Esq;
    NRArea = 0;
    Linf = INT_MIN;
    Lsup = INT_MAX;

    bool OndeLer = true;
    TipoRegistro UltLido, R;

    fseek(*ArqLi, (Li-1)*sizeof(TipoRegistro), SEEK_SET);
    fseek(*ArqEi, (Li-1)*sizeof(TipoRegistro), SEEK_SET);
    
    *i = Esq - 1;
    *j = Dir + 1;

    while(Ls >= Li){
        if(NRArea < TAMAREA - 1){
            if(OndeLer)
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, control);
            else
                LeInf(ArqLi, &UltLido, &Li, &OndeLer, control);

            InserirArea(&Area, &UltLido, &NRArea, control);
            continue;
        }
        if(Ls == Es)
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, control);
        else if(Li == Ei) 
            LeInf(ArqLi, &UltLido, &Li, &OndeLer, control);
        else if(OndeLer) 
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, control);
        else
            LeInf(ArqLi, &UltLido, &Li, &OndeLer, control);
        
        control->comp = control->comp + 1;
        if(UltLido.nota > Lsup){
            *j = Es; 
            EscreveMax(ArqLEs, UltLido, &Es, control);
            continue;
        }
        control->comp = control->comp + 1;
        if(UltLido.nota < Linf){
            *i = Ei;
            EscreveMin(ArqEi, UltLido, &Ei, control);
            continue;
        }
        
        InserirArea(&Area, &UltLido, &NRArea, control);

        if(Ei - Esq < Dir - Es){
            RetiraMin(&Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei, control);
            Linf = R.nota;
        }
        else{
            RetiraMax(&Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es, control);
            Lsup = R.nota;
        }     
    }
    while(Ei <= Es){
        RetiraMin(&Area, &R, &NRArea);
        EscreveMin(ArqEi, R, &Ei, control);
    }
}

void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, bool *OndeLer, Controle *control){
    fseek(*ArqLEs, (*Ls - 1)*sizeof(TipoRegistro), SEEK_SET);
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLEs);
    (*Ls)--;
    *OndeLer = false;
    control->transfL = control->transfL + 1;
}

void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, bool *OndeLer, Controle *control){
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLi);
    (*Li)++;
    *OndeLer = true;
    control->transfL = control->transfL + 1;
}

void InserirArea(TipoArea *Area, TipoRegistro *UltLido, int *NRArea, Controle *control){
    InsereItem(*UltLido, Area, control);
    *NRArea = ObterNumCelOCupadas(Area);
}

void EscreveMax(FILE **ArqLEs, TipoRegistro R, int *Es, Controle *control){
    fseek(*ArqLEs, (*Es - 1)*sizeof(TipoRegistro), SEEK_SET);
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqLEs);
    (*Es)--;
    control->transfE = control->transfE + 1;
}

void EscreveMin(FILE **ArqEi, TipoRegistro R, int *Ei, Controle *control){
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqEi);
    (*Ei)++;
    control->transfE = control->transfE + 1;
}

void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea){
    RetiraUltimo(Area, R);
    *NRArea = ObterNumCelOCupadas(Area);
}

void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea){
    RetiraPrimeiro(Area, R);
    *NRArea = ObterNumCelOCupadas(Area);
}

void InsereItem(TipoRegistro UltLido, TipoArea *Area, Controle *control){
    Area->Vetor[Area->tam] = UltLido;
    Area->tam = Area->tam + 1;

    TipoRegistro aux;

    int i, j;
    for(i=1; i<Area->tam; i++){
        aux = Area->Vetor[i];
        for(j = i-1; j >=0 && aux.nota < Area->Vetor[j].nota; j--){
            control->comp = control->comp + 1;
            Area->Vetor[j+1] = Area->Vetor[j];
        }
        Area->Vetor[j+1] = aux;
    }
}

void RetiraUltimo(TipoArea *Area, TipoRegistro *R){
    *R = Area->Vetor[Area->tam - 1];
    Area->tam = Area->tam - 1;
}

void RetiraPrimeiro(TipoArea *Area, TipoRegistro *R){
    *R = Area->Vetor[0];
    
    for(int i=1; i<Area->tam; i++){
        Area->Vetor[i-1] = Area->Vetor[i];
    }

    Area->tam = Area->tam - 1;
}

int ObterNumCelOCupadas(TipoArea *Area){
    return Area->tam;
}

void FAVazia(TipoArea *Area){
    Area->tam = 0;
}