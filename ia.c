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
    if(IaState->pieceEnnemy[idMoved].booleanHasMoved == 0){
        IaState->pieceEnnemy[idMoved].booleanHasMoved = 1;
       //Attributio d'une proba IaState->pieceEnnemy[idMoved].value =
    }
    printf("\nMouvement ennemi :\nid : %d\n[%d][%d] vers [%d][%d]\n",idMoved,ennemyMove.start.line,ennemyMove.start.col,ennemyMove.end.line,ennemyMove.end.col);

    IaState->pieceEnnemy[idMoved].line = ennemyMove.end.line;
    IaState->pieceEnnemy[idMoved].column = ennemyMove.end.col;
    printf("Position connue actualisee : [%d][%d]\n",IaState->pieceEnnemy[idMoved].line,IaState->pieceEnnemy[idMoved].column);

    //MAJ idBoard
    IaState->idBoard[ennemyMove.start.line][ennemyMove.start.col] = -1;
    IaState->idBoard[ennemyMove.end.line][ennemyMove.end.col] = idMoved;

    IaState->pieceEnnemy[idMoved].lastMove = ennemyMove;

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

//Ally gagne contre Ennemy
void allyWin(SPos posAlly, int idAlly, SPos posEnnemy, int idEnnemy){

    ennemyBox ennemyEmpty;

    IaState->pieceEnnemy[idEnnemy] = ennemyEmpty;
    IaState->idBoard[posEnnemy.line][posEnnemy.col] = idAlly;
    IaState->idBoard[posAlly.line][posAlly.col] = -1;
    IaState->pieceAlly[idAlly].line = posEnnemy.line;
    IaState->pieceAlly[idAlly].column = posEnnemy.col;
}

//Ennemy gagne contre Ally
void ennemyWin(SPos posAlly, int idAlly, SPos posEnnemy, int idEnnemy){

    playerBox allyEmpty;

    IaState->pieceAlly[idAlly] = allyEmpty;
    IaState->idBoard[posAlly.line][posAlly.col] = idEnnemy;
    IaState->idBoard[posEnnemy.line][posEnnemy.col] = -1;
    IaState->pieceEnnemy[idEnnemy].line = posAlly.line;
    IaState->pieceEnnemy[idEnnemy].column = posAlly.col;
}

//Ally perd contre Ennemy
void allyLoose(SPos posAlly, int idAlly){

    playerBox playerEmpty;

    IaState->pieceAlly[idAlly] = playerEmpty;
    IaState->idBoard[posAlly.line][posAlly.col] = -1;
}

//Ennemy perd contre Ally
void ennemyLoose(SPos posEnnemy, int idEnnemy){

    ennemyBox ennemyEmpty;

    IaState->pieceEnnemy[idEnnemy] = ennemyEmpty;
    IaState->idBoard[posEnnemy.line][posEnnemy.col] = -1;
}


void neutralisation(SPos posAlly, int idAlly, SPos posEnnemy, int idEnnemy){

    playerBox playerEmpty;
    ennemyBox ennemyEmpty;

    IaState->pieceEnnemy[idEnnemy] = ennemyEmpty;
    IaState->pieceAlly[idAlly] = playerEmpty;
    IaState->idBoard[posEnnemy.line][posEnnemy.col] = -1;
    IaState->idBoard[posAlly.line][posAlly.col] = -1;

}

//Prise en compte d'une attackResult lorsque l'attaquant est un allié
//Utilisation du booleen attackBoolean pour différencier le cas où l'ally attaque l'ennemy et inversement
// attackBoolean == 1 => ally attaque ennemy
void updateAttack(SPos posAlly, EPiece ally, SPos posEnnemy, EPiece ennemy,int attackBoolean){
    int idAlly = IaState->idBoard[posAlly.line][posAlly.col];
    int idEnnemy = IaState->idBoard[posEnnemy.line][posEnnemy.col];

    //Ally attaque Ennemy
    if(attackBoolean == 1){
        //Gestion des cas particuliers
        if(ennemy == EPbomb && ally != EPminer){
            allyLoose(posAlly, idAlly);
        }
        else if(ennemy == EPbomb && ally == EPminer){
            allyWin(posAlly, idAlly, posEnnemy, idEnnemy);
        }
        else if(ally == EPspy && ennemy == EPmarshal){
            allyWin(posAlly, idAlly, posEnnemy, idEnnemy);
        }
        else if(ally == ennemy){
            neutralisation(posAlly, idAlly, posEnnemy, idEnnemy);
        }
        else if(ally < ennemy){
            allyLoose(posAlly, idAlly);
        }
        else{
            allyWin(posAlly, idAlly, posEnnemy, idEnnemy);
        }
    }
    //Ennemy attaque Ally
    else{
        if(ally == EPbomb && ennemy != EPminer){
            ennemyLoose(posAlly, idAlly);
        }
        else if(ally == EPbomb && ennemy == EPminer){
            ennemyWin(posAlly, idAlly, posEnnemy, idEnnemy);
        }
        else if(ennemy == EPspy && ally == EPmarshal){
            ennemyWin(posAlly, idAlly, posEnnemy, idEnnemy);
        }
        else if(ally == ennemy){
            neutralisation(posAlly, idAlly, posEnnemy, idEnnemy);
        }
        else if(ennemy < ally){
            ennemyLoose(posAlly, idAlly);
        }
        else{
            ennemyWin(posAlly, idAlly, posEnnemy, idEnnemy);
        }
    }
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
    move1->end.line = 4;
    move1->end.col = 0;

    updateEnnemy(*move1);
    affichTab();

    SPos * posAlly;
    posAlly = malloc(sizeof(SPos));
    posAlly->line = 3; posAlly->col = 0;
    SPos * posEnnemy;
    posEnnemy = malloc(sizeof(SPos));
    posEnnemy->line = 4; posEnnemy->col = 0;
    EPiece ennemy = EPspy;
    EPiece ally = EPbomb;

    updateAttack(*posAlly, ally, *posEnnemy, ennemy, 1);
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


