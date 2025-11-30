# Segmentação de Imagens via Grafos: AGM e Arborescência

Este projeto apresenta a implementação de duas abordagens distintas baseadas em Teoria dos Grafos para a segmentação de imagens digitais: **Árvore Geradora Mínima (MST)** e **Arborescência Mínima (Directed MST)**.

O software foi desenvolvido em C++ e utiliza o sistema de build CMake para facilitar a compilação em diferentes ambientes.

## Visão Geral

O objetivo principal deste trabalho é modelar uma imagem digital como um grafo, onde:
* **Vértices:** Representam os pixels da imagem.
* **Arestas:** Representam a conectividade entre pixels vizinhos (4-conexos ou 8-conexos).
* **Pesos:** Representam a dissimilaridade (diferença de cor/intensidade) entre os pixels.

A partir dessa modelagem, aplicamos algoritmos de otimização para particionar o grafo em regiões homogêneas.

### Métodos Implementados

1.  **Árvore Geradora Mínima (AGM / MST):**
    * Abordagem clássica para grafos não-direcionados (baseada no algoritmo de Kruskal).
    * Constrói uma árvore que conecta todos os pixels com o custo mínimo total.
    * A segmentação é obtida através do método *K-Cut*, removendo as arestas de maior peso para gerar componentes desconexos.

2.  **Arborescência Mínima (Directed MST):**
    * Abordagem para grafos direcionados (baseada no algoritmo de Edmonds/Chu-Liu).
    * Explora a simetria das distâncias de cor RGB para construir uma arborescência enraizada.
    * Utiliza a orientação de uma AGM via Busca em Largura (BFS) para definir a hierarquia dos segmentos.

---

## Estrutura do Projeto

O projeto está dividido em dois módulos principais. Certifique-se de acessar a pasta correta para compilar o algoritmo desejado:

```text
TrabalhoFinalTGC/
├── AGM/
│   ├── src/
│   └── CMakeLists.txt 
│
└── Aborescencia/
    └── trabalho-segmentacao/
        ├── x64/
        └── CMakeLists.txt
```

## Compilação e Build

Para facilitar o gerenciamento de dependências e a compilação, o projeto utiliza o **CMake**. Existem arquivos `CMakeLists.txt` independentes para cada algoritmo.

O diretório correto para cada um se encontra nos seguintes caminhos:

* **Para AGM:** `TrabalhoFinalTGC/AGM/`
* **Para Arborescência:** `TrabalhoFinalTGC/Aborescencia/trabalho-segmentacao/`

1. Navegue até a aplicação que deseja executar (onde o arquivo `CMakeLists.txt` está localizado).

2. Execute a sequência de comandos abaixo para criar a pasta de build e compilar o projeto:

```bash
mkdir build
cd build

cmake -G "MinGW Makefiles" ..

cmake --build .
```

### Rodando Arvore Geradora Mínima
Após fazer o build da AGM, para rodar é apenas necessário rodar o seguinte comando

```bash
.\segmentador.exe [caminho.\imagem-entrada] [numero-de-segmentaçõs]
```

### Rodando Arvore Geradora Arborescência
```bash
.\segmentador.exe [caminho.\imagem-entrada] [caminho.\imagem-saída]
```
Após isso, poderá escolher a quantidade de segmentações.

## Autores

* **Daniel Collina**
* **Diego Santos**
* **Felipe Silva**
* **Kauan Hauger**
* **Lucas Jardim**
* **Pedro Debs**
* **Samuel Horta de Faria**