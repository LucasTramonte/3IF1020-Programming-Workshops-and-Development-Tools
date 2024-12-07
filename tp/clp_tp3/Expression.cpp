/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°3
 *
 * Expression.cpp
 */

#include <iostream>
#include <string>
#include <utility>

#include "Expression.hpp"

int Expression::instances_creées_ = 0;
int Expression::instances_détruites_ = 0;

int Expression::nombreInstancesCreées() {
    return instances_creées_;
}

int Expression::nombreInstancesDétruites() {
    return instances_détruites_;
}


