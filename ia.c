//#include <skynet.h>
#include "ia.h"
#include "stratego.h"
#include <stdio.h>
#include <string.h>
static SIaState* IaState;




int main()
{
    IaState=malloc(sizeof(SIaState));
    EPiece boardInit[4][10]= {{0,0,2,0,0,0,1,2,0,2},{2,2,2,2,2,3,3,3,3,3},{4,4,4,4,5,5,5,5,6,6},{6,6,7,7,7,8,8,9,10,11}};
    StartGame(ECred,boardInit);
    affichTab();



}/*
void StartMatch();

void EndGame();
void EndMatch();*/

void affichTab()
{
    int i,j;
    printf("\n");
    for (i=0; i<10; i++)
    {
        for(j=0; j<10; j++)
        {
            printf(" %d ",IaState->idBoard[i][j]);

        }
        printf("\n");
    }
}
void StartGame(const EColor color,EPiece boardInit[4][10])
{




    int i,j;
    for (i=0; i<10; i++)
    {
        for(j=0; j<10; j++)
        {
            IaState->idBoard[i][j]=-1;

        }

    }
    IaState->idBoard[4][2]=-2;
    IaState->idBoard[4][3]=-2;
    IaState->idBoard[4][6]=-2;
    IaState->idBoard[4][7]=-2;
    IaState->idBoard[5][2]=-2;
    IaState->idBoard[5][3]=-2;
    IaState->idBoard[5][6]=-2;
    IaState->idBoard[5][7]=-2;

    for (i=0; i<40; i++)
    {

        //initialisation des pions ennemis
        IaState->idBoard[6+(i/10)][i%10]=i+40;
        IaState->pieceEnnemy[i].column=i%10;
        IaState->pieceEnnemy[i].line=6+(i/10);
        for (j=0; j<12; j++) IaState->pieceEnnemy[i].value[j]=1/12; //Attribution des probabilités que le pion ai cette valeur

    }
    getPlacement(boardInit);




}


void getPlacement(EPiece boardInit[4][10])
{
    int i,j,id=0;


    //(fonction selectionnant le placement  des pions à appeler ici
    for (id=0; id<40; id++)
        { IaState->pieceAlly[id].value=-1;
        }
    for (i=0; i<4; i++)
    {
        for (j=0; j<10; j++)
        {
            switch (boardInit[i][j])
            {

            case 0:
                id=0;//indice de la premiere bombe dans le tableau
                break;
            case 1:
                id=6;//indice du premier éclaireur
                break;
            case 2:
                id=7;
                break;
            case 3:
                id=15;
                break;
            case 4:
                id=20;
                break;
            case 5:
                id=24;
                break;
            case 6:
                id=28;
                break;
            case 7:
                id=32;
                break;
            case 8:
                id=35;
                break;
            case 9:
                id=37;
                break;
            case 10:
                id=38;
                break;
            case 11:
                id=39;
                break;

            case 12:
                break;
            }

            while (IaState->pieceAlly[id].value==boardInit[i][j])
            {

                id++;
            } //On passe*/

            IaState->pieceAlly[id].value=boardInit[i][j];
            IaState->pieceAlly[id].column=j;
            IaState->pieceAlly[id].line=i;
            IaState->pieceAlly[id].booleanKnown=0;
            IaState->idBoard[i][j]=id;
        }

    }

}


