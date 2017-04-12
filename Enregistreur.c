#include "defs.h"

/*
Fichier tableau de score
Fichier parties en cours

Menu principal:
    Afficher scores
    Charger partie
    Nouvelle partie - IA ou Local
    Quitter

en jeu:
    Fonctions d'affichages
    Initialisation
    Fonction tour suivant
    Deplacer un pion -> Maj
    Selectionner un pion
    Prochain tour d'IA
    Fonction fin de partie et enregistrement du score
    *MaJ d'un fichier à chaque coup : positions + données de jeu

*/

void deplacement(int id, int toX, int toY){
    FILE *fic;
    fic = fopen("PartiesListe.dat","r+");
    game lPart;
    while(lPart.id != id){
        fread(&lPart, sizeof(lPart), 1, fic);
    }
    lPart.pions[id].x = toX;
    lPart.pions[id].y = toY;
    fseek(fic,-sizeof(lPart),SEEK_CUR);
    fwrite(&lPart, sizeof(lPart), 1, fic);
}
//***************************************************************************************************************
Game nouvellePartie(){
    FILE *fic;
    int pos = 0;
    fic = fopen("Sauvegardes/PartiesListe.dat","a");
    fgetpos(fic,&pos);
    if(fic == NULL){fic = fopen("Sauvegardes/PartiesListe.dat","w");pos = 0;}
    game nPart;
    printf("1 = Avec IA / 0 = 2 joueurs\n");
    scanf("%d",&nPart.ia);
    nPart.id = pos/sizeof(game);
    nPart.scoreB = 0 ;
    nPart.scoreW = 0 ;
    nPart.turn = 1 ; //(Tour impair = blanc)
    int i; int j;
    init_pawn(&nPart.pions);
    fwrite(&nPart, sizeof(nPart), 1, fic);
    fclose(fic);
}
//***************************************************************************************************************
Game chargerPartie(int id){
    FILE *fic;
    fic = fopen("Sauvegardes/PartiesListe.dat","r");
    game lPart;
    if(fic != NULL){
        Partie lPart;
        while(lPart.id != id){
            fread(&lPart, sizeof(lPart), 1, fic);
        }
        return lPart;
    }else{
        /* Retourner une Erreur */
        return lPart;
    }
}
//***************************************************************************************************************
int listerParties(){
    FILE *fic;
    fic = fopen("Sauvegardes/PartiesListe.dat","r");
    Partie lPart;
    if(fic != NULL){
        Partie lPart;
        printf(" IA/ id : blanc - noir  tour\n");
            fread(&lPart, sizeof(lPart), 1, fic);
        while(feof(fic)!=1){
            printf("%3d(%d) : %3d  -  %3d  %4d\n",lPart.id,lPart.ia,lPart.scoreB,lPart.scoreN,lPart.tour);
            fread(&lPart, sizeof(lPart), 1, fic);
        }
        return 1;
    }else{
        printf("Pas de parties sauvegardée");
        return 0;
    }
}
//***************************************************************************************************************
void afficherScores(){
    FILE *fic;
    fic = fopen("Sauvegardes/PartiesListe.dat","r");
    Partie lPart;
    if(fic != NULL){
        Partie lPart;
        printf(" IA/ id : blanc - noir  tour\n");
            fread(&lPart, sizeof(lPart), 1, fic);
        while(feof(fic)!=1){
            printf("%3d(%d) : %3d  -  %3d  %4d\n",lPart.id,lPart.ia,lPart.scoreB,lPart.scoreN,lPart.tour);
            fread(&lPart, sizeof(lPart), 1, fic);
        }
        return 1;
    }else{
        printf("Pas de parties sauvegardée");
        return 0;
    }
}
//***************************************************************************************************************

