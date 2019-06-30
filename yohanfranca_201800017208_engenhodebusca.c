#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#define MAX 101

typedef struct busca busca;
typedef struct servidor servidor;

char string[MAX];
static int32_t count = 0, size = 0, tokens = 0, pesquisas = 0;
int32_t sum = 0, hsum = 0, dsum = 0, indice = 0;

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
        server[i].pesquisa = calloc(capacidade,sizeof(busca));
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

    int32_t ascii = 0, check = 0;

    for(int32_t i = 0; i < strlen(string); i++){

        if(string[i] == ' '){
            ascii = 0;
        }

        else{
            ascii = string[i];
        }

        check = check ^ ascii;

    }

    return check;

};

int32_t hash(servidor* server, int32_t index, int32_t try, int32_t cont){

    sum = ((7919 * index) + ((try * 104729) * (index + 123))) % cont;

    if(try == 0){

        hsum = sum;
    
    }
    
    if(ocupado(&server[sum]) == 1){

        return hash(server, index, try + 1, cont);
    
    }

    dsum = sum;

    return sum;

};

void imprimir(servidor* server, int32_t index, FILE* OUTPUT){

    fprintf(OUTPUT, "[S%d] ", index);

    for(int32_t i = 0; i < server->tamanho - 1; i++){

        fprintf(OUTPUT, "%s, ", server->pesquisa[i].string);
    
    }
    
    fprintf(OUTPUT, "%s\n", server->pesquisa[server->tamanho - 1].string);

};

int main(int argc, char** argv){

    FILE* INPUT = fopen(argv[1], "r");
    FILE* OUTPUT = fopen(argv[2], "w");

    fscanf(INPUT, "%d %d\n", &count, &size);
    printf("Servidores %d\nCapacidade %d\n", count, size);    
    
    servidor* servers = iniciarservidores(count, size);

    fscanf(INPUT, "%d\n", &pesquisas);
    printf("Buscas %d\n", pesquisas);

    for(int32_t i = 0; i < pesquisas; i++){
        
        fscanf(INPUT, "%d %[^\n]\n", &tokens, string);
        
        indice = checksum(string);
        
        sum = hash(servers, indice, 0, count);
        
        inserirbusca(&servers[sum], string);
        
        if(hsum != dsum){

            fprintf(OUTPUT, "S%d->S%d\n", hsum, dsum);
        
        }

        imprimir(&servers[sum], sum, OUTPUT);
    
    }

    fclose(INPUT);
    fclose(OUTPUT);
    return 0;
}