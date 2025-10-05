#include <iostream>
#include <vector>
#include <algorithm> // Para std::swap, std::min, e AGORA, std::sort
#include <chrono>    // Para medição de tempo
#include <random>    // Para números aleatórios
#include <cmath>     // Para std::floor

using namespace std;
using namespace std::chrono;

// --- ESTRUTURAS E VARIÁVEIS DE MÉTRICAS ---
struct Metrics {
    long long comparisons;
    long long swaps;
};

// Variável global para armazenar as métricas durante a execução de cada algoritmo
Metrics currentMetrics;

void resetMetrics() {
    currentMetrics = {0, 0};
}

// Função auxiliar para trocar elementos e contabilizar trocas
void swapElements(int& a, int& b) {
    swap(a, b);
    currentMetrics.swaps++;
}

// Função auxiliar para contabilizar comparações
bool compare(int a, int b) {
    currentMetrics.comparisons++;
    return a < b; // Retorna true se a for menor que b
}

// --- FUNÇÕES DE GERAÇÃO DE DADOS ---

// 1. Gera vetor com elementos aleatórios
vector<int> generate_random(int size) {
    vector<int> arr(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, size * 10);
    
    for (int i = 0; i < size; ++i) {
        arr[i] = distrib(gen);
    }
    return arr;
}

// 2. Gera vetor inversamente ordenado
vector<int> generate_inversely_ordered(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = size - i;
    }
    return arr;
}

// 3. Gera vetor "quase ordenado" (ordenado com 10% de desordem no final)
vector<int> generate_quasi_ordered(int size) {
    // Para que as métricas de troca não interfiram, usamos um objeto de métricas local
    Metrics tempMetrics = currentMetrics; 
    
    vector<int> arr(size);
    
    // Inicia totalmente ordenado
    for (int i = 0; i < size; ++i) {
        arr[i] = i + 1;
    }

    // Desordena os últimos 10%
    int start_index = size * 0.9; 
    random_device rd;
    mt19937 gen(rd());
    
    // Temporariamente, ignoramos a contagem de trocas para a desordem inicial
    for (int i = start_index; i < size; ++i) {
        uniform_int_distribution<> distrib(start_index, size - 1);
        int j = distrib(gen);
        std::swap(arr[i], arr[j]);
    }
    
    currentMetrics = tempMetrics; // Restaura a contagem para zero
    return arr;
}

// --- ALGORITMOS DE ORDENAÇÃO (com métricas) ---

// 1. Bubble Sort
void bubble_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; ++j) {
            if (!compare(arr[j], arr[j + 1])) { // Se arr[j] >= arr[j+1]
                swapElements(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// 2. Insertion Sort
void insertion_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        
        // Loop para encontrar a posição (conta as comparações)
        while (j >= 0) { 
            currentMetrics.comparisons++; // Conta a comparação
            if (arr[j] > key) {
                 arr[j + 1] = arr[j];
                 currentMetrics.swaps++; // Contabiliza o deslocamento
                 j = j - 1;
            } else {
                 break;
            }
        }
        arr[j + 1] = key;
    }
}

// 3. Selection Sort
void selection_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            // Compara e conta a comparação
            if (compare(arr[j], arr[min_idx])) {
                min_idx = j;
            }
        }
        // Troca (se necessário) e conta a troca
        if (min_idx != i) {
            swapElements(arr[i], arr[min_idx]);
        }
    }
}

// Funções Auxiliares para Merge Sort e Quick Sort
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0; 
    int j = 0; 
    int k = left; 

    while (i < n1 && j < n2) {
        // Compara e conta
        currentMetrics.comparisons++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            k++;
            i++;
        } else {
            arr[k] = R[j];
            currentMetrics.swaps++; // Contabiliza deslocamento como troca
            k++;
            j++;
        }
    }

    while (i < n1) {
        arr[k] = L[i];
        k++;
        i++;
    }

    while (j < n2) {
        arr[k] = R[j];
        k++;
        j++;
    }
}

// 4. Merge Sort
void merge_sort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high]; 
    int i = (low - 1); 

    for (int j = low; j <= high - 1; ++j) {
        // Compara e conta
        currentMetrics.comparisons++;
        if (arr[j] < pivot) {
            i++;
            swapElements(arr[i], arr[j]);
        }
    }
    swapElements(arr[i + 1], arr[high]);
    return (i + 1);
}

// 5. Quick Sort
void quick_sort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

// --- FUNÇÃO EXTRA (std::sort) para BÔNUS ---
// std::sort usa um algoritmo híbrido (IntroSort) muito otimizado.

