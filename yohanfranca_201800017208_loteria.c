#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#define MAX 33

typedef struct arquivo arquivo;
typedef struct node node;

struct arquivo{
    int32_t premio, apostas, sorteados[10], apostados[15];
    char codigo[MAX];
};

struct node{
    char codigo[MAX];
    int32_t acertos;
};

node* iniciar(int32_t size){

    node* new = calloc(size, sizeof(node));

    if(new == NULL)
        return NULL;

    else{
        for(int32_t i = 0; i < size; i++){
            strcpy(new[i].codigo, "");
            new[i].acertos = 0;
        }
        return new;
    }

};

void crescente(node* vetor, int32_t tamanho){
    
    for(int32_t j = 0; j < tamanho - 1; j++){

        int32_t iaux;
        char caux[MAX];

        for(int32_t i = 0; i < tamanho - 1; i++){

            if(vetor[i].acertos > vetor[i + 1].acertos){ 
                iaux = vetor[i].acertos;
                strcpy(caux, vetor[i].codigo);
                vetor[i].acertos = vetor[i + 1].acertos;
                strcpy(vetor[i].codigo, vetor[i + 1].codigo);
                vetor[i + 1].acertos = iaux;
                strcpy(vetor[i + 1].codigo, caux);
            }   
        }
    }

};

void decrescente(node* vetor, int32_t tamanho){
    
    for(int32_t j = 0; j < tamanho - 1; j++){

        int32_t iaux;
        char caux[MAX];

        for(int32_t i = 0; i < tamanho - 1; i++){

            if(vetor[i].acertos < vetor[i + 1].acertos){ 
                iaux = vetor[i].acertos;
                strcpy(caux, vetor[i].codigo);
                vetor[i].acertos = vetor[i + 1].acertos;
                strcpy(vetor[i].codigo, vetor[i + 1].codigo);
                vetor[i + 1].acertos = iaux;
                strcpy(vetor[i + 1].codigo, caux);
            }   
        }
    }

};

int main(int argc, char** argv){

    FILE* INPUT = fopen(argv[1], "r");
	FILE* OUTPUT = fopen(argv[2], "w");

    arquivo file;

    fscanf(INPUT, "%d\n", &file.premio);
    fscanf(INPUT, "%d\n", &file.apostas);
    node* heap = iniciar(file.apostas);

    for(int32_t i = 0; i < 10; i++)
        fscanf(INPUT, "%d ", &file.sorteados[i]);

    for(int32_t j = 0; j < file.apostas; j++){
    
        fscanf(INPUT, "%s ", file.codigo);
        
        strcpy(heap[j].codigo, file.codigo);
        
        int32_t sum = 0;

        for(int32_t i = 0; i < 15; i++){

            fscanf(INPUT, "%d ", &file.apostados[i]);

            for(int32_t f = 0; f < 10; f++){
                if(file.sorteados[f] == file.apostados[i])
                    sum += 1;
            }

        }

        heap[j].acertos = sum;

    }

    int32_t gmax = 1, gmin = 1, max = file.premio / 2, min = file.premio / 2, amax = 0, amin = 0;
    
    decrescente(heap, file.apostas);

    amax = heap[0].acertos;
    for(int32_t i = 1; i < file.apostas; i++){
        if(heap[i].acertos == amax)
            gmax++;
    }

    printf("[%d:%d:%d]\n", gmax, amax, max/gmax);

    for(int32_t i = 0; i < gmax; i++)
        printf("%s\n", heap[i].codigo);

    crescente(heap, file.apostas);

    amin = heap[0].acertos;
    for(int32_t i = 1; i < file.apostas; i++){
        if(heap[i].acertos == amin)
            gmin++;
    }

    printf("[%d:%d:%d]\n", gmin, amin, min/gmin);

    for(int32_t i = 0; i < gmin; i++)
        printf("%s\n", heap[i].codigo);

    fclose(INPUT);
    fclose(OUTPUT);

    return 0;
}