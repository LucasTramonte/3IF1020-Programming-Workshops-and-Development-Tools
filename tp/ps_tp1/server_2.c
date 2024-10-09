/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * server_2.c
 * cc -o server_2 server_2.c
 */

// Commencer par recopier ici le code de server_1.c


// For printf()
#include <stdio.h>
// For rand(), srand(), sleep(), EXIT_SUCCESS
#include <stdlib.h>
// For sigaction(), perror(), exit()
#include <signal.h>
// For getpid(), getppid(), getpgrp()
#include <unistd.h>
// Pour le type booléen et la gestion de la variable volatile
#include <stdbool.h>
// Pour la gestion des erreurs
#include <errno.h>

// Déclaration de la variable globale 'running'
volatile bool running = true;

// Fonction qui gère le signal SIGINT
void signal_handler(int sig) {
    printf("Signal reçu: %d\n", sig);
    printf("Arrêt du programme...\n");
    running = false;
}

// Fonction qui affiche un message à la fin du programme
void message_de_fin(void) {
    printf("Message de fin: Le programme s'est terminé correctement.\n");
}

int main() {
    // Installer la fonction message_de_fin avec atexit()
    if (atexit(message_de_fin) != 0) {
        perror("Erreur lors de l'installation de la fonction de fin");
        exit(EXIT_FAILURE);
    }

    // Installer le gestionnaire de signal pour SIGINT
    struct sigaction sa;
    sa.sa_handler = signal_handler; // Définit la fonction qui gère le signal
    sigemptyset(&sa.sa_mask); // Aucun signal supplémentaire n'est bloqué pendant l'exécution du gestionnaire
    sa.sa_flags = 0; // Aucune flag n'est utilisée

    // Installer le gestionnaire de signal pour SIGINT
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Erreur lors de l'installation du signal SIGINT");
        exit(EXIT_FAILURE);
    }

    // Installer le gestionnaire de signal pour SIGTERM
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Erreur lors de l'installation du signal SIGTERM");
        exit(EXIT_FAILURE);
    }

    // Message initial
    printf("Démarrage du programme server_2...\n");

    // Boucle tant que 'running' est vrai
    while (running) {
        // Afficher l'ID du processus, de son père et de son groupe
        printf("PID: %d, PPID: %d, GID: %d\n", getpid(), getppid(), getpgrp());
        
        // Générer et afficher un nombre aléatoire entre 0 et 99
        int random_number = rand() % 100;
        printf("Nombre aléatoire: %d\n", random_number);
        
        // Mettre en pause pendant 1 seconde
        sleep(1);
    }

    // Message de fin
    printf("Fin du programme server_2.\n");
    
    return EXIT_SUCCESS;
}

/*
    Dans Term1, lorsque le programme est exécuté de nouveau, et que dans Term2 la commande "kill -s INT [PID]" est utilisée :
    - Le message "Signal reçu: 2" et "Arrêt du programme..." sont affichés, ce qui indique que le programme a bien reçu le signal SIGINT.

    En utilisant simplement "kill [PID]" sans l'option -s INT :
    - Le message n'est pas affiché, car le signal par défaut (TERM) ne passe pas par le gestionnaire de signal que nous avons défini.
    
    Modification du programme pour afficher le message même avec "kill [PID]" :
    - Il est nécessaire de gérer le signal TERM de la même manière que SIGINT pour que le message soit également affiché dans ce cas.

    Dans Term1, lorsque le programme est exécuté de nouveau, et que dans Term2 la commande "kill -s KILL [PID]" est utilisée :
    - Le message **n'est pas** affiché car le signal KILL (SIGKILL) ne peut pas être intercepté ou géré par le programme. 
    - Une fois reçu, le processus est immédiatement terminé par le système, sans chance d'exécuter le gestionnaire de signal.

        Est-ce que le message a été affiché ? 
        - Non, il n'y a pas d'affichage de message lorsque KILL est utilisé.

        Peut-on faire en sorte qu'il le soit ?
        - Non, c'est une caractéristique fondamentale de SIGKILL, qui ne peut pas être géré, ignoré ou capturé par un programme.

        Que se passe-t-il si vous donnez comme argument à la commande kill (sans, puis avec l'option -s KILL) le numéro du processus père ?
        - Le programme ne recevra pas de signal, car le signal KILL ne peut pas être traité.
        - En revanche, lorsque le numéro du processus père est donné avec l'option -s KILL, le processus père sera tué immédiatement, sans possibilité de gestion de signal. Cela fermera également le terminal, car le shell qui l'exécute est tué.

        Après avoir modifié la variable running:
            - Le programme ne s'arrête plus avec CTRL-C car la variable 'running' n'est pas modifiée dans le gestionnaire de signal, ce qui fait qu'il continue de s'exécuter même après avoir reçu le signal SIGINT. 
            - De plus, il ne s'arrête pas avec un kill standard (SIGTERM) car ce signal est ignoré. 
            - Cependant, si on utilise kill -9 (SIGKILL), le programme sera immédiatement terminé par le système, sans exécuter le gestionnaire de signal et sans afficher de message.

        La fonction de fin est toujours activée sauf en cas d'utilisation de kill -9.
*/

