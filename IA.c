#include "defs.h"
#define _DEBUG 0


void listActions(possibillite actionlist[] , int color ,Joueur population[], int joueurNoir , int joueurBlanc , Square plateau[][8]){
    int actAdr = 0;
    int colorDir;
    int oppcolor;
    if(color == _BLANC){colorDir=-1;}else{colorDir=1;}
    if(color == _BLANC){oppcolor=_NOIR;}else{oppcolor=_BLANC;}

    for(int j = 0;j < 40 ; j++){actionlist[j].aX = -1;actionlist[j].aY = -1;actionlist[j].deX = -1;actionlist[j].deY = -1;actionlist[j].poids = -10000000;}
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            if(plateau[c][l].pawn == color){
                if(plateau[c][l+colorDir].pawn == _VIDE){
                    actionlist[actAdr].poids = poids(color,plateau,c,l,c,l+colorDir,&population[*joueurBlanc],&population[*joueurNoir]);
                    actionlist[actAdr].deX = c;
                    actionlist[actAdr].deY = l;
                    actionlist[actAdr].aX = c;
                    actionlist[actAdr].aY = l+colorDir;
                    actAdr++;
                }
                if((c-1>=0)&(l+colorDir>=0))if(plateau[c-1][l+colorDir].pawn == oppcolor){
                    actionlist[actAdr].poids = poids(color,plateau,c,l,c-1,l+colorDir,&population[*joueurBlanc],&population[*joueurNoir]);
                    actionlist[actAdr].deX = c;
                    actionlist[actAdr].deY = l;
                    actionlist[actAdr].aX = c-1;
                    actionlist[actAdr].aY = l+colorDir;
                    actAdr++;
                }
                if((c+1<8)&(l+colorDir>=0))if(plateau[c+1][l+colorDir].pawn == oppcolor){
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

int findBestAction(possibillite *bestAction,int color,Square plateau[][8],joueur population[],int *joueurNoir,int *joueurBlanc){
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
int deplacement(possibillite dpl, Square plat[][8],int color, int debug){

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

int deplacement(possibillite dpl, Square plat[][8],int color, int debug){

        if((plat[dpl.deX][dpl.deY].pawn!=color)||(plat[dpl.aX][dpl.aY].pawn==color)){
            /*for(int l = 0;l<8;l++){
                if(debug>=0){printf("\n");}
                for(int c = 0;c<8;c++){
                     if(debug>=0){printf("[%c]" , colorconverter(plat[c][l]));}
                }
            }
            printf("\n!erreur : (%c)deplacement %d %d (%c)> %d %d (%c) interdit , partie annule\n",
             colorconverter(color) ,
             dpl.deX , dpl.deY , colorconverter(plat[dpl.deX][dpl.deY].pawn) ,
             dpl.aX , dpl.aY , colorconverter(plat[dpl.aX][dpl.aY].pawn));

            fflush(stdin);
            char c;
            if(debug>=0){scanf("%c",&c);}*/
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

