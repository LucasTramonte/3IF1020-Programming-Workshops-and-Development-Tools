/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmations système - TP n°2
 *
 * Threads.cpp
 * c++ -std=c++20 Threads.cpp -o Threads -lpthread
 */

#include <iostream>
#include <thread>
 
 
#include "../Random.hpp"

#include "osyncstream.hpp"
#include "Producer.hpp"
#include "Consumer.hpp"
#include "MessageBox.hpp"
 
/*
 * Test avec 1 producteur et 1 consommateur
 */
void one_producer_one_consumer() {
    // TODO :
    // - Créer un générateur de nombres aléatoires
    // - Créer une boîte à lettres
    // - Créer un producteur et un consommateur
    // - Créer les threads correspondants
    // - Attendre la fin des threads

    MessageBox box;
    Random random_engine(50);
    
    Producer producer(1, box, random_engine, 20);
    Consumer consumer(2, box, random_engine, 20);
    
    std::thread producer_thread(producer);
    std::thread consumer_thread(consumer);
    
    producer_thread.join();
    consumer_thread.join();
 
}

/*
 * Test avec plusieurs producteurs et consommateurs
 */
void several_producers_and_consumers() {
    // TODO :
    // - Créer un générateur de nombres aléatoires

    Random randomizer(200);
    std::vector<std::thread> activeThreads;

    // Générer un nombre entre 10 et 20
    int numConsumers = rand() % 11 + 10;  

    // - Créer une boîte à lettres

    MessageBox message_box;

    // - Créer entre 10 et 20 consommateurs et 2 fois plus de producteurs 

    for (int i = 0; i < numConsumers; ++i) {
        activeThreads.push_back(std::thread(Consumer(i, message_box, randomizer, 20)));
    }

    // - Créer les threads correspondants

    for (int i = 0; i < 2 * numConsumers; ++i) {
        activeThreads.push_back(std::thread(Producer(i, message_box, randomizer, 10)));
    }

    // - Attendre la fin des threads

    for (auto& th : activeThreads) {
        th.join();
    }

    // Affichage du bon fonctionnement
    std::cout << "Tous les producteurs et consommateurs ont terminé." << std::endl;
 
}

int main() {
    // one_producer_one_consumer();
    several_producers_and_consumers();
    return 0;
}

/*
.
.
.
Erreur: Consommateur 2 a retiré un message négatif!
Erreur: Consommateur 2 a retiré un message négatif!
*/

/*
Après modifier thread/MessageBox.hpp : 

Producteur 1 a déposé le message: 0
Consommateur 2 a retiré le message: 0
Producteur 1 a déposé le message: 1
Consommateur 2 a retiré le message: 1
Producteur 1 a déposé le message: 2
Producteur 1 a déposé le message: 3
Consommateur 2 a retiré le message: 2
Producteur 1 a déposé le message: 4
Consommateur 2 a retiré le message: 3
Consommateur 2 a retiré le message: 4
Producteur 1 a déposé le message: 5
Producteur 1 a déposé le message: 6
Consommateur 2 a retiré le message: 5
Producteur 1 a déposé le message: 7
Consommateur 2 a retiré le message: 6
Producteur 1 a déposé le message: 8
Consommateur 2 a retiré le message: 7
Producteur 1 a déposé le message: 9
Consommateur 2 a retiré le message: 8
Producteur 1 a déposé le message: 10
Consommateur 2 a retiré le message: 9
Producteur 1 a déposé le message: 11
Consommateur 2 a retiré le message: 10
Producteur 1 a déposé le message: 12
Consommateur 2 a retiré le message: 11
Producteur 1 a déposé le message: 13
Consommateur 2 a retiré le message: 12
Producteur 1 a déposé le message: 14
Consommateur 2 a retiré le message: 13
Consommateur 2 a retiré le message: 14
Producteur 1 a déposé le message: 15
Consommateur 2 a retiré le message: 15
Producteur 1 a déposé le message: 16
Consommateur 2 a retiré le message: 16
Producteur 1 a déposé le message: 17
Consommateur 2 a retiré le message: 17
Producteur 1 a déposé le message: 18
Consommateur 2 a retiré le message: 18
Producteur 1 a déposé le message: 19
Consommateur 2 a retiré le message: 19

*/
