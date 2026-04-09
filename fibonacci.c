#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

// Fibonacci folosind GMP (GNU Multiple Precision) pentru numere mari
void fibonacci_gmp(mpz_t result, unsigned long n) {
    mpz_t a, b, temp;
    mpz_init_set_ui(a, 0);
    mpz_init_set_ui(b, 1);
    mpz_init(temp);
    
    if (n == 0) {
        mpz_set(result, a);
    } else if (n == 1) {
        mpz_set(result, b);
    } else {
        for (unsigned long i = 2; i <= n; i++) {
            mpz_add(temp, a, b);
            mpz_set(a, b);
            mpz_set(b, temp);
        }
        mpz_set(result, b);
    }
    
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(temp);
}

// Fibonacci optimizat cu matrix exponentiation - MULT mai rapid pentru n mare
void fibonacci_matrix(mpz_t result, unsigned long n) {
    if (n == 0) {
        mpz_set_ui(result, 0);
        return;
    }
    if (n == 1) {
        mpz_set_ui(result, 1);
        return;
    }
    
    mpz_t a, b, c, ta, tb, tc;
    mpz_init_set_ui(a, 1);
    mpz_init_set_ui(b, 1);
    mpz_init_set_ui(c, 1);
    mpz_init(ta);
    mpz_init(tb);
    mpz_init(tc);
    
    unsigned long mask = 1UL << 62;
    while (mask && !(n & mask)) mask >>= 1;
    mask >>= 1;
    
    while (mask) {
        // Pătrat: F(2k) = F(k)[2F(k+1) - F(k)]
        mpz_mul(ta, b, b);
        mpz_mul(tb, a, a);
        mpz_add(tc, ta, tb);
        
        mpz_mul_ui(ta, b, 2);
        mpz_sub(ta, ta, a);
        mpz_mul(ta, ta, a);
        
        mpz_set(a, ta);
        mpz_set(b, tc);
        
        if (n & mask) {
            // Incrementare: F(k+1) = F(k) + F(k-1)
            mpz_add(tc, a, b);
            mpz_set(a, b);
            mpz_set(b, tc);
        }
        
        mask >>= 1;
    }
    
    mpz_set(result, a);
    
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(c);
    mpz_clear(ta);
    mpz_clear(tb);
    mpz_clear(tc);
}

double masuraTimpFibonacci(unsigned long n) {
    mpz_t result;
    mpz_init(result);
    
    clock_t start = clock();
    fibonacci_matrix(result, n);
    clock_t end = clock();
    
    double timp = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    // Afișare rezultat (primele și ultimele cifre pentru numere foarte mari)
    size_t num_digits = mpz_sizeinbase(result, 10);
    
    printf("\nFibonacci(%lu):\n", n);
    printf("Număr de cifre: %zu\n", num_digits);
    
    if (num_digits <= 200) {
        gmp_printf("Valoare: %Zd\n", result);
    } else {
        char* str = mpz_get_str(NULL, 10, result);
        printf("Primele 100 cifre: %.100s...\n", str);
        printf("Ultimele 100 cifre: ...%s\n", str + num_digits - 100);
        free(str);
    }
    
    printf("Timp de execuție: %.6f ms\n", timp);
    
    mpz_clear(result);
    return timp;
}

void analizaEmpiricaFibonacci() {
    unsigned long pozitii[] = {50, 100, 1000, 10000, 100000, 1000000};
    int num_poz = 6;
    
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║         ANALIZĂ EMPIRICĂ - FIBONACCI (MATRIX)             ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
    
    printf("┌──────────────┬──────────────────┬──────────────────┐\n");
    printf("│      N       │  Timpul (ms)     │  Cifre în număr  │\n");
    printf("├──────────────┼──────────────────┼──────────────────┤\n");
    
    for (int i = 0; i < num_poz; i++) {
        unsigned long n = pozitii[i];
        mpz_t result;
        mpz_init(result);
        
        clock_t start = clock();
        fibonacci_matrix(result, n);
        clock_t end = clock();
        
        double timp = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        size_t num_digits = mpz_sizeinbase(result, 10);
        
        printf("│ %-12lu │ %14.6f   │ %-16zu │\n", n, timp, num_digits);
        
        mpz_clear(result);
    }
    
    printf("└──────────────┴──────────────────┴──────────────────┘\n");
}

