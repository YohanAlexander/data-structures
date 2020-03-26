#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct No no;
typedef struct Regiao regiao;

struct No {
	uint32_t H;
	no *P;
};

/* Globals */
FILE *entrada, *saida;
uint32_t num_orgaos, num_pessoas;
no **coords;

uint32_t myrand() {
	static uint32_t next = 1;
	next = next * 1103515245 + 12345;
	return next;
}

no *criar_conjunto(void) {
	no *x = malloc(sizeof(no));
	x->P = x;
	x->H = 0;
	return x;
}

no *encontrar_conjunto(no *x) {
	if (x != x->P) {
		x->P = encontrar_conjunto(x->P);
	}
	return x->P;
}

void unir_conjuntos(no *x, no *y) {
	no *rx = encontrar_conjunto(x);
	no *ry = encontrar_conjunto(y);
	if (rx->H > ry->H) {
		ry->P = rx;
	} else {
		rx->P = ry;
		if (rx->H == ry->H) {
			ry->H++;
		}
	}
}

void adicionar_regiao(char *buffer) {
	uint32_t area, i, altura, largura, x, y;

	altura = largura = x = y = 0;
	
	for (i = 0; buffer[i] != ' '; i++)
		altura = altura * 10 + buffer[i] - 48;
	i++;
	for (; buffer[i] != ' '; i++)
		largura = largura * 10 + buffer[i] - 48;
	i++;
	for (; buffer[i] != ' '; i++)
		x = x * 10 + buffer[i] - 48;
	i++;
	for (; buffer[i] != '\n'; i++)
		y = y * 10 + buffer[i] - 48;

	area = tamanho * largura;
	coords = malloc(area * sizeof(no));
	for (i = 0; i < area; i++)
		coords[i] = criar_conjunto();
}

void propagar(void) {
	uint32_t x, y, tam;
}

int main(int argc, char **argv) {
	char *buffer;
	uint32_t i;
	
	entrada = fopen(argv[1], "r");
	saida = fopen(argv[2], "w");

	buffer = calloc(150, sizeof(char));
	fgets(buffer, 150, entrada);
	regioes = 0;
	for (i = 0; buffer[i] != '\n'; i++)
		regios = regioes * 10 + buffer[i] - 48;
	free(buffer);

	for (i = 0; i < regioes; i++) {
		buffer = calloc(150, sizeof(char));
		fgets(buffer, 150, entrada);
		adicionar_regiao(buffer);
		free(buffer);
		propagar();
		free(coords);
	}

	fclose(entrada);
	fclose(saida);
    
	return 0;
}

