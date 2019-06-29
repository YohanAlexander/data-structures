#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#define MAX 51

typedef struct elemento elemento;
typedef struct lista lista;

struct elemento{
    char nome[MAX];
    elemento *prev, *next;
};

struct lista{
    int32_t tamanho;
    elemento *head, *tail;
};

lista* iniciar(){
    
    lista* lista = malloc(sizeof(lista));

    lista->head = NULL;
    lista->tail = NULL;
    lista->tamanho = 0;
    return lista;

};

elemento* buscar(lista* lista, char pessoa[MAX]){
   
   elemento* atual = lista->head;
   int32_t f = 0;
   
   while(f != lista->tamanho){

        if(strcmp(atual->nome, pessoa) == 0){
            f++; 
            return atual;
        }

        else{
            f++; 
            atual = atual->next;
        }
        
   }

   return NULL;

};

int32_t comparar(lista* lista, char pessoa[MAX]){
   
   elemento* atual = lista->head;
   int32_t f = 0;
   
   while(f != lista->tamanho){

        if(strcmp(atual->nome, pessoa) == 0){
            f++; 
            return 0;
        }

        else{
            f++; 
            atual = atual->next;
        }

   }

   return -1;

};

int32_t inserir(lista* lista, char pessoa[MAX]){
    
    elemento* new = malloc(sizeof(elemento));
    
    if(lista->head == NULL){
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

int32_t remover(lista* lista, char pessoa[MAX]){
    
    elemento* new;

    if(strcmp(pessoa, lista->head->nome) == 0){
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

int main(int argc, char** argv){
    
    FILE* INPUT = fopen(argv[1], "r");
    FILE* OUTPUT = fopen(argv[2], "w");

    char pessoa[MAX], token[MAX];
    
    lista* string = iniciar();
    
    while(fscanf(INPUT, "%s %[^\n]\n", token, pessoa) != EOF){

        if(strcmp(token, "ADD") == 0){

            if(inserir(string, pessoa) == 0){
                fprintf(OUTPUT, "[ OK  ] ADD %s\n", pessoa);
            }

            else{
                fprintf(OUTPUT, "[ERROR] ADD %s\n", pessoa);
            }

        }

        if(strcmp(token, "REMOVE") == 0){

            if(remover(string, pessoa) == 0){
                fprintf(OUTPUT, "[ OK  ] REMOVE %s\n", pessoa);
            }

            else{
                fprintf(OUTPUT, "[ERROR] REMOVE %s\n", pessoa);
            }

        }

        if(strcmp(token, "SHOW") == 0){

            if(comparar(string, pessoa) == 0){

                elemento* atual = buscar(string, pessoa);
                fprintf(OUTPUT, "[ OK  ] %s<-%s->%s\n", atual->prev->nome, atual->nome, atual->next->nome);
            
            }

            else{
                fprintf(OUTPUT, "[ERROR] ?<-%s->?\n", pessoa);
            }

        }

    }

    fclose(INPUT);
    fclose(OUTPUT);
    return 0;
}