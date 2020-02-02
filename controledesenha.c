#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct Pessoa pessoa;
typedef struct Orgao orgao;

struct Pessoa {
	char *nome;
	pessoa *p;
};

struct Orgao {
	char *nome;
	uint32_t capacidade;
	pessoa *i_conv, *f_conv, *i_pref, *f_pref;
};

/* Globals */
FILE *entrada, *saida;
uint32_t num_orgaos, num_pessoas;
orgao **orgaos;

uint32_t atender(uint32_t i) {
	orgao *org = orgaos[i];
	if (org->i_pref) {
		fprintf(saida, "%s", org->i_pref->nome);
		org->i_pref = org->i_pref->p;
	} else if (org->i_conv) {
		fprintf(saida, "%s", org->i_conv->nome);
		org->i_conv = org->i_conv->p;
	} else {
		return 0;
	}
			
	return 1;
}

void adicionar_pessoa(char *buffer) {
	uint32_t i, j, k, idade;
	char *nome, *orgao;
	pessoa *nova;

	nova = malloc(sizeof(pessoa));

	orgao = calloc(51, sizeof(char));
	for (i = 0; buffer[i] != '|'; i++)
		orgao[i] = buffer[i];
	orgao[i] = '\0';
	i++;
	
	for (j = 0; strcmp(orgaos[j]->nome, orgao) != 0; j++);
	free(orgao);

	nome = calloc(51, sizeof(char));
	for (k = 0; buffer[i] != '|'; i++, k++)
		nome[k] = buffer[i];
	nome[k] = '\0';
	i++;

	nova->nome = nome;

	idade = 0;
	for (; buffer[i] != '\n'; i++)
		idade = idade * 10 + buffer[i] - 48;

	if (idade < 60) {
 		if (orgaos[j]->i_conv) {
			orgaos[j]->f_conv->p = nova;
			orgaos[j]->f_conv = nova;
		} else {
			orgaos[j]->i_conv = nova;
			orgaos[j]->f_conv = orgaos[j]->i_conv;
		}
	} else {
		if (orgaos[j]->i_pref == NULL) {
			orgaos[j]->i_pref = nova;
			orgaos[j]->f_pref = orgaos[j]->i_pref;
		} else {
			orgaos[j]->f_pref->p = nova;
			orgaos[j]->f_pref = nova;
		}		 
	}
}

orgao *adicionar_orgao(char *buffer) {
	uint32_t i;
	char *nome;
	orgao *novo;

	novo = malloc(sizeof(orgao));
	novo->nome = NULL;
	novo->capacidade = 0;
	novo->i_conv = NULL;
	novo->f_conv = NULL;
	novo->i_pref = NULL;
	novo->f_pref = NULL;
	
	nome = calloc(150, sizeof(char));
	for (i = 0; buffer[i] != ' '; i++)
		nome[i] = buffer[i];
	nome[i] = '\0';
	i++;

	for (; buffer[i] != '\n'; i++)
		novo->capacidade = novo->capacidade + buffer[i] - 48;

	novo->nome = nome;

	return novo;
}

int main(int argc, char **argv) {
	char *buffer;
	uint32_t i;
	
	entrada = fopen(argv[1], "r");
	saida = fopen(argv[2], "w");

	buffer = calloc(150, sizeof(char));
	fgets(buffer, 150, entrada);
	num_orgaos = 0;
	for (i = 0; buffer[i] != '\n'; i++)
		num_orgaos = num_orgaos * 10 + buffer[i] - 48;
	free(buffer);

	orgaos = malloc(num_orgaos * sizeof(orgao *));
	
	for (i = 0; i < num_orgaos; i++) {
		buffer = calloc(150, sizeof(char));
		fgets(buffer, 150, entrada);
		orgaos[i] = adicionar_orgao(buffer);
		free(buffer);
	}
	
	buffer = calloc(150, sizeof(char));
	fgets(buffer, 150, entrada);
	num_pessoas = 0;
	for (i = 0; buffer[i] != '\n'; i++) {
		num_pessoas = num_pessoas * 10 + buffer[i] - 48;
	}
	free(buffer);

	for (i = 0; i < num_pessoas; i++) {
		buffer = calloc(155, sizeof(char));
		fgets(buffer, 155, entrada);
		adicionar_pessoa(buffer);
		free(buffer);
	}

	uint32_t atendidos, j, to_print;
	atendidos = 0;
	to_print = 0;
	while (atendidos < num_pessoas) {
		for (i = 0; i < num_orgaos && atendidos < num_pessoas; i++) {
			fprintf(saida, "%s:", orgaos[i]->nome);
			for (j = 0; j < orgaos[i]->capacidade && atendidos < num_pessoas; j++) {
				if (j > 0)
					fprintf(saida, ",");
				atendidos += atender(i);
			}
			fprintf(saida, "\n");
		}
	}

	fclose(entrada);
	fclose(saida);
    
	return 0;
}

