#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#define MAX 4096

typedef struct arquivo{
    uint32_t printers, files, paginas[MAX];
    char *impressoras[MAX], *documentos[MAX];
} arquivo;

typedef struct documento{
    char nome[MAX];
    uint32_t paginas, capacidade, print;
} documento;

typedef struct fila{
    uint32_t capacidade, head, tail, tamanho;
    documento* doc;
} fila;

typedef struct pilha{
    uint32_t capacidade, topo;
    documento* doc;
} pilha;

typedef struct impressora{
    uint32_t capacidade;
    char* nome[MAX];
    pilha* stack;
} impressora;

arquivo* iniciararquivo(){

    arquivo* new;
    new = malloc(sizeof(char**)); 
    if(new == NULL){
        return NULL;
    }
    else{
        new->printers = 0;
        new->files = 0;
        return new;
    }
};

fila* iniciarfila(uint32_t size){

    fila* fila;
    fila = malloc(sizeof(fila)); 
    if(fila == NULL){
        return NULL;
    }
    else{
        fila->capacidade = 0;
        fila->head = 0;
        fila->tail = 0;
        fila->tamanho = 0;
        fila->doc = malloc(size*sizeof(documento*));
        return fila;
    }
};

pilha* iniciarpilha(uint32_t size){

    pilha* pilha;
    pilha = malloc(sizeof(pilha)); 
    if(pilha == NULL){
        return NULL;
    }
    else{
        pilha->capacidade = 0;
        pilha->topo = -1;
        pilha->doc = malloc(size*sizeof(documento*));
        return pilha;
    }
};

documento* criardocs(uint32_t size, char* nomes[MAX], uint32_t* paginas){

    documento* new;
    new = malloc(size*sizeof(documento*)); 
    if(new == NULL){
        return NULL;
    }
    else{
        new->capacidade = size;
        printf("%d DOCUMENTOS\n", new->capacidade);
        for(uint32_t i = 0; i < size; i++){
            strcpy(new[i].nome, nomes[i]);
            new[i].paginas = paginas[i];
            new[i].print = new[i].paginas;
        }
    return new;
    }
};

impressora* criarimpressoras(uint32_t size, char* nomes[MAX], uint32_t capacidade){
    
    impressora* impressora;
    impressora = malloc(size*sizeof(impressora));
    if(impressora == NULL){
        return NULL;
    }
    else{
        impressora->capacidade = size;
        printf("%d IMPRESSORAS\n", impressora->capacidade);
        for(uint32_t i = 0; i < size; i++){
            strcpy((char* restrict)impressora[i].nome, (char* restrict)nomes[i]);
            impressora[i].stack = iniciarpilha(capacidade);
        }
        return impressora;
    }
};

uint32_t enfileirar(fila* fila, uint32_t capacidade, documento* doc){

    fila->capacidade = fila->capacidade + capacidade;

    if(fila->doc == NULL){
        return -1;
    }

    else if(fila->tamanho == 0 || fila->tamanho != 0){
        //printf("ENFILEIRAR\n");
        for(uint32_t j = 0; j < capacidade; j++){
            //printf("%s %d\n", doc[j].nome, doc[j].paginas);
            strcpy(fila->doc[((fila->head + fila->tail) % fila->capacidade)].nome, doc[j].nome);
            fila->doc[((fila->head + fila->tail) % fila->capacidade)].paginas = doc[j].paginas;
            fila->doc[((fila->head + fila->tail) % fila->capacidade)].print = doc[j].print;
            fila->tail = ((fila->tail + 1) % fila->capacidade);
            fila->tamanho++;
        }
        return 0;
    }

    else{
        return -1;
    }
};

uint32_t desenfilerirar(fila* fila){

    if(fila->doc == NULL){
        return -1;
    }

    else if(fila->capacidade == fila->tamanho || fila->capacidade != fila->tamanho){
        documento* atual;
        atual = &fila->doc[fila->head];
        strcpy(atual->nome, "");
        atual->paginas = 0;
        fila->head = ((fila->head + 1) % fila->capacidade);
        fila->tamanho--;
        return 0;
    }

    else{
        return -1;
    }
};

uint32_t empilhar(pilha* pilha, uint32_t capacidade, documento* doc){

    pilha->capacidade = pilha->capacidade + capacidade;

    if(pilha->doc == NULL){
        return -1;
    }

    else if(pilha->topo == -1 || pilha->topo != -1){
        //printf("EMPILHAR\n");
        for(uint32_t j = 0; j < capacidade; j++){
            //printf("%s %d\n", doc[j].nome, doc[j].paginas);
            pilha->topo++;
            strcpy((char* restrict)&pilha->doc[pilha->topo].nome, doc[j].nome);
            pilha->doc[pilha->topo].paginas = doc[j].paginas;
            pilha->doc[pilha->topo].print = doc[j].print;
        }
        return 0;
    }

    else{
        return -1;
    }
};

uint32_t desempilhar(pilha* pilha){

    if(pilha->doc == NULL){
        return -1;
    }

    else if(pilha->topo == (pilha->capacidade - 1) || pilha->topo != (pilha->capacidade - 1)){
        documento* atual;
        atual = &pilha->doc[pilha->topo];
        strcpy(atual->nome, "");
        atual->paginas = 0;
        pilha->topo--;
        pilha->capacidade--;
        return 0;
    }

    else{
        return -1;
    }
};

