#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <windows.h>
#define _VIDE 0
#define _BLANC 1
#define _NOIR 2
#define _DEBUG 0

//réglages :
#define _NBPOPULATION 50
#define _SURVIVANTPARGEN 15
#define _NEWRANDOMPLAYERSPARGEN 10
#define _BASEMUTABILLITE 80 //en pourcentage


typedef struct joueur{
    int generation;
    int nbVictoires;
    int nbDefaites;
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
float poids(int color,int implt[][8],int frmX,int frmY,int toX,int toY,float Bgamma[],float Ngamma[]){
    int tmpclr = implt[toX][toY];
    //    P, nombre de pions
    if(_DEBUG>=1){
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
    implt[frmX][frmY] = _VIDE;
    implt[toX][toY] = color;
    int pB = 0;
    int pN = 0;
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            if(implt[c][l] == _BLANC){pB++;}
            if(implt[c][l] == _NOIR){pN++;}
        }
    }
    if(_DEBUG>=1){printf(" pN = %2d ",pN);}
    if(_DEBUG>=1){printf(" pB = %2d ",pB);}

    //    D, défendu par un allié
    int dB = 0;
    int dN = 0;
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            if(arrAcc(implt,c,l) == _BLANC){if(arrAcc(implt,c-1,l+1)==_BLANC||arrAcc(implt,c+1,l+1)==_BLANC){dB++;}}
            if(arrAcc(implt,c,l) == _NOIR){if(arrAcc(implt,c-1,l-1)==_NOIR||arrAcc(implt,c+1,l-1)==_NOIR){dN++;}}
        }
    }
    if(_DEBUG>=1){printf(" dN = %2d ",dN);}
    if(_DEBUG>=1){printf(" dB = %2d ",dB);}

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
    if(_DEBUG>=1){printf(" bN = %2d ",bN);}
    if(_DEBUG>=1){printf(" bB = %2d ",bB);}

    //    I, avancement : position du pion le plus avancé
    int iB = 0;
    int iN = 0;
    for(int l = 0;l<8;l++){
        for(int c = 0;c<8;c++){
            if((arrAcc(implt,c,l)==_BLANC)&(7-l>iB)){iB=7-l;}
            if((arrAcc(implt,c,l)==_NOIR)&(l>iN)){iN=l;}
        }
    }
    if(_DEBUG>=1){printf(" iN = %2d ",iN);}
    if(_DEBUG>=1){printf(" iB = %2d ",iB);}

    //    A : avancement total  (somme des avancements sur chaque colone)
    int aB = 0;
    int aN = 0;
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            if(implt[c][l] == _BLANC){aB+=7-l;}
            if(implt[c][l] == _NOIR){aN+=l;}
        }
    }
    if(_DEBUG>=1){printf(" aN = %2d ",aN);}
    if(_DEBUG>=1){printf(" aB = %2d ",aB);}

    //    M : Coup qui rend un pion mangeable
    int mB = 0;
    int mN = 0;
    if(color == _BLANC){
        if((implt[toX-1][toY-1] == _NOIR)||(implt[toX+1][toY-1] == _NOIR)){mB = 1;}
    }else{
        if((implt[toX-1][toY+1] == _BLANC)||(implt[toX+1][toY+1] == _BLANC)){mN = 1;}
    }
    if(_DEBUG>=1){printf(" mN = %2d ",mN);}
    if(_DEBUG>=1){printf(" mB = %2d ",mB);}

    //    W, Situation de victoire
    int wB = 0;
    int wN = 0;
    for(int c = 0;c<8;c++){
        if(arrAcc(implt,c,7) == _NOIR){wN=1;}
        if(arrAcc(implt,c,0) == _BLANC){wB=1;}
    }
    if(_DEBUG>=1){printf(" wN = %2d ",wN);}
    if(_DEBUG>=1){printf(" wB = %2d ",wB);}

    implt[frmX][frmY] = color;
    implt[toX][toY] = tmpclr;
    //    f(p) = gamma(P-P') + gamma2(D-D') + gamma2(B-B') + gamma2(I-I')  + gamma6(M-M') + 1000*(W-W')
    float poids;
    if(color == _BLANC){
        poids = Bgamma[0]*(pB-pN) + Bgamma[1]*(dB-dN) + Bgamma[2]*(bB-bN) + Bgamma[3]*(iB-iN)  + Bgamma[4]*(mB-mN) + Bgamma[5]*(aB-aN)/100 + Bgamma[6]*(wB-wN);
        if(_DEBUG>=1){printf(" poids = %f",poids);}
        return poids;
    }else{
        poids = Ngamma[0]*(pN-pB) + Ngamma[1]*(dN-dB) + Ngamma[2]*(bN-bB) + Ngamma[3]*(iN-iB)  + Ngamma[4]*(mN-mB) + Bgamma[5]*(aN-aB)/100 + Ngamma[6]*(wN-wB);
        if(_DEBUG>=1){printf(" poids = %f",poids);}
        return poids;
    }
}

