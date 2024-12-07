/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique - Campus de Paris-Saclay
 * 3IF1020 – Ateliers de programmation et outils de développement - TD n°1
 * https://wdi.centralesupelec.fr/3IF1020/ProgTD1
 *
 * c++ -std=c++20 -o td1 td1.cpp
 * ./td1
 */


#include <iostream>
#include <cmath>
#include <functional>
#include <numbers>


// Définir une fonction qui calcule et retourne la valeur de 𝑓 en 𝑥
double sin_x_plus_cos_sqrt2_times_x( double x )
{
    return std::sin(x) + std::cos(std::sqrt(2) * x);
}

// Définir une fonction test_11() qui appelle la précédente avec 1 puis avec -4.5 comme
// argument, et qui affiche les résultats retournés.
void test_11()
{
    std::cout << "test_11:\n";
    std::cout << "Résultat pour x = 1 : " << sin_x_plus_cos_sqrt2_times_x(1) << std::endl;
    std::cout << "Résultat pour x = -4.5 : " << sin_x_plus_cos_sqrt2_times_x(-4.5) << std::endl;
}

// Définir une fonction test_12() qui demande une valeur à l'utilisateur, appelle la
// fonction sin_x_plus_cos_sqrt2_times_x() avec la valeur fournie par l'utilisateur
// comme argument et affiche le résultat retourné. 
void test_12()
{

    std::cout << "test_12:\n";
    double user_input;
    std::cout << "Veuillez entrer une valeur: ";
    std::cin >> user_input;
    double result = sin_x_plus_cos_sqrt2_times_x(user_input);
    std::cout << "Le résultat est: " << result << std::endl;
}

// Définir une fonction test_21() qui demande une valeur à l'utilisateur, et affiche le
// résultat de l'appel de la fonction sin_x_plus_cos_sqrt2_times_x() pour 10 valeurs
// espacées de 1 en commençant avec la valeur fournie par l'utilisateur.
// Vous utiliserez une boucle for avec une variable de boucle commençant à 0.
void test_21()
{
    std::cout << "test_21: \n";
    double x;
    std::cout << "Entrez une valeur de départ : ";
    std::cin >> x;

    for (int i = 0; i < 10; ++i)
    {
        std::cout << "x = " << x + i << " : " << sin_x_plus_cos_sqrt2_times_x(x + i) << std::endl;
    }
}

// Définir une fonction qui affiche le résultat de la fonction sin_x_plus_cos_sqrt2_times_x()
// pour toutes les valeurs entre begin et end avec un pas de step. 
void print_sin_x_plus_cos_sqrt2_times_x( double begin, double end, double step )
{
    for (double x = begin; x <= end; x += step)
    {
        std::cout << "x = " << x << " : " << sin_x_plus_cos_sqrt2_times_x(x) << std::endl;
    }
}

// Définir une fonction test_22() qui appelle la précédente avec -10, 10 et 2 comme arguments
void test_22()
{
    std::cout << "test_22:\n";
    print_sin_x_plus_cos_sqrt2_times_x(-10, 10, 2);
}

// Définir une fonction test_23() qui demande à l'utilisateur une borne basse, une borne
// haute (qui devra être supérieure à la borne basse) et un nombre de valeurs à afficher,
// puis appelle la fonction print_sin_x_plus_cos_sqrt2_times_x() pour afficher les valeurs
// demandées. 
// Vous utiliserez une instruction if pour vérifier que la borne haute donnée par l'utilisateur
// est strictement supérieure à la borne basse, et une instruction while pour refaire une
// demande en cas de test négatif ; vous ferez de même pour vérifier qu'au moins 2 valeurs
// sont demandées. 
void test_23()
{
    std::cout << "test_23: TODO\n";
    double begin, end;
    int n;

    // Demander la borne basse
    std::cout << "Entrez la borne basse : ";
    std::cin >> begin;

    // Demander la borne haute avec vérification
    do {
        std::cout << "Entrez la borne haute (doit être supérieure à la borne basse) : ";
        std::cin >> end;
    } while (end <= begin);

    // Demander le nombre de valeurs avec vérification
    do {
        std::cout << "Entrez le nombre de valeurs à afficher (au moins 2) : ";
        std::cin >> n;
    } while (n < 2);

    double step = (end - begin) / (n - 1);
    print_sin_x_plus_cos_sqrt2_times_x(begin, end, step);
}

// Définir une fonction qui retourne la valeur estimée de la dérivée de la fonction func en x
double compute_derivative( std::function< double( double ) > func, double x, double epsilon )
{
    return (func(x + epsilon) - func(x)) / epsilon;
}

// Définir une fonction test_31() qui affiche l'estimation de la dérivée de
// sin_x_plus_cos_sqrt2_times_x() dans l'intervalle [-4.6, -4.5] par pas de 0.01 avec une
// valeur d'epsilon égale à 10-5. 
void test_31()
{
    std::cout << "test_31: \n";
    double epsilon = 1e-5;
    for (double x = -4.6; x <= -4.5; x += 0.01)
    {
        std::cout << "Dérivée en x = " << x << " : " << compute_derivative(sin_x_plus_cos_sqrt2_times_x, x, epsilon) << std::endl;
    }
}

