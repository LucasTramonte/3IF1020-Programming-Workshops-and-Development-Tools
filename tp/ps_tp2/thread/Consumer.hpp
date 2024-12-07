/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmations système - TP n°2
 *
 * Consumer.hpp
 */

#pragma once

#include <iostream>
#include <thread>

#include "../ProdOrCons.hpp"
 
#include "osyncstream.hpp"
#include "MessageBox.hpp"


/*
 * Consommateur de messages
 */
class Consumer : public ProdOrCons< MessageBox > {
public:
    // Le constructeur de ProdOrCons sera utilisé comme constructeur de Consumer
    using ProdOrCons::ProdOrCons;
 
    void operator()() override {
        // TODO :
        // - Retirer de box_ nb_messages_ entiers avec attente aléatoire avant
        //   chaque retrait.
        // - Afficher des messages entre chaque étape pour suivre l'avancement.
        // - Afficher un message d'erreur si un nombre négatif est extrait.

        for (unsigned i = 0; i < nb_messages_; ++i) {
            int message = box_.get();  // Retrait du message de la boîte
        
            if (message < 0) {
                osyncstream{ std::cout } << "Erreur: Consommateur " << name_ 
                                     << " a retiré un message négatif!\n";
            } else {
                osyncstream{ std::cout } << "Consommateur " << name_ 
                                     << " a retiré le message: " << message << "\n";
            }
        
            // Attente aléatoire avant de consommer le prochain message
            using milliseconds = std::chrono::duration<int, std::milli>;
            std::this_thread::sleep_for(milliseconds{ random_engine_() });
        }
    }
};

