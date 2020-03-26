#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct No no;
typedef struct Dados dados;

struct No {
	char **nome;
	uint32_t *tamanho;
	char **hash;
	no **P;
	uint32_t n;
};

struct Dados {
	char *nome;
	uint32_t tamanho;
	char *hash;
};

/* Globals */
FILE *input;
FILE *output;
uint32_t k;
no *pai = NULL;

no *busca_diferenciada(no *x, char *c)
{
	no *r;
	
   	if (x->nome == NULL) {
		uint32_t i;
		i = 0;
		while (i < x->n) {
			if (strcmp(c, x->hash[i]) > 0)
				i++;
			else
				break;
		}

		if (x->hash[i]) {
		   if (strcmp(c, x->hash[i]) == 0)
			   r = busca_diferenciada(x->P[i + 1], c);
		   else
			   r = busca_diferenciada(x->P[i], c);
		} else {
			r = busca_diferenciada(x->P[i], c);
		}
	} else {
		r = x;
	}
  		
	return r;
}

void busca(no *x, char *c)
{
	uint32_t i, j;
	i = 0;
	while (i < x->n) {
		if (strcmp(c, x->hash[i]) > 0)
	   		i++;
   		else
			break;
	}

	if (x->hash[i]) {
		if (strcmp(c, x->hash[i]) == 0) {
			if (x->nome == NULL)
				busca(x->P[i + 1], c);
			else
				for (j = 0; j < x->n; j++)
					fprintf(output, "%s:size=%d,hash=%s\n", x->nome[j], x->tamanho[j], x->hash[j]);
			return;
		}
	}
	if (x->nome == NULL)
		busca(x->P[i], c);
	else
		fprintf(output, "-\n");
}

void busca_intervalo(char *a, char *b) {
	char *ini, *fim;
	if (strcmp(a, b) < 0) {
		ini = a;
		fim = b;
	} else {
		ini = b;
		fim = a;
	}
	no* f = busca_diferenciada(pai, ini);
	uint32_t i;
	for (i = 0; i < f->n && strcmp(f->hash[i], ini) < 0; i++);
	if (i == f->n) {
		fprintf(output, "-\n");
		return;
	}
   	while (f != NULL) {
		for (; i < f->n; i++) {
			if (strcmp(f->hash[i], fim) > 0)
				return;
			fprintf(output, "%s:size=%d,hash=%s\n", f->nome[i], f->tamanho[i], f->hash[i]);
		}
		f = f->P[0];
		i = 0;
	}
}

no *criar_no(void)
{
	uint32_t i;
	no *novo;

	novo = malloc(sizeof(no));
	novo->nome = NULL;
	novo->tamanho = NULL;
	novo->hash = malloc(k * sizeof(char *));
	novo->P = malloc((k + 1) * sizeof(no *));
	for (i = 0; i < k; i++) {
		novo->hash[i] = NULL;
		novo->P[i] = NULL;
	}
	novo->P[i] = NULL;
	novo->n = 0;

	return novo;
}

no *criar_no_filho(void)
{
  	uint32_t i;
	no *novo;

	novo = malloc(sizeof(no));
	novo->nome = malloc(k * sizeof(char *));
	novo->tamanho = malloc(k * sizeof(uint32_t));
	novo->hash = malloc(k * sizeof(char *));
	novo->P = malloc(sizeof(no *));
	for (i = 0; i < k; i++)
		novo->hash[i] = NULL;
	novo->P[0] = NULL;
	novo->n = 0;

	return novo;
}

void divisao_no_qualquer(no *x, uint32_t wwe)
{
    uint32_t i, j;
	no *y = x->P[wwe];
	no *z = criar_no();
	
  	for (i = 0, j = (k / 2) + 1; j < k; i++, j++) {
		z->hash[i] = y->hash[j];
		z->P[i] = y->P[j];
		z->n++;
		y->n--;
	}	
	z->P[i] = y->P[k];

	for (i = x->n; i > wwe; i--) {
		x->hash[i] = x->hash[i - 1];
		x->P[i + 1] = x->P[i];
	}

	x->hash[wwe] = y->hash[k / 2];
	x->P[wwe + 1] = z;
	x->n++;
	y->n--;
}

void divisao_no_filho(no *x, uint32_t wwe)
{
    uint32_t i, j;
    no *y = x->P[wwe];
	no *z = criar_no_filho();

	y->n++;
	for (i = 0, j = (k / 2); j < k; i++, j++) {
		z->nome[i] = y->nome[j];
		z->hash[i] = y->hash[j];
		z->tamanho[i] = y->tamanho[j];
		z->n++;
		y->n--;
	}

	for (i = x->n; i > wwe; i--) {
		x->hash[i] = x->hash[i - 1];
		x->P[i + 1] = x->P[i];
	}

	x->hash[wwe] = y->hash[k / 2];
	x->P[wwe + 1] = z;
	x->n++;
	y->n--;

	z->P[0] = y->P[0];
	y->P[0] = z;
}

