#include "stratego.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_ttf.h>

//**************************
// DECLARATION DES VARIABLES
//**************************
SGameState etat;
SBox plateau;
EColor joueurActuel;
SDL_Surface *ecran = NULL, *cases[10][10] = { NULL }, *texteCases[10][10] = { NULL };
SDL_Surface *prompt = NULL;
SDL_Surface *textePrompt = NULL;
TTF_Font *police = NULL;

int i, j, clic = 0;
int coordx = 0;
int coordy = 0;
SDL_Rect position;
SDL_Event event; // Cette variable servira plus tard à gérer les événements
int continuer = 1; // Idem
int quitte = 0;
int menu = 0; // Choix du premier menu : 1 si un joueur, 2 si deux
int changeCouleur = 0; // Pour faire alterner les couleurs du damier

SDL_Color couleurNoire = { 0, 0, 0 };


SGameState PlacerPion(EPiece piece);
int estDansZonePlacable(int coordx, int coordy);
int estDansZoneDamier(int coordx, int coordy);
void desssinerGrille();

int main(int argc, char *argv[])
{	
	
	
	//**************************
	// INITIALISATION DE LA FENETRE
	//**************************
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();


	SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);
	ecran = SDL_SetVideoMode(600, 660, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Stratego", NULL);
	

	

	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 80, 80, 180));


	//**************************
	// MENU
	//**************************
	SDL_Surface *titre = NULL;
	police = TTF_OpenFont("calibri.ttf", 45);
	titre = TTF_RenderText_Blended(police, "Stratego", couleurNoire);
	position.x = 220;
	position.y = 150;
	SDL_BlitSurface(titre, NULL, ecran, &position); /* Blit du texte */

	SDL_Surface *menu1 = NULL;
	police = TTF_OpenFont("calibri.ttf", 35);
	menu1 = TTF_RenderText_Blended(police, "Un joueur", couleurNoire);
	position.x = 225;
	position.y = 250;
	SDL_BlitSurface(menu1, NULL, ecran, &position); /* Blit du texte */	

	SDL_Surface *menu2 = NULL;
	police = TTF_OpenFont("calibri.ttf", 35);
	menu2 = TTF_RenderText_Blended(police, "Deux joueurs", couleurNoire);
	position.x = 205;
	position.y = 350;
	SDL_BlitSurface(menu2, NULL, ecran, &position); /* Blit du texte */

	SDL_Flip(ecran); // Mise à jour de l'écran


	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
			// Clic souris
		case SDL_MOUSEBUTTONUP:
			//Récupérer les coordonées
			if (estDansDamier(event.button.x, event.button.y)==1)
			{
				coordx = event.button.x;
				coordy = event.button.y;

				// Savoir quel menu on a cliqué
				
				if (coordx >= 225 && coordx <= 355 && coordy >= 250 && coordy <= 300) menu = 1;
				if (coordx >= 205 && coordx <= 455 && coordy >= 350 && coordy <= 400) menu = 2;

				if (menu == 1 || menu == 2) continuer = 0;
			}
			break;
			// Appui sur la croix de la fenêtre
		case SDL_QUIT:
			continuer = 0;
			quitte = 1;
			break;
		}
	}

	if (quitte == 1){ SDL_Quit(); return EXIT_SUCCESS; }

	SDL_FreeSurface(titre);
	SDL_FreeSurface(menu1);
	SDL_FreeSurface(menu2);

	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 220, 80, 80));


	if (menu == 2)
	{
		//**************************
		// JEU A DEUX JOUEURS 
		//**************************
	

	// Allocation de la surface
	for (i = 0; i < 10; i++)
	for (j = 0; j < 10; j++)
		cases[i][j] = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 60, 32, 0, 0, 0, 0);;

	prompt = SDL_CreateRGBSurface(SDL_HWSURFACE, 600, 60, 32, 0, 0, 0, 0);;


	position.x = 0; // Les coordonnées de la première surface seront (0, 0)
	position.y = 0;

	// dessin de la grille
	
	for (j = 0; j < 10; j++)
	{
		for (i = 0; i < 10; i++)
		{
			if (changeCouleur == 0)
			{
				SDL_FillRect(cases[i][j], NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); // Remplissage de la surface avec du blanc			
				SDL_BlitSurface(cases[i][j], NULL, ecran, &position); // Collage de la surface sur l'écran
				changeCouleur = 1;
			}
			else
			{
				SDL_FillRect(cases[i][j], NULL, SDL_MapRGB(ecran->format, 80, 80, 80)); // Remplissage de la surface avec du gris		
				SDL_BlitSurface(cases[i][j], NULL, ecran, &position); // Collage de la surface sur l'écran
				changeCouleur = 0;
			}
			
			position.x = position.x + 60; // Mise à jour des coordonées abscisse
		}
		position.x = 0;
		if (changeCouleur == 0) 
		{
			changeCouleur = 1;
		} 
		else
		{
			changeCouleur = 0;
		}
		position.y = position.y + 60; // Mise à jour des coordonées ordonnée
	}
	
	// Coloration du bas de la grille pour montrer la surface où on peut poser le drapeau
	position.x = 0; // Les coordonnées de la première surface seront (0, 0)
	position.y = 360;
	changeCouleur = 0;	
	
	for (j = 0; j < 10; j++)
	{
		for (i = 0; i < 10; i++)
		{
			if (changeCouleur == 0)
			{
				SDL_FillRect(cases[i][j], NULL, SDL_MapRGB(ecran->format, 205, 234, 248)); // Remplissage de la surface avec du bleu clair		
				SDL_BlitSurface(cases[i][j], NULL, ecran, &position); // Collage de la surface sur l'écran
				changeCouleur = 1;
			}
			else
			{
				SDL_FillRect(cases[i][j], NULL, SDL_MapRGB(ecran->format, 14, 70, 97)); // Remplissage de la surface avec du bleu foncé	
				SDL_BlitSurface(cases[i][j], NULL, ecran, &position); // Collage de la surface sur l'écran
				changeCouleur = 0;
			}

			position.x = position.x + 60; // Mise à jour des coordonées abscisse
		}

		position.x = 0;
		if (changeCouleur == 0)
		{
			changeCouleur = 1;
		}
		else
		{
			changeCouleur = 0;
		}
		position.y = position.y + 60; // Mise à jour des coordonées ordonnée
	}

	
	

	// Mise en place de la zone de texte

	SDL_FillRect(prompt, NULL, SDL_MapRGB(ecran->format, 200, 200, 200)); // Remplissage de la surface avec du gris clair
	position.x = 0;
	position.y = 600;
	SDL_BlitSurface(prompt, NULL, ecran, &position); // Collage de la surface sur l'écran
	
	SDL_Flip(ecran); // Mise à jour de l'écran

	joueurActuel = ECblue;
	
	etat = PlacerPion(EPflag, etat);
	etat = PlacerPion(EPbomb, etat);
	etat = PlacerPion(EPspy, etat);
	etat = PlacerPion(EPscout, etat);
	etat = PlacerPion(EPminer, etat);
	etat = PlacerPion(EPsergeant, etat);
	etat = PlacerPion(EPlieutenant, etat);
	etat = PlacerPion(EPcaptain, etat);
	etat = PlacerPion(EPmajor, etat);
	etat = PlacerPion(EPcolonel, etat);
	etat = PlacerPion(EPgeneral, etat);
	etat = PlacerPion(EPmarshal, etat);


	// Coloration du bas de la grille pour montrer la surface où on peut poser le drapeau
	position.x = 0; // Les coordonnées de la première surface seront (0, 0)
	position.y = 360;
	changeCouleur = 0;

	for (j = 0; j < 10; j++)
	{
		for (i = 0; i < 10; i++)
		{
			if (changeCouleur == 0)
			{
				SDL_FillRect(cases[i][j], NULL, SDL_MapRGB(ecran->format, 248, 205, 234)); // Remplissage de la surface avec du bleu clair		
				SDL_BlitSurface(cases[i][j], NULL, ecran, &position); // Collage de la surface sur l'écran
				changeCouleur = 1;
			}
			else
			{
				SDL_FillRect(cases[i][j], NULL, SDL_MapRGB(ecran->format, 97, 14, 70)); // Remplissage de la surface avec du bleu foncé	
				SDL_BlitSurface(cases[i][j], NULL, ecran, &position); // Collage de la surface sur l'écran
				changeCouleur = 0;
			}

			position.x = position.x + 60; // Mise à jour des coordonées abscisse
		}

		position.x = 0;
		if (changeCouleur == 0)
		{
			changeCouleur = 1;
		}
		else
		{
			changeCouleur = 0;
		}
		position.y = position.y + 60; // Mise à jour des coordonées ordonnée
	}
	

	

	// Mise en place de la zone de texte

	SDL_FillRect(prompt, NULL, SDL_MapRGB(ecran->format, 200, 200, 200)); // Remplissage de la surface avec du gris clair
	position.x = 0;
	position.y = 600;
	SDL_BlitSurface(prompt, NULL, ecran, &position); // Collage de la surface sur l'écran

	SDL_Flip(ecran); // Mise à jour de l'écran


	joueurActuel = ECred;

	etat = PlacerPion(EPflag, etat);
	etat = PlacerPion(EPbomb, etat);
	etat = PlacerPion(EPspy, etat);
	etat = PlacerPion(EPscout, etat);
	etat = PlacerPion(EPminer, etat);
	etat = PlacerPion(EPsergeant, etat);
	etat = PlacerPion(EPlieutenant, etat);
	etat = PlacerPion(EPcaptain, etat);
	etat = PlacerPion(EPmajor, etat);
	etat = PlacerPion(EPcolonel, etat);
	etat = PlacerPion(EPgeneral, etat);
	etat = PlacerPion(EPmarshal, etat);

	desssinerGrille();

	// Boucle pour que le jeu ne se coupe pas
	
	continuer = 1;
	while (continuer)
	{
		clic = 0;
		SDL_WaitEvent(&event);

		switch (event.type)
		{

			// Clic souris
		case SDL_MOUSEBUTTONUP:
			//Récupérer les coordonées
			if (estDansZonePlacable(event.button.x, event.button.y))
			{
				coordx = event.button.x;
				coordy = event.button.y;

				// Savoir dans quelle case on a cliqué
				coordx = coordx / 60;
				coordy = coordy / 60;

				clic = 1;
			}
			break;

			// Appui sur la croix de la fenêtre
		case SDL_QUIT:
			continuer = 0;
			break;

			// Appui sur une touche
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE: /* Appui sur la touche Echap, on arrête le programme */
				continuer = 0;
				break;
			}
			break;


		}


	}


	//**************************
	// FERMETURE DU PROGRAMME
	//**************************

	// Libération de la mémoire
	for (i = 0; i < 5; i++) 
		for (j = 0; j < 10; j++)
		SDL_FreeSurface(cases[i][j]);
	SDL_Quit();

	return EXIT_SUCCESS;
	}
}

