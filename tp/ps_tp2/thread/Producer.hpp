/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmations système - TP n°2
 *
 * Producer.hpp
 */

#pragma once

#include <iostream>
#include <thread>

#include "../ProdOrCons.hpp"
 
#include "osyncstream.hpp"
#include "MessageBox.hpp"

/*
 * Producteur de messages
 */
class Producer : public ProdOrCons< MessageBox > {
public:
    // Le constructeur de ProdOrCons peut être utilisé pour Producer
    using ProdOrCons::ProdOrCons;
 
    void operator()() override {
        // TODO : 
        // - Déposer dans box_ nb_messages_ entiers positifs avec attente
        //   aléatoire avant chaque dépôt.
        // - Afficher des messages entre chaque étape pour suivre l'avancement.

        for (unsigned i = 0; i < nb_messages_; ++i) {
            int message = i;  // Génération d'un message 
            box_.put(message);  // Dépôt du message dans la boîte
        
            osyncstream{ std::cout } << "Producteur " << name_ 
                                 << " a déposé le message: " << message << "\n";
        
            // Attente aléatoire avant de produire le prochain message
            using milliseconds = std::chrono::duration<int, std::milli>;
            std::this_thread::sleep_for(milliseconds{ random_engine_() });
       }
    }
};
 
