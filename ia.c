
typedef struct
{
    int idBoard[10][10];	// Tableau du jeu de 10 cases par 10. dim1=ligne dim2=colonne
    playerBox player[40];	// Tableau contenant des playerBox renseignant les infos sur les pieces alliés.
    unsigned int blueOut[11];	// Tableau de comptage des pièces bleues éliminées (indexées par la valeur de EPiece : EPbomb=0, ..., EPmarshal=10
} SIaState;

typedef struct
{
    int ligne; //ligne dans laquelle se trouve la piece
    int colonne;//colonne dans laquelle se trouve la piece
    int value; //valeur de la piece
    int booleanKnown; //Boolean vallant vrai si la valeur de cette piece a deja ete rEvele a l'ennemi
} playerBox;

typedef struct
{
    int ligne; //ligne dans laquelle se trouve la piece
    int colonne;//colonne dans laquelle se trouve la piece
    int value; //valeur supposee de la piece
    //possiblement value[12] pourcentage de proba pour chaque type de piece
    int booleanKnown; //Boolean vallant vrai si la valeur de cette piece a deja ete rEvele a l'ennemi
} ennemyBox;