// Définir une fonction qui retourne un nombre compris dans l'intervalle [begin, end] pour
// lequel le résultat de l'appel de func sur ce nombre est inférieur en valeur absolue à
// precision.
// Cette fonction a pour précondition que func( begin ) et func( end ) sont de signes
// opposés ; si cette précondition n'est pas vérifiée, la fonction retournera 0.
// Vous procéderez par dichotomie.
double find_zero( std::function< double( double ) > func, double begin, double end, double precision )
{
    if (func(begin) * func(end) > 0)
        return 0; // Précondition non respectée

    while ((end - begin) > precision)
    {
        double mid = (begin + end) / 2;
        if (func(mid) == 0.0)
            return mid;
        else if (func(begin) * func(mid) < 0)
            end = mid;
        else
            begin = mid;
    }
    return (begin + end) / 2;
}

// Définir une fonction test_32() qui cherche un zéro de la fonction sin_x_plus_cos_sqrt2_times_x()
// dans l'intervalle [-2, 0] avec une précision de 10-5
void test_32()
{
    std::cout << "test_32: \n";
    double zero = find_zero(sin_x_plus_cos_sqrt2_times_x, -2, 0, 1e-5);
    std::cout << "Zéro trouvé : " << zero << std::endl;
}

// Définir une fonction qui cherche dans chaque intervalle de largeur width (de la forme
// [begin + n * width, begin + (n + 1) * width] où n est un naturel quelconque), sans
// dépasser end comme borne haute, un nombre pour lequel le résultat de l'appel de func
// est inférieur en valeur absolue à precision ; au maximum, max_number_of_results seront
// retournés dans le tableau results ; la fonction a pour résultat le nombre exact de
// valeurs présentes dans results.
int find_all_zeros( std::function< double( double ) > func, double begin, double end, double width,
                    double precision, double results[], double max_number_of_results )
{
    int count = 0;
    for (double x = begin; x < end && count < max_number_of_results; x += width)
    {
        double zero = find_zero(func, x, x + width, precision);
        if (zero != 0) // Si un zéro a été trouvé
        {
            results[count++] = zero;
        }
    }
    return count;
}

// Définir une fonction test_41() qui cherche les zéros de la fonction
// sin_x_plus_cos_sqrt2_times_x() dans l'intervalle [-10, 10], avec une largeur de 0.5,
// une précision de 10-5 et un maximum de 10 zéros retournés. 
void test_41()
{
    std::cout << "test_41:\n";
    double results[10];
    int count = find_all_zeros(sin_x_plus_cos_sqrt2_times_x, -10, 10, 0.5, 1e-5, results, 10);
    std::cout << "Zéros trouvés (" << count << ") : ";
    for (int i = 0; i < count; ++i)
    {
        std::cout << results[i] << " ";
    }
    std::cout << std::endl;
}

// Définir une fonction qui cherche dans chaque intervalle de largeur width (de la forme
// [begin + n * width, begin + (n + 1) * width] où n est un naturel quelconque), sans
// dépasser end comme borne haute, un extrema de func, un extrema étant défini comme un
// nombre pour lequel la valeur estimée (calculée en utilisant epsilon) de la dérivée de
// func en ce nombre est inférieur en valeur absolue à precision ; au maximum,
// max_number_of_results seront retournés dans le tableau results ; la fonction a pour
// résultat le nombre exact de valeurs présentes dans results.
int find_all_extrema( std::function< double( double ) > func, double begin, double end, double width,
                      double precision, double epsilon, double results[], double max_number_of_results )
{
    int count = 0;
    for (double x = begin; x < end && count < max_number_of_results; x += width)
    {
        double derivative = compute_derivative(func, x, epsilon);
        if (std::abs(derivative) < precision)
        {
            results[count++] = x;
        }
    }
    return count;
}

// Définir une fonction test_42() qui cherche les extrema de la fonction sin_x_plus_cos_sqrt2_times_x()
// dans l'intervalle [-10, 10], avec une largeur de 0.5, une précision de 10-5, une dérivée
// estimée en utilisant 10-5 pour epsilon et un maximum de 10 extrema retournés. 
void test_42()
{
    std::cout << "test_42:\n";
    
    double begin = -10.0;
    double end = 10.0;
    double width = 0.5;
    double precision = 1e-5;
    double epsilon = 1e-5;
    double results[10]; 
    int count = find_all_extrema(sin_x_plus_cos_sqrt2_times_x, begin, end, width, precision, epsilon, results, 10);

    if (count > 0) {
        std::cout << "Extrema trouvés (" << count << "): ";
        for (int i = 0; i < count; i++) {
            std::cout << results[i] << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "Aucun extrême trouvé dans l'intervalle spécifié.\n"; 
    }
}

int main()
{
    test_11();
    test_12();
    test_21();
    test_22();
    test_23();
    test_31();
    test_32();
    test_41();
    test_42();
}