int deplacement(possibillite dpl, int plat[][8],int color, int debug){


        if(plat[dpl.deX][dpl.deY]!=color){
            for(int c = 0;c<8;c++){
                printf("\n");
                for(int l = 0;l<8;l++){
                     printf("[%d]" , plat[c][l] );
                }
            }
            printf("\n!erreur!");
            if(color == _NOIR){
                if(debug>=1){printf("\nNoir  :");}
            }else{
                if(debug>=1){printf("\nBlanc :");}
            }
            if(debug>=1){printf("  Deplacement: %d %d > %d %d \n" , dpl.deX , dpl.deY , dpl.aX , dpl.aY );}

            fflush(stdin);
            char c;
            if(debug>=2){scanf("%c",&c);}
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
        if(debug>=1){printf("\n");}
        for(int c = 0;c<8;c++){
            if(debug>=1){
                if(plt[c][l] == _BLANC){printf("[B]");}
                if(plt[c][l] == _NOIR){printf("[N]");}
                if(plt[c][l] == _VIDE){printf("[ ]");}
            }
        }
    }
}

void affScores(joueur population[],int taille,int gammaActivated){
    for(int i = 0;i < taille ; i++){
        printf("\n %3d : gen%d Win : %d  Loss : %d ",i,population[i].generation,population[i].nbVictoires,population[i].nbDefaites)  ;
        if(gammaActivated == 1||_DEBUG>=1){
            printf(" %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f ",population[i].gamma[0],population[i].gamma[1],population[i].gamma[2],population[i].gamma[3],population[i].gamma[4],population[i].gamma[5],population[i].gamma[6])  ;
        }
    }
}

