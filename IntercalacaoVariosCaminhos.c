#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#include "IntercalacaoVariosCaminhos.h"
#include "Structs.h"

void Intercalacao_Varios_Caminhos_Selecao(long int quantidade, Controle *control){

    char** nomeFita = CriaNomeFitas();
    
    FILE* fitas[2*F];
    for (int i = 0; i < 2*F; i++)
        fitas[i] = fopen(nomeFita[i], "w+b");   

    FILE *arq = fopen("PROVAO.bin", "r+b");

    int ultimaFitaEntrada, fitaFinal;
    int blocos = 0;

    double tempoP = 0;

    clock_t beginP = clock();

    ultimaFitaEntrada = CriaBlocosSelecao(arq, fitas, quantidade, &blocos, control);

    clock_t endP = clock();

    tempoP += (double)(endP - beginP) / CLOCKS_PER_SEC;

    control->tempoPre = tempoP;
    if(blocos >= 20)
        ultimaFitaEntrada = 20;
    for (int i = 0; i < 2*F; i++)
        rewind(fitas[i]);

    double tempoI = 0;

    clock_t beginI = clock();

    fitaFinal = Intercala0_19(fitas, 0, 20, ultimaFitaEntrada, blocos, control);

    clock_t endI = clock(); 

    tempoI += (double)(endI - beginI) / CLOCKS_PER_SEC;
    
    control->tempo = tempoI;

    rewind(fitas[fitaFinal]);
    rewind(arq);

    TipoRegistro R;
    for(int i=0; i<quantidade; i++){
        fread(&R, sizeof(TipoRegistro), 1, fitas[fitaFinal]);
        fwrite(&R, sizeof(TipoRegistro), 1, arq);
    }

    for (int i = 0; i < 2*F; i++)
        fclose(fitas[i]);     
    fclose(arq);
}

void Intercalacao_Varios_Caminhos(long int quantidade, Controle *control){
    
    char** nomeFita = CriaNomeFitas(); //criamos os nomes identificando com qual fita estamos lidando
    
    FILE* fitas[2*F];
    for (int i = 0; i < 2*F; i++)
        fitas[i] = fopen(nomeFita[i], "w+b"); //ponteiro para cada fita 

    FILE *arq = fopen("PROVAO.bin", "r+b");

    int fitaFinal; //receberá o número da fita que contém o arquivo ordenado

    CriaBlocos(arq, fitas, quantidade, control, &fitaFinal); //criando os blocos iniciais nas fitas de entrada

    rewind(fitas[fitaFinal]); //reposicionando o ponteiro para o início do arquivo
    rewind(arq);

    TipoRegistro R;
    for(int i=0; i<quantidade; i++){ //mostrando a fita com o arquivo ordenado
        fread(&R, sizeof(TipoRegistro), 1, fitas[fitaFinal]);
        fwrite(&R, sizeof(TipoRegistro), 1, arq);
    }

    for (int i = 0; i < 2*F; i++) //fechando cada uma das fitas
        fclose(fitas[i]);      
    fclose(arq); //fechando o arquivo original

}

char** CriaNomeFitas (){
    //criamos todos os arquivos com o padrão que preferimos para referenciar as fitas
    char** nomeFita = (char**) malloc(40 * sizeof(char*));
    for (int i = 0; i < 2 * F; i++){
        nomeFita[i] = (char*) malloc(12 * sizeof(char));
        nomeFita[i][0] = 'f';
        nomeFita[i][1] = 'i';
        nomeFita[i][2] = 't';
        nomeFita[i][3] = 'a';
        nomeFita[i][4] = '_';
        if (i < 10){
            nomeFita[i][5] = i + '0';
            nomeFita[i][6] = '.';
            nomeFita[i][7] = 'b';
            nomeFita[i][8] = 'i';
            nomeFita[i][9] = 'n';
            nomeFita[i][10] = '\0';
        }
        else if (i < 20){
            nomeFita[i][5] = '1';
            nomeFita[i][6] = (i - 10) + '0';
            nomeFita[i][7] = '.';
            nomeFita[i][8] = 'b';
            nomeFita[i][9] = 'i';
            nomeFita[i][10] = 'n';
            nomeFita[i][11] = '\0';        
        }
        else if (i < 30){
            nomeFita[i][5] = '2';
            nomeFita[i][6] = (i - 20) + '0';
            nomeFita[i][7] = '.';
            nomeFita[i][8] = 'b';
            nomeFita[i][9] = 'i';
            nomeFita[i][10] = 'n';
            nomeFita[i][11] = '\0';
        }
        else {
            nomeFita[i][5] = '3';
            nomeFita[i][6] = (i - 30) + '0';
            nomeFita[i][7] = '.';
            nomeFita[i][8] = 'b';
            nomeFita[i][9] = 'i';
            nomeFita[i][10] = 'n';
            nomeFita[i][11] = '\0';
        }
    }

    return nomeFita;
}

