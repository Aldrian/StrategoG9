
typedef struct
{
	int idBoard[10][10];	// Tableau du jeu de 10 cases par 10. dim1=ligne dim2=colonne. Chaque pion est renseigne par un id, les lacs par -1 et les cases vides par 0
	playerBox player[40];	// Tableau contenant des playerBox renseignant les infos sur les pieces alliés.
	ennemyBox ennemy[40];	// Tableau contenant des playerBox renseignant les infos sur les pieces ennemies.
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
	int booleanHasMoved; //Boolean vallant vrai si la piece s'est déja déplacé (potentiellement supprimable: probabilité drapeau/bombe non nulle par défaut, passé à 0 des premier déplacement)
		} ennemyBox;
