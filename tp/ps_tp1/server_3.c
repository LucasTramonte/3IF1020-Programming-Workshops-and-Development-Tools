/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * server_3.c
 * cc -o server_3 server_3.c
 */

// Commencer par recopier ici le code de server_2.c

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
// Pour waitpid()
#include <sys/wait.h>  

// Déclaration de la variable globale 'running'
volatile bool running = true;

// Variable globale pour stocker le PID du fils
pid_t child_pid = 0;

//Fonction qui gère le signal SIGINT
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

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Erreur lors de l'installation des signaux");
        exit(EXIT_FAILURE);
    }

    // Message initial
    printf("Démarrage du programme server_3...\n");

    // Fork pour dupliquer le processus
    child_pid = fork();

    if (child_pid == -1) {
        perror("Erreur lors du fork");
        exit(EXIT_FAILURE);
    }
    /*
    //Boucle principale
    while (running) {
        if (child_pid == 0) {
            // Processus fils
            printf("[Fils] PID: %d, PPID: %d, GID: %d\n", getpid(), getppid(), getpgid(getpid()));
        } else {
            // Processus père
            printf("[Père] PID: %d, PPID: %d, GID: %d\n", getpid(), getppid(), getpgid(getpid()));
        }
        printf("Nombre aléatoire: %d\n", rand() % 100);
        sleep(1);
    }
    */

    if (child_pid == 0) {
        // Code du processus fils
        while (running) {
            printf("[Fils] PID: %d, PPID: %d, GID: %d\n", getpid(), getppid(), getpgid(0));
            printf("Nombre aléatoire: %d\n", rand() % 100);
            sleep(1);

            // Modifier votre programme pour que le père, quand il est arrêté par un signal, arrête le fils en lui envoyant un signal.
            // Vérification si le père est toujours en vie
            if (getppid() == 1) {  // Si le père est devenu 'init', il est mort
                printf("Le père est mort. Le fils va s'arrêter...\n");
                running = false;
            }
        }
        printf("Le fils s'arrête...\n");
        exit(EXIT_SUCCESS);
    } else {
        // Code du processus père
        while (running) {
            printf("[Père] PID: %d, PPID: %d, GID: %d\n", getpid(), getppid(), getpgid(0));
            printf("Nombre aléatoire: %d\n", rand() % 100);
            sleep(1);
            
            // Modifier votre code pour que le père s'arrète quand il détecte (via un signal) que le fils s'est arrêté.
            // Vérification si le processus fils est terminé
            int status;
            pid_t result = waitpid(child_pid, &status, WNOHANG);
            if (result == child_pid) {
                printf("Le fils a terminé. Le père va s'arrêter.\n");
                running = false;
            }
        }
    }

    return EXIT_SUCCESS;
}

/*

Que constatez-vous pour les numéros de groupe ?
- Les deux processus, le père et le fils, partagent le même numéro de groupe de processus (GID: 25107), ce qui est attendu car le processus fils hérite du groupe de son père.

Est-ce que les 2 processus s'arrêtent avec CTRL-C ?
- Oui

Pouvez-vous expliquer pourquoi ?
- Les deux processus partagent le même numéro de groupe de processus (GID), donc lorsqu'un signal comme SIGINT est envoyé, il est transmis à tout le groupe. C'est pourquoi le père et le fils reçoivent tous les deux le signal et s'arrêtent simultanément.


~/workspace/3if1020/tp/ps_tp1: ps aj
   PPID     PID    PGID     SID TTY        TPGID STAT   UID   TIME COMMAND
    672     769     769     769 pts/1      26747 Ss     911   0:03 /bin/zsh -i
    672   15592   15592   15592 pts/2      26607 Ss     911   0:01 /bin/zsh -i
  15592   26607   26607   15592 pts/2      26607 S+     911   0:00 ./server_3
  26607   26608   26607   15592 pts/2      26607 S+     911   0:00 ./server_3
    769   26747   26747     769 pts/1      26747 R+     911   0:00 ps aj
~/workspace/3if1020/tp/ps_tp1: kill 26608   
~/workspace/3if1020/tp/ps_tp1: ps aj     
   PPID     PID    PGID     SID TTY        TPGID STAT   UID   TIME COMMAND
    672     769     769     769 pts/1      27186 Ss     911   0:04 /bin/zsh -i
    672   15592   15592   15592 pts/2      26607 Ss     911   0:01 /bin/zsh -i
  15592   26607   26607   15592 pts/2      26607 S+     911   0:00 ./server_3
  26607   26608   26607   15592 pts/2      26607 Z+     911   0:00 [server_3] <defunct>
    769   27186   27186     769 pts/1      27186 R+     911   0:00 ps aj

~/workspace/3if1020/tp/ps_tp1: kill 26607
~/workspace/3if1020/tp/ps_tp1: ps aj     
   PPID     PID    PGID     SID TTY        TPGID STAT   UID   TIME COMMAND
    672     769     769     769 pts/1      27747 Ss     911   0:04 /bin/zsh -i
    672   15592   15592   15592 pts/2      15592 Ss+    911   0:01 /bin/zsh -i
    769   27747   27747     769 pts/1      27747 R+     911   0:00 ps aj

Utiliser kill pour arrêter le processus fils puis ps aj de nouveau : que remarquez-vous ?
    - Après avoir tué le processus fils (PID: 26608), il devient un processus « zombie » (<defunct>). Cela signifie que le processus fils est terminé, mais son père ne l'a pas encore "récolté". Il est donc toujours visible dans la liste des processus sous forme de zombie.

Maintenant, tuer le père, est-ce que le fils est toujours visible ?
    - Après avoir tué le processus père (PID: 26607), le fils zombie n'est plus visible. Le processus fils disparu car son père a été éliminé, et le système a nettoyé les processus zombies.

Modifier votre code pour que le père s'arrète quand il détecte (via un signal) que le fils s'est arrêté.
Relancer votre programme et commencer cette fois par tuer le père : qu'observez-vous ?
            .
            .
            .
[Père] PID: 29259, PPID: 15592, GID: 29259
Nombre aléatoire: 40
[Fils] PID: 29260, PPID: 29259, GID: 29259
Nombre aléatoire: 40
Signal reçu: 15
Arrêt du programme...
Arrêt du programme père...
Fin du programme server_3.
Message de fin: Le programme s'est terminé correctement.
~/workspace/3if1020/tp/ps_tp1: [Fils] PID: 29260, PPID: 1, GID: 29259
Nombre aléatoire: 26
[Fils] PID: 29260, PPID: 1, GID: 29259
Nombre aléatoire: 72
            .
            .
            .

Lorsque on tue le père, on observe que le fils continue à s'exécuter. En effet, après la mort du processus père, le fils est adopté par le processus init (PID 1), ce qui lui permet de continuer à fonctionner normalement.
  
En revanche, si on tue le fils, il s'arrêtera, et le message d'arrêt sera affiché. Dans cet exemple, le père ne détecte pas immédiatement la mort du fils sans la modification du code pour surveiller l'arrêt du processus fils via un signal.

*/