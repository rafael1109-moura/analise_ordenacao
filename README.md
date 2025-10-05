# 📊 Análise Comparativa de Algoritmos de Ordenação

Este repositório contém o código-fonte e o relatório da Análise Experimental e Comparativa de Algoritmos de Ordenação, realizada para a disciplina de Estrutura de Dados Básicas 1.

O objetivo é comparar a performance de algoritmos com diferentes complexidades assintóticas: O(n²) (Quadrático) vs. O(n log n) (Log-Linear), em diversos cenários.

---

## 🚀 Algoritmos Analisados

| Classe de Complexidade | Algoritmo | Complexidade (Pior Caso) |
| :--- | :--- | :--- |
| **O(n²)** | Bubble Sort | O(n²) |
| **O(n²)** | Insertion Sort | O(n²) |
| **O(n²)** | Selection Sort | O(n²) |
| **O(n log n)** | Merge Sort | O(n log n) |
| **O(n log n)** | Quick Sort | O(n²) (Pior Caso) |
| **Bônus** | std::sort (IntroSort) | O(n log n) |

---

## 🧪 Metodologia Experimental

### 1. Cenários e Tamanhos (N)

Os algoritmos foram testados em três cenários principais para vetores de 10.000 a 50.000 elementos:

| Cenário | Descrição |
| :--- | :--- |
| **Aleatório** | Distribuição uniforme, representando o Caso Médio. |
| **Inversamente Ordenado** | Pior Caso para a maioria dos algoritmos (Ex: Quick Sort). |
| **Quase Ordenado** | Melhor Caso para algoritmos adaptativos (Ex: Insertion Sort). |

### 2. Métricas Coletadas

As seguintes métricas foram coletadas para cada execução:

| Métrica | Unidade | Objetivo |
| :--- | :--- | :--- |
| **Tempo de Execução** | Milissegundos (ms) | Medir a performance em tempo real. |
| **Comparações** | Contagem | Medir o número de operações de comparação entre elementos. |
| **Trocas** | Contagem | Medir o custo de movimentação de dados (Memória/Cache). |

---


