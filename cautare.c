#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
    int* date;
    int dimensiune;
} Tablou;

void alocareTableau(Tablou* t, int dimensiune);
void eliberareTableau(Tablou* t);
void initializareAleatorie(Tablou* t);
void afisareTableau(Tablou* t);
void sortareTableau(Tablou* t);

int linearSearch(int arr[], int n, int target);
int binarySearch(int arr[], int n, int target);

double masuraTimpLinearSearch(int arr[], int n, int target);
double masuraTimpBinarySearch(int arr[], int n, int target);
void analizaEmpiricaCautare();

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void alocareTableau(Tablou* t, int dimensiune) {
    t->date = (int*)malloc(dimensiune * sizeof(int));
    if (t->date == NULL) {
        printf("Eroare la alocarea memoriei!\n");
        exit(1);
    }
    t->dimensiune = dimensiune;
    printf("Tablou alocat cu succes pentru %d elemente.\n", dimensiune);
}

void eliberareTableau(Tablou* t) {
    if (t->date != NULL) {
        free(t->date);
        t->date = NULL;
        t->dimensiune = 0;
        printf("Memoria a fost eliberată.\n");
    }
}

void initializareAleatorie(Tablou* t) {
    srand(time(NULL));
    for (int i = 0; i < t->dimensiune; i++) {
        t->date[i] = rand() % 10000;
    }
    printf("Tabloul a fost inițializat cu numere aleatorii.\n");
}

void afisareTableau(Tablou* t) {
    if (t->dimensiune > 100) {
        printf("\nPrimele 50 elemente:\n");
        for (int i = 0; i < 50; i++) {
            printf("%d ", t->date[i]);
            if ((i + 1) % 10 == 0) printf("\n");
        }
        printf("...\n");
    } else {
        for (int i = 0; i < t->dimensiune; i++) {
            printf("%d ", t->date[i]);
            if ((i + 1) % 10 == 0) printf("\n");
        }
    }
    printf("\n");
}

void sortareTableau(Tablou* t) {
    for (int i = 0; i < t->dimensiune - 1; i++) {
        for (int j = 0; j < t->dimensiune - i - 1; j++) {
            if (t->date[j] > t->date[j + 1]) {
                int temp = t->date[j];
                t->date[j] = t->date[j + 1];
                t->date[j + 1] = temp;
            }
        }
    }
    printf("Tabloul a fost sortat.\n");
}

// Linear Search - O(n)
int linearSearch(int arr[], int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

// Binary Search - O(log n) - necesită tablou sortat
int binarySearch(int arr[], int n, int target) {
    int left = 0;
    int right = n - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        }
        
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}

double masuraTimpLinearSearch(int arr[], int n, int target) {
    clock_t start = clock();
    int result = linearSearch(arr, n, target);
    clock_t end = clock();
    
    double timp = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    printf("\nLinear Search:\n");
    if (result != -1) {
        printf("Element găsit la poziția: %d\n", result);
    } else {
        printf("Element nu a fost găsit.\n");
    }
    printf("Timp de execuție: %.6f ms\n", timp);
    
    return timp;
}

double masuraTimpBinarySearch(int arr[], int n, int target) {
    clock_t start = clock();
    int result = binarySearch(arr, n, target);
    clock_t end = clock();
    
    double timp = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    printf("\nBinary Search:\n");
    if (result != -1) {
        printf("Element găsit la poziția: %d\n", result);
    } else {
        printf("Element nu a fost găsit.\n");
    }
    printf("Timp de execuție: %.6f ms\n", timp);
    
    return timp;
}