int estDansDamier(int coordx, int coordy)
{
	if (coordx >= 0 && coordx <= 600 && coordy >= 0 && coordy <= 600) return 1;	else return 0;
}


int estDansZonePlacable(int coordx, int coordy)
{
	if (coordx >= 0 && coordx <= 600 && coordy >= 360 && coordy <= 600) return 1;	else return 0;
}

SGameState PlacerPion(EPiece piece, SGameState etat)
{
	SGameState etatActuel = etat;
	int enPlace[10][10] = {0}; // Matrice pour ne pas placer les pions au même endroit
	char* pion ="";
	char* valeur="";
	char chaine[256]="";
	int nombre;
	int nombreRestant;
	switch (piece)
	{
	case EPbomb: 
		pion = "bombe"; 
		valeur = "B"; 
		nombre = 6; 
		nombreRestant = nombre;
		break;
	case EPspy: 
		pion = "espion"; 
		valeur = "1"; 
		nombre = 1; 
		nombreRestant = nombre;
		break;
	case EPscout: 
		pion = "éclaireur";
		valeur = "2";
		nombre = 8; 
		nombreRestant = nombre;
		break;
	case EPminer: 
		pion = "démineur";
		valeur = "3"; 
		nombre = 5;
		nombreRestant = nombre;
		break;
	case EPsergeant:
		pion = "sergent"; 
		valeur = "4";
		nombre = 4;
		nombreRestant = nombre;
		break;
	case EPlieutenant: 
		pion = "lieutenant";
		valeur = "5";
		nombre = 4;
		nombreRestant = nombre;
		break;
	case EPcaptain:
		pion = "capitaine"; 
		valeur = "6";
		nombre = 4;
		nombreRestant = nombre;
		break;
	case EPmajor:
		pion = "commandant";
		valeur = "7";
		nombre = 3;
		nombreRestant = nombre;
		break;
	case EPcolonel: 
		pion = "colonel";
		valeur = "8"; 
		nombre = 2;
		nombreRestant = nombre;
		break;
	case EPgeneral:
		pion = "général"; 
		valeur = "9";
		nombre = 1; 
		nombreRestant = nombre;
		break;
	case EPmarshal:
		pion = "maréchal";
		valeur = "10"; 
		nombre = 1;
		nombreRestant = nombre;
		break;
	case EPflag: 
		pion = "drapeau";
		valeur = "F";	
		nombre = 1; 
		nombreRestant = nombre;
		break;
	case EPnone:
		pion = ""; 
		valeur = "";
		nombre = 1; 
		nombreRestant = nombre;
		break;
	} 
	police = TTF_OpenFont("calibri.ttf", 30);
	sprintf(chaine, "Joueur 1 : placer %s (%s) / %d restant ", pion, valeur, nombreRestant);
	textePrompt = TTF_RenderText_Blended(police, chaine, couleurNoire);
	position.x = 15;
	position.y = 615;
	SDL_BlitSurface(textePrompt, NULL, ecran, &position); /* Blit du texte */
	SDL_Flip(ecran);


	do
	{
		position.x = 0;
		position.y = 600;
		SDL_BlitSurface(prompt, NULL, ecran, &position); // Collage de la surface sur l'écran
		police = TTF_OpenFont("calibri.ttf", 30);
		sprintf(chaine, "Joueur 1 : placer %s (%s) / %d restant ", pion, valeur, nombreRestant);
		textePrompt = TTF_RenderText_Blended(police, chaine, couleurNoire);
		position.x = 15;
		position.y = 615;
		SDL_BlitSurface(textePrompt, NULL, ecran, &position); /* Blit du texte */
		SDL_Flip(ecran);

		continuer = 1;
		while (continuer)
		{
			clic = 0;
			SDL_WaitEvent(&event);

			switch (event.type)
			{

				// Clic souris
			case SDL_MOUSEBUTTONUP:
				//Récupérer les coordonées
				if (estDansZonePlacable(event.button.x, event.button.y))
				{
					
					coordx = event.button.x;
					coordy = event.button.y;

					// Savoir dans quelle case on a cliqué
					coordx = coordx / 60;
					coordy = coordy / 60;
					if (enPlace[coordx][coordy] == 0)
					{
						clic = 1;
					}
					
				}
				break;

				// Appui sur la croix de la fenêtre
			case SDL_QUIT:
				continuer = 0;
				quitte = 1;
				break;

		}
			if (quitte == 1){ SDL_Quit();}

			/* On place le pion */
			if (clic == 1)
			{
				// Mise à jour du gamestate
				if (joueurActuel == ECblue) etatActuel.board[coordx][coordy].piece = piece;
				else etatActuel.board[9-coordx][9-coordy].piece = piece; // on fait saisir les pièces du joueur rouge en face
				
				etatActuel.board[coordx][coordy].content = joueurActuel;
				
				enPlace[coordx][coordy] = 1;
				position.x = coordx * 60 + 20; // Mise à jour des coordonées pour blitter la surface
				position.y = coordy * 60 + 10;
				SDL_Surface *pion = NULL;
				police = TTF_OpenFont("calibri.ttf", 35);
				pion = TTF_RenderText_Blended(police, valeur, couleurNoire);
				SDL_BlitSurface(pion, NULL, ecran, &position); // On blitte de nouveau la surface		
				/* On met à jour l'affichage */
				SDL_Flip(ecran);
				continuer = 0;
			}
			

		}
		nombreRestant--;
	} while (nombreRestant!=0);

	
	return etatActuel;
}

