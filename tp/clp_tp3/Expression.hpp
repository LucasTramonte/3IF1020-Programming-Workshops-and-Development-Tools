/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°3
 *
 * Expression.hpp
 */

#ifndef EXPRESSION_HPP_INCLUDED
#define EXPRESSION_HPP_INCLUDED

#include <iostream>
#include <string>
#include <utility>
#include <memory> // Pour l'utilisation de std::unique_ptr
#include <cassert>

class Expression {
public:
    Expression() {++instances_creées_;}
    virtual ~Expression() {++instances_détruites_;}// Destructeur
    virtual void afficher(std::ostream &os) const = 0; 
    // Renvoie un pointeur unique vers une nouvelle instance représentant la dérivée.
    virtual std::unique_ptr<Expression> derivée(const std::string &variable) const = 0;
    virtual std::unique_ptr<Expression> clone() const = 0; // Clonage

    // Surcharge de l'opérateur de flux pour l'affichage
    friend std::ostream &operator<<(std::ostream &os, const Expression &expr) {
        expr.afficher(os);
        return os;
    }

    // Accesseurs pour les statistiques
    static int nombreInstancesCreées();
    static int nombreInstancesDétruites();

    private:
    static int instances_creées_;   // Nombre d'instances créées
    static int instances_détruites_; // Nombre d'instances détruites

};

class Nombre : public Expression {
public:
    explicit Nombre(double valeur) : valeur_{valeur} {}
    void afficher(std::ostream &os) const override {
        os << valeur_;
    }
    std::unique_ptr<Expression> derivée(const std::string &variable) const override {
        return std::make_unique<Nombre>(0.0);
    }
    std::unique_ptr<Expression> clone() const override {
        return std::make_unique<Nombre>(*this);
    }
private:
    double valeur_;
};

class Variable : public Expression {
public:
    explicit Variable(std::string nom) : nom_{std::move(nom)} {}
    void afficher(std::ostream &os) const override {
        os << nom_;
    }
    std::unique_ptr<Expression> derivée(const std::string &variable) const override {
        return std::make_unique<Nombre>(nom_ == variable ? 1.0 : 0.0);
    }
    std::unique_ptr<Expression> clone() const override {
        return std::make_unique<Variable>(*this);
    }
private:
    std::string nom_;
};

class Operation : public Expression {
public:
    Operation(std::unique_ptr<Expression> gauche, std::unique_ptr<Expression> droite)
        : gauche_{std::move(gauche)}, droite_{std::move(droite)} {
        assert(gauche_ != nullptr && "L'opérande gauche ne doit pas être nul");
        assert(droite_ != nullptr && "L'opérande droite ne doit pas être nul");
    }

    virtual ~Operation() {}

protected:
    virtual void afficherSymbole(std::ostream &os) const = 0;

    void afficher(std::ostream &os) const override {
        os << "(";
        gauche_->afficher(os);
        afficherSymbole(os);
        droite_->afficher(os);
        os << ")";
    }

    std::unique_ptr<Expression> gauche_;
    std::unique_ptr<Expression> droite_;
};

class Addition : public Operation {
public:
    Addition(std::unique_ptr<Expression> gauche, std::unique_ptr<Expression> droite)
        : Operation(std::move(gauche), std::move(droite)) {}

    void afficherSymbole(std::ostream &os) const override {
        os << " + ";
    }

    std::unique_ptr<Expression> derivée(const std::string &variable) const override {
        return std::make_unique<Addition>(gauche_->derivée(variable), droite_->derivée(variable));
    }

    std::unique_ptr<Expression> clone() const override {
        return std::make_unique<Addition>(gauche_->clone(), droite_->clone());
    }
};

class Multiplication : public Operation {
public:
    Multiplication(std::unique_ptr<Expression> gauche, std::unique_ptr<Expression> droite)
        : Operation(std::move(gauche), std::move(droite)) {}

    void afficherSymbole(std::ostream &os) const override {
        os << " * ";
    }

    std::unique_ptr<Expression> derivée(const std::string &variable) const override {
        // Règle de la dérivée du produit : (f * g)' = f' * g + f * g'
        auto dérivée_gauche = gauche_->derivée(variable);
        auto dérivée_droite = droite_->derivée(variable);
        return std::make_unique<Addition>(
            std::make_unique<Multiplication>(std::move(dérivée_gauche), droite_->clone()),
            std::make_unique<Multiplication>(gauche_->clone(), std::move(dérivée_droite))
        );
    }

    std::unique_ptr<Expression> clone() const override {
        return std::make_unique<Multiplication>(gauche_->clone(), droite_->clone());
    }
};


