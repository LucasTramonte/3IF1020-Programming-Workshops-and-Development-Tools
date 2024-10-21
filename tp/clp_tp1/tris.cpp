/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°1
 *
 * tris.cpp
 */

#include <iostream>
#include <vector>
#include <utility>
#include <functional>

#include <cstdlib>
#include <ctime>

//------------ 1.1. Affichage d'un tableau ------------

using int_tab_t = std::vector< int >;

// Fonction pour afficher le tableau
void print_tab( const int_tab_t &tab ) {
    std::cout << "[ ";
    for( const auto &elem : tab ) {
        std::cout << elem << " ";
    }
    std::cout << "]" << std::endl;
}

// Fonction de test
void test_11() {
    std::cout << "*** test_11 ***" << std::endl;
    
    // Tableau de 6 entiers ayant les valeurs indiquées
    const int_tab_t tab{ 1, -2, 3, -4, 5, -6 };
    print_tab( tab );
}

//------------ 1.2. Remplissage d'un tableau avec des nombres aléatoires ------------

// Fonction pour remplir le tableau avec des nombres aléatoires
void random_tab( int_tab_t &tab ) {
    for( auto &elem : tab ) {
        // Générer un nombre aléatoire entre -10 et 10
        elem = std::rand() % 21 - 10;
    }
}

// Fonction de test pour la question 1.2
void test_12() {
    std::cout << "*** test_12 ***" << std::endl;

    int_tab_t tab( 10 );
    random_tab( tab );
    print_tab( tab );
}

//------------ 1.3. Tri par ordre croissant ------------

void sort_tab_1( int_tab_t &tab ) {
    int n = tab.size();

    for( int i = 0; i < n - 1; ++i ) {
        int min_index = i;
        // Trouver l'index du plus petit élément dans le reste du tableau
        for( int j = i + 1; j < n; ++j ) {
            if( tab[j] < tab[min_index] ) {
                min_index = j;
            }
        }
        std::swap( tab[i], tab[min_index] );
    }
}

void test_13() {
    std::cout << "*** test_13 ***" << std::endl;

    int_tab_t tab( 10 );
    random_tab( tab );

    std::cout << "Tableau non trié : ";
    print_tab( tab );
    sort_tab_1( tab );

    std::cout << "Tableau trié : ";
    print_tab( tab );
}

//------------ 1.4. Tri selon un autre critère ------------

// renvoie vrai si a < b
bool less( int a, int b ) {
    return a < b;
}

// renvoie vrai si a > b
bool greater( int a, int b ) {
    return a > b;
}

// prend un pointeur sur fonction de comparaison
void sort_tab_2( int_tab_t &tab, bool (*compare)(int, int) ) {
    int n = tab.size();

    for( int i = 0; i < n - 1; ++i ) {
        int min_or_max_index = i;
        for( int j = i + 1; j < n; ++j ) {
            if( compare(tab[j], tab[min_or_max_index]) ) {
                min_or_max_index = j;
            }
        }
        // Échanger l'élément trouvé avec l'élément à la position courante
        std::swap( tab[i], tab[min_or_max_index] );
    }
}

void test_14() {
    std::cout << "*** test_14 ***" << std::endl;

    int_tab_t tab( 10 );
    random_tab( tab );

    std::cout << "Tableau non trié : ";
    print_tab( tab );

    std::cout << "Tri avec less() (ordre croissant) : ";
    sort_tab_2( tab, less );
    print_tab( tab );

    random_tab( tab );
    std::cout << "Tableau non trié (après remplissage) : ";
    print_tab( tab );

    std::cout << "Tri avec greater() (ordre décroissant) : ";
    sort_tab_2( tab, greater );
    print_tab( tab );
}

//------------ 1.5. Tri avec fonction lambda ------------

void sort_tab_3( int_tab_t &tab, std::function< bool(int, int) > compare ) {
    int n = tab.size();

    for( int i = 0; i < n - 1; ++i ) {
        int min_or_max_index = i;
        for( int j = i + 1; j < n; ++j ) {
            if( compare(tab[j], tab[min_or_max_index]) ) {
                min_or_max_index = j;
            }
        }
        // Échanger l'élément trouvé avec l'élément à la position courante
        std::swap( tab[i], tab[min_or_max_index] );
    }
}

void test_15() {
    std::cout << "*** test_15 ***" << std::endl;

    int_tab_t tab( 10 );

    random_tab( tab );
    std::cout << "Tableau non trié : ";
    print_tab( tab );

    std::cout << "Tri croissant sur la valeur absolue : ";
    sort_tab_3( tab, [](int a, int b) { return std::abs(a) < std::abs(b); });
    print_tab( tab );

    random_tab( tab );
    std::cout << "Tableau non trié (après remplissage) : ";
    print_tab( tab );

    std::cout << "Tri décroissant sur la valeur absolue : ";
    sort_tab_3( tab, [](int a, int b) { return std::abs(a) > std::abs(b); });
    print_tab( tab );
}


int main()
{
    std::srand( std::time( nullptr ));
    test_11();
    test_12();
    test_13();
    test_14();
    test_15();
    return 0;
}