void analizaEmpiricaCautare() {
    int dimensiuni[] = {100, 1000, 10000, 100000};
    int num_dim = 4;
    int num_cautari = 10000;
    
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║    ANALIZĂ EMPIRICĂ - ALGORITMI DE CĂUTARE (10000x)      ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
    
    printf("Se efectuează %d căutări pentru fiecare dimensiune...\n\n", num_cautari);
    
    printf("┌──────────┬──────────────────┬──────────────────┐\n");
    printf("│    N     │  Linear Search   │  Binary Search   │\n");
    printf("│          │      (ms)        │      (ms)        │\n");
    printf("├──────────┼──────────────────┼──────────────────┤\n");
    
    for (int i = 0; i < num_dim; i++) {
        int n = dimensiuni[i];
        Tablou t;
        alocareTableau(&t, n);
        initializareAleatorie(&t);
        
        // Sortare pentru binary search
        sortareTableau(&t);
        
        // Linear search - 10000 căutări
        clock_t start_linear = clock();
        for (int j = 0; j < num_cautari; j++) {
            int target = rand() % 10000;
            linearSearch(t.date, n, target);
        }
        clock_t end_linear = clock();
        double timp_linear = ((double)(end_linear - start_linear)) / CLOCKS_PER_SEC * 1000.0;
        
        // Binary search - 10000 căutări
        clock_t start_binary = clock();
        for (int j = 0; j < num_cautari; j++) {
            int target = rand() % 10000;
            binarySearch(t.date, n, target);
        }
        clock_t end_binary = clock();
        double timp_binary = ((double)(end_binary - start_binary)) / CLOCKS_PER_SEC * 1000.0;
        
        printf("│ %-8d │ %14.6f   │ %14.6f   │\n", n, timp_linear, timp_binary);
        
        eliberareTableau(&t);
    }
    
    printf("└──────────┴──────────────────┴──────────────────┘\n");
}

void afisareMeniu() {
    printf("\n╔═══════════════════════════════════════════╗\n");
    printf("║   ALGORITMI DE CĂUTARE - ANALIZĂ         ║\n");
    printf("╚═══════════════════════════════════════════╝\n");
    printf("  1. Inițializarea tabloului cu numere aleatorii\n");
    printf("  2. Afișarea tabloului\n");
    printf("  3. Eliberarea memoriei tabloului\n");
    printf("  4. Linear Search (căutare liniară)\n");
    printf("  5. Binary Search (căutare binară - necesită sortare)\n");
    printf("  6. Sortarea tabloului (pentru binary search)\n");
    printf("  7. Analiză empirică completă (10000 căutări)\n");
    printf("  8. Ieșire\n");
    printf("───────────────────────────────────────────\n");
    printf("  Alegeți opțiunea: ");
}

int main() {
    Tablou tablou;
    tablou.date = NULL;
    tablou.dimensiune = 0;
    int optiune;
    int continua = 1;
    int sortat = 0;
    
    printf("\n╔═══════════════════════════════════════════╗\n");
    printf("║   Bun venit la Sistemul de Căutare!      ║\n");
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
                sortat = 0;
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
                sortat = 0;
                break;
            }
            
            case 4: {
                if (tablou.date == NULL) {
                    printf("Tabloul nu este alocat!\n");
                    break;
                }
                int target;
                printf("Numărul de căutat: ");
                scanf("%d", &target);
                masuraTimpLinearSearch(tablou.date, tablou.dimensiune, target);
                break;
            }
            
            case 5: {
                if (tablou.date == NULL) {
                    printf("Tabloul nu este alocat!\n");
                    break;
                }
                if (!sortat) {
                    printf("ATENȚIE: Tabloul trebuie sortat pentru binary search!\n");
                    printf("Folosiți opțiunea 6 pentru sortare.\n");
                    break;
                }
                int target;
                printf("Numărul de căutat: ");
                scanf("%d", &target);
                masuraTimpBinarySearch(tablou.date, tablou.dimensiune, target);
                break;
            }
            
            case 6: {
                if (tablou.date == NULL) {
                    printf("Tabloul nu este alocat!\n");
                    break;
                }
                sortareTableau(&tablou);
                sortat = 1;
                break;
            }
            
            case 7: {
                analizaEmpiricaCautare();
                break;
            }
            
            case 8: {
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
        
        if (continua && optiune != 8) {
            printf("\nApăsați Enter pentru a continua...");
            clearInputBuffer();
            getchar();
        }
    }
    
    return 0;
}