void divisao_no(no *x, uint32_t wwe)
{
	no *y = x->P[wwe];

	if (y->n < k)
		return;
	if (y->nome == NULL) {
		divisao_no_qualquer(x, wwe);
	} else {
		divisao_no_filho(x, wwe);
	}

	/*	for (uint32_t i = 0; i < x->n; i++)
	  fprintf(stderr, "%s ", x->hash[i]);
	fprintf(stderr, "<- ");
	for (uint32_t i = 0; i < x->P[wwe]->n; i++)
	  fprintf(stderr, "%s ", x->P[wwe]->hash[i]);
	fprintf(stderr, "-> ");
	for (uint32_t i = 0; i < x->P[wwe + 1]->n; i++)
	  fprintf(stderr, "%s ", x->P[wwe + 1]->hash[i]);
	  fprintf(stderr, "\n");*/
}

void inserir(no *no_atual, dados *dados)
{
	uint32_t i, j;
	no *filho = NULL;
	i = 0;

	while (i < no_atual->n) {
		if (strcmp(dados->hash, no_atual->hash[i]) >= 0)
			i++;
		else
			break;
	}

	if (no_atual->nome == NULL)
		filho = no_atual->P[i];

	if (!filho) { //Nó folha
		for (j = no_atual->n; j > i; j--) {
			no_atual->nome[j] = no_atual->nome[j - 1];
			no_atual->tamanho[j] = no_atual->tamanho[j - 1];
			no_atual->hash[j] = no_atual->hash[j - 1];
		}

		no_atual->nome[i] = dados->nome;
		no_atual->tamanho[i] = dados->tamanho;
		no_atual->hash[i] = dados->hash;
		no_atual->n++;
	} else {
		inserir(filho, dados);
		divisao_no(no_atual, i);
	}
}

void inserir_na_raiz(dados *dados)
{
	inserir(pai, dados);

	if (pai->n == k) {
		no *filho = pai; /* nasce um novo filho! */
		pai = criar_no();
		pai->P[0] = filho;
		divisao_no(pai, 0);
	}
}

void adicionar_no(char *buffer)
{
	uint32_t i, j, tamanho;
	char *nome, *hash;
	dados *d;

	nome = calloc(50, sizeof(char));
	hash = calloc(50, sizeof(char));
	tamanho = 0;

	for (i = 0; buffer[i] != ' '; i++)
		nome[i] = buffer[i];
	nome[i] = '\0';
	i++;

	for (; buffer[i] != ' '; i++)
		tamanho = tamanho * 10 + buffer[i] - 48;
	i++;

	for (j = 0; buffer[i] != '\n'; i++, j++)
		hash[j] = buffer[i];
	hash[j] = '\0';

	d = malloc(sizeof(dados));
	d->nome = nome;
	d->tamanho = tamanho;
	d->hash = hash;

	inserir_na_raiz(d);
}

void operar(char *buffer)
{
	uint32_t i, j;
	char *operacao, *info;

	operacao = calloc(10, sizeof(char));
	info = calloc(300, sizeof(char));

	for (i = 0; buffer[i] != ' '; i++)
		operacao[i] = buffer[i];
	operacao[i] = '\0';
	i++;
	
       	for (j = 0; buffer[i] != '\0'; i++, j++)
		info[j] = buffer[i];
	info[j] = '\0';

	if (strcmp(operacao, "INSERT") == 0)
		adicionar_no(info);
	else if (strcmp(operacao, "SELECT") == 0) {
		char *range = calloc(10, sizeof(char));
		for (i = 0; i < 5; i++)
			range[i] = info[i];
		range[i] = '\0';
		i++;
		if (strcmp(range, "RANGE") == 0) {
			char *a, *b;
			a = calloc(50, sizeof(char));
			b = calloc(50, sizeof(char));
			for (j = 0; info[i] != ' '; i++, j++)
				a[j] = info[i];
			a[j] = '\0';
			i++;
			for (j = 0; info[i] != '\n'; i++, j++)
				b[j] = info[i];
			b[j] = '\0';
			fprintf(output, "[%s,%s]\n", a, b);
			busca_intervalo(a, b);
			free(a);
			free(b);
   		} else {
			info[strcspn(info, "\n")] = 0;
			fprintf(output, "[%s]\n", info);
			busca(pai, info);
		}
		free(range);
	}

	free(operacao);
	free(info);
}

int main(int argc, char **argv)
{
	char *buffer;
	uint32_t i, numarquivos, numoperacoes;

	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w");

	k = 0;
	buffer = calloc(150, sizeof(char));
	fgets(buffer, 150, input);
	for (i = 0; buffer[i] != '\n'; i++)
		k = k * 10 + buffer[i] - 48;
	free(buffer);

	pai = criar_no_filho();

	numarquivos = 0;
	buffer = calloc(150, sizeof(char));
	fgets(buffer, 150, input);
	for (i = 0; buffer[i] != '\n'; i++)
		numarquivos = numarquivos * 10 + buffer[i] - 48;
	free(buffer);
	for (i = 0; i < numarquivos; i++) {
		buffer = calloc(150, sizeof(char));
		fgets(buffer, 150, input);
		adicionar_no(buffer);
		free(buffer);
	}
	
	numoperacoes = 0;
	buffer = calloc(150, sizeof(char));
	fgets(buffer, 150, input);
	for (i = 0; buffer[i] != '\n'; i++)
		numoperacoes = numoperacoes * 10 + buffer[i] - 48;
	free(buffer);

	for (i = 0; i < numoperacoes; i++) {
		buffer = calloc(150, sizeof(char));
		fgets(buffer, 150, input);
		operar(buffer);
		free(buffer);
	}

	fclose(input);
	fclose(output);
	return 0;
}

