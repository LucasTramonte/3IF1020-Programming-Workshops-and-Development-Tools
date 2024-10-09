/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * client_6.c
 * cc -o client_6 client_6.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>  // Pour les fonctions réseau
#include <signal.h>
#include <stdbool.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

volatile bool running = true;

// Gestionnaire de signal pour SIGINT
void signal_handler(int sig) {
    if (sig == SIGINT) {
        printf("Signal reçu: %d. Arrêt du client...\n", sig);
        running = false;
    }
}

// Fonction pour afficher le message de fin
void message_de_fin(void) {
    printf("Message de fin : Le client s'est terminé correctement.\n");
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

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
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Définir l'adresse du serveur
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);  // Conversion du port en représentation réseau

    // Convertir l'adresse IP
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Erreur lors de la conversion de l'adresse IP");
        exit(EXIT_FAILURE);
    }

    // Se connecter au serveur
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erreur lors de la connexion");
        exit(EXIT_FAILURE);
    }

    // Boucle principale pour lire les nombres du serveur
    while (running) {
        int nombre_recu;
        int bytes_read = read(sock, &nombre_recu, sizeof(int));

        if (bytes_read == 0) {
            // Le serveur a fermé la connexion
            printf("[Client] Le serveur a fermé la connexion. Arrêt du client...\n");
            running = false;
        } else if (bytes_read > 0) {
            nombre_recu = ntohl(nombre_recu);  // Conversion de la représentation réseau à la machine
            printf("[Client] Nombre reçu : %d\n", nombre_recu);
        } else {
            perror("Erreur lors de la lecture");
            running = false;
        }
    }

    // Fermer le socket avant de quitter
    close(sock);

    return EXIT_SUCCESS;
}