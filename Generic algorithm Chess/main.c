#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <windows.h>
#define _VIDE 0
#define _BLANC 1
#define _NOIR 2
#define _DEBUG 0 // 0-5

//réglages :
#define _NBPOPULATION 100
#define _SURVIVANTPARGEN 60
#define _FACTEURDECONVERGENCE 1
#define _NEWRANDOMPLAYERSPARGEN 10
#define _BASEMUTABILLITE 40 //en pourcentage


typedef struct joueur{
    int generation;
    int nbVictoires;
    int nbDefaites;
    int type;
    float typeCoeff;
    float gamma[7] ;
} joueur;

typedef struct possibillite{
    int deX;
    int deY;
    int aX;
    int aY;
    float poids;
} possibillite;

int arrAcc(int plt[][8],int x, int y){
    if( (x<8) & (x>=0) & (y<8) & (y>=0) ){
        return plt[x][y];
    }else{
        return _VIDE;
    }
}
//fonction qui renvoie le valeur d'une future situation eventuelle
float poids(int color,int plt[][8],int frmX,int frmY,int toX,int toY,joueur *jB,joueur *jN){
    int implt[8][8];
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            implt[c][l] = plt[c][l];
        }
    }
    implt[frmX][frmY] = _VIDE;
    implt[toX][toY] = color;


    if(_DEBUG>=5){
            printf("\n");
            if(color == _BLANC){printf("> b(");}else{printf("> n(");}
            if(implt[frmX][frmY] == _BLANC){
                    printf("b) ");
            }else{if(implt[frmX][frmY] == _NOIR){
                printf("n) ");
            }else{
                printf("v) ");
            }}
            printf("[%d;%d] -> [%d;%d] ",frmX,frmY,toX,toY);
    }
    //    P, nombre de pions
    int pB = 0;
    int pN = 0;
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            if(implt[c][l] == _BLANC){pB++;}
            if(implt[c][l] == _NOIR){pN++;}
        }
    }
    if(_DEBUG>=5){printf(" pN = %2d ",pN);}
    if(_DEBUG>=5){printf(" pB = %2d ",pB);}

    //    D, défendu par un allié
    int dB = 0;
    int dN = 0;
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            if(arrAcc(implt,c,l) == _BLANC){if(arrAcc(implt,c-1,l+1)==_BLANC||arrAcc(implt,c+1,l+1)==_BLANC){dB++;}}
            if(arrAcc(implt,c,l) == _NOIR){if(arrAcc(implt,c-1,l-1)==_NOIR||arrAcc(implt,c+1,l-1)==_NOIR){dN++;}}
        }
    }
    if(_DEBUG>=5){printf(" dN = %2d ",dN);}
    if(_DEBUG>=5){printf(" dB = %2d ",dB);}

    //    B, bloqué : pions n'ayant plus de mouvements possibles sans mourrir
    int bB = 0;
    int bN = 0;
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            if(arrAcc(implt,c,l) == _BLANC){
                if(
                   ((arrAcc(implt,c,l-1) != _VIDE)||(arrAcc(implt,c-1,l-2) == _NOIR)||(arrAcc(implt,c+1,l-2) == _NOIR))
                   &((arrAcc(implt,c-1,l-1) != _NOIR)||(arrAcc(implt,c,l-2) != _NOIR)||(arrAcc(implt,c-2,l-2) != _NOIR))
                   &((arrAcc(implt,c+1,l-1) != _NOIR)||(arrAcc(implt,c,l-2) != _NOIR)||(arrAcc(implt,c+2,l-2) != _NOIR))
                   )
                {bB++;}
            }
            if(arrAcc(implt,c,l) == _NOIR){
                if(
                   ((arrAcc(implt,c,l+1) != _VIDE)||(arrAcc(implt,c-1,l+2) == _BLANC)||(arrAcc(implt,c+1,l+2) == _BLANC))
                   &((arrAcc(implt,c-1,l+1) != _BLANC)||(arrAcc(implt,c,l+2) != _BLANC)||(arrAcc(implt,c-2,l+2) != _BLANC))
                   &((arrAcc(implt,c+1,l+1) != _BLANC)||(arrAcc(implt,c,l+2) != _BLANC)||(arrAcc(implt,c+2,l+2) != _BLANC))
                    )
                   {bN++;}
            }
        }
    }
    if(_DEBUG>=5){printf(" bN = %2d ",bN);}
    if(_DEBUG>=5){printf(" bB = %2d ",bB);}

    //    I, avancement : position du pion le plus avancé
    int iB = 0;
    int iN = 0;
    for(int l = 0;l<8;l++){
        for(int c = 0;c<8;c++){
            if((arrAcc(implt,c,l)==_BLANC)&(7-l>iB)){iB=7-l;}
            if((arrAcc(implt,c,l)==_NOIR)&(l>iN)){iN=l;}
        }
    }
    if(_DEBUG>=5){printf(" iN = %2d ",iN);}
    if(_DEBUG>=5){printf(" iB = %2d ",iB);}

    //    A : avancement total  (somme des avancements sur chaque colone)
    int aB = 0;
    int aN = 0;
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            if(implt[c][l] == _BLANC){aB+=7-l;}
            if(implt[c][l] == _NOIR){aN+=l;}
        }
    }
    if(_DEBUG>=5){printf(" aN = %2d ",aN);}
    if(_DEBUG>=5){printf(" aB = %2d ",aB);}

    //    M : Coup qui rend un pion mangeable
    int mB = 0;
    int mN = 0;
    if(color == _BLANC){
        if((implt[toX-1][toY-1] == _NOIR)||(implt[toX+1][toY-1] == _NOIR)){mB = 1;}
    }else{
        if((implt[toX-1][toY+1] == _BLANC)||(implt[toX+1][toY+1] == _BLANC)){mN = 1;}
    }
    if(_DEBUG>=5){printf(" mN = %2d ",mN);}
    if(_DEBUG>=5){printf(" mB = %2d ",mB);}

    //    W, Situation de victoire
    int wB = 0;
    int wN = 0;
    for(int c = 0;c<8;c++){
        if(arrAcc(implt,c,7) == _NOIR){wN=1;}
        if(arrAcc(implt,c,0) == _BLANC){wB=1;}
    }

    if(_DEBUG>=5){printf(" wN = %2d ",wN);}
    if(_DEBUG>=5){printf(" wB = %2d ",wB);}

    //implt[frmX][frmY] = color;
    //implt[toX][toY] = tmpclr;
    //    f(p) = gamma(P-P') + gamma2(D-D') + gamma2(B-B') + gamma2(I-I')  + gamma6(M-M') + 1000*(W-W')

    float poids;
    if(color == _BLANC){
        poids = jB->gamma[0]*(pB-pN) + jB->gamma[1]*(dB-dN)/50 + jB->gamma[2]*(bB-bN) + jB->gamma[3]*(iB-iN)*5  + jB->gamma[4]*(mB-mN) + jB->gamma[5]*(aB-aN)/100 + jB->gamma[6]*(wB-wN)*100;
        if(_DEBUG>=5){printf(" | poids = %f",poids);}
        return poids;
    }else{
        poids = jN->gamma[0]*(pN-pB) + jN->gamma[1]*(dN-dB)/50 + jN->gamma[2]*(bN-bB) + jN->gamma[3]*(iN-iB)*5  + jN->gamma[4]*(mN-mB) + jN->gamma[5]*(aN-aB)/100 + jN->gamma[6]*(wN-wB)*100;
        if(_DEBUG>=5){printf(" poids = %f",poids);}
        return poids;
    }
}



