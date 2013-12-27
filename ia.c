//#include <skynet.h>
#include "ia.h"
#include "stratego.h"
#include <stdio.h>
#include <string.h>


SIaState * initIaState(EPiece boardInit[4][10])
{
    int indexId[12];
    SIaState* IaState = malloc(sizeof(SIaState));
    int i,j;
    for (i=0; i<10; i++){
        for(j=0; j<10; j++){
           IaState->idBoard[i][j]=0;

        }

     }
     IaState->idBoard[4][2]=-1;
     IaState->idBoard[4][3]=-1;
     IaState->idBoard[4][6]=-1;
     IaState->idBoard[4][7]=-1;
     IaState->idBoard[5][2]=-1;
     IaState->idBoard[5][3]=-1;
     IaState->idBoard[5][6]=-1;
     IaState->idBoard[5][7]=-1;

    for (i=0; i<40; i++)
    {

        //initialisation des pions ennemis
        IaState->idBoard[i/10][i%10]=i+41;
        IaState->pieceEnnemy[i].column=i%10;
        IaState->pieceEnnemy[i].line=i/10;
        for (j=0; j<12; j++) IaState->pieceEnnemy[i].value[j]=1/12; //Attribution des probabilités que le pion ai cette valeur


        //initialisation des pions ennemis
        int allyColumn= 9-i%10;
        int allyLine= 9-i/10;
        j=0;
        printf("valeur piece: %d",boardInit[i/10][i%10]);
        switch (boardInit[i/10][i%10])
        {

        case 0:
            j=1;//indice de la premiere bombe dans le tableau
            break;
        case 1:
            j=7;
            break;
        case 2:
            j=8;
            break;
        case 3:
            j=16;
            break;
        case 4:
            j=21;
            break;
        case 5:
            j=25;
            break;
        case 6:
            j=29;
            break;
        case 7:
            j=33;
            break;
        case 8:
            j=36;
            break;
        case 9:
            j=38;
            break;
        case 10:
            j=39;
            break;
        case 11:
            j=40;
            break;
        }
        printf("indice : %d    coordonees %d-%d  j :%d: \n",i, i/10,i%10,j);
        printf("piece: %d indice:%d \n",boardInit[i/10][i%10],j);
        while (IaState->pieceAlly[j].value==boardInit[i/10][i%10]){
            printf("piece: %d indice:%d \n",boardInit[i/10][i%10],j);
            j++;
        } //On passe
        IaState->pieceAlly[j].value=boardInit[allyLine][allyColumn];
        IaState->pieceAlly[j].column=allyColumn;
        IaState->pieceAlly[j].line=allyLine;
        IaState->pieceAlly[j].booleanKnown=0;
        IaState->idBoard[allyLine][allyColumn]=j;
        printf("position finale :%d\n\n\n",j);


    }
    return IaState;
}

/*int main(){

     EPiece boardInit[4][10]={{0,0,0,0,0,0,1,2,2,2},{2,2,2,2,2,3,3,3,3,3},{4,4,4,4,5,5,5,5,6,6},{6,6,7,7,7,8,8,9,10,11}};
     SIaState * IaState=initIaState(boardInit);
     int i,j;
     printf("\n");
     for (i=0; i<10; i++){
        for(j=0; j<10; j++){
            printf(" %d ",IaState->idBoard[i][j]);

        }
        printf("\n");
     }


};
*/
