/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * server_5.c
 * cc -o server_5 server_5.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>   // Pour open()
#include <stdbool.h>

#define FIFO_PATH "/tmp/my_fifo"

volatile bool running = true;

// Gestionnaire de signal pour SIGINT
void signal_handler(int sig) {
    if (sig == SIGINT|sig == SIGTERM) {
        printf("Signal reçu: %d. Arrêt du serveur...\n", sig);
        running = false;
    }
}

// Fonction pour afficher le message de fin
void message_de_fin(void) {
    printf("Message de fin : Le serveur s'est terminé correctement.\n");
}

int main() {
    // Installer la fonction message_de_fin avec atexit()
    if (atexit(message_de_fin) != 0) {
        perror("Erreur lors de l'installation de la fonction de fin");
        exit(EXIT_FAILURE);
    }

    // Installer le gestionnaire de signal pour SIGINT
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Erreur lors de l'installation des signaux");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Erreur lors de l'installation du signal SIGTERM");
        exit(EXIT_FAILURE);
    }


    // Ouverture de la FIFO en écriture seule
    int fifo_fd = open(FIFO_PATH, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Erreur lors de l'ouverture de la FIFO");
        exit(EXIT_FAILURE);
    }

    // Boucle principale pour envoyer des nombres
    while (running) {
        int nombre_aleatoire = rand() % 100;
        printf("[Serveur] Envoi du nombre : %d\n", nombre_aleatoire);

        if (write(fifo_fd, &nombre_aleatoire, sizeof(int)) == -1) {
            perror("Erreur lors de l'écriture dans la FIFO");
            running = false;
        }

        sleep(1);
    }

    // Fermer la FIFO avant de quitter
    close(fifo_fd);

    return EXIT_SUCCESS;
}