void DesalocaNomeFitas(char** nomeFitas){
    //desalocando todos os ponteiros para as fitas
    for (int i = 0; i < 2*F; i++){
        free(nomeFitas[i]);
    }
    free(nomeFitas);
}

int CriaBlocosSelecao(FILE *arq, FILE **fitas, int tam, int *blocos, Controle *control){

    TipoRegistro R;
    TipoRegistro aux;

    aux.nota = INT_MIN;

    int cont = 0;
    int marcados = 0;
    int fit = 0;
    TipoAreaHeap A;
    A.tam = 0;

    while(cont < tam){
        fread(&R, sizeof(TipoRegistro), 1, arq);
        control->transfLPre = control->transfLPre + 1; 

        if(R.nota < aux.nota){
            A.Vetor[A.tam].r = R;
            A.Vetor[A.tam].marca = 1;
            A.tam++;
        }
        else{
            A.Vetor[A.tam].r = R;
            A.Vetor[A.tam].marca = 0;
            A.tam++;
        }

        HeapSort(A.Vetor, A.tam, control);

        if(A.tam == F){
            marcados = 0;
            for(int i=0; i<F; i++){
                if(A.Vetor[i].marca == 1)
                    marcados++;
            }

            if(marcados == 20){
                for(int i=0; i<F; i++){
                    A.Vetor[i].marca = 0;
                }
                aux.nota = -1;
                fwrite(&aux, sizeof(TipoRegistro), 1, fitas[fit]);
                (*blocos)++;
                fit++;
                if(fit == F)
                    fit = 0;
            }

            aux = A.Vetor[0].r;
            fwrite(&aux, sizeof(TipoRegistro), 1, fitas[fit]);
            control->transfEPre = control->transfEPre + 1; 

            for(int i=1; i<A.tam; i++){
                A.Vetor[i-1] = A.Vetor[i];
            }
            A.tam--;
        }
        cont++;
    }

    int i;
    for(i=0; i<A.tam; i++){
        if(A.Vetor[i].marca == 1)
            break;
        fwrite(&A.Vetor[i].r, sizeof(TipoRegistro), 1, fitas[fit]);
        control->transfEPre = control->transfEPre + 1; 
    }
    aux.nota = -1;
    fwrite(&aux, sizeof(TipoRegistro), 1, fitas[fit]);
    (*blocos)++;

    fit++;
    if(fit == 20)
        fit = 0;
    
    int verif = 0;
    
    for( ; i<A.tam; i++){
        verif = 1;
        fwrite(&A.Vetor[i].r, sizeof(TipoRegistro), 1, fitas[fit]);
        control->transfEPre = control->transfEPre + 1; 
    }

    if(verif){
        aux.nota = -1;
        fwrite(&aux, sizeof(TipoRegistro), 1, fitas[fit]);
        fit++;
        (*blocos)++;
    }

    return fit;

}

