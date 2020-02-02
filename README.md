

<!-- ABOUT THE PROJECT -->
# Projetos de Estrutura de dados

Implementação em C de estruturas de dados estáticas e dinâmicas, como lista, fila, pilha, heap, fila de prioridade, árvore de prefixos, árvore binária de busca, árvore AVL, árvore B, árvore B+, conjuntos union-find, e tabela hash.

<!-- GETTING STARTED -->
## Funcionamento

### Pré-requisitos

Para o funcionamento adequado são necessárias ferramentas para compilação de código C.
* GCC
* Make

Em sistemas linux utilize os comandos no terminal:
```sh
sudo apt install build-essential
sudo apt install make
```

### Compilação

Com as ferramentas instaladas para compilar o arquivo basta utilizar o comando `make` com o target a ser compilado :
```sh
make foo=loteria
```
Caso queira limpar os arquivos de compilação use `make clean` :
```sh
make clean foo=loteria
```

<!-- USAGE EXAMPLES -->
### Uso
Utilize os argumentos em linha de comando no terminal para modificar os parâmetros de leitura e escrita nos arquivos de entrada e saída :
```sh
./loteria loteria.input loteria.output
```

