#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#define MAX 101

typedef struct arquivo arquivo;
typedef struct documento documento;
typedef struct fila fila;
typedef struct pilha pilha;
typedef struct impressora impressora;

int32_t soma = 0, vazia = 0, len = 0;
impressora* printers = NULL;
fila* documentos = NULL;
pilha* entrega = NULL;

struct arquivo{
    int32_t printers, files, paginas;
    char impressora[MAX], documento[MAX];
};

struct documento{
    char* nome[MAX];
    int32_t paginas, print;
};

struct fila{
    int32_t capacidade, head, tail, tamanho;
    documento* doc;
};

struct pilha{
    int32_t capacidade, topo;
    documento* doc;
};

struct impressora{
    char nome[MAX];
    pilha* stack;
};

fila* iniciarfila(int32_t size){

    fila* fila;
    fila = malloc(sizeof(fila)); 
    if(fila == NULL){
        return NULL;
    }
    else{
        fila->capacidade = size;
        fila->head = 0;
        fila->tail = 0;
        fila->tamanho = 0;
        fila->doc = malloc(size*sizeof(documento));
        return fila;
    }
};

pilha* iniciarpilha(int32_t size){

    pilha* pilha;
    pilha = malloc(sizeof(pilha)); 
    if(pilha == NULL){
        return NULL;
    }
    else{
        pilha->capacidade = size;
        pilha->topo = -1;
        pilha->doc = malloc(size*sizeof(documento));
        return pilha;
    }
};

int32_t enfileirar(fila* fila, documento* doc){

    if(fila->doc == NULL){
        return -1;
    }

    else if(fila->tamanho == 0 || fila->tamanho != 0){
        strcpy((char * restrict)fila->doc[((fila->head + fila->tail) % fila->capacidade)].nome, (char * restrict)doc->nome);
        fila->doc[((fila->head + fila->tail) % fila->capacidade)].paginas = doc->paginas;
        fila->doc[((fila->head + fila->tail) % fila->capacidade)].print = doc->print;
        fila->tail = ((fila->tail + 1) % fila->capacidade);
        fila->tamanho++;
        return 0;
    }

    else{
        return -1;
    }
};

int32_t desenfilerirar(fila* fila){

    if(fila->doc == NULL){
        return -1;
    }

    else if(fila->capacidade == fila->tamanho || fila->capacidade != fila->tamanho){
        documento atual;
        atual = fila->doc[fila->head];
        strcpy((char * restrict)atual.nome, (char * restrict)"");
        atual.paginas = 0;
        fila->head = ((fila->head + 1) % fila->capacidade);
        fila->tamanho--;
        return 0;
    }

    else{
        return -1;
    }
};

int32_t empilhar(pilha* pilha, documento* doc){

    if(pilha->doc == NULL){
        return -1;
    }

    else if(pilha->topo == -1 || pilha->topo != -1){
        pilha->topo++;
        strcpy((char* restrict)&pilha->doc[pilha->topo].nome, (char * restrict)doc->nome);
        pilha->doc[pilha->topo].paginas = doc->paginas;
        pilha->doc[pilha->topo].print = doc->print;
        return 0;
    }

    else{
        return -1;
    }
};

int32_t desempilhar(pilha* pilha){

    if(pilha->doc == NULL){
        return -1;
    }

    else if(pilha->topo == (pilha->capacidade - 1) || pilha->topo != (pilha->capacidade - 1)){
        documento atual;
        atual = pilha->doc[pilha->topo];
        strcpy((char * restrict)atual.nome, (char * restrict)"");
        atual.paginas = 0;
        pilha->topo--;
        pilha->capacidade--;
        return 0;
    }

    else{
        return -1;
    }
};

int32_t ocupada(impressora* printer){
    if(printer->stack->topo == -1){
        return 0;
    }
    if(printer->stack->doc[printer->stack->topo].print > 0){
        return 1;
    }
    else{
        return 0;
    }
};

