/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°2
 *
 * Number.cpp
 */

#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

#include "Number.hpp"

/*
Number::Number(unsigned long l) : first_(nullptr) {
    initFromLong(l);
}

Number::Number(const Number& other) : first_(nullptr) {
    copyFrom(other);
}

// Affectation par copie avec idiome du swap
Number& Number::operator=(Number other) {
    swap(*this, other);  // Utilisation de swap pour échanger les valeurs
    return *this;
}

Number::~Number() {
    delete first_;
}

void Number::initFromLong(unsigned long l) {
    if (l == 0) {
        first_ = new Digit(0);
        return;
    }

    Digit* current = nullptr;
    while (l > 0) {
        Digit* new_digit = new Digit(l % number_base);
        if (current) {
            new_digit->next_ = current;
        }
        current = new_digit;
        l /= number_base;
    }
    first_ = current;
}

void Number::copyFrom(const Number& other) {
    if (other.first_ == nullptr) {
        first_ = nullptr;
        return;
    }

    Digit* other_digit = other.first_;
    Digit* previous_digit = nullptr;
    first_ = nullptr;

    while (other_digit) {
        Digit* new_digit = new Digit(other_digit->digit_);
        if (previous_digit) {
            previous_digit->next_ = new_digit;
        } else {
            first_ = new_digit;
        }
        previous_digit = new_digit;
        other_digit = other_digit->next_;
    }
}
*/

Number::Number( unsigned long l )  : first_{ new Digit( l ) }{}

Number::~Number() {
    delete first_;
}

Number::Number(const Number& other) : first_{ new Digit( *other.first_ ) }{}

// Affectation par copie avec idiome du swap
Number& Number::operator=(Number other) {
    swap(*this, other);  // Utilisation de swap pour échanger les valeurs
    return *this;
}

void Number::print(std::ostream& out) const {
    if (first_) {
        first_->print(out);  // Appel de la méthode print de la première Digit
    } else {
        out << "0";  // Si le nombre est nul, on affiche "0"
    }
}

void Number::add(unsigned int value) {
    // Ajoute la valeur au premier chiffre et gère les éventuels débordements.
    if (first_ != nullptr) {
        first_->ajouterChiffre(value);
    }
}

void Number::multiply(unsigned int i) {
    // Si i est 0, on réinitialise le nombre à zéro.
    if (i == 0) {
        *this = Number(0); // Remise à zéro avec le constructeur Number(0).
    } else {
        if (first_ != nullptr) {
            first_->multiplierChiffres(i);
        }
    }
}

// Constructeur avec un chiffre et un pointeur vers le suivant
Number::Digit::Digit(DigitType digit, Digit* next): digit_{digit}, next_{next} {}

// Constructeur qui crée un chiffre à partir d'un nombre entier long
Number::Digit::Digit(unsigned long l)
    : digit_{  static_cast<Digit_type>(l % number_base) }, next_{ nullptr }{
    // Si la valeur dépasse la base, on crée le prochain chiffre
    if (l >= number_base) {
        next_ = new Digit(l / number_base);
    }
}

Number::Digit::~Digit() {
    delete next_;
}

Number::Digit::Digit( const Digit & other )
    : digit_{ other.digit_ }, next_{ other.next_ != nullptr ? new Digit(*other.next_) : nullptr }{}

void Number::Digit::print(std::ostream & out) const {
    if (next_ != nullptr) {
        next_->print(out);
    }
    out << digit_;
}

void Number::Digit::ajouterChiffre( unsigned int i ) {
    digit_ += i;
    if (digit_ >= number_base) {
        if (next_ == nullptr) {
            next_ = new Digit( static_cast<unsigned long>(digit_/number_base));
        } else {
            next_->ajouterChiffre(digit_/number_base);
        }
        digit_ = digit_ % number_base;
    }
}

void Number::Digit::multiplierChiffres( unsigned int i ) {

    digit_ *= i;

    if (digit_ >= number_base) {
        if (next_ == nullptr) {
            next_ = new Digit( static_cast<unsigned long>(digit_/number_base));
        } else {
            next_->multiplierChiffres(i);
            next_->ajouterChiffre(digit_/number_base);
        }
        digit_ = digit_ % number_base;
    } else {
        if (next_ != nullptr) {
            next_->multiplierChiffres(i);
        }
    }
}

Number factorial( unsigned int n ) {
    Number result{ 1 };
    for (unsigned int i = 2; i <= n; ++i) {
        result.multiply(i);
    }
    return result;
}