documento* imprimir(impressora* printer){

    uint32_t menor = 0;
    menor = printer[0].stack->doc[printer[0].stack->topo].print;
    documento* impresso = NULL;

    for(uint32_t i = 0; i < printer->capacidade; i++){
        if(printer[i].stack->doc[printer[i].stack->topo].print < menor) menor = printer[i].stack->doc[printer[i].stack->topo].print;
    }

    for(uint32_t i = 0; i < printer->capacidade; i++){
        if(printer[i].stack->doc[printer[i].stack->topo].print - menor == 0){
            printer[i].stack->doc[printer[i].stack->topo].print = printer[i].stack->doc[printer[i].stack->topo].print - menor;
            impresso = &(printer[i].stack->doc[printer[i].stack->topo]);
        }
        else{
            printer[i].stack->doc[printer[i].stack->topo].print = printer[i].stack->doc[printer[i].stack->topo].print - menor;
        }
    }
    return impresso;
};

uint32_t ocupada(impressora* printer, uint32_t atual){
    if(printer[atual].stack->topo == -1){
        return 0;
    }
    else if(printer[atual].stack->doc[printer[atual].stack->topo].print != 0){
        return 1;
    }
    else{
        return 0;
    }
};

void printimpressora(impressora* printer, FILE* OUTPUT){
    for(uint32_t i = 0; i < printer->capacidade; i++){
        uint32_t vazia = ocupada(printer, i);
        if(vazia == 0){
            fprintf(OUTPUT, "[%s] ", (char*)printer[i].nome);
            if(printer[i].stack->topo > 0){
                for(uint32_t j = printer[i].stack->capacidade - 1; j > 0; j--){
                    fprintf(OUTPUT, "%s-%dp, ", printer[i].stack->doc[j].nome, printer[i].stack->doc[j].paginas);
                }
                    fprintf(OUTPUT, "%s-%dp\n", printer[i].stack->doc[0].nome, printer[i].stack->doc[0].paginas);        
                }
            else{
                fprintf(OUTPUT, "%s-%dp\n", printer[i].stack->doc[0].nome, printer[i].stack->doc[0].paginas);
            }
        }
    }
};

void printpilha(pilha* stack, uint32_t size, FILE* OUTPUT){
    fprintf(OUTPUT, "%dp\n", size);
    for(uint32_t i = stack->capacidade; i > 0; i--){
        fprintf(OUTPUT,"%s-%dp\n", stack->doc[stack->topo].nome, stack->doc[stack->topo].paginas);
        desempilhar(stack);
    }
};

uint32_t somapag(documento* docs){
    uint32_t soma = 0;
    for(uint32_t i = 0; i < docs->capacidade; i++){
        soma = soma + docs[i].paginas;
    }
    printf("%d PAGINAS\n", soma);
    return soma;
};

arquivo* lerarquivo(FILE* INPUT){

    uint32_t linhas = 0, i = 0;
    char linha[MAX], **entrada;
    
    while(fgets(linha, MAX, INPUT)){linhas++;}
   
    entrada = (char**)malloc(linhas*sizeof(char*));
    rewind(INPUT);

    while(fgets(linha, MAX, INPUT) != NULL && i <= linhas){
            strtok(linha, "\n");
            entrada[i] = (char*)strdup(linha);
            i++;
    }

    arquivo* new = iniciararquivo();
    new->printers = atoi(strtok(entrada[0], "\n"));
    new->files = atoi(strtok(entrada[new->printers + 1], "\n"));

    for(uint32_t i = 0; i < new->printers; i++){
        printf("%s\n", entrada[i + 1]);
        new->impressoras[i] = strtok(entrada[i + 1], "\n");
    }

    for(uint32_t i = 0; i < new->files; i++){
        printf("%s\n", entrada[new->printers + i + 2]);
        new->documentos[i] = strtok(entrada[new->printers + i + 2], " ");
        new->paginas[i] = atoi(strtok(NULL, "\n"));
    }

    return new;     
};

uint32_t soma, vazia;
documento* docs, *impresso;
impressora* printers;
fila* documentos;
pilha* entrega;

int main(int argc, char** argv){
    
    FILE* INPUT = fopen(argv[1], "r");
    FILE* OUTPUT = fopen(argv[2], "w");
    arquivo* file = lerarquivo(INPUT);

    printers = criarimpressoras(file->printers, file->impressoras, file->files);
    docs = criardocs(file->files, file->documentos, file->paginas);
    
    entrega = iniciarpilha(docs->capacidade);
    soma = somapag(docs);

    documentos = iniciarfila(docs->capacidade);
    enfileirar(documentos, docs->capacidade, docs);
            
    while(documentos->tamanho != -1){
        for(uint32_t i = 0; i < printers->capacidade; i++){  
            vazia = ocupada(printers, i);
            if(vazia == 0){
            empilhar(printers[i].stack, 1, &documentos->doc[documentos->head]);
            desenfilerirar(documentos);
            }
        }
        impresso = imprimir(printers);
        empilhar(entrega, 1, impresso);
        printimpressora(printers, OUTPUT);
    }

    printpilha(entrega, soma, OUTPUT);

    fclose(INPUT);
    fclose(OUTPUT);
    return 0;
}