void verificareCalcule() {
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║            VERIFICARE CALCULE FIBONACCI                   ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
    
    mpz_t result;
    mpz_init(result);
    
    // Test fib(0) == 0
    fibonacci_matrix(result, 0);
    printf("fib(0) = ");
    gmp_printf("%Zd ", result);
    printf("%s\n", mpz_cmp_ui(result, 0) == 0 ? "✓" : "✗");
    
    // Test fib(1) == 1
    fibonacci_matrix(result, 1);
    printf("fib(1) = ");
    gmp_printf("%Zd ", result);
    printf("%s\n", mpz_cmp_ui(result, 1) == 0 ? "✓" : "✗");
    
    // Test fib(5) == 5
    fibonacci_matrix(result, 5);
    printf("fib(5) = ");
    gmp_printf("%Zd ", result);
    printf("%s\n", mpz_cmp_ui(result, 5) == 0 ? "✓" : "✗");
    
    // Test fib(10) == 55
    fibonacci_matrix(result, 10);
    printf("fib(10) = ");
    gmp_printf("%Zd ", result);
    printf("%s\n", mpz_cmp_ui(result, 55) == 0 ? "✓" : "✗");
    
    // Test fib(100)
    fibonacci_matrix(result, 100);
    printf("fib(100) = ");
    gmp_printf("%Zd\n", result);
    printf("(număr foarte mare cu %zu cifre)\n", mpz_sizeinbase(result, 10));
    
    mpz_clear(result);
}

void afisareMeniu() {
    printf("\n╔═══════════════════════════════════════════╗\n");
    printf("║        CALCULATOR FIBONACCI               ║\n");
    printf("╚═══════════════════════════════════════════╝\n");
    printf("  1. Calculare Fibonacci pentru poziția N\n");
    printf("  2. Verificare calcule (fib(0) ... fib(100))\n");
    printf("  3. Analiză empirică completă\n");
    printf("  4. Test performanță (fib(1000000))\n");
    printf("  5. Ieșire\n");
    printf("───────────────────────────────────────────\n");
    printf("  Alegeți opțiunea: ");
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int optiune;
    int continua = 1;
    
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║   Bun venit la Calculatorul Fibonacci (suport numere mari)║\n");
    printf("║   Implementare: Matrix Exponentiation + GMP Library      ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    while (continua) {
        afisareMeniu();
        scanf("%d", &optiune);
        
        switch (optiune) {
            case 1: {
                unsigned long n;
                printf("\nIntroduceți poziția N (50 - 1000000): ");
                scanf("%lu", &n);
                
                if (n < 0 || n > 10000000) {
                    printf("Poziție invalidă! Alegeți între 50 și 1000000.\n");
                    break;
                }
                
                masuraTimpFibonacci(n);
                break;
            }
            
            case 2: {
                verificareCalcule();
                break;
            }
            
            case 3: {
                analizaEmpiricaFibonacci();
                break;
            }
            
            case 4: {
                printf("\nCalculăm fib(1000000) - POATE DURA CÂTEVA SECUNDE...\n");
                masuraTimpFibonacci(1000000);
                break;
            }
            
            case 5: {
                printf("\nLa revedere!\n");
                continua = 0;
                break;
            }
            
            default: {
                printf("Opțiune invalidă!\n");
                break;
            }
        }
        
        if (continua && optiune != 5) {
            printf("\nApăsați Enter pentru a continua...");
            clearInputBuffer();
            getchar();
        }
    }
    
    return 0;
}