int partie(joueur population[],int plateau[][8],int joueurBlanc,int joueurNoir,int debug){
    int winner = _VIDE;
            while(winner == _VIDE){
                //etablissement des actions possibles (Blanc) ===============================================================
                if(debug>=1){system("cls");}
                affPlt(plateau,debug);
                possibillite actionlistB[40];
                int actAdr = 0;
                for(int j = 0;j < 40 ; j++){actionlistB[j].aX = -1;actionlistB[j].aY = -1;actionlistB[j].deX = -1;actionlistB[j].deY = -1;actionlistB[j].poids = -10000000;}
                for(int c = 0;c<8;c++){
                    for(int l = 0;l<8;l++){
                        if((plateau[c][l] == _BLANC)&(l-1>=0)){
                            if(plateau[c][l-1] == _VIDE){
                                    actionlistB[actAdr].poids = poids(_BLANC,plateau,c,l,c,l-1,population[joueurBlanc].gamma,population[joueurNoir].gamma);
                                    actionlistB[actAdr].deX = c;
                                    actionlistB[actAdr].deY = l;
                                    actionlistB[actAdr].aX = c;
                                    actionlistB[actAdr].aY = l-1;
                                    actAdr++;
                            }
                            if(c-1>=0)if(plateau[c-1][l-1] == _NOIR){
                                    actionlistB[actAdr].poids = poids(_BLANC,plateau,c,l,c-1,l-1,population[joueurBlanc].gamma,population[joueurNoir].gamma);
                                    actionlistB[actAdr].deX = c;
                                    actionlistB[actAdr].deY = l;
                                    actionlistB[actAdr].aX = c-1;
                                    actionlistB[actAdr].aY = l-1;
                                    actAdr++;
                            }
                            if(c+1<=7)if(plateau[c+1][l-1] == _NOIR){
                                    actionlistB[actAdr].poids = poids(_BLANC,plateau,c,l,c+1,l-1,population[joueurBlanc].gamma,population[joueurNoir].gamma);
                                    actionlistB[actAdr].deX = c;
                                    actionlistB[actAdr].deY = l;
                                    actionlistB[actAdr].aX = c+1;
                                    actionlistB[actAdr].aY = l-1;
                                    actAdr++;
                            }
                        }
                    }
                    if((plateau[c][6] == _BLANC)&&(plateau[c][4] == _VIDE)){
                        actionlistB[actAdr].poids = poids(_BLANC,plateau,c,6,c,4,population[joueurBlanc].gamma,population[joueurNoir].gamma);
                        actionlistB[actAdr].deX = c;
                        actionlistB[actAdr].deY = 6;
                        actionlistB[actAdr].aX = c;
                        actionlistB[actAdr].aY = 4;
                        actAdr++;
                    }
                    if((plateau[c][7] == _BLANC)&&(plateau[c][5] == _VIDE)){
                        actionlistB[actAdr].poids = poids(_BLANC,plateau,c,7,c,5,population[joueurBlanc].gamma,population[joueurNoir].gamma);
                        actionlistB[actAdr].deX = c;
                        actionlistB[actAdr].deY = 7;
                        actionlistB[actAdr].aX = c;
                        actionlistB[actAdr].aY = 5;
                        actAdr++;
                    }
                }
                int seeker = 0;
                possibillite bestActionB;
                bestActionB.poids = -1000000;
                if(_DEBUG>=1){printf("\n");}
                while(actionlistB[seeker].aX!=-1){
                    if(_DEBUG>=1){printf("/");}
                    if(actionlistB[seeker].poids>bestActionB.poids){
                        bestActionB.poids = actionlistB[seeker].poids;
                        bestActionB.deX = actionlistB[seeker].deX;
                        bestActionB.deY = actionlistB[seeker].deY;
                        bestActionB.aX = actionlistB[seeker].aX;
                        bestActionB.aY = actionlistB[seeker].aY;
                    }
                    seeker ++;
                }
                if(debug>=1){printf("\n");}
                if(deplacement(bestActionB,plateau,_BLANC,debug) == 1){winner = 666;}
                char c;
                if(debug>=1){scanf("%c",&c);}


                //etablissement des actions possibles (Noir) =================================================================
                if(debug>=1){system("cls");}
                affPlt(plateau,debug);
                possibillite actionlistN[40];
                actAdr = 0;
                for(int j = 0;j < 40 ; j++){actionlistN[j].aX = -1;actionlistN[j].aY = -1;actionlistN[j].deX = -1;actionlistN[j].deY = -1;actionlistN[j].poids = -1000000;}
                for(int c = 0;c<8;c++){
                    for(int l = 0;l<8;l++){
                        if((plateau[c][l] == _NOIR)&(l+1<=7)){
                            if(plateau[c][l+1] == _VIDE){
                                    actionlistN[actAdr].poids = poids(_NOIR,plateau,c,l,c,l+1,population[joueurBlanc].gamma,population[joueurNoir].gamma);
                                    actionlistN[actAdr].deX = c;
                                    actionlistN[actAdr].deY = l;
                                    actionlistN[actAdr].aX = c;
                                    actionlistN[actAdr].aY = l+1;
                                    actAdr++;
                                }
                            if(c-1>0)if(plateau[c-1][l+1] == _BLANC){
                                actionlistN[actAdr].poids = poids(_NOIR,plateau,c,l,c-1,l+1,population[joueurBlanc].gamma,population[joueurNoir].gamma);
                                    actionlistN[actAdr].deX = c;
                                    actionlistN[actAdr].deY = l;
                                    actionlistN[actAdr].aX = c-1;
                                    actionlistN[actAdr].aY = l+1;
                                    actAdr++;
                                }
                            if(c+1<8)if(plateau[c+1][l+1] == _BLANC){
                                actionlistN[actAdr].poids = poids(_NOIR,plateau,c,l,c+1,l+1,population[joueurBlanc].gamma,population[joueurNoir].gamma);
                                    actionlistN[actAdr].deX = c;
                                    actionlistN[actAdr].deY = l;
                                    actionlistN[actAdr].aX = c+1;
                                    actionlistN[actAdr].aY = l+1;
                                    actAdr++;
                                }
                        }
                    }
                    if((plateau[c][0] == _NOIR)&&(plateau[c][2] == _VIDE)){
                        actionlistN[actAdr].poids = poids(_NOIR,plateau,c,0,c,2,population[joueurBlanc].gamma,population[joueurNoir].gamma);
                        actionlistN[actAdr].deX = c;
                        actionlistN[actAdr].deY = 0;
                        actionlistN[actAdr].aX = c;
                        actionlistN[actAdr].aY = 2;
                        actAdr++;
                    }
                    if((plateau[c][1] == _NOIR)&&(plateau[c][3] == _VIDE)){
                        actionlistN[actAdr].poids = poids(_NOIR,plateau,c,1,c,3,population[joueurBlanc].gamma,population[joueurNoir].gamma);
                        actionlistN[actAdr].deX = c;
                        actionlistN[actAdr].deY = 1;
                        actionlistN[actAdr].aX = c;
                        actionlistN[actAdr].aY = 3;
                        actAdr++;
                    }
                }
                seeker = 0;
                possibillite bestActionN;
                bestActionN.poids = -1000000;
                if(_DEBUG>=1){printf("\n");}
                while(actionlistN[seeker].aX!=-1){
                    if(_DEBUG>=1){printf("/");}
                    if(actionlistN[seeker].poids>bestActionN.poids){
                        bestActionN.poids = actionlistN[seeker].poids;
                        bestActionN.deX = actionlistN[seeker].deX;
                        bestActionN.deY = actionlistN[seeker].deY;
                        bestActionN.aX = actionlistN[seeker].aX;
                        bestActionN.aY = actionlistN[seeker].aY;
                    }
                    seeker++;
                }
                    if(debug>=1){printf("\n");}

                if(deplacement(bestActionN,plateau,_NOIR,debug) == 1){winner = 666;} //si erreur dans la déplacement, abandonner cette partie
                if(debug>=1){scanf("%c",&c);}



                for(int c = 0;c<8;c++){
                    if(plateau[c][0] == _BLANC){
                            winner=_BLANC;
                            population[joueurNoir].nbVictoires++;
                            population[joueurBlanc].nbDefaites++;
                    }
                }
                for(int c = 0;c<8;c++){
                    if(plateau[c][7] == _NOIR){
                            winner=_NOIR;
                            population[joueurBlanc].nbVictoires++;
                            population[joueurNoir].nbDefaites++;
                    }
                }
            }
    return winner;
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
            population[i].gamma[j] = 50-(rand()%100);
        }
        population[i].generation = 1;
        population[i].nbVictoires = 0;
        population[i].nbDefaites = 0;
    }
        affScores(population,_NBPOPULATION,1);
        printf("\nNombre d iteration par generation : ");
        scanf("%d",&nbIterations);
        printf("\nNombre de generation a calculer: ");
        scanf("%d",&nbGEN);
   //////////////////////////////////////////////////////////////////////////////////
    do{
        int plateau[8][8];
        int nbParties = 0;
        int partieParJoueur[_NBPOPULATION];
        for(int i = 0;i < _NBPOPULATION ; i++){
            partieParJoueur[i] = 0;
        }
        int joueurBlanc = -1;
        int joueurNoir = -1;
        for(int i = 0;i < nbIterations*_NBPOPULATION ; i++){

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
            //selection de deux joueurs
            joueurBlanc = ( joueurBlanc + 1 )%_NBPOPULATION;
            do{
                    joueurNoir = rand()%_NBPOPULATION;
            }while((joueurBlanc==joueurNoir));
            partieParJoueur[joueurBlanc]++;
            partieParJoueur[joueurNoir]++;
            /*      ====        Partie      ====        */
            system("cls");
            printf("---------------- Partie %d / %d / %d -----------------\n",nbParties+1,nbIterations*_NBPOPULATION,nbIterations*_NBPOPULATION*nbGEN);
            affScores(population,_NBPOPULATION,0);
            printf("\n-------- Joueur %d vs Joueur %d ------- (total: %d )  -----\n",joueurBlanc,joueurNoir,_NBPOPULATION);

            partie(population,plateau,joueurBlanc,joueurNoir,debug);


            /*      ====       Fin d'une Partie      ====        */
        nbParties++;
        }
        system("cls");
        printf("--------------------------Processus---------------------\n");
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
                printf("\n %d : gen%d Win : %d  Loss : %d ",k,population[k].generation,population[k].nbVictoires,population[k].nbDefaites)  ;
                printf(" %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f ",population[k].gamma[0],population[k].gamma[1],population[k].gamma[2],population[k].gamma[3],population[k].gamma[4],population[k].gamma[5],population[k].gamma[6])  ;

        }
        printf("\n----------------------#  Mutations   #-------------------\n");
        // On fait mutter les descendants   #deltagamma
        for(int i = 0 ; i < _NBPOPULATION ; i++){
            for( int j=0; j<6; j++ ){
                population[i].gamma[j] += (5-(rand()%100))*deltagamma/100;
            }
        }
        deltagamma *= 0.99;
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