void printimpressora(impressora* printer, FILE* OUTPUT){
    fprintf(OUTPUT, "[%s] ", (char*)printer->nome);
    for(int32_t i = printer->stack->topo; i > 0; i--){
        fprintf(OUTPUT, "%s-%dp, ", (char*)printer->stack->doc[i].nome, printer->stack->doc[i].paginas);
    }
    fprintf(OUTPUT, "%s-%dp\n", (char*)printer->stack->doc[0].nome, printer->stack->doc[0].paginas);
};

void printpilha(pilha* stack, FILE* OUTPUT){
    for(int32_t i = stack->capacidade; i > 0; i--){
        fprintf(OUTPUT,"%s-%dp\n", (char*)stack->doc[stack->topo].nome, stack->doc[stack->topo].paginas);
        desempilhar(stack);
    }
};

void imprimir(impressora* printer, pilha* stack, FILE* OUTPUT){

    uint32_t menor = 0;
    menor = printer[0].stack->doc[printer[0].stack->topo].print;
    documento* impresso;

    for(uint32_t i = 0; i < len; i++){
        if(printer[i].stack->doc[printer[i].stack->topo].print < menor) menor = printer[i].stack->doc[printer[i].stack->topo].print;
    }

    for(int32_t i = 0; i < len; i++){
        if(printer[i].stack->doc[printer[i].stack->topo].print - menor == 0){
            printer[i].stack->doc[printer[i].stack->topo].print = printer[i].stack->doc[printer[i].stack->topo].print - menor;
            impresso = &(printer[i].stack->doc[printer[i].stack->topo]);
            empilhar(stack, impresso);
            printimpressora(&printer[i], OUTPUT);
        }
        else{
            printer[i].stack->doc[printer[i].stack->topo].print = printer[i].stack->doc[printer[i].stack->topo].print - menor;
        }
    }
};

int main(int argc, char** argv){
    
    FILE* INPUT = fopen(argv[1], "r");
    FILE* OUTPUT = fopen(argv[2], "w");

    arquivo entrada;
 
    fscanf(INPUT, "%d\n", &entrada.printers);
    printf("%d IMPRESSORAS\n", entrada.printers);

    len = entrada.printers;
 
    printers = malloc(entrada.printers*sizeof(impressora));

    for(int i = 0; i < entrada.printers; i++){
        fscanf(INPUT, "%s\n", entrada.impressora);
        impressora printer;
        strcpy((char * restrict)printer.nome, (char * restrict)entrada.impressora);
        printers[i] = printer;
    }

    fscanf(INPUT, "%d\n", &entrada.files);
    printf("%d DOCUMENTOS\n", entrada.files);

    for(int i = 0; i < entrada.files; i++){printers[i].stack = iniciarpilha(entrada.files);}

    documentos = iniciarfila(entrada.files);
 
    for(int i = 0; i < entrada.files; i++){
        fscanf(INPUT, "%s %d\n", entrada.documento, &entrada.paginas);
        documento doc;
        strcpy((char * restrict)doc.nome, (char * restrict)entrada.documento);
        doc.paginas = entrada.paginas;
        doc.print = entrada.paginas;
        enfileirar(documentos, &doc);
        soma += entrada.paginas;
    }
    
    printf("%d PAGINAS\n", soma);
    
    entrega = iniciarpilha(documentos->capacidade);

    while(documentos->tamanho >= 0){
        for(int32_t i = 0; i < len; i++){
            vazia = ocupada(&printers[i]);
            if(vazia == 0){
            empilhar(printers[i].stack, &documentos->doc[documentos->head]);
            desenfilerirar(documentos);
            }
        }
        imprimir(printers, entrega, OUTPUT);
    }

    fprintf(OUTPUT, "%dp\n", soma);
    printpilha(entrega, OUTPUT);

    fclose(INPUT);
    fclose(OUTPUT);
    return 0;
}