char colorconverter(int color){
    if(color==_NOIR)return 'N';
    if(color==_BLANC)return 'B';
    if(color==_VIDE)return ' ';
    return 'X';
}

int deplacement(possibillite dpl, int plat[][8],int color, int debug){

        if((plat[dpl.deX][dpl.deY]!=color)||(plat[dpl.aX][dpl.aY]==color)){
            for(int l = 0;l<8;l++){
                if(debug>=0){printf("\n");}
                for(int c = 0;c<8;c++){
                     if(debug>=0){printf("[%c]" , colorconverter(plat[c][l]));}
                }
            }
            printf("\n!erreur : (%c)deplacement %d %d (%c)> %d %d (%c) interdit , partie annule\n",
             colorconverter(color) ,
             dpl.deX , dpl.deY , colorconverter(plat[dpl.deX][dpl.deY]) ,
             dpl.aX , dpl.aY , colorconverter(plat[dpl.aX][dpl.aY]));

            fflush(stdin);
            char c;
            if(debug>=0){scanf("%c",&c);}
        return 1;
    }else{
        if(color == _NOIR){
            if(debug>=1){printf("\nNoir  :");}
        }else{
            if(debug>=1){printf("\nBlanc :");}
        }
        if(debug>=1){printf("  Deplacement: %d %d > %d %d \n" , dpl.deX , dpl.deY , dpl.aX , dpl.aY );}
        plat[dpl.deX][dpl.deY] = _VIDE;
        plat[dpl.aX][dpl.aY] = color;

        return 0;
    }
}

