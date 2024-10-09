/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * server_1.c
 * cc -o server_1 server_1.c
 */

// for printf()
#include <stdio.h>
// For rand(), srand(), sleep(), EXIT_SUCCESS
#include <stdlib.h>
// For time()
#include <time.h>
// For getpid(), getppid()
#include <unistd.h>

int main()
{
    // Afficher un message initial
    printf("Démarrage du programme server_1...\n");

    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));  // Utiliser le temps actuel pour initialiser le générateur

    // Boucle infinie
    while (1) {
        // Afficher l'ID du processus, l'ID du processus père et l'ID du groupe
        printf("PID: %d, PPID: %d, GID: %d\n", getpid(), getppid(), getpgrp());

        // Afficher un nombre aléatoire entre 0 et 99
        int random_number = rand() % 100;  
        printf("Nombre aléatoire: %d\n", random_number);

        // Pause pendant une seconde
        sleep(1);
    }

    // Afficher un message après la boucle (ce message ne sera jamais atteint)
    printf("Fin du programme server_1.\n");

        
    return EXIT_SUCCESS;
}

/*
     Sortie du terminal lors de l'exécution de la commande `ps aj` :
     
     ~/workspace: cd 3if1020/tp/ps_tp1          
     ~/workspace/3if1020/tp/ps_tp1: ps aj
        PPID     PID    PGID     SID TTY        TPGID STAT   UID   TIME COMMAND
         672     683     683     683 pts/0       2620 Ss     911   0:01 /bin/zsh -i
         672     769     769     769 pts/1       3012 Ss     911   0:00 /bin/zsh -i
         683    2620    2620     683 pts/0       2620 S+     911   0:00 ./server_1
         769    3012    3012     769 pts/1       3012 R+     911   0:00 ps aj
     
     - Le processus `./server_1` a le PID 2620, et son PPID est 683 (le shell zsh).
     - Le PGID est le même que le PID, indiquant qu'il est le leader de son groupe de processus.
     
     Après avoir exécuté la commande plusieurs fois et en utilisant `kill 3805` :

     ~/workspace/3if1020/tp/ps_tp1: ps aj
        PPID     PID    PGID     SID TTY        TPGID STAT   UID   TIME COMMAND
            672     683     683     683 pts/0       3805 Ss     911   0:01 /bin/zsh -i
            672     769     769     769 pts/1       3969 Ss     911   0:00 /bin/zsh -i
            683    3805    3805     683 pts/0       3805 S+     911   0:00 ./server_1
            769    3969    3969     769 pts/1       3969 R+     911   0:00 ps aj
     
     ~/workspace/3if1020/tp/ps_tp1: kill 3805  
     
     - Le processus `./server_1` a été arrêté avec succès, ne figurant plus dans la liste des processus actifs:
    
            .
            .
            .
     Nombre aléatoire: 28
     PID: 3805, PPID: 683, GID: 3805
     Nombre aléatoire: 80
     PID: 3805, PPID: 683, GID: 3805
     Nombre aléatoire: 64
     [1]    3805 terminated  ./server_1
*/


