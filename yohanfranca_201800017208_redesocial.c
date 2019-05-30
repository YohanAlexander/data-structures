#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#define MAX 61

uint32_t linhas = 0, i;
char linha[MAX], *pessoa, **entrada;

typedef struct elemento{
    char nome[MAX];
    struct elemento *prev, *next;
} elemento;

typedef struct lista{
    uint32_t tamanho;
    struct elemento *head, *tail;
} lista;

lista* iniciar(){
    
    lista* lista;
    lista = malloc(sizeof(lista)); 
    if(lista == NULL){
        return NULL;
    }
    else{
        lista->head = NULL;
        lista->tail = NULL;
        lista->tamanho = 0;
        return lista;
    }
};

elemento* buscar(lista* lista, char pessoa[MAX]){
   
   elemento* atual = lista->head;
   uint32_t f = 0;
   
   while(f != lista->tamanho){
        if(strcmp(atual->nome, pessoa) == 0){f++; return atual;}
        else{f++; atual = atual->next;}
   }
   return NULL;
};

uint32_t comparar(lista* lista, char pessoa[MAX]){
   
   elemento* atual = lista->head;
   uint32_t f = 0;
   
   while(f != lista->tamanho){
        if(strcmp(atual->nome, pessoa) == 0){f++; return 0;}
        else{f++; atual = atual->next;}
   }
   return -1;
};

uint32_t inserir(lista* lista, char pessoa[MAX]){
    
    elemento* new;
    new = malloc(sizeof(elemento));
    
    if(new == NULL){
        return -1;
    }
    
    else if(lista->head == NULL){
        strcpy(new->nome, pessoa);
        lista->head = lista->tail = new;
        new->prev = new->next = lista->head;
        lista->tamanho++;
        return 0;
    }

    else if(lista-> head != NULL && comparar(lista, pessoa) != 0){
        strcpy(new->nome, pessoa);
        lista->tail->next = new;
        lista->head->prev = new;
        new->next = lista->head;
        new->prev = lista->tail; 
        lista->tail = new;
        lista->tamanho++;
        return 0;
    }

    else{
        return -1;
    }
};

uint32_t remover(lista* lista, char pessoa[MAX]){
    
    elemento* new;

    if(lista->head == NULL){
        return -1;
    }

    else if(strcmp(pessoa, lista->head->nome) == 0){
        new = lista->head;
        lista->head = new->next;
        lista->head->prev = lista->tail;
        lista->tail->next = lista->head;
        lista->tamanho--;
        free(new);
        return 0;
   }

   else if(strcmp(pessoa, lista->tail->nome) == 0){
       new = lista->tail;
       lista->tail = new->prev;
       lista->tail->next = lista->head;
       lista->head->prev = lista->tail;
       lista->tamanho--;
       free(new);
       return 0;
   }

   else if(buscar(lista, pessoa) != NULL){
       new = buscar(lista, pessoa);
       new->prev->next = new->next;
       new->next->prev = new->prev;
       lista->tamanho--;
       free(new);
       return 0;
    }

    else{
        return -1;
    }
};

uint32_t terminal(lista* lista){
    
    if(lista->tamanho == 0){
        printf("NULL\n");
        return -1;
    }
    else{
        elemento* atual = lista->head;
        for(uint32_t j = 0; j < lista->tamanho; j++){
            printf("%s<-%s->%s\n", atual->prev->nome, atual->nome, atual->next->nome);
            printf("%d\n", lista->tamanho);
            atual = atual->next; 
        }
        return 0;
    }
};

int main(int argc, char** argv){
    
    FILE* INPUT = fopen(argv[1], "r");
    FILE* OUTPUT = fopen(argv[2], "w");
    
    while(fgets(linha, MAX, INPUT)){linhas++;}
   
    entrada = (char**)malloc(linhas*sizeof(char*));
    rewind(INPUT);

    while(fgets(linha, MAX, INPUT) != NULL && i <= linhas){
            strtok(linha, "\n");
            entrada[i] = (char*)strdup(linha);
            i++;
    }

    /*char entrada[10][MAX] = {"ADD Jose da Silva",
                "SHOW Jose da Silva",
                "ADD Jose da Silva",
                "ADD Joao dos Santos",
                "ADD Maria da Penha",
                "REMOVE Joao dos Santos",
                "REMOVE Maria da Silva",
                "ADD Alan Turing",
                "SHOW Maria da Penha",
                "SHOW Bruno Prado"};*/
    
    lista* string = iniciar();
    
    for(uint32_t j = 0; j < linhas; j++){
        if(strstr(entrada[j], "ADD")){
            pessoa = entrada[j] + 4;
            if(inserir(string, pessoa) == 0){
                fprintf(OUTPUT, "[ OK  ] ADD %s\n", pessoa);
            }
            else{
                fprintf(OUTPUT, "[ERROR] ADD %s\n", pessoa);
            }
        }
        if(strstr(entrada[j], "REMOVE")){
            pessoa = entrada[j] + 7;
            if(remover(string, pessoa) == 0){
                fprintf(OUTPUT, "[ OK  ] REMOVE %s\n", pessoa);
            }
            else{
                fprintf(OUTPUT, "[ERROR] REMOVE %s\n", pessoa);
            }
        }
        if(strstr(entrada[j], "SHOW")){
            pessoa = entrada[j] + 5;
            if(comparar(string, pessoa) == 0){
                elemento* atual = buscar(string, pessoa);
                fprintf(OUTPUT, "[ OK  ] %s<-%s->%s\n", atual->prev->nome, atual->nome, atual->next->nome);
            }
            else{
                fprintf(OUTPUT, "[ERROR] ?<-%s->?\n", pessoa);
            }
        }
    }

    terminal(string);

    fclose(INPUT);
    fclose(OUTPUT);
    return 0;
}