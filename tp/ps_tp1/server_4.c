/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * server_4.c
 * cc -o server_4 server_4.c
 */

// Commencer par recopier ici le code de server_3.c

// Pour printf()
#include <stdio.h>
// Pour rand(), srand(), sleep(), EXIT_SUCCESS
#include <stdlib.h>
// Pour sigaction(), perror(), exit()
#include <signal.h>
// Pour getpid(), getppid(), getpgrp()
#include <unistd.h>
// Pour le type booléen et la gestion de volatile
#include <stdbool.h>
// Pour la gestion des erreurs
#include <errno.h>
// Pour waitpid()
#include <sys/wait.h>

volatile bool running = true;
pid_t child_pid = 0;
int pipe_fd[2]; // Descripteurs du tube : pipe_fd[0] pour la lecture, pipe_fd[1] pour l'écriture

// Fonction qui gère le signal SIGINT
void signal_handler(int sig) {
    if (sig == SIGINT) {
        printf("Signal reçu: %d\n", sig);
        printf("Arrêt du programme...\n");
        running = false;
    }
}

// Fonction qui affiche un message à la fin du programme
void message_de_fin(void) {
    printf("Message de fin: Le programme s'est terminé correctement.\n");
}

int main() {
    // Enregistrer la fonction de fin avec atexit()
    if (atexit(message_de_fin) != 0) {
        perror("Erreur lors de l'installation de la fonction de fin");
        exit(EXIT_FAILURE);
    }

    // Configurer le gestionnaire de signal pour SIGINT
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Erreur lors de l'installation des signaux");
        exit(EXIT_FAILURE);
    }

    // Création du tube
    if (pipe(pipe_fd) == -1) {
        perror("Erreur lors de la création du tube");
        exit(EXIT_FAILURE);
    }

    // Fork pour créer le processus fils
    child_pid = fork();

    if (child_pid == -1) {
        perror("Erreur lors du fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Code du processus fils
        close(pipe_fd[1]); // Fermer l'extrémité d'écriture

        int nombre_reçu;
        while (running) {
            int bytes_read = read(pipe_fd[0], &nombre_reçu, sizeof(int));
            if (bytes_read == 0) {
                // Le père a fermé le tube
                printf("[Fils] Le père a fermé le tube. Le fils va s'arrêter...\n");
                running = false;
            } else if (bytes_read > 0) {
                printf("[Fils] Nombre reçu: %d\n", nombre_reçu);
            } else {
                perror("[Fils] Erreur lors de la lecture du tube");
                running = false;
            }
        }

        close(pipe_fd[0]); // Fermer l'extrémité de lecture avant de quitter
        printf("[Fils] Le fils s'arrête...\n");
        exit(EXIT_SUCCESS);
    } else {
        // Code du processus père
        close(pipe_fd[0]); // Fermer l'extrémité de lecture

        while (running) {
            int nombre_aleatoire = rand() % 100;
            printf("[Père] Envoi du nombre: %d\n", nombre_aleatoire);

            if (write(pipe_fd[1], &nombre_aleatoire, sizeof(int)) == -1) {
                perror("[Père] Erreur lors de l'écriture dans le tube");
                running = false;
            }

            sleep(1);

            // Vérifier si le fils s'est terminé
            int status;
            pid_t result = waitpid(child_pid, &status, WNOHANG);
            if (result == child_pid) {
                printf("[Père] Le fils s'est terminé. Le père va s'arrêter.\n");
                running = false;
            }
        }

        close(pipe_fd[1]); // Fermer l'extrémité d'écriture

        // Attendre que le processus fils se termine s'il est encore en cours d'exécution
        waitpid(child_pid, NULL, 0);
        printf("[Père] Le père s'arrête...\n");
    }

    return EXIT_SUCCESS;
}

/*

// Comparer les messages affichés quand on arrête le père en premier, et ceux quand on arrête le fils en premier.

Si le père termine en premier, le message suivant est écrit : Le père a terminé. Le fils va s'arrêter...
Le fils s'arrête. Mais si le fils s'arrête en premier, aucun message n'est écrit

                .
                .
                .
[Père] Envoi du nombre: 90
[Fils] Nombre reçu: 90
[Fils] Le père a fermé le tube. Le fils va s'arrêter...
[Fils] Le fils s'arrête...
Message de fin: Le programme s'est terminé correctement.
[1]    2134 terminated  ./server_4

//Faire en sorte que le père affiche aussi son message de fin quand on arrête le fils en premier.

                .
                .
                .
[Père] Envoi du nombre: 59
[Fils] Nombre reçu: 59
[Père] Le fils s'est terminé. Le père va s'arrêter.
[Père] Le père s'arrête...
Message de fin: Le programme s'est terminé correctement.
*/
