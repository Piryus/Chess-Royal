#include "defs.h"

/*
Deux fichiers :
    Fichier de joueurs : JoueurListe.dat
    Fichier parties en cours : PartiesListe.dat

Menu principal:
    S�l�ctionner les joueurs
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
    *MaJ d'un fichier � chaque coup : positions + donn�es de jeu

*/
void nouveauJoueur(char nom[]){
    //recherche de l'id maximal
    FILE *fic;
    fic = fopen("JoueurListe.dat","r");
    Joueur X;
    int maxid = 0;
    if(fic != NULL){
            fread(&X, sizeof(Joueur), 1, fic);
        while(!feof(fic)){
            if(X.id>maxid){maxid = X.id;}
            fread(&X, sizeof(Joueur), 1, fic);
        }
        fclose(fic);
    }
    printf("MAX - ID : %d \n",maxid);
    //cr�ation du joueur
    Joueur newPlayer;
    newPlayer.id = maxid + 1 ;
    strcpy(newPlayer.Nom,nom);
    newPlayer.nbEgal = 0;
    newPlayer.nbWin = 0;
    newPlayer.nbLose = 0;
    newPlayer.nbGame = 0;

    //Ecriture
    fic = fopen("JoueurListe.dat","a");
    if(fic == NULL){
        fic = fopen("JoueurListe.dat","w+");
        printf("Cr�ation du fichier de sauvegarde\n");
    }
    printf("Creation du joueur %s (id : %d)\n ", nom, maxid +1);
    fwrite(&newPlayer, sizeof(Joueur), 1, fic);
    fclose(fic);
}

void chargerJoueur(int id, Joueur * joueur){
    FILE *fic;
    fic = fopen("JoueurListe.dat","r");
    if(fic != NULL){
        while((joueur->id != id)&&(feof(fic)!=1)){
            fread(joueur, sizeof(joueur), 1, fic);
        }
        if(joueur->id != id)
            joueur->id = -1;
    }else{
        joueur->id = -1;
    }
    fclose(fic);

}

void modifJoueur(int ia, int winner , int njN, int njB){
    Joueur jB;
    chargerJoueur(njB, &jB);
    //chargerJoueur(1, &jB);
    if(winner == _BLANC){jB.nbWin++;jB.nbGame++;}
    if(winner == _NOIR){jB.nbLose++;jB.nbGame++;}
    if(winner == _VIDE){jB.nbEgal++;jB.nbGame++;}
    SaveJoueur( &jB );
    if(ia == 0){
        Joueur jN;
        chargerJoueur(njN, &jN);
        //chargerJoueur(2, &jN);
        if(winner == _BLANC){jN.nbWin++;jN.nbGame++;}
        if(winner == _NOIR){jN.nbLose++;jN.nbGame++;}
        if(winner == _VIDE){jN.nbEgal++;jN.nbGame++;}
        SaveJoueur( &jN );
    }
}

void SaveJoueur( Joueur * joueur ){
    printf("----------------  SAVE J  -----------------\n");
    FILE *fic;
    int size = saveSize();
    printf("    Size : %d \n",size);
    //on charge tout dans un meme tableau
    fic = fopen("JoueurListe.dat","r");
    Joueur ljoueur[size];
    for(int a = 0; a<size ; a++){
        fread(&ljoueur[a], sizeof(Joueur), 1, fic);
        printf("fread %d : j[id:%d]  \n",a,ljoueur[a].id);
    }
    fclose(fic);
    //on cherche la joueur apropri�e
    int i = 0;
    while(ljoueur[i].id != joueur->id){
        i++;
    }
    //printf(" id %d = id %d >OK\n",ljoueur[i].id ,joueur->id);
    //on modifie la joueur apropri�e
    ljoueur[i].id = joueur->id;
    strcpy(ljoueur[i].Nom,joueur->Nom);
    ljoueur[i].nbWin = joueur->nbWin;
    ljoueur[i].nbLose = joueur->nbLose;
    ljoueur[i].nbGame = joueur->nbGame;
    ljoueur[i].nbEgal = joueur->nbEgal;
    //on r��crit tout dans le fichier
    fic = fopen("JoueurListe.dat","w+");
   // printf("rewrite all \n");
    for(int a = 0; a<size ; a++){
        fwrite(&ljoueur[a], sizeof(Joueur), 1, fic);
    }
    fclose(fic);
   // printf("--------------  END SAVE  ---------------\n");
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
        size --; //correction n�cessaire pour windows
    }
    return size;
}

