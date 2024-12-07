/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°2
 *
 * TestNumber.cpp
 * c++ -std=c++20 -o TestNumber Number.cpp TestNumber.cpp -lgtest -lpthread
 */

#include <exception>
#include <sstream>
#include <string>
#include <utility>

#include <cstdlib>
#include <ctime>


#include <gtest/gtest.h>

#include "Number.hpp"

 
TEST( TestNumber, TestNumber0 )
{
    Number n{ 0 };
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "0" );
}
 
TEST( TestNumber, TestNumber12345678 )
{
    Number n{ 12345678 };
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "12345678" );
}
 
TEST( TestNumber, TestNumberBig )
{
    Number n{ 12345123451234512345UL };
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "12345123451234512345" );
}

TEST( TestNumber, TestNumberRandom )
{
    auto r{ std::rand() };
    Number n{ static_cast< unsigned long >( r )};
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), std::to_string( r ));
}

// Test de construction par copie
TEST(TestNumber, CopyConstructor) {
    Number n1(12345);
    Number n2(n1);  // Constructeur de copie
    std::ostringstream os;
    os << n2;
    EXPECT_EQ(os.str(), "12345");  // Vérifie que n2 est une copie de n1
}

TEST(TestNumber, CopyAssignment) {
    Number n1(67890);
    Number n2 = n1;  // Opérateur d'affectation
    std::ostringstream os;
    os << n2;
    EXPECT_EQ(os.str(), "67890");  // Vérifie que n2 est identique à n1
}

TEST(TestNumber, TestAddSmallNumber) {
    Number n(5);
    n.add(7);
    std::ostringstream os;
    os << n;
    EXPECT_EQ(os.str(), "12");
}

TEST(TestNumber, TestMultiplySmallNumber) {
    Number n(5);
    n.multiply(3);
    std::ostringstream os;
    os << n;
    EXPECT_EQ(os.str(), "15");
}

TEST( TestNumber, TestFactorial123 )
{
    std::ostringstream os;
    os << factorial( 123 );;
    EXPECT_EQ( os.str(), "121463043670253296757662432418812958554542170884833823153289181618292"
                         "358923621676688311569606126402021707358352212940477825910915704116514"
                         "72186029519906261646730733907419814952960000000000000000000000000000" );
}

int main(int argc, char* argv[]) {
    std::srand(std::time(nullptr));
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*

[==========] Running 4 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 4 tests from TestNumber
[ RUN      ] TestNumber.TestNumber0
[       OK ] TestNumber.TestNumber0 (0 ms)
[ RUN      ] TestNumber.TestNumber12345678
[       OK ] TestNumber.TestNumber12345678 (0 ms)
[ RUN      ] TestNumber.TestNumberBig
[       OK ] TestNumber.TestNumberBig (0 ms)
[ RUN      ] TestNumber.TestNumberRandom
[       OK ] TestNumber.TestNumberRandom (0 ms)
[----------] 4 tests from TestNumber (2 ms total)

[----------] Global test environment tear-down
[==========] 4 tests from 1 test suite ran. (2 ms total)
[  PASSED  ] 4 tests.
 
*/

/*

[==========] Running 6 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 6 tests from TestNumber
[ RUN      ] TestNumber.TestNumber0
[       OK ] TestNumber.TestNumber0 (0 ms)
[ RUN      ] TestNumber.TestNumber12345678
[       OK ] TestNumber.TestNumber12345678 (0 ms)
[ RUN      ] TestNumber.TestNumberBig
[       OK ] TestNumber.TestNumberBig (0 ms)
[ RUN      ] TestNumber.TestNumberRandom
[       OK ] TestNumber.TestNumberRandom (0 ms)
[ RUN      ] TestNumber.CopyConstructor
[       OK ] TestNumber.CopyConstructor (0 ms)
[ RUN      ] TestNumber.CopyAssignment
[       OK ] TestNumber.CopyAssignment (0 ms)
[----------] 6 tests from TestNumber (0 ms total)

[----------] Global test environment tear-down
[==========] 6 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 6 tests.

Avez-vous vraiment besoin de l'affectation par copie d'un Digit ?

Non, l'affectation par copie d'un Digit n'est pas nécessaire, car chaque Digit fait partie d'une 
  liste chaînée où les éléments sont reliés par des pointeurs. 
Copier chaque Digit serait non seulement inefficace, mais aussi compliquerait la gestion de la mémoire.
Il est donc préférable de travailler avec des pointeurs ou des références, et l'utilisation de 
 l'idiome swap() permet de gérer les échanges de manière plus efficace sans avoir à dupliquer les éléments.
*/

/*
[==========] Running 9 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 9 tests from TestNumber
[ RUN      ] TestNumber.TestNumber0
[       OK ] TestNumber.TestNumber0 (0 ms)
[ RUN      ] TestNumber.TestNumber12345678
[       OK ] TestNumber.TestNumber12345678 (0 ms)
[ RUN      ] TestNumber.TestNumberBig
[       OK ] TestNumber.TestNumberBig (0 ms)
[ RUN      ] TestNumber.TestNumberRandom
[       OK ] TestNumber.TestNumberRandom (0 ms)
[ RUN      ] TestNumber.CopyConstructor
[       OK ] TestNumber.CopyConstructor (0 ms)
[ RUN      ] TestNumber.CopyAssignment
[       OK ] TestNumber.CopyAssignment (0 ms)
[ RUN      ] TestNumber.TestAddSmallNumber
[       OK ] TestNumber.TestAddSmallNumber (0 ms)
[ RUN      ] TestNumber.TestMultiplySmallNumber
[       OK ] TestNumber.TestMultiplySmallNumber (0 ms)
[ RUN      ] TestNumber.TestFactorial123
[       OK ] TestNumber.TestFactorial123 (0 ms)
[----------] 9 tests from TestNumber (0 ms total)

[----------] Global test environment tear-down
[==========] 9 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 9 tests.
*/