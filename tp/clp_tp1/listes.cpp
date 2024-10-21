/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°1
 *
 * listes.cpp
 */

#include <iostream>
#include <forward_list>
#include <functional>
#include <limits>

#include <cstdlib>
#include <ctime>

//------------ 2.1. Création, affichage ------------

using int_list_t = std::forward_list< int >;

int_list_t random_list() {
    int_list_t list;
    int nb_elements = std::rand() % 10;  // Nombre d'éléments aléatoires entre 0 et 9

    for( int i = 0; i < nb_elements; ++i ) {
        int random_value = std::rand() % 100;  // Valeurs aléatoires entre 0 et 99
        list.push_front( random_value );       
    }
    return list;
}

void print_list( const int_list_t &list ) {
    std::cout << "( ";
    for( auto elem : list ) {
        std::cout << elem << " ";
    }
    std::cout << ")" << std::endl;
}

void test_21() {
    std::cout << "*** test_21 ***" << std::endl;

    int_list_t list = random_list();
    print_list( list );
}

//------------ 2.2. Application ------------

int_list_t map_iter(const int_list_t &list, std::function<int(int)> func) {
    int_list_t result;
    for (auto elem : list) {
        result.push_front(func(elem));  
    }
    return result;  
}

void test_22() {
    std::cout << "*** test_22 ***" << std::endl;

    int_list_t list = random_list();

    std::cout << "Liste initiale : ";
    print_list(list);

    int_list_t mapped_list = map_iter(list, [](int x) { return x * 3; });
    std::cout << "Liste après map_iter (multiplication par 3) : ";
    print_list(mapped_list);
}

//------------ 2.3. Filtrage ------------

int_list_t filter_iter(const int_list_t &list, std::function<bool(int)> predicate) {
    int_list_t result;
    for (auto elem : list) {
        if (predicate(elem)) {
            result.push_front(elem);  
        }
    }
    return result;  
}

void test_23() {
    std::cout << "*** test_23 ***" << std::endl;

    int_list_t list = random_list();

    std::cout << "Liste initiale : ";
    print_list(list);

    int_list_t mapped_list = map_iter(list, [](int x) { return x * 3; });
    std::cout << "Liste après map_iter (multiplication par 3) : ";
    print_list(mapped_list);

    int_list_t filtered_list = filter_iter(mapped_list, [](int x) { return x % 2 == 0; });

    std::cout << "Liste après filter_iter (éléments pairs) : ";
    print_list(filtered_list);
}

//------------ 2.4. Capture ------------

void test_24() {
    std::cout << "*** test_24 ***" << std::endl;

    int_list_t list = random_list();
    std::cout << "Liste initiale : ";
    print_list(list);

    int coef = std::rand() % 5 + 1;  // Coefficient aléatoire entre 1 et 5
    std::cout << "Coefficient de multiplication : " << coef << std::endl;

    int_list_t mapped_list = map_iter(list, [coef](int x) { return x * coef; });
    std::cout << "Liste après map_iter (multiplication par " << coef << ") : ";
    print_list(mapped_list);

    int_list_t filtered_list = filter_iter(mapped_list, [](int x) { return x % 2 == 0; });
    std::cout << "Liste après filter_iter (éléments pairs) : ";
    print_list(filtered_list);
}

//------------ 2.5. Réduction ------------

int reduce(const int_list_t &list, int initial_value, std::function<int(int, int)> func) {
    int result = initial_value;
    for (auto elem : list) {
        result = func(result, elem);  
    }
    return result; 
}

void test_25() {
    std::cout << "*** test_25 ***" << std::endl;

    int_list_t list = random_list();

    std::cout << "Liste initiale : ";
    print_list(list);

    int min_value = reduce(list, std::numeric_limits<int>::max(), [](int a, int b) {
        return (a < b) ? a : b; 
    });
    std::cout << "Plus petit élément : " << min_value << std::endl;

    int max_value = reduce(list, std::numeric_limits<int>::min(), [](int a, int b) {
        return (a > b) ? a : b;  
    });
    std::cout << "Plus grand élément : " << max_value << std::endl;
}

//------------ 3.1. Récursion pour reduce() ------------

int fold_left_aux(int_list_t::const_iterator it, int_list_t::const_iterator end, int initial_value, std::function<int(int, int)> func) {
    if (it == end) {  
        return initial_value;  
    }
    return fold_left_aux(std::next(it), end, func(initial_value, *it), func);
}

int fold_left(const int_list_t &list, int initial_value, std::function<int(int, int)> func) {
    return fold_left_aux(list.cbegin(), list.cend(), initial_value, func);
}

