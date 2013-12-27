typedef struct
{
	int line; //ligne dans laquelle se trouve la piece
	int column;//colonne dans laquelle se trouve la piece
	int value[12]; //valeur supposee de la piece
	//possiblement value[12] pourcentage de proba pour chaque type de piece
	int booleanHasMoved; //Boolean vallant vrai si la piece s'est déja déplacé (potentiellement supprimable: probabilité drapeau/bombe non nulle par défaut, passé à 0 des premier déplacement)
		} ennemyBox;

		typedef struct
{
	int line; //ligne dans laquelle se trouve la piece
	int column;//colonne dans laquelle se trouve la piece
	int value; //valeur de la piece
	int booleanKnown; //Boolean vallant 1 si la valeur de cette piece a deja ete rEvele a l'ennemi
		} playerBox;




		typedef struct
{
	int idBoard[10][10];	// Tableau du jeu de 10 cases par 10. dim1=ligne dim2=colonne. Chaque pion est renseigne par un id, les lacs par -1 et les cases vides par 0
	playerBox pieceAlly[40];	// Tableau contenant des playerBox renseignant les infos sur les pieces alliés.
	ennemyBox pieceEnnemy[40];	// Tableau contenant des playerBox renseignant les infos sur les pieces ennemies.
} SIaState;

