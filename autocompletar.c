#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct No no;

struct No {
	no **P;	
	uint32_t *V;
};	

/* Globals */
uint32_t termos, requisicoes, num_words;
no *pai;
FILE *entrada, *saida;

no *criar_no(void)
{
	no *novo;
	uint32_t i;

	novo = malloc(sizeof(no));
	novo->P = malloc(26 * sizeof(no *)); /* Número de letras no alfabeto */
	for (i = 0; i < 26; i++)
		novo->P[i] = NULL;
	novo->V = NULL;

	return novo;
}

uint32_t indice(char *p, uint32_t d)
{
	return p[d] - 97;
}

void adicionar(no **x, char *p, uint32_t d, uint32_t len)
{
	if (!(*x))
		*x = criar_no();
	if (d == len) {
		(*x)->V = malloc(sizeof(uint32_t));
		*((*x)->V) = 1;
	} else
		adicionar(&((*x)->P[indice(p, d)]), p, d + 1, len);
}

void new_requisitar(no *x, char *p, char *word, uint32_t d, double prefix_size) */
{
	uint32_t i;

	if (d >= 2 * prefix_size) {
		if (x->V) {
			if (num_words == 0) {
				fprintf(saida, "%s", word);
				num_words++;
			}
			else
				fprintf(saida, ",%s", word);
		}
		if (d > 2 * prefix_size)
			return;
	}

	for (i = 0; i < 26; i++) {
		if (x->P[i]) {
			word[d] = 97 + i;
			word[d + 1] = 0;
			cont_requisitar(x->P[i], p, word, d + 1, prefix_size);
		}
	}
}

void cont_requisitar(no *x, char *p, char *word, uint32_t d, double prefix_size)
{
	uint32_t i;

	if (x->V) {
		if (num_words == 0) {
			fprintf(saida, "%s", word);
			num_words++;
		}
		else
			fprintf(saida, ",%s", word);
	}

	if (d + 1 > 2 * prefix_size)
		return;
	
	for (i = 0; i < 26; i++) {
		if (x->P[i]) {
			word[d] = 97 + i;
			word[d + 1] = 0;
			cont_requisitar(x->P[i], p, word, d + 1, prefix_size);
		}
	}
}

void main_requisitar(no *x, char *p, char *word, uint32_t d, uint32_t tam) {
	uint32_t i, j;

	if (d == tam)
		return;

	i = indice(p, d);

	if (x->P[i]) {
		word[d] = 97 + i;
		word[d + 1] = 0;
		cont_requisitar(x->P[i], p, word, d + 1, d + 1);
		word[d] = 97 + i;
		word[d + 1] = 0;
		main_requisitar(x->P[i], p, word, d + 1, tam);
	}
}

void requisitar(no *x, char *p) {
	uint32_t i, tam;
	char *word;

	tam = strlen(p);
	word = calloc(2 * tam, sizeof(char));
	num_words = 0;
	i = indice(p, 0);
	
	fprintf(saida, "%s:", p);

	if (x->P[i]) {
		word[0] = p[0];
		main_requisitar(x->P[i], p, word, 1, tam);
	}

	free(word);
}

int main(int argc, char **argv)
{
	uint32_t i, j;
	char *buffer;

	entrada = fopen(argv[1], "r");
	saida = fopen(argv[2], "w");

	buffer = calloc(30, sizeof(char));
	fgets(buffer, 30, entrada);
	for (termos = 0, i = 0; buffer[i] != '\n'; i++)
		termos = termos * 10 + buffer[i] - 48;	
	free(buffer);

	pai = criar_no();

	for (i = 0, j = 0; i < termos; i++, j++) {
		buffer = calloc(30, sizeof(char));
		fgets(buffer, 30, entrada);
		adicionar(&pai, buffer, 0, strlen(buffer) - 1);
		free(buffer);
    }

    buffer = calloc(30, sizeof(char));
    fgets(buffer, 30, entrada);
    for (requisicoes = 0, i = 0; buffer[i] != '\n'; i++)
		requisicoes = requisicoes * 10 + buffer[i] - 48;
    free(buffer);

    for (i = 0; i < requisicoes; i++) {
		buffer = calloc(30, sizeof(char));
		fgets(buffer, 30, entrada);
		buffer[strcspn(buffer, "\n")] = 0;
		requisitar(pai, buffer);
		if (num_words == 0)
			fprintf(saida, "-");
		fprintf(saida, "\n");
		free(buffer);
    }
}

