/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmations système - TP n°2
 *
 * MessageBox.hpp
 */

#pragma once

#include <mutex>
#include <condition_variable>

#include "../BasicMessageBox.hpp"

/*
 * FIFO d'echange de messages entre producteurs et consommateurs
 * Version pour synchronisation entre threads
 */
class MessageBox : public BasicMessageBox {
public:
    void put( int message ) {
        // TODO :
        // - Ajouter les instructions de synchronisation
        // - Ne pas faire d'affichage dans cette méthode
        std::unique_lock<std::mutex> lock(mutex_);
        
        // Attendre jusqu'à ce qu'il y ait de la place dans la boîte
        condition_.wait(lock, [this] { 
            return n < box_size_; 
        });

        // Dépôt du message
        basic_put(message);
        n++;

        // Notifier un consommateur potentiel que la boîte n'est plus vide
        lock.unlock();
        condition_.notify_one();
    }
 
    int get() {
        // TODO :
        // - Ajouter les instructions de synchronisation
        // - Ne pas faire d'affichage dans cette méthode
        std::unique_lock<std::mutex> lock(mutex_);
        // Attendre jusqu'à ce qu'il y ait un message à consommer
        condition_.wait(lock, [this] { 
            return n > 0; 
        });
        
        // Retirer le message
        int message{ basic_get() };
        n--;

        // Notifier un producteur potentiel que la boîte n'est plus pleine
        lock.unlock();
        condition_.notify_one();
        
        return message;
    }
private:
    // TODO : 
    // - Ajouter les objets de synchronisation
    int n  = 0;  // Compteur d'éléments dans la boîte

    std::mutex mutex_;
    std::condition_variable condition_;
};
 
