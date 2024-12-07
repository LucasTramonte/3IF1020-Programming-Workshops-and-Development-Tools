/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°2
 *
 * Number.hpp
 */

#ifndef NUMBER_HPP_INCLUDED
#define NUMBER_HPP_INCLUDED

#include <iostream>
#include <string>
#include <utility>

class Number {
public:
    Number( unsigned long l );
    Number(const Number& other);  // Constructeur de copie
    Number& operator=(Number other);    // Affectation par copie avec idiome du swap
    ~Number(); 

    // Swap (échanger les valeurs de deux objets Number)
    friend void swap(Number& first, Number& second) {
        std::swap(first.first_, second.first_);
    }
    
    void print( std::ostream & out ) const; 
    void add( unsigned int i );
    void multiply( unsigned int i );

private:
    using DigitType = unsigned int;
    static const DigitType number_base{ 10u }; // Base 10 pour la représentation des chiffres

    // Classe interne représentant un chiffre
    class Digit {
    public:
        using Digit_type = DigitType;

        Digit( DigitType digit, Digit * next ); 
        Digit( unsigned long l ); 
        ~Digit();
        Digit( const Digit & other ); 

        // Swap (échanger les valeurs de deux objets)
        friend void swap(Digit& first, Digit& second) {
            std::swap(first.digit_, second.digit_);
            std::swap(first.next_, second.next_);
        };
        void ajouterChiffre( unsigned int i );
        void multiplierChiffres( unsigned int i );
        void print( std::ostream & out ) const;

    private:
        DigitType digit_;
        Digit * next_;
    };

    Digit * first_;

};

inline std::ostream& operator<<(std::ostream& out, const Number& n) {
    n.print(out);
    return out;
}

Number factorial( unsigned int n );


#endif 
