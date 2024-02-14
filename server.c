#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>


// PROTOTYPES
void    print_pid(void);
void    print_msg(int signum);

int binaire = 0;

// CODE
int main()
{
    // affichage du pid du processus
    print_pid();
    // affichage qu'on attend un message du client
    printf("En attente de nouveaux messages\n");

    // en attente des signaux du client
    signal(SIGUSR1, print_msg);
    signal(SIGUSR2, print_msg);

    // boucle parce que sinon le serveur ne fait rien et nous on veut constamment vérifier si on recoit un signal 
    while (1)
    {
        sleep(1);
    }
    
    return 0;
}

// AFFICHER LE CODE
void    print_msg(int signum)
{
    // création variable static sinon entre chaque appel on perdrait le bit reçu avant et donc pas possible de former 8 bits pour charactère et donc pas possible de former un msg, même logique pour les deux autres
    static int bits = 0;
    static char msg[] = {0};
    static int taille = 0;

    // si on a un sigurs1 on décale à gauche et on ajoute 0 pour confirmer que le message a bien été décalé (sinon on ne pourrait pas dire qu'on a bien reçu un 0 et on pourrait croire que rien n'a été traité)
    // si on a un surgurs2 on ajoute 1 à la valeur 
    if (signum == SIGUSR1)
    {
        binaire = (binaire << 1) | 0;
    }   
    else if (signum == SIGUSR2)
    {
        binaire = (binaire << 1) | 1;
    }
    bits++;

    // si on a 8 bits on a un charactère alors on le fou dans message pour commencer ou continuer celui-ci
    // si on a le charactère nul alors on affiche le message complet puis on créer le fichier de log si il n'existe pas ou alors on le met dedans si il existe déjà  (grâce à "a" dans fopen)
    if (bits == 8)
    {
        //printf("Caractère reçu : %c\n", (char)binaire);
        msg[taille++] = (char)binaire;

        if (binaire == '\0')
        {
            printf("Message : %s\n", msg);

            // Écrire dans le fichier de log
            FILE *log_file = fopen("conversations.log", "a");
            if (log_file != NULL) {

                // Enregistrement de de l'heure et la date
                time_t now;
                time(&now);
                // Supprimer le saut de ligne à la fin de la chaîne de temps
                char *time_str = ctime(&now);
                time_str[strcspn(time_str, "\n")] = '\0';
                // ajout du message et de la date dans le fichier de log
                fprintf(log_file, "[%s] %s\n",time_str, msg);
                fclose(log_file);
            } 
            // S'il y a une erreur dans l'ouverture du fichier de log, un message d'erreur est fournit
            else {
                perror("Erreur lors de l'ouverture du fichier de log");
            }

            memset(msg,0,sizeof(msg));
            taille = 0;
        }
        bits = 0;
        binaire = 0;
    }
}


// Affichage du pid du serveur
void    print_pid(void)
{
    int pid;
    
    pid = getpid();
    printf("LE PID DU SERVEUR : %i\n", pid);
}