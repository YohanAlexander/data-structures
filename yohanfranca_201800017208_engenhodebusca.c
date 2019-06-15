#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#define MAX 103

typedef struct arquivo arquivo;
typedef struct busca busca;
typedef struct servidor servidor;

int32_t count = 0, sum = 0, hsum = 0, dsum = 0;
servidor* servers = NULL;

struct arquivo{
    int32_t servidores, capacidade, buscas, tokens;
    char string[MAX];
};

struct busca{
    char string[MAX];
    busca* next;
};

struct servidor{
    int32_t capacidade, tamanho;
    busca *head, *tail;
};

servidor* iniciarservidores(int32_t size, int32_t capacidade){

    servidor* servers;

    servers = malloc(size*sizeof(servidor)); 
    
    if(servers == NULL){
        return NULL;
    }
    else{
        for(int32_t i = 0; i < size; i++){
            servers[i].capacidade = capacidade;
            servers[i].tamanho = 0;
            servers[i].head = NULL;
            servers[i].tail = NULL;
        }
        return servers;
    }
};

int32_t ocupado(servidor* server){
    if(server->tamanho < server->capacidade){
        return 0;
    }
    else{
        return 1;
    }
};

int32_t checksum(char string[MAX]){
    int32_t ascii[MAX], hash = 0;
    for(int32_t i = 0; i < strlen(string); i++){
        if(string[i] == ' '){
            ascii[i] = 0;
        } 
        else{
            ascii[i] = string[i];
        }
    }
    for(int32_t i = 0; i < strlen(string); i++){
        hash = hash ^ ascii[i];
    }
    return hash;
};

void imprimir(int32_t index, FILE* OUTPUT){

    busca* pesquisa = malloc(sizeof(busca)); 
    pesquisa = servers[index].head; 

    fprintf(OUTPUT, "[S%d] ", index);

    while(pesquisa->next != NULL){
        fprintf(OUTPUT, "%s, ", pesquisa->string);
        pesquisa = pesquisa->next;
    }
    fprintf(OUTPUT, "%s\n", pesquisa->string);

};

int32_t hash(char string[MAX], FILE* OUTPUT){

    int32_t sum = -1, tentativas = 0;

    for(uint32_t i = 0; sum == -1; i++){
        sum = ((7919 * checksum(string)) + ((i * 104729 * checksum(string)) + 123)) % count;
        if(i == 0){
            hsum = sum;
        }
        if(ocupado(&servers[sum]) == 1){
            sum = -1;
        }
        tentativas++;
    }

    dsum = sum;

    if(tentativas > 1){
        fprintf(OUTPUT, "S%d->S%d\n", hsum, dsum);
    }  
    printf("[hash] %d\n", sum);
    return sum;
};

void inserirbusca(servidor* server, char string[MAX]){
    
    busca* pesquisa;
    pesquisa = malloc(sizeof(busca));
    pesquisa->next = NULL;
    
    if(server->head == NULL){
        strcpy(pesquisa->string, string);
        server->head = server->tail = pesquisa;
    }
    
    else{
        strcpy(pesquisa->string, string);
        server->tail->next = pesquisa;
        server->tail = pesquisa;
    }
    server->tamanho++;
};

int main(int argc, char** argv){

    FILE* INPUT = fopen(argv[1], "r");
    FILE* OUTPUT = fopen(argv[2], "w");

    arquivo entrada;
    
    fscanf(INPUT, "%d %d\n", &entrada.servidores, &entrada.capacidade);
    printf("%d %d\n", entrada.servidores, entrada.capacidade);    
    
    servers = iniciarservidores(entrada.servidores, entrada.capacidade);
    count = entrada.servidores;

    fscanf(INPUT, "%d\n", &entrada.buscas);
    printf("%d\n", entrada.buscas);

    for(int32_t i = 0; i < entrada.buscas; i++){
        fscanf(INPUT, "%d %[^\n]\n", &entrada.tokens, entrada.string);
        printf("(%s) ", entrada.string);
        printf("%d ", checksum(entrada.string));
        sum = hash(entrada.string, OUTPUT);
        inserirbusca(&servers[sum], entrada.string);
        imprimir(sum, OUTPUT);
    }

    fclose(INPUT);
    fclose(OUTPUT);
    return 0;
}