int listerJoueur(Joueur joueurs[]){ // OK ; �crit la liste des partie dans le tableau que l'on lui donne et retourne 1  en cas d'erreur
    FILE *fic;
    fic = fopen("JoueurListe.dat","r");
    if(fic != NULL){
        // on met tout dans un tableau
        int size = nbJoueur();
        for(int a = 0; a<size ; a++){
            fread(&joueurs[a], sizeof(Joueur), 1, fic);
            printf("fread %d : j[id:%d]  \n",a,joueurs[a].id);
        }
        fclose(fic);
        return 0;
    }else{
        printf("Pas de Joueur sauvegard�");
        return 1;
    }
}

void Save( Game * partie ){  //OK
    printf("----------------  SAVE  -----------------\n");
    FILE *fic;
    int size = saveSize();
   // printf("    Size : %d \n",size);
    //on charge tout dans un meme tableau
    fic = fopen("PartiesListe.dat","r");
    Game lPart[size];
    for(int a = 0; a<size ; a++){
        fread(&lPart[a], sizeof(Game), 1, fic);
        printf("fread %d : [id:%d]  \n",a,lPart[a].id);
    }
    fclose(fic);
    //on cherche la partie apropri�e
    int i = 0;
    while(lPart[i].id != partie->id){
        i++;
    }
    //printf(" id %d = id %d >OK\n",lPart[i].id ,partie->id);
    //on modifie la partie apropri�e
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
    //on r��crit tout dans le fichier
    fic = fopen("PartiesListe.dat","w+");
   // printf("rewrite all \n");
    for(int a = 0; a<size ; a++){
        fwrite(&lPart[a], sizeof(Game), 1, fic);
    }
    fclose(fic);
    //printf("--------------  END SAVE  ---------------\n");
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
    printf("MAX - ID : %d \n",maxid);
    //cr�ation de la partie
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
        printf("Cr�ation du fichier de sauvegarde\n");
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
int listerParties(Game parties[]){ // OK ; �crit la liste des partie dans le tableau que l'on lui donne et retourne 1  en cas d'erreur
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
        printf("Pas de parties sauvegard�e");
        return 1;
    }
}

void supprimerPartie(int id){
printf("----------------  SUPP  -----------------\n");

    FILE *fic;
    int size = saveSize();
   // printf("    Size : %d \n",size);
    //on charge tout dans un meme tableau
    fic = fopen("PartiesListe.dat","r");
    Game lPart[size+1];
    for(int a = 0; a<size ; a++){
        fread(&lPart[a], sizeof(Game), 1, fic);
        printf("fread %d : [id:%d]  \n",a,lPart[a].id);
    }
    fclose(fic);
    //on cherche la partie apropri�e
    int i = 0;
    while(lPart[i].id != id){
        i++;
    }
    //printf(" id %d = id %d >OK\n",lPart[i].id ,partie->id);
    //ond�cale toutes les parties qui suivent
    while(i<size){
        printf("iter : %d",i);
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
    //on r��crit tout dans le fichier
    printf("size : %d\n",size);
    if(size>0){
        fic = fopen("PartiesListe.dat","w+");
       // printf("rewrite all \n");
        for(int a = 0; a<size ; a++){
            printf("a : %d\n",a);
            fwrite(&lPart[a], sizeof(Game), 1, fic);
        }
        fclose(fic);
    }else{
        remove("PartiesListe.dat");
    }
   // printf("--------------  END SAVE  ---------------\n");
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
        size --; //correction n�cessaire pour windows
    }
    return size;
}