#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "Structs.h"
#include "QuickSortExterno.h"
#include "IntercalacaoVariosCaminhos.h"

void Criar_Bin(int quantidade, char* nome);
void Imprime_Registro(int quantidade);
void Imprime_Controle(Controle control, int metodo);
void Ordena(int metodo, long int quantidade, int situacao, Controle *control);
void Nome_Arquivo(int situacao, char* nome);

int main(int argc, char* argv[]) {

    if(argc !=4 && argc != 5){ //verificação se a quantidade de parametros é correta
        printf("entrada invalida\n");

        return 1;
    }

    int metodo, situacao;
    long int quantidade;

    metodo = atoi(argv[1]);
    quantidade = atoi(argv[2]);
    situacao = atoi(argv[3]);

    if(metodo<1 || metodo>3){ //verficação se método escolhido é valido
        printf("metodo invalido\n");
        
        return 1;
    }
    if(quantidade<0 || quantidade>471705){ //verificação se a quantidade de registros é valida
        printf("quantidade invalida\n");

        return 1;
    }
    if(situacao<1 || situacao>3){ //verificação se a situação é valida 
        printf("situacao inexistente\n");

        return 1;
    }

    Controle control;
    control.comp = 0;
    control.transfL = 0;
    control.transfE = 0;
    control.compPre = 0;
    control.transfLPre = 0;
    control.transfEPre = 0;

    char *nome = malloc(11 * sizeof(char));
    
    Nome_Arquivo(situacao, nome);

    Criar_Bin(quantidade, nome);

    if(argc == 5){
        //argumento opcional
        if(strcmp(argv[4], "-P") != 0){ //verificação se o argumento opcional é valido
            printf("argumento inválido\n");
           
            return 1;
        }

        printf("\t\t---Arquivo Antes de Ordenar---\n");
        Imprime_Registro(quantidade);
        
        Ordena(metodo, quantidade, situacao, &control);
        printf("\t\t---Arquivo Depois de Ordenar---\n");

        Imprime_Registro(quantidade);
        
        Imprime_Controle(control, metodo);

        free(nome);

        return 0;
    }

    Ordena(metodo, quantidade, situacao, &control);

    Imprime_Controle(control, metodo);
    free(nome);

    return 0;
}

void Criar_Bin(int quantidade, char *nome){

    TipoRegistro R;
    FILE *temp = fopen(nome, "r");
    FILE *arq = fopen("PROVAO.bin", "wb");

    int cont = 0;
    while(cont < quantidade){
        fscanf(temp, "%ld %lf %s\n", &R.numeroIns, &R.nota, R.estado);
        fgets(R.cidade, 50, temp);
        fgets(R.curso, 30, temp);

        fwrite(&R, sizeof(TipoRegistro), 1, arq);
        cont++;
    }

    fclose(temp);
    fclose(arq);
}

void Nome_Arquivo(int situacao, char* nome){
    
    if(situacao == 1)
        strcpy(nome, "PROVAO_ASC.txt");
    else if(situacao == 2)
        strcpy(nome, "PROVAO_DES.txt");
    else
        strcpy(nome, "PROVAO_ALE.txt");
}

void Ordena(int metodo, long int quantidade, int situacao, Controle *control){

    if(metodo == 1){
        //Intercalacao Balancead de Varios caminhos
        Intercalacao_Varios_Caminhos(quantidade, control);
    }
    else if(metodo == 2){
        //Intercalacao Balancead de Varios caminhos, com Selecao
        Intercalacao_Varios_Caminhos_Selecao(quantidade, control);
    }
    else if(metodo == 3){
        //Quick Sort Externo
        Quick_sort_externo(quantidade, control);
    }
}

void Imprime_Registro(int quantidade){
    TipoRegistro R;
    FILE *arq = fopen("PROVAO.bin", "rb");
    int cont = 0;

    while(cont < quantidade){
        fread(&R, sizeof(TipoRegistro), 1, arq);
        printf("%08ld %05.1lf %s %s %s\n", R.numeroIns, R.nota, R.estado, R.cidade, R.curso);

        cont++;
    }
}

void Imprime_Controle(Controle control, int metodo){

    if(metodo == 3){
        printf("\nVariaveis de controle: \n");
        printf("Tempo de execução:        %lf\n", control.tempo);
        printf("Numero de comparações:    %d\n", control.comp);
        printf("Numero de Transf Leitura: %d\n", control.transfL);
        printf("Numero de Transf Escrita: %d\n", control.transfE);
    }
    else{
        printf("\nVariaveis de controle: \n");
        printf("-----Pré Processamento-----\n");
        printf("Tempo de execução:        %lf\n", control.tempoPre);
        printf("Numero de comparações:    %d\n", control.compPre);
        printf("Numero de Transf Leitura: %d\n", control.transfLPre);
        printf("Numero de Transf Escrita: %d\n", control.transfEPre);
        printf("-----Pós Processamento-----\n");
        printf("Tempo de execução:        %lf\n", control.tempo);
        printf("Numero de comparações:    %d\n", control.comp);
        printf("Numero de Transf Leitura: %d\n", control.transfL);
        printf("Numero de Transf Escrita: %d\n", control.transfE);
    }
}