void CriaBlocos(FILE* arqBin, FILE* fitas[2*F], int tamanhoArq, Controle* control, int* fitaFinal){
    clock_t beginPre = clock(); //inicio contagem de clock
    //variáveis auxiliares:
    TipoRegistro* registro = (TipoRegistro*) malloc(TAMAREA * sizeof(TipoRegistro)); int j = 0, i = 1; 
    int numeroDeBlocos = 0, confere = 0, tamanho = 0; localizadorFitas localizador; TipoRegistro fimDeBloco;

    //"zerando" registro que indixará o fim de um bloco
    fimDeBloco.numeroIns = -1;
    fimDeBloco.nota = -1.0;
    fimDeBloco.estado[0] = '\0';
    fimDeBloco.cidade[0] = '\0';
    fimDeBloco.curso[0] = '\0';
    
    //"zerando" a memória principal
    for (int i = 0; i < TAMAREA; i++){
        registro[i].numeroIns = -1;
        registro[i].nota = -1.0;
        registro[i].estado[0] = '\0';
        registro[i].cidade[0] = '\0';
        registro[i].curso[0] = '\0';
    } 

    i = 1;
    while(fread(&registro[0], sizeof(TipoRegistro), 1, arqBin)){
        //ordenando a memória principal com o insertion sort
        OrdenaMemInterna(registro, control);

        if (i == 20){
            //caso já tenha lido 20 registros, armazeno eles na fita de saída
            for(int k = 0; k < 20; k++){
                fwrite(&registro[k], sizeof(TipoRegistro), 1, fitas[j]);
                control->transfEPre++;
            }

            //identifico o final de um bloco
            fwrite(&fimDeBloco, sizeof(TipoRegistro), 1, fitas[j]);
            
            control->transfLPre += i;
            i = 0; numeroDeBlocos++;

            //zerando novamente a memória principal
            for (int i = 0; i < TAMAREA; i++){
                registro[i].numeroIns = -1;
                registro[i].nota = -1.0;
                registro[i].estado[0] = '\0';
                registro[i].cidade[0] = '\0';
                registro[i].curso[0] = '\0';
            }
            
            j++;
            if (j == F) //ciclo de 0 a 19 
                j = 0;
        }

        if (tamanho == tamanhoArq-1) //todos os registros necessários já foram lidos
            break;
    
        i++; tamanho++;
    }

    control->transfLPre += i;

    //se a memória interna ainda tem registros para serem armazenados em fita
    for (int k = 0; k < TAMAREA; k++){
        if (registro[k].nota != -1.0)
            confere = 1;
    }

    if (confere == 1){ //armazenando na fita os registros que sobraram
        for(int k = 0; k < 20; k++){
            if(registro[k].nota != -1){
                fwrite(&registro[k], sizeof(TipoRegistro), 1, fitas[j]);
                control->transfEPre++;
            }
        }
        
        //identificando final de bloco
        fwrite(&fimDeBloco, sizeof(TipoRegistro), 1, fitas[j]);

        numeroDeBlocos++;
    }

    //fim da contagem de clock
    clock_t endPre = clock(); 

    double tempo = 0;
    tempo += (double)(endPre - beginPre) / CLOCKS_PER_SEC; //calculo do tempo da execução do pré-processamento
    control->tempoPre = tempo;

    localizador.comeco.entrada = 0; //inicio das fitas de entrada

    //fim das fitas de entrada
    if (numeroDeBlocos >  F){
        localizador.fim.entrada = F;
    }
    else {
        localizador.fim.entrada = numeroDeBlocos;
    }

    //inicio e fim das fitas de saida
    localizador.comeco.saida = F; localizador.fim.saida = 2*F;

    clock_t beginI = clock(); //inicio da contagem de clock
    Intercala(fitas, &numeroDeBlocos, tamanhoArq, &localizador, fitaFinal, control); //intercalando os blocos
    clock_t endI = clock(); //fim da contagem de clock
    tempo = 0;
    tempo += (double)(endI - beginI) / CLOCKS_PER_SEC; //calculo do tempo de execução da intercalação
    control->tempo = tempo;
} 

void HeapSort(TipoHeap *v, int n, Controle *control){    
    TipoHeap aux;

    HeapConstroi(v, n, control);
    while(n>1){
        aux = v[n-1];
        v[n-1] = v[0];
        v[0] = aux;
        n = n-1;
        HeapRefaz(v, 0, n-1, control);
    }
}