void std_sort_wrapper(vector<int>& arr) {
    // O algoritmo nativo não expõe suas métricas internas de comparações/trocas
    // de forma trivial, então focamos apenas no tempo.
    
    resetMetrics(); 
    
    // Inicia a medição de tempo
    auto start = high_resolution_clock::now();
    
    // Executa o algoritmo nativo
    std::sort(arr.begin(), arr.end());
    
    // Para a medição de tempo
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); // Usamos microsegundos para maior precisão

    // Imprime o resultado. Note que as métricas de Comparações e Trocas serão "N/A"
    cout << "std::sort" << "\t\t" 
         << "N/A" << "\t\t\t"
         << "N/A" << "\t\t\t" 
         << duration.count() << endl; // tempo em microssegundos
}

// --- FUNÇÃO PRINCIPAL DE TESTE (MAIN) ---

void runTest(const string& algorithmName, void (*sortFunc)(vector<int>&), vector<int> data) {
    
    // Se não for o std::sort, resetamos as métricas
    if (algorithmName != "std::sort") {
        resetMetrics();
    }
    
    // Inicia a medição de tempo
    auto start = high_resolution_clock::now();
    
    // Executa o algoritmo
    sortFunc(data);
    
    // Para a medição de tempo
    auto end = high_resolution_clock::now();
    // Medimos em Milissegundos (ms) para os algoritmos lentos e em Microsegundos (us) para o std::sort
    auto duration_ms = duration_cast<milliseconds>(end - start);
    
    // Se for std::sort, a função já imprime a saída, pois usa tempo diferente (us)
    if (algorithmName == "std::sort") {
        // Se for o std::sort, a função wrapper já imprimiu a saída e usou microsegundos
        return; 
    }

    // Imprime o resultado para os 5 algoritmos da atividade
    cout << algorithmName << "\t\t" 
         << currentMetrics.comparisons << "\t\t" 
         << currentMetrics.swaps << "\t\t" 
         << duration_ms.count() << endl;
}

int main() {
    // Definir os tamanhos dos vetores para teste. 
    // Mantenha 10000, 50000 e 100000 conforme o que já foi testado.
    vector<int> sizes = {10000, 50000, 100000}; 

    // O ponteiro de função 'sortFunc' será usado para chamar a versão correta
    typedef void (*SortFunction)(vector<int>&);

    // Mapeia o nome do algoritmo para a função
    vector<pair<string, SortFunction>> algorithms = {
        {"Bubble Sort", bubble_sort},
        {"Insertion Sort", insertion_sort},
        {"Selection Sort", selection_sort}
    };

    // Merge Sort e Quick Sort precisam de tratamento especial devido aos argumentos (low, high)
    // Usamos Lambdas como wrappers para simplificar a chamada.
    auto merge_sort_wrapper = [](vector<int>& arr) { merge_sort(arr, 0, arr.size() - 1); };
    auto quick_sort_wrapper = [](vector<int>& arr) { quick_sort(arr, 0, arr.size() - 1); };
    
    algorithms.push_back({"Merge Sort", merge_sort_wrapper});
    algorithms.push_back({"Quick Sort", quick_sort_wrapper});
    
    // *** LINHA PARA O BÔNUS: Adiciona std::sort na lista de testes ***
    algorithms.push_back({"std::sort", std_sort_wrapper});


    cout << "--- Análise Comparativa de Algoritmos de Ordenação ---\n" << endl;
    cout << "Tempo em (ms) = Milissegundos / Tempo do std::sort em (us) = Microsegundos.\n" << endl;

    for (int size : sizes) {
        cout << "========================================================================\n";
        cout << "TESTE COM TAMANHO DO VETOR: " << size << "\n";
        cout << "========================================================================\n";

        // --- TESTES COM VETOR ALEATÓRIO ---
        cout << "\n--- Caso Aleatório ---\n";
        cout << "Algoritmo\tComparações\tTrocas\t\tTempo (ms/us)\n";
        cout << "------------------------------------------------------------------------\n";
        vector<int> random_data = generate_random(size);
        for (auto const& [name, func] : algorithms) {
            runTest(name, func, random_data);
        }

        // --- TESTES COM VETOR INVERSAMENTE ORDENADO (PIOR CASO) ---
        cout << "\n--- Caso Inversamente Ordenado ---\n";
        cout << "Algoritmo\tComparações\tTrocas\t\tTempo (ms/us)\n";
        cout << "------------------------------------------------------------------------\n";
        vector<int> inversely_data = generate_inversely_ordered(size);
        for (auto const& [name, func] : algorithms) {
            runTest(name, func, inversely_data);
        }

        // --- TESTES COM VETOR QUASE ORDENADO ---
        cout << "\n--- Caso Quase Ordenado ---\n";
        cout << "Algoritmo\tComparações\tTrocas\t\tTempo (ms/us)\n";
        cout << "------------------------------------------------------------------------\n";
        vector<int> quasi_data = generate_quasi_ordered(size);
        for (auto const& [name, func] : algorithms) {
            runTest(name, func, quasi_data);
        }
    }

    return 0;
}