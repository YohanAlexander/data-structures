#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#define MAX 101

typedef struct busca busca;
typedef struct servidor servidor;

int32_t count = 0, size = 0, tokens = 0, pesquisas = 0, sum = 0, hsum = 0, dsum = 0;

struct busca{
    char string[MAX];
};

struct servidor{
    int32_t capacidade, tamanho;
    busca* pesquisa;
};

servidor* iniciarservidores(int32_t servidores, int32_t capacidade){

    servidor* server = malloc(servidores*sizeof(servidor));

    for(int32_t i = 0; i < servidores; i++){
        server[i].capacidade = capacidade;
        server[i].tamanho = 0;
        server[i].pesquisa = malloc(capacidade*sizeof(busca));
    }
    
    return server;

};

void inserirbusca(servidor* server, char string[MAX]){

    strcpy(server->pesquisa[server->tamanho].string, string);
    server->tamanho++;

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

    int32_t ascii[MAX], check = 0;

    for(int32_t i = 0; i < strlen(string); i++){

        if(string[i] == ' '){
            ascii[i] = 0;
        }

        else{
            ascii[i] = string[i];
        }

    }

    for(int32_t i = 0; i < strlen(string); i++){
        check = check ^ ascii[i];
    }

    return check;

};

int32_t hash(servidor* server, char string[MAX], FILE* OUTPUT){

    int32_t soma = -1, tentativa = 0, indice = checksum(string);

    while(soma == -1){

        soma = ((7919 * indice) + ((tentativa * 104729 * indice) + 123)) % count;

        if(tentativa == 0){
            hsum = soma;
        }

        if(ocupado(&server[soma]) == 1){
            soma = -1;
        }

        tentativa++;

    }

    dsum = soma;

    if(tentativa > 1){
        fprintf(OUTPUT, "S%d->S%d\n", hsum, dsum);
    }

    return soma;

};

void imprimir(servidor* server, int32_t indice, FILE* OUTPUT){

    fprintf(OUTPUT, "[S%d] ", indice);

    if(server->tamanho == 1){
        fprintf(OUTPUT, "%s\n", server->pesquisa[0].string);
    }

    else{
        for(int32_t i = 0; i < server->tamanho - 1; i++){
            fprintf(OUTPUT, "%s, ", server->pesquisa[i].string);
        }
        fprintf(OUTPUT, "%s\n", server->pesquisa[server->tamanho - 1].string);
    }

};

int main(int argc, char** argv){

    FILE* INPUT = fopen(argv[1], "r");
    FILE* OUTPUT = fopen(argv[2], "w");

    char string[MAX];

    fscanf(INPUT, "%d %d\n", &count, &size);
    printf("Servidores %d\nCapacidade %d\n", count, size);    
    
    servidor* servers = iniciarservidores(count, size);

    fscanf(INPUT, "%d\n", &pesquisas);
    printf("Buscas %d\n", pesquisas);

    for(int32_t i = 0; i < pesquisas; i++){
        fscanf(INPUT, "%d %[^\n]\n", &tokens, string);
        sum = hash(servers, string, OUTPUT);
        inserirbusca(&servers[sum], string);
        imprimir(&servers[sum], sum, OUTPUT);
    }

    fclose(INPUT);
    fclose(OUTPUT);
    return 0;
}