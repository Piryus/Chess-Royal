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

void Save( Game * partie ){  //OK
    printf("----------------  SAVE  -----------------\n");
    FILE *fic;
    int size = saveSize();
    printf("    Size : %d \n",size);
    //on charge tout dans un meme tableau
    fic = fopen("PartiesListe.dat","r");
    Game lPart[size];
    for(int a = 0; a<size ; a++){
        fread(&lPart[a], sizeof(Game), 1, fic);
        printf("fread %d : [id:%d]  \n",a,lPart[a].id);
    }
    fclose(fic);
    //on cherche la partie apropriée
    int i = 0;
    while(lPart[i].id != partie->id){
        i++;
    }
    printf(" id %d = id %d >OK\n",lPart[i].id ,partie->id);
    //on modifie la partie apropriée
    lPart[i].ia = partie->ia;
    lPart[i].id = partie->id;
    lPart[i].winner = partie->winner;
    lPart[i].scoreB = partie->scoreB;
    lPart[i].scoreN = partie->scoreN;
    lPart[i].tour = partie->tour;
    for(int c = 0; c<8 ; c++){
        for(int l = 0; l<8 ; l++){
            lPart[i].plateau[c][l] = partie->plateau[c][l];
        }
    }
    //on réécrit tout dans le fichier
    fic = fopen("PartiesListe.dat","w+");
    printf("rewrite all \n");
    for(int a = 0; a<size ; a++){
        fwrite(&lPart[a], sizeof(Game), 1, fic);
    }
    fclose(fic);
    printf("--------------  END SAVE  ---------------\n");
}
//***************************************************************************************************************
void nouvellePartie(int ia, Game * partie){// OK
    //recherche de l'id maximal
    FILE *fic;
    fic = fopen("PartiesListe.dat","r");
    Game X;
    int maxid = 0;
    if(fic != NULL){
        while(!feof(fic)){
            fread(&X, sizeof(Game), 1, fic);
            if(X.id>maxid){maxid = X.id;}
        }
        fclose(fic);
    }
    printf("MAX - ID : %d \n",maxid);
    //création de la partie
    partie->id = maxid+1;
    partie->ia = ia;
    partie->winner = _VIDE ;
    partie->scoreB = 0 ;
    partie->scoreN = 0 ;
    partie->tour = 1 ; //(Tour impair = blanc)
    for(int c = 0;c<8;c++){
        partie->plateau[c][0] = _NOIR;
        partie->plateau[c][1] = _NOIR;
        partie->plateau[c][2] = _VIDE;
        partie->plateau[c][3] = _VIDE;
        partie->plateau[c][4] = _VIDE;
        partie->plateau[c][5] = _VIDE;
        partie->plateau[c][6] = _BLANC;
        partie->plateau[c][7] = _BLANC;
    }
    //Ecriture
    fic = fopen("PartiesListe.dat","a");
    if(fic == NULL){
        fic = fopen("PartiesListe.dat","w+");
        printf("Création du fichier de sauvegarde\n");
    }
    printf("Creation de la sauvegarde %d (IA:%d)", partie->id , partie->ia);
    fwrite(partie, sizeof(Game), 1, fic);
    fclose(fic);
}
//***************************************************************************************************************
void chargerPartie(int id, Game * partie){ // OK
    FILE *fic;
    fic = fopen("PartiesListe.dat","r");
    if(fic != NULL){
        while((partie->id != id)&&(feof(fic)!=1)){
            fread(partie, sizeof(Game), 1, fic);
        }
        if(partie->id != id)
            partie->id = -1;
    }else{
        partie->id = -1;
    }
    fclose(fic);

}
//***************************************************************************************************************
int listerParties(Game parties[]){ // OK ; écrit la liste des partie dans le tableau que l'on lui donne et retourne 1  en cas d'erreur
    FILE *fic;
    fic = fopen("PartiesListe.dat","r");
    if(fic != NULL){
        // on met tout dans un tableau
        int size = saveSize();
        for(int a = 0; a<size ; a++){
            fread(&parties[a], sizeof(Game), 1, fic);
            printf("fread %d : [id:%d]  \n",a,parties[a].id);
        }
        fclose(fic);
        return 0;
    }else{
        printf("Pas de parties sauvegardée");
        return 1;
    }
}
//***************************************************************************************************************
int saveSize(){
    Game X;
    FILE *fic;
    fic = fopen("PartiesListe.dat","r");
    int size = 0;
    if(fic != NULL){
        while(!feof(fic)){
            size ++;
            fread(&X, sizeof(Game), 1, fic);
        }
        fclose(fic);
        size --; //correction nécessaire pour windows
    }
    return size;
}
