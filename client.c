#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


// PROTOTYPES
void    send_msg(int pid, char *msg);


// CODE
int main(int argc, char *argv[])
{
    // Condition pour vérifier que nous avons nos 2 paramètres au lancement du fichier
    if (argc == 3)
    {
        int pid = atoi(argv[1]); // Transformation d'une chainne de caractère en int (pid du serveur)
        char *msg = argv[2]; // récupération du message à envoyer
        send_msg(pid,msg); // appel de la fonction pour envoyer les messages
    }
    // Si il n'y a pas les 2 paramètres alors ... 
    else
    {
        printf("Veillez mettre en paramettre le pid du serveur et le message !!\n"); // Message d'erreur
        return 1; // Fin du programme
    }
}

void send_msg(int pid, char *msg)
{
    // Boucle while qui s'arrete que quand elle reçoit un charactère null
    while (*msg != '\0') {
        // Traitement de chaque caractères bits par bits
        for (int i = 7; i >= 0; i--) {
            // Bits 1
            int bit = (*msg >> i) & 1; // Détermine si le i-ème bit du caractère pointé par msg est 1 ou 0 avec un ET logique
            if (bit == 0) // si le binaire est 0
            {
                kill(pid, SIGUSR1); // Envoye du signal SIGUSR1 qui est égal au bit 0
            }
            else
            {
                kill(pid, SIGUSR2); // Envoye du signal SIGUSR2 qui est égal au bit 1
            }
            usleep(700); // temps de pause pour laisser le temps au serveur de recevoir le bit
        }
        msg++; // changement de caractère
    } 
    // Boucle qui permet d'envoyer un caractère null, necessaire au serveur pour comprendre que c'est la fin de la transmission.
    for (int i = 7; i >= 0; i--) {
    // Bits 1
    int bit = (*msg >> i) & 1;
    if (bit == 0)
    {
        kill(pid, SIGUSR1);
    }
    else
    {
        kill(pid, SIGUSR2);
    }
    usleep(1000);
}}