void desssinerGrille()
{
	// dessin de la grille
	position.x = 0;
	position.y = 0;
	for (j = 0; j < 10; j++)
	{
		for (i = 0; i < 10; i++)
		{
			if (changeCouleur == 0)
			{
				SDL_FillRect(cases[i][j], NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); // Remplissage de la surface avec du blanc			
				SDL_BlitSurface(cases[i][j], NULL, ecran, &position); // Collage de la surface sur l'écran
				changeCouleur = 1;
			}
			else
			{
				SDL_FillRect(cases[i][j], NULL, SDL_MapRGB(ecran->format, 80, 80, 80)); // Remplissage de la surface avec du gris		
				SDL_BlitSurface(cases[i][j], NULL, ecran, &position); // Collage de la surface sur l'écran
				changeCouleur = 0;
			}

			position.x = position.x + 60; // Mise à jour des coordonées abscisse
		}
		position.x = 0;
		if (changeCouleur == 0)
		{
			changeCouleur = 1;
		}
		else
		{
			changeCouleur = 0;
		}
		position.y = position.y + 60; // Mise à jour des coordonées ordonnée
	}

	police = TTF_OpenFont("calibri.ttf", 35);
	char chaine[3]="";
	// Dessiner le texte des cases
	if (joueurActuel == ECblue)
	{
		// on ne tourne pas le plateau
		for (j = 0; j < 10; j++)
		{
			for (i = 0; i < 10; i++)
			{
				position.x = i * 60 + 20; 
				position.y = j * 60 + 10;
				switch (etat.board[i][j].piece)
				{
				case EPflag:
					texteCases[i][j] = TTF_RenderText_Blended(police, "F", couleurNoire);
					break;
				case EPbomb:
					texteCases[i][j] = TTF_RenderText_Blended(police, "B", couleurNoire);
					break;
				default:
					sprintf(chaine, "%d", etat.board[i][j].piece);
					texteCases[i][j] = TTF_RenderText_Blended(police, chaine, couleurNoire);
					break;
				}

				SDL_BlitSurface(texteCases[i][j], NULL, ecran, &position); // On blitte de nouveau la surface

			}
		}
	}
	else

	{
		// on ne tourne pas le plateau
		for (j = 0; j < 10; j++)
		{
			for (i = 0; i < 10; i++)
			{
				position.x = i * 60 + 20;
				position.y = j * 60 + 10;
				switch (etat.board[i][j].piece)
				{
				case EPflag:
					texteCases[i][j] = TTF_RenderText_Blended(police, "F", couleurNoire);
					break;
				case EPbomb:
					texteCases[i][j] = TTF_RenderText_Blended(police, "B", couleurNoire);
					break;
				default:
					sprintf(chaine, "%d", etat.board[i][j].piece);
					texteCases[i][j] = TTF_RenderText_Blended(police, chaine, couleurNoire);
					break;
				}

				SDL_BlitSurface(texteCases[i][j], NULL, ecran, &position); // On blitte de nouveau la surface

			}
		}
	}
	SDL_Flip(ecran);
}