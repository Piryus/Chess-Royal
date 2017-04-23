#include "defs.h"

/*
Deux fichiers :
    Fichier de joueurs : JoueurListe.dat
    Fichier parties en cours : PartiesListe.dat

Menu principal:
    Séléctionner les joueurs
    Afficher scores des joueurs
    Charger partie
    Nouvelle partie - IA ou Local
    Quitter

en jeu:
    Fonctions d'affichages
    Fonctions d'Initialisation
    Fonctions de manipulation des fichier
    Fonction tour suivant
    Selectionner & Deplacer un pion
    Calcul du tour de l'IA
    Fonctions fin de partie ( retour menu , enregistrement des score , etc)
    *MaJ d'un fichier à chaque coup : positions + données de jeu

*/
void nouveauJoueur(char nom[]){
    //recherche de l'id maximal
    FILE *fic;
    fic = fopen("JoueurListe.dat","r");
    Joueur X;
    int maxid = -1;
    if(fic != NULL){
        fread(&X, sizeof(Joueur), 1, fic);
        while(!feof(fic)){
            if(X.id>maxid){maxid = X.id;}
            fread(&X, sizeof(Joueur), 1, fic);
        }
        fclose(fic);
    }
    printf("MAX - JID : %d \n",maxid);
    //création du joueur
    Joueur newPlayer;
    newPlayer.id = maxid+1;
    strcpy(newPlayer.Nom,nom);
    newPlayer.nbEgal = 0;
    newPlayer.nbWin = 0;
    newPlayer.nbLose = 0;
    newPlayer.nbGame = 0;

    //Ecriture
    fic = fopen("JoueurListe.dat","a");
    if(fic == NULL){
        fic = fopen("JoueurListe.dat","w+");
        printf("Création du fichier de sauvegarde\n");
    }
    printf("Creation du joueur %s \n", nom);
    fwrite(&newPlayer, sizeof(Joueur), 1, fic);
    fclose(fic);
}

void chargerJoueur(int id, Joueur * joueur){
    FILE *fic;
    fic = fopen("JoueurListe.dat","r");
    if(fic != NULL){
        fread(joueur, sizeof(Joueur), 1, fic);
        while((joueur->id != id)&&(!feof(fic))){
            printf("    > %s  ( ID : %2d)\n",joueur->Nom,joueur->id);
            fread(joueur, sizeof(Joueur), 1, fic);
        }
        printf("    > %s ( ID : %2d)\n",joueur->Nom,joueur->id);
        if(joueur->id != id){
            joueur->id = -1;
            printf("Impossible de charger le joueur\n");
        }else{
            printf("Joueur %s Charge ( ID : %2d)\n",joueur->Nom,joueur->id);
        }
    }else{
        joueur->id = -1;
    }
    fclose(fic);

}

void SaveJoueur( Joueur * joueur ){
    printf("----------------  SAVE J ----------------");
    FILE *fic;
    int size = nbJoueur();
   // printf("    Size : %d \n",size);
    //on charge tout dans un meme tableau
    fic = fopen("JoueurListe.dat","r");
    Joueur ljoueur[size];
    for(int a = 0; a<size ; a++){
        fread(&ljoueur[a], sizeof(Joueur), 1, fic);
    }
    fclose(fic);
    //on cherche le joueur approprié
    printf("\nrecherche du joueur %s\n",joueur->Nom);
    int i = 0;
    while(ljoueur[i].id != joueur->id){
        printf("    %3d>joueur %s  W:%2d  L:%2d  E:%2d  G:%2d \n",ljoueur[i].id,ljoueur[i].Nom,ljoueur[i].nbWin,ljoueur[i].nbLose,ljoueur[i].nbEgal,ljoueur[i].nbGame);
        i++;
    }
    printf("    %3d>joueur %s  W:%2d  L:%2d  E:%2d  G:%2d \n",ljoueur[i].id,ljoueur[i].Nom,ljoueur[i].nbWin,ljoueur[i].nbLose,ljoueur[i].nbEgal,ljoueur[i].nbGame);
    printf("Affectation des nouvelle valeurs : \n");
    //ljoueur[i].id = joueur->id;
    //strcpy(ljoueur[i].Nom,joueur->Nom);

    ljoueur[i].nbWin = joueur->nbWin;
    ljoueur[i].nbLose = joueur->nbLose;
    ljoueur[i].nbGame = joueur->nbGame;
    ljoueur[i].nbEgal = joueur->nbEgal;
    printf("    %3d>joueur %s  W:%2d  L:%2d  E:%2d  G:%2d \n",ljoueur[i].id,ljoueur[i].Nom,ljoueur[i].nbWin,ljoueur[i].nbLose,ljoueur[i].nbEgal,ljoueur[i].nbGame);
    //on réécrit tout dans le fichier
    remove("JoueurListe.dat");
    fic = fopen("JoueurListe.dat","w+");
   // printf("rewrite all \n");
    for(int a = 0; a<size ; a++){
        fwrite(&ljoueur[a], sizeof(Joueur), 1, fic);
    }
    fclose(fic);
    printf("X\n");
}