void test_31() {
    std::cout << "*** test_31 ***" << std::endl;

    int_list_t list = random_list();
    std::cout << "Liste initiale : ";
    print_list(list);

    int min_value = fold_left(list, std::numeric_limits<int>::max(), [](int a, int b) {
        return (a < b) ? a : b;  
    });
    std::cout << "Plus petit élément : " << min_value << std::endl;


    int max_value = fold_left(list, std::numeric_limits<int>::min(), [](int a, int b) {
        return (a > b) ? a : b;  
    });
    std::cout << "Plus grand élément : " << max_value << std::endl;
}

//------------ 3.2. Récursion pour map() et filter() ------------

void map_aux(int_list_t::const_iterator it, int_list_t::const_iterator end, std::function<int(int)> func, int_list_t &result) {
    if (it == end) {  
        return;
    }
    result.push_front(func(*it));  
    map_aux(std::next(it), end, func, result);
}

int_list_t map(const int_list_t &list, std::function<int(int)> func) {
    int_list_t result;  
    map_aux(list.cbegin(), list.cend(), func, result);
    result.reverse(); 
    return result;
}

void filter_aux(int_list_t::const_iterator it, int_list_t::const_iterator end, std::function<bool(int)> pred, int_list_t &result) {
    if (it == end) {  
        return;
    }
    if (pred(*it)) {
        result.push_front(*it);  
    }
    filter_aux(std::next(it), end, pred, result);
}

int_list_t filter(const int_list_t &list, std::function<bool(int)> pred) {
    int_list_t result;  
    filter_aux(list.cbegin(), list.cend(), pred, result);
    result.reverse(); 
    return result;
}

void test_32() {
    std::cout << "*** test_32 ***" << std::endl;

    int_list_t list = random_list();
    std::cout << "Liste initiale : ";
    print_list(list);

    int coef = std::rand() % 5 + 1;  // Coefficient entre 1 et 5
    std::cout << "Coefficient de multiplication : " << coef << std::endl;

    int_list_t mapped_list = map(list, [coef](int x) { return x * coef; });
    std::cout << "Liste après map() : ";
    print_list(mapped_list);

    int_list_t filtered_list = filter(mapped_list, [](int x) { return x % 2 == 0; });
    std::cout << "Liste après filter() : ";
    print_list(filtered_list);
}

//------------ 4.1. Application partielle ------------

int_list_t map2(const int_list_t &list, std::function<int(int)> func) {
    int_list_t result;  
    for (auto elem : list) {
        result.push_front(func(elem)); 
    }
    result.reverse();  
    return result;
}

void test_41() {
    std::cout << "*** test_41 ***" << std::endl;

    int_list_t list = random_list();
    std::cout << "Liste initiale : ";
    print_list(list);

    int coef = std::rand() % 5 + 1;  // Coefficient entre 1 et 5
    std::cout << "Coefficient de multiplication : " << coef << std::endl;

    std::multiplies<int> multiply_op;  
    auto multiply_by_coef = [multiply_op, coef](int x) { return multiply_op(x, coef); };  

    int_list_t mapped_list = map2(list, multiply_by_coef);
    std::cout << "Liste après map() : ";
    print_list(mapped_list);

    int_list_t filtered_list = filter(mapped_list, [](int x) { return x % 2 == 0; });
    std::cout << "Liste après filter() : ";
    print_list(filtered_list);
}

//------------ 4.2. Réduction inversée ------------

int fold_right(int_list_t::const_iterator it, int_list_t::const_iterator end, int init, std::function<int(int, int)> func) {
    if (it == end) {
        return init;  
    } else {
        return fold_right(std::next(it), end, func(*it, init), func);
    }
}

void test_42() {
    std::cout << "*** test_42 ***" << std::endl;

    int_list_t list = random_list();
    std::cout << "Liste initiale : ";
    print_list(list);

    auto add = [](int a, int b) { return a + b; };

    int result_left = fold_left(list, 0, add);
    int result_right = fold_right(list.cbegin(), list.cend(), 0, add);

    std::cout << "Résultat de fold_left : " << result_left << std::endl;
    std::cout << "Résultat de fold_right : " << result_right << std::endl;

    if (result_left != result_right) {
        std::cout << "Les résultats sont différents." << std::endl;
    } else {
        std::cout << "Les résultats sont identiques." << std::endl;
    }
}


int main(){
    std::srand( std::time( nullptr ));
    test_21();
    test_22();
    test_23();
    test_24();
    test_25();
    test_31();
    test_32();
    test_41();
    test_42();

    return 0;
}