void affPlt(int plt[][8],int debug){
    for(int l = 0;l<8;l++){
        if(debug>=2){printf("\n");}
        for(int c = 0;c<8;c++){
            if(debug>=2){
                if(plt[c][l] == _BLANC){printf("[B]");}
                if(plt[c][l] == _NOIR){printf("[N]");}
                if(plt[c][l] == _VIDE){printf("[ ]");}
            }
        }
    }
}

void affScores(joueur population[],int taille,int gammaActivated){
    for(int i = 0;i < taille ; i++){
        printf("\n %3d : Gen%3d | Win : %2d | Loss : %2d",i,population[i].generation,population[i].nbVictoires,population[i].nbDefaites)  ;
        if(gammaActivated == 1||_DEBUG>=1){
            printf(" |> %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f ",population[i].gamma[0],population[i].gamma[1],population[i].gamma[2],population[i].gamma[3],population[i].gamma[4],population[i].gamma[5],population[i].gamma[6])  ;
        }
    }
}

void listActions(possibillite actionlist[] , int color ,joueur population[], int *joueurNoir , int *joueurBlanc , int plateau[][8]){
    int actAdr = 0;
    int colorDir;
    int oppcolor;
    if(color == _BLANC){colorDir=-1;}else{colorDir=1;}
    if(color == _BLANC){oppcolor=_NOIR;}else{oppcolor=_BLANC;}

    for(int j = 0;j < 40 ; j++){actionlist[j].aX = -1;actionlist[j].aY = -1;actionlist[j].deX = -1;actionlist[j].deY = -1;actionlist[j].poids = -10000000;}
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            if(plateau[c][l] == color){
                if(plateau[c][l+colorDir] == _VIDE){
                    actionlist[actAdr].poids = poids(color,plateau,c,l,c,l+colorDir,&population[*joueurBlanc],&population[*joueurNoir]);
                    actionlist[actAdr].deX = c;
                    actionlist[actAdr].deY = l;
                    actionlist[actAdr].aX = c;
                    actionlist[actAdr].aY = l+colorDir;
                    actAdr++;
                }
                if((c-1>=0)&(l+colorDir>=0))if(plateau[c-1][l+colorDir] == oppcolor){
                    actionlist[actAdr].poids = poids(color,plateau,c,l,c-1,l+colorDir,&population[*joueurBlanc],&population[*joueurNoir]);
                    actionlist[actAdr].deX = c;
                    actionlist[actAdr].deY = l;
                    actionlist[actAdr].aX = c-1;
                    actionlist[actAdr].aY = l+colorDir;
                    actAdr++;
                }
                if((c+1<8)&(l+colorDir>=0))if(plateau[c+1][l+colorDir] == oppcolor){
                    actionlist[actAdr].poids = poids(color,plateau,c,l,c+1,l+colorDir,&population[*joueurBlanc],&population[*joueurNoir]);
                    actionlist[actAdr].deX = c;
                    actionlist[actAdr].deY = l;
                    actionlist[actAdr].aX = c+1;
                    actionlist[actAdr].aY = l+colorDir;
                    actAdr++;
                }
            }
        }
        float f = 3.5 - 2.5*colorDir; // soit 1 soit 6
        if((plateau[c][(int)f] == color)&(plateau[c][(int)f+2*colorDir] == _VIDE)){
            actionlist[actAdr].poids = poids(color,plateau,c,6,c,4,&population[*joueurBlanc],&population[*joueurNoir]);
            actionlist[actAdr].deX = c;
            actionlist[actAdr].deY = (int)f;
            actionlist[actAdr].aX = c;
            actionlist[actAdr].aY = (int)f+2*colorDir;
            actAdr++;
        }
        if((plateau[c][(int)f-colorDir] == color)&(plateau[c][(int)f+colorDir] == _VIDE)){
            actionlist[actAdr].poids = poids(color,plateau,c,7,c,5,&population[*joueurBlanc],&population[*joueurNoir]);
            actionlist[actAdr].deX = c;
            actionlist[actAdr].deY = (int)f-colorDir;
            actionlist[actAdr].aX = c;
            actionlist[actAdr].aY = (int)f+colorDir;
            actAdr++;
        }
    }
}

