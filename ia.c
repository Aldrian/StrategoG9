//#include <skynet.h>
#include "ia.h"
#include "stratego.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static SIaState* IaState;

void affichTab();

void getPlacement(EPiece [4][10]);

void StartGame(const EColor,EPiece [4][10]);

//Mise à jour du idBoard et pieceEnnemy avec le dernier coup adverse
void updateEnnemy(SMove ennemyMove){
    int idMoved = IaState->idBoard[ennemyMove.start.line][ennemyMove.start.col];

    if(idMoved < 40 || idMoved > 79)printf("L'élément n'est pas une pièce ennemie\n\n");
    if(IaState->idBoard[ennemyMove.end.line][ennemyMove.end.col] != -1)printf("La case d'arrivée n'est pas disponible\n\n");

    //MAJ de la ennemyBox
    IaState->pieceEnnemy[idMoved].booleanHasMoved = 1;
    printf("\nMouvement ennemi :\nid : %d\n[%d][%d] vers [%d][%d]\n",idMoved,ennemyMove.start.line,ennemyMove.start.col,ennemyMove.end.line,ennemyMove.end.col);

    IaState->pieceEnnemy[idMoved].line = ennemyMove.end.line;
    IaState->pieceEnnemy[idMoved].column = ennemyMove.end.col;
    printf("Position connue actualisee : [%d][%d]\n",IaState->pieceEnnemy[idMoved].line,IaState->pieceEnnemy[idMoved].column);

    //MAJ idBoard
    IaState->idBoard[ennemyMove.start.line][ennemyMove.start.col] = -1;
    IaState->idBoard[ennemyMove.end.line][ennemyMove.end.col] = idMoved;

}

//Mise a jour de l'idBoard et du pieceAlly avec le coup decide par l'ia
void updateAlly(SMove allyMove, int id){
    int idMoved = id;

    //MAJ de la allyBox
    printf("\nMouvement allie :\nid : %d\n[%d][%d] vers [%d][%d]\n",idMoved,allyMove.start.line,allyMove.start.col,allyMove.end.line,allyMove.end.col);

    IaState->pieceAlly[idMoved].line = allyMove.end.line;
    IaState->pieceAlly[idMoved].column = allyMove.end.col;
    printf("Position connue actualisee : [%d][%d]\n",IaState->pieceAlly[idMoved].line,IaState->pieceAlly[idMoved].column);

    //MAJ idBoard
    IaState->idBoard[allyMove.start.line][allyMove.start.col] = -1;
    IaState->idBoard[allyMove.end.line][allyMove.end.col] = idMoved;

}

int main()
{
    IaState=malloc(sizeof(SIaState));
    EPiece boardInit[4][10]= {{0,0,2,0,0,0,1,2,0,2},{2,2,2,2,2,3,3,3,3,3},{4,4,4,4,5,5,5,5,6,6},{6,6,7,7,7,8,8,9,10,11}};
    StartGame(ECred,boardInit);
    affichTab();

    SMove * move1;
    move1 = malloc(sizeof(SMove));
    move1->start.line = 6;
    move1->start.col = 0;
    move1->end.line = 5;
    move1->end.col = 0;

    updateEnnemy(*move1);
    affichTab();

    return 0;

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