void HeapConstroi(TipoHeap *v, int n, Controle *control){
    int esq = (n/2)-1;
    while(esq>=0){
        HeapRefaz(v, esq, n-1, control);
        esq--;
    }
}

void HeapRefaz(TipoHeap *v, int esq, int dir, Controle *control){
    int i, j;
    TipoHeap aux;
    i = esq;
    j = i*2 + 1;
    aux = v[i];

    while(j<=dir){
        control->compPre = control->compPre + 2;
        if(j<dir && Compara(v[j], v[j+1]) == 0)
            j=j+1;
        if(Compara(aux, v[j]))
            break;
        v[i] = v[j];
        i = j;
        j = i*2 + 1;    
    }
    v[i] = aux;
}

int Compara(TipoHeap valor1, TipoHeap valor2){
	if(valor1.marca == valor2.marca){
		if(valor1.r.nota >= valor2.r.nota){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		if(valor1.marca != 0){
			return 1;
		}
		else{
			return 0;
		}
	}
}

int Intercala0_19(FILE **fitas, int entrada, int saida, int fim_entrada, int blocosTotais, Controle *control){

    TipoRegistro R;
    TipoAreaIntercala Area;
    Area.tam = 0;

    for(int i=entrada; i<fim_entrada; i++){
        fread(&R, sizeof(TipoRegistro), 1, fitas[i]);
        control->transfL = control->transfL + 1;

        Area.Vetor[Area.tam] = R;
        Area.fita[Area.tam] = i;
        Area.valida[Area.tam] = 1;
        Area.tam++;
    }


    double minimo;
    int fita_minimo, posicao, contador = 0, valida, blocosLidos = 0, blocos = 0;
    TipoRegistro registro_minimo;

    while(1){
        minimo = 1000;

        for(int i=0; i<Area.tam; i++){
            control->comp = control->comp + 1;

            if(Area.Vetor[i].nota < minimo){
                minimo = Area.Vetor[i].nota;
                registro_minimo = Area.Vetor[i];
                fita_minimo = Area.fita[i];
                valida = Area.valida[i];
                posicao = i;
            }
        }

        fwrite(&registro_minimo, sizeof(TipoRegistro), 1, fitas[saida]);
        control->transfE = control->transfE + 1;

        if(valida){
            fread(&R, sizeof(TipoRegistro), 1, fitas[fita_minimo]);
            control->transfL = control->transfL + 1;

            if(R.nota == -1){
                Area.valida[posicao] = 0;
                Area.Vetor[posicao].nota = INT_MAX;
                blocosLidos++;
            }
            else{
                Area.Vetor[posicao] = R;
            }
        }

        contador = 0;
        for(int i=0; i<Area.tam; i++){
            if(Area.valida[i] == 0){
                contador++;
            }
        }

        if(contador == fim_entrada){
            registro_minimo.nota = -1;
            fwrite(&registro_minimo, sizeof(TipoRegistro), 1, fitas[saida]);
            blocos++;
            if(blocosLidos < blocosTotais){//numero de blocos ja intercala < numero de blocos totais
                
                Area.tam = 0;
                int final;
                if(blocosTotais - blocosLidos >= 20)
                    final = 20;
                else
                    final = blocosTotais - blocosLidos;
                for(int i=entrada; i<final; i++){
                    fread(&R, sizeof(TipoRegistro), 1, fitas[i]);
                    control->transfL = control->transfL + 1;

                    Area.Vetor[Area.tam] = R;
                    Area.fita[Area.tam] = i;
                    Area.valida[Area.tam] = 1;
                    Area.tam++;
                }
                saida++;
                if(saida == 40)
                    saida = 20;
                fim_entrada = final;
            }
            else
                break;
        }
    }

    if(blocos > 1){
        for (int i = 0; i < 2*F; i++)
            rewind(fitas[i]);

        saida++;
        return Intercala20_39(fitas, 20, 0, saida, blocos, control);
    }
    return saida;
}

int Intercala20_39(FILE **fitas, int entrada, int saida, int fim_entrada, int blocosTotais, Controle *control){

    TipoRegistro R;

    TipoAreaIntercala Area;
    Area.tam = 0;

    for(int i=entrada; i<fim_entrada; i++){
        fread(&R, sizeof(TipoRegistro), 1, fitas[i]);
        control->transfL = control->transfL + 1;

        Area.Vetor[Area.tam] = R;
        Area.fita[Area.tam] = i;
        Area.valida[Area.tam] = 1;
        Area.tam++;
    }


    double minimo;
    int fita_minimo, posicao, contador = 0, valida, blocosLidos = 0, blocos = 0;
    TipoRegistro registro_minimo;

    while(1){
        minimo = 1000;

        for(int i=0; i<Area.tam; i++){
            if(Area.Vetor[i].nota < minimo){
                control->comp = control->comp + 1;

                minimo = Area.Vetor[i].nota;
                registro_minimo = Area.Vetor[i];
                fita_minimo = Area.fita[i];
                valida = Area.valida[i];
                posicao = i;
            }
        }

        fwrite(&registro_minimo, sizeof(TipoRegistro), 1, fitas[saida]);
        control->transfE = control->transfE + 1;

        if(valida){
            fread(&R, sizeof(TipoRegistro), 1, fitas[fita_minimo]);
            control->transfL = control->transfL + 1;

            if(R.nota == -1){
                Area.valida[posicao] = 0;
                Area.Vetor[posicao].nota = INT_MAX;
                blocosLidos++;
            }
            else{
                Area.Vetor[posicao] = R;
            }
        }

        contador = 0;
        for(int i=0; i<Area.tam; i++){
            if(Area.valida[i] == 0){
                contador++;
            }
        }

        if(contador == fim_entrada - 20){
            registro_minimo.nota = -1;
            fwrite(&registro_minimo, sizeof(TipoRegistro), 1, fitas[saida]);
            blocos++;
            if(blocosLidos < blocosTotais){//numero de blocos ja intercala < numero de blocos totais
                
                Area.tam = 0;
                int final;
                if(blocosTotais - blocosLidos >= 20)
                    final = 40;
                else
                    final = blocosTotais - blocosLidos + 20;
                for(int i=entrada; i<final; i++){
                    fread(&R, sizeof(TipoRegistro), 1, fitas[i]);
                    control->transfL = control->transfL + 1;

                    Area.Vetor[Area.tam] = R;
                    Area.fita[Area.tam] = i;
                    Area.valida[Area.tam] = 1;
                    Area.tam++;
                }
                saida++;
                if(saida == 20)
                    saida = 0;
                fim_entrada = final;
            }
            else
                break;
        }
    }

    if(blocos > 1){
        for (int i = 0; i < 2*F; i++)
            rewind(fitas[i]);

        saida++;
        return Intercala0_19(fitas, 0, 20, saida, blocos, control);
    }
    
    return saida;
}

void Intercala(FILE* fitas[2*F], int* numeroDeBlocos, int tamanhoArq, localizadorFitas* localizador, int* fitaFinal, Controle* control){ 
    TipoRegistro mem_principal[TAMAREA]; //vetor da memória interna
    int aux = 0, contador = 0, controle = 0, zero;
    TipoRegistro position; //variáveis auxiliares
    
    for (int i = 0; i < 2*F; i++){ //reposicionando os ponteiros para o início das fitas
        fseek(fitas[i], 0, SEEK_SET);
    }

    *numeroDeBlocos = 0; int j = localizador->comeco.saida;

    while (j < localizador->fim.saida){ // escrevendo nas fitas de saida

        zero = localizador->comeco.entrada; aux = 0;

        for (int i = 0; i < TAMAREA; i++){ //"zerando" a memória principal 
            mem_principal[i].nota = -1.0;
        }

        //MONTANDO O VETOR INTERNO 
        for (int i = 0; i < TAMAREA; i++){ //tamanho do vetor interno

            if (zero >= localizador->fim.entrada){
                break;
            }

            if (fread(&position, sizeof(TipoRegistro), 1, fitas[zero])){ //trazendo o primeiro elemento do bloco da fita para memória principal
                control->transfL++;
                mem_principal[i] = position;
            }

            else { //não há elemento para trazer
                aux++;
            }
            zero++;
        }


        if (aux == F){ //se não há mais bloco nas fitas
            break;
        }

        menorValor menor_valor; menor_valor.numeroFita = 0;

        contador = 0; 
        *numeroDeBlocos = *numeroDeBlocos + 1;
        for (int i = 0; i < TAMAREA; i++){ //"zerando" variável que marcará fim do bloco
            position.numeroIns = -1;
            position.nota = -1.0;
            position.estado[0] = '\0';
            position.cidade[0] = '\0';
            position.curso[0] = '\0';
        }

        while (aux != F){ //enquanto ainda haver registros na memória principal
            aux = 0; menor_valor.valor = 1.79769e+308;

            for (int i = 0; i < TAMAREA; i++){
                control->comp++;
                if (mem_principal[i].nota != -1.0 && mem_principal[i].nota <= menor_valor.valor){ //pegando a menor nota dos registros da memória principal
                    menor_valor.valor = mem_principal[i].nota;
                    menor_valor.numeroFita = i;
                }
            }

            int indiceVet = localizador->comeco.entrada + menor_valor.numeroFita; //índice da fita que veio o menor registro

            fwrite(&mem_principal[menor_valor.numeroFita], sizeof(TipoRegistro), 1, fitas[j]); //escrevendo na fita de saída menor elemento encontrado
            if (mem_principal[menor_valor.numeroFita].nota != -1.0){
                control->transfE++;
            }

            contador++; controle++;
            if (controle == tamanhoArq){
                break;
            }
            
            fread(&mem_principal[menor_valor.numeroFita], sizeof(TipoRegistro), 1, fitas[indiceVet]); //trazendo pra memória principal o próximo registro da fita de entrada
            if (mem_principal[menor_valor.numeroFita].nota != -1.0){
                control->transfL++;
            }
            
            for (int i = 0; i < TAMAREA; i++){
                if (mem_principal[i].nota == -1){ //se o bloco já tiver acabado
                    aux++;
                }
            }
        }

        if (contador == tamanhoArq){ //todo o arquivo foi ordenado
            *fitaFinal = j;
            return;
        }

        fwrite(&position, sizeof(TipoRegistro), 1, fitas[j]); //sinaliza um fim de bloco

        if (controle == tamanhoArq){ //todos os registros das fitas de entrada já foram processados
            break;
        }

        j++; 
        if (j == localizador->fim.saida){ //voltando a escrever na primeira fita de saída
            j = localizador->comeco.saida;
        }
    }

    //ajustando os comecos e fins para a próxima intercalação
    if (localizador->comeco.entrada == 0){
        localizador->comeco.saida = 0; localizador->fim.saida = F;
        localizador->comeco.entrada = F; 

        if (*numeroDeBlocos >  F){
            localizador->fim.entrada = localizador->comeco.entrada + F;
        }
        else {
            localizador->fim.entrada = localizador->comeco.entrada + *numeroDeBlocos;
        }
        
    }
    else { //tambloco comeco entrada == F
        localizador->comeco.saida = F; localizador->fim.saida = 2*F;
        localizador->comeco.entrada = 0; 

        if (*numeroDeBlocos >  F){
            localizador->fim.entrada = localizador->comeco.entrada + F;
        }
        else {
            localizador->fim.entrada = localizador->comeco.entrada + (*numeroDeBlocos);
        }
    }

    //recursão para próxima intercalação
    Intercala(fitas, numeroDeBlocos, tamanhoArq, localizador, fitaFinal, control);
}

void OrdenaMemInterna(TipoRegistro* registro, Controle* control){
    //método insertion sort - insere o registro no devido lugar e "arreda" os outros para frente
    TipoRegistro aux; int j;
    for (int i = 1; i < TAMAREA; i++){
        aux = registro[i];
        j = i - 1;
        while (j >= 0 && aux.nota < registro[j].nota){
            control->compPre++;
            registro[j+1] = registro[j];
            j--;
        }
        registro[j+1] = aux;
    }
}