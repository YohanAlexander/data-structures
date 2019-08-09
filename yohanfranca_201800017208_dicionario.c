#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#define MAX 33

typedef struct arquivo arquivo;
typedef struct node node;
typedef struct node* arvore;

arquivo entrada;

struct arquivo{
    char palavra[MAX], consulta[MAX];
    int32_t palavras, tokens, consultas;
    char sinonimos[10][MAX];
};

struct node{
    int32_t altura;
    node *left, *right;
    arquivo key;
    char sinonimos[10][MAX];
};

node* buscar(arvore* root, char word[MAX], FILE* OUTPUT){

    if(*root == NULL)
        return NULL;

    int32_t cmp = strcmp((*root)->key.palavra, word);

    if(cmp < 0){
        fprintf(OUTPUT, "%s->", (*root)->key.palavra);
        return buscar(&(*root)->left, word, OUTPUT);
    }

    if(cmp > 0){
        fprintf(OUTPUT, "%s->", (*root)->key.palavra);
        return buscar(&(*root)->right, word, OUTPUT);
    }

    else{
        return *root;
    }

};

int32_t maior(int32_t x, int32_t y){
    
    if(x > y)
        return x;
    
    else
        return y;

};

int32_t altura(node* root){
   
   if(root == NULL) 
      return -1;

   else{
      int32_t h_left = altura(root->left);
      int32_t h_right = altura(root->right);
      if(h_left < h_right) return h_right + 1;
      else return h_left + 1;
   }

};  

int32_t altura_node(node* root){
    
    if(root == NULL)
        return -1;

    else
        return root->altura;

};

int32_t balanceamento(node* root){

	return altura_node(root->left) - altura_node(root->right);

};

void copiar(node* root, arquivo* file){

    strcpy(root->key.palavra, file->palavra);
    root->key.tokens = file->tokens;
    for(int32_t i = 0; i < 10; i++){
        strcpy(root->sinonimos[i], file->sinonimos[i]);
    }

};

void rotacao_left(arvore* root){
    
    node* eixo = (*root)->right;
    (*root)->right = eixo->left;
    eixo->left = *root;
    (*root)->altura = maior(altura_node((*root)->left), altura((*root)->right)) + 1;
    eixo->altura = maior(altura(eixo->right), (*root)->altura) + 1;
    *root = eixo;

};

void rotacao_right(arvore* root){
    
    node* eixo = (*root)->left;
    (*root)->left = eixo->right;
    eixo->right = *root;
    (*root)->altura = maior(altura_node((*root)->left), altura((*root)->right)) + 1;
    eixo->altura = maior(altura(eixo->left), (*root)->altura) + 1;
    *root = eixo;

}

void rotacao_left_right(arvore* root){

    rotacao_left(&(*root)->left);
    rotacao_right(root);

};

void rotacao_right_left(arvore* root){

    rotacao_right(&(*root)->right);
    rotacao_left(root);

};

int32_t inserir(arvore* root, arquivo* file){

    int32_t out;

    if(*root == NULL){
        
        node* new = malloc(sizeof(node));

        if(new == NULL)
            return 0;

        copiar(new, file);
        new->altura = 0;
        new->left = NULL;
        new->right = NULL;
        *root = new;
        return 1;

    }

    node *atual = *root;
    
    if(strcmp(atual->key.palavra, entrada.palavra) < 0){
        if((out = inserir(&(atual->left), file)) == 1){
            if(balanceamento(atual) == 2){
                if(balanceamento(atual->left) == 1){
                    rotacao_right(root);
                }
                else{
                    rotacao_left_right(root);
                }
            }
        }
    }
    
    else{
        
        if(strcmp(atual->key.palavra, entrada.palavra) > 0){
            if((out = inserir(&(atual->right), file)) == 1){
                if(balanceamento(atual) == -2){
                    if(balanceamento(atual->right) == -1){
                        rotacao_left(root);
                    }else{
                        rotacao_right_left(root);
                    }
                }
            }
        }

        else{
            return 0;
        }

    }

    atual->altura = maior(altura_node(atual->left), altura_node(atual->right)) + 1;

    return out;

};

void ordem(arvore* root, FILE* OUTPUT){

    if(*root != NULL){
        ordem(&(*root)->left, OUTPUT);
        printf("%s %d ", (*root)->key.palavra, (*root)->key.tokens);
        
        for(int32_t i = 0; i < 9; i++){
            printf("%s ", (*root)->sinonimos[i]);
        }
        
        printf("%s\n", (*root)->sinonimos[9]);
        ordem(&(*root)->right, OUTPUT);
    }

};

int main(int argc, char** argv){

    FILE* INPUT = fopen(argv[1], "r");
	FILE* OUTPUT = fopen(argv[2], "w");

    arvore* tree = malloc(sizeof(arvore));
    
    fscanf(INPUT, "%d\n", &entrada.palavras);

    for(int32_t i = 0; i < entrada.palavras; i++){
        
        fscanf(INPUT, "%s %d ", entrada.palavra, &entrada.tokens);

        for(int32_t f = 0; f < entrada.tokens; f++){
            fscanf(INPUT, "%s", entrada.sinonimos[f]);
        }

        for(int32_t f = entrada.tokens; f < 10; f++){
            strcpy(entrada.sinonimos[f], "");
        }

        inserir(tree, &entrada);

    }

    //ordem(tree, OUTPUT);

    node* no = malloc(sizeof(node));
    
    fscanf(INPUT, "%d\n", &entrada.consultas);
    
    for(int32_t i = 0; i < entrada.consultas; i++){

        fscanf(INPUT, "%s\n", entrada.consulta);
        
        fprintf(OUTPUT, "[");
        no = buscar(tree, entrada.consulta, OUTPUT);
        if(no == NULL)
            fprintf(OUTPUT, "?");
        else
            fprintf(OUTPUT, "%s", no->key.palavra);
        fprintf(OUTPUT, "]\n");

        if(no == NULL)
            fprintf(OUTPUT, "-\n");

        else{
            for(int32_t i = 0; i < no->key.tokens - 1; i++){
                fprintf(OUTPUT, "%s,", no->sinonimos[i]);
            }
            fprintf(OUTPUT, "%s\n", no->sinonimos[no->key.tokens - 1]);
        }

    }

    fclose(INPUT);
    fclose(OUTPUT);
    return 0;
}