#endif 

/*

Définir (diagramme de classes UML) l'arbre d'héritage permettant de manipuler de manière générale de telles expressions.

                            +------------------+
                            |   Expression     |
                            +------------------+
                            |  + afficher()    |
                            |  + evaluer()     |
                            +------------------+
                                     ^
                                     |
                   +-----------------+------------------+
                   |                                    |
           +--------------+                     +------------------+
           |    Nombre    |                     |     Variable     |
           +--------------+                     +------------------+
           | - valeur_    |                     | - nom_           |
           | + valeur()   |                     | + nom()          |
           +--------------+                     +------------------+
                                     ^
                                     |
                             +------------------+
                             |    Operation     |
                             +------------------+
                             | - operandes_     |
                             | + evaluer()      |
                             | + afficher()     |
                             +------------------+
                                     ^
                     +---------------+---------------+
                     |                               |
             +------------------+            +---------------------+
             |     Addition      |            |   Multiplication    |
             +------------------+            +---------------------+
             | + evaluer()      |            | + evaluer()         |
             | + afficher()     |            | + afficher()        |
             +------------------+            +---------------------+

- Expression : classe abstraite qui définit une interface commune avec des méthodes comme <afficher> et <evaluer>.
- Nombre : constante numérique. 
- Variable : inconnue avec un nom.
- Operation : classe abstraite pour les opérations arithmétiques. Elle regroupe les opérandes et définit l'affichage & l'évaluation.
- Les sous-classes Addition et Multiplication héritent d'Operation et implémentent leurs propres logiques spécifiques.

Quel est l'intérêt d'avoir une super-classe Operation ?

 Une super-classe Operation permet de regrouper le comportement commun à toutes les opérations arithmétiques. 
 Par exemple, chaque opération doit pouvoir afficher son contenu sous forme lisible ou être dérivée (calcul symbolique). 
 Cela respecte le principe de substitution de Liskov (le L de SOLID), où toute instance d’une sous-classe d'Operation doit 
 pouvoir être manipulée comme une instance d'Operation sans affecter la logique.

Nous souhaitons ajouter un comportement de dérivation d'une expression par rapport à une variable 
(la méthode recevra le nom de la variable en argument) : ce comportement peut soit modifier l'instance courante, 
soit créer une nouvelle instance représentant la dérivée de l'objet courant. Quelle est la bonne approche à retenir ici ?

Spécifier aussi précisément que possible, sous forme de commentaire dans le fichier d'entête, le comportement de dérivation d'une expression
 (argument, type de retour, effet sur l'objet récepteur, type d'allocation si nécessaire).


- La méthode derivée prend en argument le nom de la variable par rapport à laquelle l'expression doit être dérivée.
- Elle renvoie un std::unique_ptr<Expression> pour gérer la mémoire dynamiquement.
- Chaque classe implémente sa propre logique de dérivation.
Par exemple :
    - Pour Nombre, la dérivée est toujours 0.
    - Pour Variable, la dérivée est 1 si elle correspond à la variable reçue, sinon 0.

1.2. Itération 2 (1 point)

Comment représenter en C++ les liens entre un opérateur et ses opérandes ?
   
   Les opérandes sont représentés par des pointeurs (Expression *). 

Signature du constructeur :

   Le constructeur reçoit deux pointeurs :Operation(Expression *gauche, Expression *droite). 
   Ces pointeurs doivent pointer vers des objets alloués dynamiquement pour être gérés correctement.

Appropriation ou copie des arguments ?

L'opération doit s'approprier les opérandes reçus, c'est-à-dire prendre possession d'eux et être responsable de leur désallocation.

Mode d'allocation mémoire des opérandes :
   Les opérandes doivent être alloués dynamiquement. 
   Cela permet de gérer le polymorphisme et d'assurer que des expressions complexes peuvent être construites.

Solution pour le constructeur de copie :
   Pour l'instant, un constructeur de copie explicite n'est pas défini. Si nécessaire, chaque opérande devrait être cloné, 
   par exemple en ajoutant une méthode virtuelle clone() à la classe Expression.

Méthode justifiant l'existence de la classe Operation :
   La méthode protégée afficherSymbole permet aux sous-classes (par exemple `Addition` et `Multiplication`) d’ajouter leur propre symbole tout en bénéficiant du comportement commun défini dans `Operation`.

Choix pour les sous-classes :
   Les sous-classes doivent uniquement définir deux éléments : leur symbole d'affichage (`+`, `*`, etc.) et leur comportement de dérivation.
*/

