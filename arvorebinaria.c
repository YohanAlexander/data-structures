#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#define MAX 51

typedef struct arquivo arquivo;
typedef struct node node;

int32_t arquivos;
arquivo entrada;

struct arquivo{
    char nome[MAX];
    char tipo[2];
    int32_t tamanho, count;
};

struct node{
    node *left, *right;
    arquivo key;
};

node* buscar(node* root, arquivo* file){

    int32_t cmp = strcmp(file->nome, root->key.nome);

    if(cmp < 0 && root->left != NULL){
        return buscar(root->left, file);
    }

    if(cmp > 0 && root->right != NULL){
        return buscar(root->right, file);
    }

    else{
        return root;
    }

};

void copiar(node* root, arquivo* file){

    strcpy(root->key.nome, file->nome);
    strcpy(root->key.tipo, file->tipo);
    root->key.tamanho = file->tamanho;
    root->key.count = file->count;

};

void inserir_left(node* root, arquivo* file){

    node* new = malloc(sizeof(node));
    new->left = NULL;
    new->right = NULL;
    root->left = new;
    copiar(root->left, file);

};

void inserir_right(node* root, arquivo* file){

    node* new = malloc(sizeof(node));
    new->left = NULL;
    new->right = NULL;
    root->right = new;
    copiar(root->right, file);

};

void ordem(node* root, FILE* OUTPUT){

    if(root != NULL){
        ordem(root->left, OUTPUT);
        fprintf(OUTPUT, "%d %s %s %d ", root->key.count, root->key.nome, root->key.tipo, root->key.tamanho);
        if(root->key.tamanho == 1)
            fprintf(OUTPUT, "byte\n");
        else
            fprintf(OUTPUT, "bytes\n");
        ordem(root->right, OUTPUT);
    }

};

void pre_ordem(node* root, FILE* OUTPUT){

    if(root != NULL){
        fprintf(OUTPUT, "%d %s %s %d ", root->key.count, root->key.nome, root->key.tipo, root->key.tamanho);
        if(root->key.tamanho == 1)
            fprintf(OUTPUT, "byte\n");
        else
            fprintf(OUTPUT, "bytes\n");
        pre_ordem(root->left, OUTPUT);
        pre_ordem(root->right, OUTPUT);
    }

};

void pos_ordem(node* root, FILE* OUTPUT){

    if(root != NULL){
        pos_ordem(root->left, OUTPUT);
        pos_ordem(root->right, OUTPUT);
        fprintf(OUTPUT, "%d %s %s %d ", root->key.count, root->key.nome, root->key.tipo, root->key.tamanho);
        if(root->key.tamanho == 1)
            fprintf(OUTPUT, "byte\n");
        else
            fprintf(OUTPUT, "bytes\n");
    }

};

int main(int argc, char** argv){

    FILE* INPUT = fopen(argv[1], "r");
	FILE* OUTPUT = fopen(argv[2], "w");

    node* tree = malloc(sizeof(node));
    node* no = malloc(sizeof(node));
    
    fscanf(INPUT, "%d\n", &arquivos);

    for(int32_t i = 0; i < arquivos; i++){
        
        fscanf(INPUT, "%s %s %d\n", entrada.nome, entrada.tipo, &entrada.tamanho);

        entrada.count = i;

        no = buscar(tree, &entrada);

        if(i == 0){
            copiar(tree, &entrada);
        }

        if(strcmp(entrada.nome, no->key.nome) == 0){
            if(strcmp(no->key.tipo, "rw") == 0){
                copiar(no, &entrada);
            }
        }

        if(strcmp(entrada.nome, no->key.nome) < 0){
            inserir_left(no, &entrada);
        }
        
        if(strcmp(entrada.nome, no->key.nome) > 0){
            inserir_right(no, &entrada);
        }
    }

    fprintf(OUTPUT, "EPD:\n");
    ordem(tree, OUTPUT);
    fprintf(OUTPUT, "PED:\n");
    pre_ordem(tree, OUTPUT);
    fprintf(OUTPUT, "EDP:\n");
    pos_ordem(tree, OUTPUT);

    fclose(INPUT);
    fclose(OUTPUT);
    return 0;
}