#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#define MAX 61

typedef struct fila fila;
typedef struct pilha pilha;
typedef struct documento documento;
typedef struct impressora impressora;

int32_t soma = 0, vazia = 0, printers = 0, files = 0, paginas = 0, menor = 0;
char printername[MAX], filename[MAX];
impressora* impressoras;
fila* documentos;
pilha* entrega;

struct documento{
    char nome[MAX];
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

void enfileirar(fila* fila, documento* doc){

    strcpy(fila->doc[((fila->head + fila->tail) % fila->capacidade)].nome, doc->nome);
    fila->doc[((fila->head + fila->tail) % fila->capacidade)].paginas = doc->paginas;
    fila->doc[((fila->head + fila->tail) % fila->capacidade)].print = doc->print;
    fila->tail = ((fila->tail + 1) % fila->capacidade);
    fila->tamanho++;

};

void desenfilerirar(fila* fila){

    strcpy(fila->doc[fila->head].nome, "");
    fila->doc[fila->head].paginas = 0;
    fila->head = ((fila->head + 1) % fila->capacidade);
    fila->tamanho--;

};

void empilhar(pilha* pilha, documento* doc){

    pilha->topo++;
    strcpy(pilha->doc[pilha->topo].nome, doc->nome);
    pilha->doc[pilha->topo].paginas = doc->paginas;
    pilha->doc[pilha->topo].print = doc->print;

};

void desempilhar(pilha* pilha){

    strcpy(pilha->doc[pilha->topo].nome, "");
    pilha->doc[pilha->topo].paginas = 0;
    pilha->topo--;
    pilha->capacidade--;

};

int32_t ocupada(impressora* printer){

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

void imprimir(impressora* printer, int32_t menor, pilha* stack, FILE* OUTPUT){

    documento impresso;
    
    for(int32_t i = 0; i < printers; i++){
        
        if(printer[i].stack->doc[printer[i].stack->topo].print - menor == 0){
            printer[i].stack->doc[printer[i].stack->topo].print -= menor;
            impresso = printer[i].stack->doc[printer[i].stack->topo];
            empilhar(stack, &impresso);
            printimpressora(&printer[i], OUTPUT);
        }

        else{
            printer[i].stack->doc[printer[i].stack->topo].print -= menor;
        }

    }

};

int main(int argc, char** argv){
    
    FILE* INPUT = fopen(argv[1], "r");
    FILE* OUTPUT = fopen(argv[2], "w");
 
    fscanf(INPUT, "%d\n", &printers);
    printf("%d Impressoras\n", printers);

    impressoras = malloc(printers*sizeof(impressora));

    for(int i = 0; i < printers; i++){
        fscanf(INPUT, "%s\n", printername);
        printf("%s\n", printername);
        impressora printer;
        strcpy(printer.nome, printername);
        impressoras[i] = printer;
    }

    fscanf(INPUT, "%d\n", &files);
    printf("%d Documentos\n", files);

    for(int i = 0; i < files; i++){impressoras[i].stack = iniciarpilha(files);}

    documentos = iniciarfila(files);
 
    for(int i = 0; i < files; i++){
        fscanf(INPUT, "%s %d\n", filename, &paginas);
        printf("%s-%dp\n", filename, paginas);
        documento doc;
        strcpy(doc.nome, filename);
        doc.paginas = doc.print = paginas;
        enfileirar(documentos, &doc);
        soma += paginas;
    }
    
    printf("%d Paginas\n", soma);
    
    entrega = iniciarpilha(documentos->capacidade);
    
    while(documentos->tamanho >= 0){
        
        for(int32_t i = 0; i < printers; i++){
            
            menor = impressoras[0].stack->doc[impressoras[0].stack->topo].print;
            if(impressoras[i].stack->doc[impressoras[i].stack->topo].print < menor){
                menor = impressoras[i].stack->doc[impressoras[i].stack->topo].print;
            }
            
            vazia = ocupada(&impressoras[i]);
            if(vazia == 0){
                empilhar(impressoras[i].stack, &documentos->doc[documentos->head]);
                desenfilerirar(documentos);
            }
        }
        imprimir(impressoras, menor, entrega, OUTPUT);
    }
    
    fprintf(OUTPUT, "%dp\n", soma);
    printpilha(entrega, OUTPUT);

    fclose(INPUT);
    fclose(OUTPUT);
    return 0;
}