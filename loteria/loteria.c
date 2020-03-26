#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct No no;

struct No {
    char *codigo;
    uint32_t acertos;
};

FILE *entrada, *saida;
uint32_t *num_sorteados, *venc, last;
no **nos1, **nos2, **piores, **melhores;

uint8_t verificar(uint32_t num) {
    uint32_t i;	
    for (i = 0; i < 10 && num_sorteados[i] != num; i++);
    return i < 10;
}

no *criar_no(char *dados) {
    uint32_t i, num;
    no *novo;

    novo = malloc(sizeof(no));
    novo->codigo = calloc(150, sizeof(char));
    novo->acertos = 0;

    for (i = 0; dados[i] != ' '; i++)
	novo->codigo[i] = dados[i];
    novo->codigo[i] = '\0';
    i++;

    num = 0;
    for (; dados[i] != '\0'; i++) {
	if (dados[i] == ' ' || dados[i] == '\n') {
	    if (verificar(num))
		novo->acertos++;
	    num = 0;
	} else
	    num = num * 10 + dados[i] - 48;
    }

    return novo;
}

no *copiar_no(no *original)
{
    no *copia;

    copia = malloc(sizeof(no));
    copia->codigo = original->codigo;
    copia->acertos = original->acertos;

    return copia;
}

uint32_t esquerdo(uint32_t i) {
    return 2 * i + 1;
}

uint32_t direito(uint32_t i) {
    return 2 * i + 2;
}

void troca(no ***V, uint32_t P, uint32_t i) {
    no *temp;
    temp = (*V)[i];
    (*V)[i] = (*V)[P];
    (*V)[P] = temp;
}

void heapify_max(uint32_t T, uint32_t i) {
    uint32_t P = i;
    uint32_t E = esquerdo(i);
    uint32_t D = direito(i);
    if (E < T && nos1[E]->acertos > nos1[P]->acertos)
	P = E;
    if (D < T && nos1[D]->acertos > nos1[P]->acertos)
	P = D;
    if (P != i) {      
	troca(&nos1, P, i);
	heapify_max(T, P);
    }
}

void heapify_min(uint32_t T, uint32_t i) {
    uint32_t P = i;
    uint32_t E = esquerdo(i);
    uint32_t D = direito(i);
    if (E < T && nos2[E]->acertos < nos2[P]->acertos)
	P = E;
    if (D < T && nos2[D]->acertos < nos2[P]->acertos)
	P = D;
    if (P != i) {
	troca(&nos2, P, i);
	heapify_min(T, P);
    }
}

void os_iguais(no **nos, uint32_t T, uint32_t i, uint32_t v) {
    if (nos[i]->acertos == v) {
	venc[last++] = i;
    
	uint32_t E = esquerdo(i);
	uint32_t D = direito(i);

	if (E < T)
	    os_iguais(nos, T, E, v);
	if (D < T)
	  os_iguais(nos, T, D, v);
    }
}

int main(int argc, char **argv) {
    uint32_t i, j, premio, apostas;
    char *buffer;

    entrada = fopen(argv[1], "r");	
    saida = fopen(argv[2], "w");

    buffer = calloc(50, sizeof(char));
    fgets(buffer, 50, entrada);
    premio = 0;
    for (i = 0; buffer[i] != '\n'; i++)
	premio = premio * 10 + buffer[i] - 48;	
    free(buffer);

    buffer = calloc(50, sizeof(char));
    fgets(buffer, 50, entrada);
    apostas = 0;
    for (i = 0; buffer[i] != '\n'; i++)
	apostas = apostas * 10 + buffer[i] - 48;
    free(buffer);

    buffer = calloc(50, sizeof(char));
    num_sorteados = malloc(10 * sizeof(uint32_t));
    fgets(buffer, 50, entrada);
    for (i = 0, j = 0; buffer[i] != '\n'; i++) {
	if (buffer[i] == ' ')
	    j++;
	else
	    num_sorteados[j] = num_sorteados[j] * 10 + buffer[i] - 48;
    }

    nos1 = malloc(apostas * sizeof(no *));
    nos2 = malloc(apostas * sizeof(no *));
    venc = malloc(apostas * sizeof(uint32_t));

    for (i = 0; i < apostas; i++) {
	buffer = calloc(200, sizeof(char));
	fgets(buffer, 200, entrada);
	nos1[i] = criar_no(buffer);
	nos2[i] = copiar_no(nos1[i]);
	free(buffer);
    }

    ssize_t root = (apostas - 1) / 2;
    while (root > -1) {
	heapify_max(apostas, root);
	heapify_min(apostas, root);
	root--;
    }

    uint32_t metade, min, max, *vencedores;
    metade = premio / 2;
    max = nos1[0]->acertos;
    min = nos2[0]->acertos;

    last = 0;
    os_iguais(nos1, apostas, 0, nos1[0]->acertos);    
    fprintf(saida, "[%u:%u:%u]\n", last, nos1[0]->acertos, metade / last);
    for (i = 0; i < last; i++)
	fprintf(saida, "%s\n", nos1[venc[i]]->codigo);
    last = 0;
    os_iguais(nos2, apostas, 0, nos2[0]->acertos);
    fprintf(saida, "[%u:%u:%u]\n", last, nos2[0]->acertos, metade / last);
    for (i = 0; i < last; i++)
    	fprintf(saida, "%s\n", nos2[venc[i]]->codigo);

    return 0;
}