int findBestAction(possibillite *bestAction,int color,int plateau[][8],joueur population[],int *joueurNoir,int *joueurBlanc){
    possibillite actionlist[50];

    listActions( actionlist , color , population, joueurNoir , joueurBlanc , plateau );

    int seeker = 0;
    bestAction->poids = -1000000;
    if(_DEBUG>=1){printf("\n");}
    int err = 1;

    while(actionlist[seeker].aX!=-1){
        if(actionlist[seeker].poids>(bestAction->poids)){
            err = 0;
            bestAction->poids = actionlist[seeker].poids;
            bestAction->deX = actionlist[seeker].deX;
            bestAction->deY = actionlist[seeker].deY;
            bestAction->aX = actionlist[seeker].aX;
            bestAction->aY = actionlist[seeker].aY;
        if(_DEBUG>=2){printf("  newBA : [%d;%d] > [%d;%d] ",actionlist[seeker].deX,actionlist[seeker].deY,actionlist[seeker].aX,actionlist[seeker].aY);}
        }
        seeker ++;
    }
    return err;
}

void partie(joueur population[],int *joueurBlanc,int *joueurNoir,int * debug ){
    int winner = _VIDE;
    int plateau[8][8];
    //nettoyage du plateau de jeu
    for(int i = 0;i<8;i++){
        plateau[i][0] = _NOIR;
        plateau[i][1] = _NOIR;
        plateau[i][2] = _VIDE;
        plateau[i][3] = _VIDE;
        plateau[i][4] = _VIDE;
        plateau[i][5] = _VIDE;
        plateau[i][6] = _BLANC;
        plateau[i][7] = _BLANC;
    }
    int turn = _BLANC;
    while(winner == _VIDE){
        //etablissement des actions possibles (Blanc) ===============================================================
        if(*debug>=1){system("cls");}
        affPlt(plateau,*debug);
        possibillite bestAction;
        int err = findBestAction(&bestAction,turn,plateau,population,joueurNoir,joueurBlanc);
        if(*debug>=1){printf("\n");}
        if(err != 1){
            if(deplacement(bestAction,plateau,turn,*debug) == 1){winner = 666;}//si erreur dans le déplacement, sauter ce coup
        }else{winner = 42;}


        for(int c = 0;c<8;c++){ //test de si il y a un gagnant
            if(plateau[c][0] == _BLANC){
                    winner=_BLANC;
                    population[*joueurNoir].nbVictoires++;
                    population[*joueurBlanc].nbDefaites++;
            }
            if(plateau[c][7] == _NOIR){
                    winner=_NOIR;
                    population[*joueurBlanc].nbVictoires++;
                    population[*joueurNoir].nbDefaites++;
            }
        }


    if(turn == _BLANC){turn = _NOIR;}else{turn = _BLANC;}

    char c;
    if(*debug>=5){scanf("%c",&c);}
    }
}

