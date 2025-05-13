#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"

#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Tile.h"
#include "Pad.h"

#include "Audio.h"
#include "Space.h" // importar

#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include <list>

//ampliaciones:
#include "CollectibleItem.h"
#include "DestructibleTile.h"
#include "JumpableMonster.h"

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);

	void mouseToControls(SDL_Event event); // USO DE MOUSE

	//prueba gamepad: 
	void gamePadToControls(SDL_Event event); // USO DE GAMEPAD

	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	void calculateScroll();
	float scrollX;
	float scrollY;


	//botón en el que se mostrará el mensaje y pause
	Actor* message;
	bool pause;


	//HUD: info al usuario
	//Elementos de interfaz
	SDL_GameController* gamePad;  //para prueba gamepad

	Pad* pad;
	Actor* buttonJump;
	Actor* buttonShoot;
	Text* textPoints;

	//cuidado: tiene que añadirse en el Space: peta si no
	Tile* cup; // Elemento de final de nivel
	Space* space;
	int mapWidth;
	int mapHeight;
	list<Tile*> tiles;

	Audio* audioBackground;
	int points;
	int newEnemyTime = 0;
	Player* player;
	Background* background;
	Actor* backgroundPoints;
	list<Enemy*> enemies;
	list<Projectile*> projectiles;
	bool controlContinue = false; //encargado de poner/quitar pausa
	bool controlPeck = false;  //cambiado space x picar suelo
	int controlMoveY = 0;
	int controlMoveX = 0;

	Actor* backgroundCollectibles;
	list<CollectibleItem*> collectibleItems;
	int collected = 0;
	Text* textCollected;

	//ampliación tiles destructibles
	list<DestructibleTile*> destructibleTiles;

	//ampliación: elemento para salvar punto de reinicio partida
	Tile* salvar; 
	bool savedPoint = false;
	int savedX = 0;
	int savedY = 0;

	//ampliación enemigo "saltable"
	list<JumpableMonster*> jumpableMonsters;
};

