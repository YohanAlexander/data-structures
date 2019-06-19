#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#define MAX 101

typedef struct arquivo arquivo;
typedef struct livro livro;
long consulta;
int32_t retorno = 0, bin = 0, inter = 0, binwin = 0, interwin = 0, binall = 0, interall = 0;
livro* livros;

struct arquivo{
    int32_t livros, consultas;
    long ISBN;
    char autor[MAX], titulo[MAX];
};

struct livro{
    long ISBN;
    char autor[MAX], titulo[MAX];
};

livro* iniciarlivros(int32_t size){

    livro* livros;

    livros = malloc(size*sizeof(livro));

    if(livros == NULL){
        return NULL;
    }
    else{
        for(int32_t i = 0; i < size; i++){
        }
        return livros;
    }
};

int32_t buscabin(livro* book, int32_t inicio, int32_t final, long ISBN){
    
    bin++;

    if(final < inicio){
        return -1;
    }
    
    int32_t index = (inicio + final) / 2;

    if(book[index].ISBN == ISBN){
        return index;
    }

    else if(book[index].ISBN < ISBN){
        return buscabin(book , index + 1, final, ISBN);
    }

    else{
        return buscabin(book, inicio, index - 1, ISBN);
    }
};

int32_t buscainter(livro* book, int32_t inicio, int32_t final, long ISBN){

    if(final < inicio){
        return -1;
    }
    
    inter++;

    int32_t index = inicio + ((book[final].ISBN - book[inicio].ISBN) % (final - inicio + 1));

    if(book[index].ISBN == ISBN){
        return index;
    }

    else if(book[index].ISBN < ISBN){
        return buscainter(book , index + 1, final, ISBN);
    }

    else{
        return buscainter(book, inicio, index - 1, ISBN);
    }
};

void imprimir(livro* book, int32_t index, int32_t bin, int32_t inter, long ISBN, FILE* OUTPUT){

    if(index == -1){
        fprintf(OUTPUT, "[%ld]B=%d,I=%d:ISBN_NOT_FOUND\n", ISBN, bin, inter);
    }

    else{
        fprintf(OUTPUT, "[%ld]B=%d,I=%d:Author:%s,Title:%s\n", ISBN, bin, inter, book[index].autor, book[index].titulo);
    }

};

int main(int argc, char** argv){

    FILE* INPUT = fopen(argv[1], "r");
    FILE* OUTPUT = fopen(argv[2], "w");

    arquivo file;

    fscanf(INPUT, "%d\n", &file.livros);
    printf("%d\n", file.livros);

    livros = malloc(file.livros*sizeof(livro));

    for(int32_t i = 0; i < file.livros; i++){
        fscanf(INPUT, "%ld %[^&]&%[^\n]\n", &file.ISBN, file.autor, file.titulo);
        livro book;
        book.ISBN = file.ISBN;
        strcpy(book.autor, file.autor);
        strcpy(book.titulo, file.titulo);
        printf("(%ld) ", book.ISBN);
        printf("%s & ", book.autor);
        printf("%s\n", book.titulo);
        livros[i] = book;
    }

    fscanf(INPUT, "%d\n", &file.consultas);
    printf("%d\n", file.consultas);

    for(int32_t i = 0; i < file.consultas; i++){
        fscanf(INPUT, "%ld\n", &consulta);
        printf("%ld\n", consulta);
        retorno = buscabin(livros, 0, file.livros - 1, consulta);
        retorno = buscainter(livros, 0, file.livros - 1, consulta);
        imprimir(livros, retorno, bin, inter, consulta, OUTPUT);
        binall += bin;
        interall += inter;
        if(bin < inter){
            binwin++;
        }
        else{
            interwin++;
        }
        bin = 0;
        inter = 0;
        retorno = 0;
    }

    fprintf(OUTPUT, "BINARY=%d:%d\n", binwin, binall / file.consultas);
    fprintf(OUTPUT, "INTERPOLATION=%d:%d\n", interwin, interall / file.consultas);

    fclose(INPUT);
    fclose(OUTPUT);
    return 0;
}