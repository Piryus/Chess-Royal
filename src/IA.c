#include "defs.h"

int arrAcc(int plt[][8],int x, int y){
    if( (x<8) & (x>=0) & (y<8) & (y>=0) ){
        return plt[x][y];
    }else{
        return _VIDE;
    }
}

void affPlt(int plt[][8]){
    for(int l = 0;l<8;l++){
        printf("\n");
        for(int c = 0;c<8;c++){
            if(plt[c][l] == _BLANC){printf("[B]");}
            if(plt[c][l] == _NOIR){printf("[N]");}
            if(plt[c][l] == _VIDE){printf("[ ]");}
        }
    }
}

float poids(int color,Square plt[][8],int frmX,int frmY,int toX,int toY,Joueur *jN){
    int implt[8][8];
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            implt[c][l] = plt[c][l].pawn;
        }
    }
    implt[frmX][frmY] = _VIDE;
    implt[toX][toY] = color;
    //affPlt(implt);
    //    P, nombre de pions
    int pB = 0;
    int pN = 0;
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            if(implt[c][l] == _BLANC){pB++;}
            if(implt[c][l] == _NOIR){pN++;}
        }
    }

    //    D, défendu par un allié
    int dB = 0;
    int dN = 0;
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            if(arrAcc(implt,c,l) == _BLANC){if(arrAcc(implt,c-1,l+1)==_BLANC||arrAcc(implt,c+1,l+1)==_BLANC){dB++;}}
            if(arrAcc(implt,c,l) == _NOIR){if(arrAcc(implt,c-1,l-1)==_NOIR||arrAcc(implt,c+1,l-1)==_NOIR){dN++;}}
        }
    }

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

    //    I, avancement : position du pion le plus avancé
    int iB = 0;
    int iN = 0;
    for(int l = 0;l<8;l++){
        for(int c = 0;c<8;c++){
            if((arrAcc(implt,c,l)==_BLANC)&(7-l>iB)){iB=7-l;}
            if((arrAcc(implt,c,l)==_NOIR)&(l>iN)){iN=l;}
        }
    }

    //    A : avancement total  (somme des avancements sur chaque colone)
    int aB = 0;
    int aN = 0;
    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            if(implt[c][l] == _BLANC){aB+=7-l;}
            if(implt[c][l] == _NOIR){aN+=l;}
        }
    }

    //    M : Coup qui rend un pion mangeable
    int mB = 0;
    int mN = 0;
    if(color == _BLANC){
        if((implt[toX-1][toY-1] == _NOIR)||(implt[toX+1][toY-1] == _NOIR)){mB = 1;}
    }else{
        if((implt[toX-1][toY+1] == _BLANC)||(implt[toX+1][toY+1] == _BLANC)){mN = 1;}
    }

    //    W, Situation de victoire
    int wB = 0;
    int wN = 0;
    for(int c = 0;c<8;c++){
        if(arrAcc(implt,c,7) == _NOIR){wN=1;}
        if(arrAcc(implt,c,0) == _BLANC){wB=1;}
    }

    float poids;
    poids = jN->gamma[0]*(pN-pB) + jN->gamma[1]*(dN-dB) + jN->gamma[2]*(bN-bB) + jN->gamma[3]*(iN-iB)  + jN->gamma[4]*(mN-mB) + jN->gamma[5]*(aN-aB) + jN->gamma[6]*(wN-wB);
    //printf(" %2.2f + %2.2f + %2.2f + %2.2f + %2.2f + %2.2f + %2.2f = %2.2f \n",jN->gamma[0]*(pN-pB),jN->gamma[1]*(dN-dB)/50,jN->gamma[2]*(bN-bB),jN->gamma[3]*(iN-iB)*5,jN->gamma[4]*(mN-mB),jN->gamma[5]*(aN-aB)/100,jN->gamma[6]*(wN-wB)*100,poids );
    return poids;

}

void listActions(Possibillite actionlist[] , Square plateau[][8], Joueur *jN){
    int actAdr = 0;

    for(int j = 0;j < 40 ; j++){actionlist[j].aX = -1;actionlist[j].aY = -1;actionlist[j].deX = -1;actionlist[j].deY = -1;actionlist[j].poids = -10000000;}

    for(int c = 0;c<8;c++){
        for(int l = 0;l<8;l++){
            if(plateau[c][l].pawn == _NOIR){
                if(plateau[c][l+1].pawn == _VIDE){
                    actionlist[actAdr].poids = poids(_NOIR,plateau,c,l,c,l+1,jN);
                    actionlist[actAdr].deX = c;
                    actionlist[actAdr].deY = l;
                    actionlist[actAdr].aX = c;
                    actionlist[actAdr].aY = l+1;
                    actAdr++;
                }
                if((c-1>=0)&(l+1>=0))if(plateau[c-1][l+1].pawn == _BLANC){
                    actionlist[actAdr].poids = poids(_NOIR,plateau,c,l,c-1,l+1,jN);
                    actionlist[actAdr].deX = c;
                    actionlist[actAdr].deY = l;
                    actionlist[actAdr].aX = c-1;
                    actionlist[actAdr].aY = l+1;
                    actAdr++;
                }
                if((c+1<8)&(l+1>=0))if(plateau[c+1][l+1].pawn == _BLANC){
                    actionlist[actAdr].poids = poids(_NOIR,plateau,c,l,c+1,l+1,jN);
                    actionlist[actAdr].deX = c;
                    actionlist[actAdr].deY = l;
                    actionlist[actAdr].aX = c+1;
                    actionlist[actAdr].aY = l+1;
                    actAdr++;
                }
            }
        }
        if((plateau[c][0].pawn == _NOIR)&(plateau[c][2].pawn == _VIDE)){
            actionlist[actAdr].poids = poids(_NOIR,plateau,c,0,c,2,jN);
            actionlist[actAdr].deX = c;
            actionlist[actAdr].deY = 0;
            actionlist[actAdr].aX = c;
            actionlist[actAdr].aY = 2;
            actAdr++;
        }
        if((plateau[c][1].pawn == _NOIR)&(plateau[c][3].pawn == _VIDE)){
            actionlist[actAdr].poids = poids(_NOIR,plateau,c,1,c,3,jN);
            actionlist[actAdr].deX = c;
            actionlist[actAdr].deY = 1;
            actionlist[actAdr].aX = c;
            actionlist[actAdr].aY = 3;
            actAdr++;
        }
    }
}

int findBestAction(Possibillite *bestAction , Square plateau[][8],Joueur *jN){
    Possibillite actionlist[50];

    listActions( actionlist , plateau ,jN);

    int seeker = 0;
    bestAction->poids = -1000000;
    int err = 1;

    while(actionlist[seeker].aX!=-1){
        if(actionlist[seeker].poids>(bestAction->poids)){
            err = 0;
            bestAction->poids = actionlist[seeker].poids;
            bestAction->deX = actionlist[seeker].deX;
            bestAction->deY = actionlist[seeker].deY;
            bestAction->aX = actionlist[seeker].aX;
            bestAction->aY = actionlist[seeker].aY;
        }
        seeker ++;
    }
    return err;
}

void deplacement(Possibillite dpl, Square plat[][8],int color){
    plat[dpl.deX][dpl.deY].pawn = _VIDE;
    plat[dpl.aX][dpl.aY].pawn = color;
}


