#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Structura pentru tablou dinamic
typedef struct {
    int* date;
    int dimensiune;
} Tablou;

// Funcții pentru gestionare tablou
void alocareTableau(Tablou* t, int dimensiune);
void eliberareTableau(Tablou* t);
void initializareAleatorie(Tablou* t);
void afisareTableau(Tablou* t);

// Algoritmi de sortare simplă (O(n²))
void selectionSort(int arr[], int n);
void insertionSort(int arr[], int n);
void bubbleSort(int arr[], int n);

// Algoritmi de sortare avansată (O(n log n))
void mergeSort(int arr[], int left, int right);
void quickSort(int arr[], int low, int high);
void heapSort(int arr[], int n);

// Funcții auxiliare pentru sortări
void merge(int arr[], int left, int mid, int right);
int partition(int arr[], int low, int high);
void heapify(int arr[], int n, int i);

// Funcție de măsurare timp
double masuraTimpSortare(void (*sortFunc)(int[], int), int arr[], int n, const char* nume);
double masuraTimpMergeSort(int arr[], int n);
double masuraTimpQuickSort(int arr[], int n);

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Alocarea tabloului
void alocareTableau(Tablou* t, int dimensiune) {
    t->date = (int*)malloc(dimensiune * sizeof(int));
    if (t->date == NULL) {
        printf("Eroare la alocarea memoriei!\n");
        exit(1);
    }
    t->dimensiune = dimensiune;
    printf("Tablou alocat cu succes pentru %d elemente.\n", dimensiune);
}

// Eliberarea memoriei
void eliberareTableau(Tablou* t) {
    if (t->date != NULL) {
        free(t->date);
        t->date = NULL;
        t->dimensiune = 0;
        printf("Memoria a fost eliberată.\n");
    }
}

// Inițializare cu numere aleatorii
void initializareAleatorie(Tablou* t) {
    srand(time(NULL));
    for (int i = 0; i < t->dimensiune; i++) {
        t->date[i] = rand() % 10000;
    }
    printf("Tabloul a fost inițializat cu numere aleatorii.\n");
}

// Afișarea tabloului
void afisareTableau(Tablou* t) {
    if (t->dimensiune > 100) {
        printf("\nPrimele 50 elemente:\n");
        for (int i = 0; i < 50; i++) {
            printf("%d ", t->date[i]);
            if ((i + 1) % 10 == 0) printf("\n");
        }
        printf("...\nUltimele 50 elemente:\n");
        for (int i = t->dimensiune - 50; i < t->dimensiune; i++) {
            printf("%d ", t->date[i]);
            if ((i - t->dimensiune + 51) % 10 == 0) printf("\n");
        }
    } else {
        for (int i = 0; i < t->dimensiune; i++) {
            printf("%d ", t->date[i]);
            if ((i + 1) % 10 == 0) printf("\n");
        }
    }
    printf("\n");
}

// ==================== ALGORITMI DE SORTARE SIMPLĂ ====================

// Selection Sort - O(n²)
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

// Insertion Sort - O(n²)
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Bubble Sort - O(n²)
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// ==================== ALGORITMI DE SORTARE AVANSATĂ ====================

// Merge Sort - O(n log n)
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    
    free(L);
    free(R);
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Quick Sort - O(n log n) average
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Heap Sort - O(n log n)
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && arr[left] > arr[largest])
        largest = left;
    
    if (right < n && arr[right] > arr[largest])
        largest = right;
    
    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    
    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}

// ==================== MĂSURARE TIMP ====================

double masuraTimpSortare(void (*sortFunc)(int[], int), int arr[], int n, const char* nume) {
    int* temp = (int*)malloc(n * sizeof(int));
    memcpy(temp, arr, n * sizeof(int));
    
    clock_t start = clock();
    sortFunc(temp, n);
    clock_t end = clock();
    
    double timp = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    printf("\n%s:\n", nume);
    printf("Timp de execuție: %.6f ms\n", timp);
    
    free(temp);
    return timp;
}

double masuraTimpMergeSort(int arr[], int n) {
    int* temp = (int*)malloc(n * sizeof(int));
    memcpy(temp, arr, n * sizeof(int));
    
    clock_t start = clock();
    mergeSort(temp, 0, n - 1);
    clock_t end = clock();
    
    double timp = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    printf("\nMerge Sort:\n");
    printf("Timp de execuție: %.6f ms\n", timp);
    
    free(temp);
    return timp;
}

double masuraTimpQuickSort(int arr[], int n) {
    int* temp = (int*)malloc(n * sizeof(int));
    memcpy(temp, arr, n * sizeof(int));
    
    clock_t start = clock();
    quickSort(temp, 0, n - 1);
    clock_t end = clock();
    
    double timp = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    printf("\nQuick Sort:\n");
    printf("Timp de execuție: %.6f ms\n", timp);
    
    free(temp);
    return timp;
}