int main(){

    printf("Selection algorithm\n\n");

    printf("--------------------Initialisation--------------------\n");

    int nbIterations;
    int nbGEN;
    int debug = _DEBUG;
    float deltagamma = _BASEMUTABILLITE;
    srand(time(NULL)); // initialisation de rand
    printf("Generation de la population\n");
    joueur population[_NBPOPULATION];
    for(int i = 0;i < _NBPOPULATION ; i++){
        for(int j = 0;j < 7 ; j++){
            population[i].gamma[j] = rand()%100;
        }
        population[i].generation = 1;
        population[i].nbVictoires = 0;
        population[i].nbDefaites = 0;
        //population[i].type = rand()%2;
        //population[i].typeCoeff = (rand()%100)/100;
    }
        affScores(population,_NBPOPULATION,1);
        printf("\nNombre d iteration par generation : ");
        scanf("%d",&nbIterations);
        printf("\nNombre de generation a calculer: ");
        scanf("%d",&nbGEN);
   //////////////////////////////////////////////////////////////////////////////////
    do{
        int nbParties = 0;
        int partieParJoueur[_NBPOPULATION];
        for(int i = 0;i < _NBPOPULATION ; i++){
            partieParJoueur[i] = 0;
        }
        int joueurBlanc = -1;
        int joueurNoir = -1;
        for(int i = 0;i < nbIterations*_NBPOPULATION ; i++){
            //selection de deux joueurs
            joueurBlanc = ( joueurBlanc + 1 )%_NBPOPULATION;
            do{
                    joueurNoir = rand()%_NBPOPULATION;
            }while((joueurBlanc==joueurNoir));
            partieParJoueur[joueurBlanc]++;
            partieParJoueur[joueurNoir]++;


/*      ====        Partie      ====
            //affScores(population,_NBPOPULATION,0);
            //printf("\n-------- Joueur %d vs Joueur %d ------- (total: %d )  -----\n",joueurBlanc,joueurNoir,_NBPOPULATION);                                                */
            printf("\x0d");
            partie(population,&joueurBlanc,&joueurNoir,&debug);
            printf("---------------- Partie %3d / %3d / %3d -----------------",nbParties+1,nbIterations*_NBPOPULATION,nbIterations*_NBPOPULATION*nbGEN);
            nbParties++;
            if(debug>2){printf("DEBUG:");scanf("%d",&debug);}
/*      ====       Fin d'une Partie      ====                                           */


        }
        //system("cls");
        printf("\n--------------------------Processus---------------------\n");
            affScores(population,_NBPOPULATION,1);
        printf("\n----------------------# Reproduction #-------------------\n");
        printf("  >Les %d meilleurs parents : ",_SURVIVANTPARGEN);

        // On détermine les N meilleurs parents
        joueur parents[_SURVIVANTPARGEN];
        for(int i = 0;i < _SURVIVANTPARGEN ; i++){
            int bestJ = 0;
            int bestJRatio = 0.01;
            for(int j = 0;j < _NBPOPULATION ; j++){
                if(population[j].nbDefaites == 0){population[j].nbDefaites++;}
                if(population[j].nbVictoires/population[j].nbDefaites> bestJRatio ){
                    bestJRatio = population[j].nbVictoires/population[j].nbDefaites;
                    bestJ = j;
                }
            }
            for( int j=0; j<7; j++ ){
                parents[i].gamma[j] = population[bestJ].gamma[j];
            }
            parents[i].generation = population[bestJ].generation;
            parents[i].nbVictoires = population[bestJ].nbVictoires;
            parents[i].nbDefaites = population[bestJ].nbDefaites;
            parents[i].type = population[bestJ].type ;
            parents[i].typeCoeff = population[bestJ].typeCoeff ;
            population[bestJ].nbVictoires = 0; //pour par le prendre en doublon
        }
        affScores(parents,_SURVIVANTPARGEN,1);

        for(int i = 0;i < _SURVIVANTPARGEN ; i++){ //on replace les parents crées au début du tableau population
            for( int j=0; j<7; j++ ){
                population[i].gamma[j] = parents[i].gamma[j];
            }
            population[i].generation = parents[i].generation;
            population[i].nbVictoires = parents[i].nbVictoires;
            population[i].nbDefaites = parents[i].nbDefaites;
            //population[i].type = parents[i].type;
            //population[i].typeCoeff = parents[i].typeCoeff;
            population[i].nbVictoires = 0;
            population[i].nbDefaites = 0;
        }

        // On leur fait des decendants dans le reste du tableau
        for(int i = _SURVIVANTPARGEN ; i < _NBPOPULATION -_NEWRANDOMPLAYERSPARGEN ; i++){

            int parentA = rand()%_SURVIVANTPARGEN;
            int parentB ;
            do{parentB = rand()%_SURVIVANTPARGEN;}while(parentB==parentA);

            for( int j=0; j<7; j++ ){
                int switching = rand()%2;
                population[i].gamma[j] = switching*population[parentA].gamma[j]+(1-switching)*population[parentB].gamma[j];
            }
            if(population[parentA].generation>population[parentB].generation){
                population[i].generation =  population[parentA].generation + 1 ;
            }else{
                population[i].generation =  population[parentB].generation + 1 ;
            }
            //population[i].type = parents[parentA].type;
            //population[i].typeCoeff = (parents[parentA].typeCoeff + parents[parentB].typeCoeff)/2;
            population[i].nbVictoires = 0;
            population[i].nbDefaites = 0;
        }
        //recréation d'un pur aléatoire en dernier

        printf("\n  >Les %d Nouveaux individus:",_NEWRANDOMPLAYERSPARGEN);
        for(int k = _NBPOPULATION-_NEWRANDOMPLAYERSPARGEN;k<_NBPOPULATION;k++){
            for( int j=0; j<6; j++ ){
                    population[k].gamma[j] = 5-(rand()%100)/10;
                }
                population[k].generation = 1;
                population[k].nbVictoires = 0;
                population[k].nbDefaites = 0;
                //population[k].type = rand()%2;
                //population[k].typeCoeff = (rand()%100)/100;
                printf("\n %3d : gen%3d Win : %2d  Loss : %2d ",k,population[k].generation,population[k].nbVictoires,population[k].nbDefaites)  ;
                printf(" %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f ",population[k].gamma[0],population[k].gamma[1],population[k].gamma[2],population[k].gamma[3],population[k].gamma[4],population[k].gamma[5],population[k].gamma[6])  ;

        }
        printf("\n----------------------#  Mutations   #-------------------\n");
        // On fait mutter les descendants   #deltagamma
        for(int i = 0 ; i < _NBPOPULATION ; i++){
            for( int j=0; j<6; j++ ){
                population[i].gamma[j] += (50-(rand()%100))*deltagamma/100;
            }
            population[i].typeCoeff += (50-(rand()%100))*deltagamma/100;
        }
        deltagamma *= _FACTEURDECONVERGENCE;
        affScores(population,_NBPOPULATION,1);
        printf("\n-----------------Press enter for next gen----------------\n");
        fflush(stdin);
        nbGEN--;
        if(nbGEN<=0){
            printf("\n   Refaire n generations (n):\n");
            scanf("%d",&nbGEN);
            printf("\n   Activer le mode analyse (0/1):\n");
            scanf("%d",&debug);
        }
        fflush(stdin);
        char c;
        if(debug>=2){scanf("%c",&c);}

    }while(nbGEN>0);
    return 0;
}