void modifJoueur(int ia, int winner , int njN, int njB){
    Joueur jB;
    chargerJoueur(njB, &jB);
    if(winner == _BLANC){jB.nbWin++;jB.nbGame++;}
    if(winner == _NOIR){jB.nbLose++;jB.nbGame++;}
    if(winner == _VIDE){jB.nbEgal++;jB.nbGame++;}
    SaveJoueur( &jB );
    if(ia == 0){
        Joueur jN;
        chargerJoueur(njN, &jN);
        if(winner == _NOIR){jN.nbWin++;jN.nbGame++;}
        if(winner == _BLANC){jN.nbLose++;jN.nbGame++;}
        if(winner == _VIDE){jN.nbEgal++;jN.nbGame++;}
        SaveJoueur( &jN );
    }
}

int nbJoueur(){
    Joueur X;
    FILE *fic;
    fic = fopen("JoueurListe.dat","r");
    int size = 0;
    if(fic != NULL){
        while(!feof(fic)){
            size ++;
            fread(&X, sizeof(Joueur), 1, fic);
        }
        fclose(fic);
        size --; //correction nécessaire pour windows
    }
    return size;
}

int listerJoueur(Joueur joueurs[]){ // OK ; écrit la liste des joueurs dans le tableau que l'on lui donne et retourne 1  en cas d'erreur
    FILE *fic;
    fic = fopen("JoueurListe.dat","r");
    if(fic != NULL){
        // on met tout dans un tableau
        int size = nbJoueur();
        for(int a = 0; a<size ; a++){
            fread(&joueurs[a], sizeof(Joueur), 1, fic);
        }
        fclose(fic);
        return 0;
    }else{
        printf("Pas de Joueur sauvegarde\n");
        return 1;
    }
}

void Save( Game * partie ){  //OK
    printf("----------------  SAVE  -----------------");
    FILE *fic;
    int size = saveSize();
   // printf("    Size : %d \n",size);
    //on charge tout dans un meme tableau
    fic = fopen("PartiesListe.dat","r");
    Game lPart[size];
    for(int a = 0; a<size ; a++){
        fread(&lPart[a], sizeof(Game), 1, fic);
    }
    fclose(fic);
    //on cherche la partie apropriée
    int i = 0;
    while(lPart[i].id != partie->id){
        i++;
    }
    //printf(" id %d = id %d >OK\n",lPart[i].id ,partie->id);
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
   // printf("rewrite all \n");
    for(int a = 0; a<size ; a++){
        fwrite(&lPart[a], sizeof(Game), 1, fic);
    }
    fclose(fic);
    printf("X\n");
}
//***************************************************************************************************************
void nouvellePartie(int ia, Game * partie ,int idJB,int idJN){// OK
    //recherche de l'id maximal
    FILE *fic;
    fic = fopen("PartiesListe.dat","r");
    Game X;
    int maxid = 0;
    if(fic != NULL){
        fread(&X, sizeof(Game), 1, fic);
        while(!feof(fic)){
            if(X.id>maxid){maxid = X.id;}
            fread(&X, sizeof(Game), 1, fic);
        }
        fclose(fic);
    }
    printf("MAX - GID : %d \n",maxid);
    //création de la partie
    partie->id = maxid+1;
    partie->ia = ia;
    partie->joueurB = idJB;
    partie->joueurN = idJN;
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
    printf("Creation de la sauvegarde %d (IA:%d)\n", partie->id , partie->ia);
    fwrite(partie, sizeof(Game), 1, fic);
    fclose(fic);
}
//***************************************************************************************************************
void chargerPartie(int id, Game * partie){ // OK
    FILE *fic;
    fic = fopen("PartiesListe.dat","r");
    if(fic != NULL){
        fread(partie, sizeof(Game), 1, fic);
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
        }
        fclose(fic);
        return 0;
    }else{
        printf("Pas de parties sauvegardée");
        return 1;
    }
}

void supprimerPartie(int id){
printf("----------------  SUPP  -----------------");

    FILE *fic;
    int size = saveSize();
   // printf("    Size : %d \n",size);
    //on charge tout dans un meme tableau
    fic = fopen("PartiesListe.dat","r");
    Game lPart[size];
    for(int a = 0; a<size ; a++){
        fread(&lPart[a], sizeof(Game), 1, fic);
    }
    fclose(fic);
    //on cherche la partie apropriée
    int i = 0;
    while(lPart[i].id != id){
        i++;
    }
    //printf(" id %d = id %d >OK\n",lPart[i].id ,partie->id);
    //ondécale toutes les parties qui suivent
    while(i<size){
        lPart[i].ia = lPart[i+1].ia;
        lPart[i].id = lPart[i+1].id;
        lPart[i].winner = lPart[i+1].winner;
        lPart[i].scoreB = lPart[i+1].scoreB;
        lPart[i].scoreN = lPart[i+1].scoreN;
        lPart[i].tour = lPart[i+1].tour;
        for(int c = 0; c<8 ; c++){
            for(int l = 0; l<8 ; l++){
                lPart[i].plateau[c][l] = lPart[i+1].plateau[c][l];
            }
        }
        i++;
    }
    size--;
    //on réécrit tout dans le fichier
    fic = fopen("PartiesListe.dat","w+");
   // printf("rewrite all \n");
    for(int a = 0; a<size ; a++){
        fwrite(&lPart[a], sizeof(Game), 1, fic);
    }
    fclose(fic);
    printf("X\n");
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