// Meniu principal
void afisareMeniu() {
    printf("\n╔═══════════════════════════════════════════╗\n");
    printf("║   ALGORITMI DE SORTARE - ANALIZĂ         ║\n");
    printf("╚═══════════════════════════════════════════╝\n");
    printf("  1. Inițializarea tabloului cu numere aleatorii\n");
    printf("  2. Afișarea tabloului\n");
    printf("  3. Eliberarea memoriei tabloului\n");
    printf("  4. Selection Sort (O(n²))\n");
    printf("  5. Insertion Sort (O(n²))\n");
    printf("  6. Bubble Sort (O(n²))\n");
    printf("  7. Merge Sort (O(n log n))\n");
    printf("  8. Quick Sort (O(n log n))\n");
    printf("  9. Heap Sort (O(n log n))\n");
    printf("  10. Analiză empirică completă\n");
    printf("  11. Ieșire\n");
    printf("───────────────────────────────────────────\n");
    printf("  Alegeți opțiunea: ");
}

void analizaEmpirica() {
    int dimensiuni[] = {100, 1000, 10000, 100000};
    int num_dim = 4;
    
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║         ANALIZĂ EMPIRICĂ - ALGORITMI DE SORTARE           ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
    
    printf("Performing empirical analysis...\n\n");
    
    printf("┌──────────┬──────────────────┬──────────────────┐\n");
    printf("│    N     │ Selection Sort   │   Heap Sort      │\n");
    printf("│          │     (ms)         │     (ms)         │\n");
    printf("├──────────┼──────────────────┼──────────────────┤\n");
    
    for (int i = 0; i < num_dim; i++) {
        int n = dimensiuni[i];
        Tablou t;
        alocareTableau(&t, n);
        initializareAleatorie(&t);
        
        double timp_selection = masuraTimpSortare(selectionSort, t.date, n, "");
        double timp_heap = masuraTimpSortare(heapSort, t.date, n, "");
        
        printf("│ %-8d │ %14.6f   │ %14.6f   │\n", n, timp_selection, timp_heap);
        
        eliberareTableau(&t);
    }
    
    printf("└──────────┴──────────────────┴──────────────────┘\n");
}

int main() {
    Tablou tablou;
    tablou.date = NULL;
    tablou.dimensiune = 0;
    int optiune;
    int continua = 1;
    
    printf("\n╔═══════════════════════════════════════════╗\n");
    printf("║   Bun venit la Sistemul de Sortare!      ║\n");
    printf("╚═══════════════════════════════════════════╝\n");
    
    while (continua) {
        afisareMeniu();
        scanf("%d", &optiune);
        
        switch (optiune) {
            case 1: {
                if (tablou.date != NULL) {
                    eliberareTableau(&tablou);
                }
                int dim;
                printf("Dimensiunea tabloului: ");
                scanf("%d", &dim);
                alocareTableau(&tablou, dim);
                initializareAleatorie(&tablou);
                break;
            }
            
            case 2: {
                if (tablou.date == NULL) {
                    printf("Tabloul nu este alocat!\n");
                    break;
                }
                afisareTableau(&tablou);
                break;
            }
            
            case 3: {
                eliberareTableau(&tablou);
                break;
            }
            
            case 4: {
                if (tablou.date == NULL) {
                    printf("Tabloul nu este alocat!\n");
                    break;
                }
                masuraTimpSortare(selectionSort, tablou.date, tablou.dimensiune, "Selection Sort");
                break;
            }
            
            case 5: {
                if (tablou.date == NULL) {
                    printf("Tabloul nu este alocat!\n");
                    break;
                }
                masuraTimpSortare(insertionSort, tablou.date, tablou.dimensiune, "Insertion Sort");
                break;
            }
            
            case 6: {
                if (tablou.date == NULL) {
                    printf("Tabloul nu este alocat!\n");
                    break;
                }
                masuraTimpSortare(bubbleSort, tablou.date, tablou.dimensiune, "Bubble Sort");
                break;
            }
            
            case 7: {
                if (tablou.date == NULL) {
                    printf("Tabloul nu este alocat!\n");
                    break;
                }
                masuraTimpMergeSort(tablou.date, tablou.dimensiune);
                break;
            }
            
            case 8: {
                if (tablou.date == NULL) {
                    printf("Tabloul nu este alocat!\n");
                    break;
                }
                masuraTimpQuickSort(tablou.date, tablou.dimensiune);
                break;
            }
            
            case 9: {
                if (tablou.date == NULL) {
                    printf("Tabloul nu este alocat!\n");
                    break;
                }
                masuraTimpSortare(heapSort, tablou.date, tablou.dimensiune, "Heap Sort");
                break;
            }
            
            case 10: {
                analizaEmpirica();
                break;
            }
            
            case 11: {
                printf("\nLa revedere!\n");
                if (tablou.date != NULL) {
                    eliberareTableau(&tablou);
                }
                continua = 0;
                break;
            }
            
            default: {
                printf("Opțiune invalidă!\n");
                break;
            }
        }
        
        if (continua && optiune != 11) {
            printf("\nApăsați Enter pentru a continua...");
            clearInputBuffer();
            getchar();
        }
    }
    
    return 0;
}
