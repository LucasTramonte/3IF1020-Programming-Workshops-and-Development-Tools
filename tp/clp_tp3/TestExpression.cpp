/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°3
 *
 * TestExpression.cpp
 * c++ -std=c++20 -o TestExpression TestExpression.cpp Expression.cpp -lgtest -lpthread
 */

#include <sstream>
#include <utility>

#include <gtest/gtest.h>

#include "Expression.hpp"

void tester_affichage(const Expression &expr, const std::string &attendu) {
    std::ostringstream sortie;
    sortie << expr;
    EXPECT_EQ(sortie.str(), attendu) << "Affichage incorrect pour l'expression.";
}

TEST(ExpressionTest, AffichageNombre) {
    Nombre nombre{42.5};
    tester_affichage(nombre, "42.5");
}

TEST(ExpressionTest, AffichageVariable) {
    Variable variable{"x"};
    tester_affichage(variable, "x");
}

TEST(ExpressionTest, AffichageNombreScientifique) {
    Nombre nombre{123.456789};
    std::ostringstream sortie;
    sortie << std::scientific << nombre;
    EXPECT_EQ(sortie.str(), "1.234568e+02") << "Affichage scientifique incorrect.";
}

TEST(ExpressionTest, DeriveeNombre) {
    Nombre nombre{42.5};
    auto dérivée = nombre.derivée("x");
    tester_affichage(*dérivée, "0"); // La dérivée d'un nombre est toujours 0.
}

TEST(ExpressionTest, DeriveeVariable) {
    Variable variable{"x"};
    auto dérivée = variable.derivée("x");
    tester_affichage(*dérivée, "1"); // La dérivée de x par rapport à x est 1.

    auto dérivée_autre = variable.derivée("y");
    tester_affichage(*dérivée_autre, "0"); // La dérivée de x par rapport à y est 0.
}

TEST(ExpressionTest, AffichageAddition) {
    Variable a("x");
    Nombre b(3.0);
    
    auto addition = std::make_unique<Addition>(
        std::make_unique<Variable>("x"), std::make_unique<Nombre>(3.0)
    );
    tester_affichage(*addition, "(x + 3)"); 
}

TEST(ExpressionTest, DerivéeAddition) {
    Variable a("x");
    Nombre b(3.0);

    auto addition = std::make_unique<Addition>(
        std::make_unique<Variable>("x"), std::make_unique<Nombre>(3.0)
    );
    
    // La dérivée de (x + 3) par rapport à "x"
    auto dérivée = addition->derivée("x");
    tester_affichage(*dérivée, "(1 + 0)");  

    // La dérivée de (x + 3) par rapport à "y"
    auto dérivée_autre = addition->derivée("y");
    tester_affichage(*dérivée_autre, "(0 + 0)");  
}

TEST(ExpressionTest, AffichageMultiplication) {
    Variable a("x");
    Nombre b(3.0);

    auto multiplication = std::make_unique<Multiplication>(
        std::make_unique<Variable>("x"), std::make_unique<Nombre>(3.0)
    );
    tester_affichage(*multiplication, "(x * 3)"); 
}

TEST(ExpressionTest, DerivéeMultiplication) {
    Variable a("x");
    Nombre b(3.0);

    auto multiplication = std::make_unique<Multiplication>(
        std::make_unique<Variable>("x"), std::make_unique<Nombre>(3.0)
    );

    // La dérivée de (x * 3) par rapport à "x"
    auto dérivée = multiplication->derivée("x");
    tester_affichage(*dérivée, "((1 * 3) + (x * 0))");  

    // La dérivée de (x * 3) par rapport à "y"
    auto dérivée_autre = multiplication->derivée("y");
    tester_affichage(*dérivée_autre, "((0 * 3) + (x * 0))");  
}

int main( int argc, char * argv[] )
{
    ::testing::InitGoogleTest( &argc, argv );
    int result = RUN_ALL_TESTS();

    std::cout << "Nombre d'instances créées : " << Expression::nombreInstancesCreées() << "\n";
    std::cout << "Nombre d'instances détruites : " << Expression::nombreInstancesDétruites() << "\n";

    return result;

}

/*
Une erreur de compilation se produit car la classe de base Expression et les classes dérivées 
Nombre et Variable ne surchargent pas l'opérateur << de sortie pour prendre en charge 
l'affichage via des flux (std::ostream).

il faut surcharger l'opérateur << dans la classe Expression


[==========] Running 9 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 9 tests from ExpressionTest
[ RUN      ] ExpressionTest.AffichageNombre
[       OK ] ExpressionTest.AffichageNombre (0 ms)
[ RUN      ] ExpressionTest.AffichageVariable
[       OK ] ExpressionTest.AffichageVariable (0 ms)
[ RUN      ] ExpressionTest.AffichageNombreScientifique
[       OK ] ExpressionTest.AffichageNombreScientifique (0 ms)
[ RUN      ] ExpressionTest.DeriveeNombre
[       OK ] ExpressionTest.DeriveeNombre (0 ms)
[ RUN      ] ExpressionTest.DeriveeVariable
[       OK ] ExpressionTest.DeriveeVariable (0 ms)
[ RUN      ] ExpressionTest.AffichageAddition
[       OK ] ExpressionTest.AffichageAddition (0 ms)
[ RUN      ] ExpressionTest.DerivéeAddition
[       OK ] ExpressionTest.DerivéeAddition (0 ms)
[ RUN      ] ExpressionTest.AffichageMultiplication
[       OK ] ExpressionTest.AffichageMultiplication (0 ms)
[ RUN      ] ExpressionTest.DerivéeMultiplication
[       OK ] ExpressionTest.DerivéeMultiplication (0 ms)
[----------] 9 tests from ExpressionTest (4 ms total)

[----------] Global test environment tear-down
[==========] 9 tests from 1 test suite ran. (4 ms total)
[  PASSED  ] 9 tests.
Nombre d'instances créées : 24
Nombre d'instances détruites : 24

*/
