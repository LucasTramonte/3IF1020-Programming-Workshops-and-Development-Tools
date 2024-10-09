/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * server_6.c
 * cc -o server_6 server_6.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>  // Pour les fonctions réseau
#include <stdbool.h>

#define PORT 8080
#define MAX_CONN 1

volatile bool running = true;

// Gestionnaire de signal pour SIGINT
void signal_handler(int sig) {
    if (sig == SIGINT) {
        printf("Signal reçu: %d. Arrêt du serveur...\n", sig);
        running = false;
    }
}

// Fonction pour afficher le message de fin
void message_de_fin(void) {
    printf("Message de fin : Le serveur s'est terminé correctement.\n");
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

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

    // Créer le socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Définir l'adresse et le port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);  // Conversion du port en représentation réseau

    // Associer le socket à l'adresse
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erreur lors du bind");
        exit(EXIT_FAILURE);
    }

    // Écouter les connexions
    if (listen(server_fd, MAX_CONN) < 0) {
        perror("Erreur lors de l'écoute");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexion sur le port %d...\n", PORT);

    // Accepter une connexion
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Erreur lors de l'acceptation de la connexion");
        exit(EXIT_FAILURE);
    }

    // Boucle principale pour envoyer des nombres
    while (running) {
        int nombre_aleatoire = rand() % 100;
        printf("[Serveur] Envoi du nombre : %d\n", nombre_aleatoire);

        // Envoyer le nombre au client
        nombre_aleatoire = htonl(nombre_aleatoire);  // Conversion en représentation réseau
        if (write(client_fd, &nombre_aleatoire, sizeof(int)) == -1) {
            perror("Erreur lors de l'envoi");
            running = false;
        }

        sleep(1);
    }

    // Fermer les sockets avant de quitter
    close(client_fd);
    close(server_fd);

    return EXIT_SUCCESS;
}