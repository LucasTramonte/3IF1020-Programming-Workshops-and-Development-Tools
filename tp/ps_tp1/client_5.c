/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * client_5.c
 * cc -o client_5 client_5.c
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
    if (sig == SIGINT| sig == SIGTERM) {
        printf("Signal reçu: %d. Arrêt du client...\n", sig);
        running = false;
    }
}

// Fonction pour afficher le message de fin
void message_de_fin(void) {
    printf("Message de fin : Le client s'est terminé correctement.\n");
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

    // Ouverture de la FIFO en lecture seule
    int fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Erreur lors de l'ouverture de la FIFO");
        exit(EXIT_FAILURE);
    }

    // Boucle principale pour lire les nombres du serveur
    while (running) {
        int nombre_recu;
        int bytes_read = read(fifo_fd, &nombre_recu, sizeof(int));

        if (bytes_read == 0) {
            // La FIFO a été fermée par le serveur
            printf("[Client] Le serveur a fermé la FIFO. Arrêt du client...\n");
            running = false;
        } else if (bytes_read > 0) {
            printf("[Client] Nombre reçu : %d\n", nombre_recu);
        } else {
            perror("Erreur lors de la lecture dans la FIFO");
            running = false;
        }
    }

    // Fermer la FIFO avant de quitter
    close(fifo_fd);
    return EXIT_SUCCESS;
}