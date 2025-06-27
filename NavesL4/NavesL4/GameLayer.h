#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"

#include "Enemy.h"
#include "Text.h"
#include "Tile.h"
#include "Chocography.h"
#include "FriendMoguri.h"

#include "Audio.h"
#include "Space.h" // importar

#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include <list>
#include "Pad.h"

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

	void loadMap(string name);
	void loadMapObject(char character, float x, float y);

	//cambio scroll: para moverse x el mapa (x grande que pantalla, pero con fondo fijo)
	void limitPlayerPosition();
	void calculateScroll();
	float scrollX;
	float scrollY;
	const int BACKGROUND_WIDTH = 600;
	const int BACKGROUND_HEIGHT = 400;
	const int WINDOW_WIDTH = 480;
	const int WINDOW_HEIGHT = 320;


	//botón en el que se mostrará el mensaje y pause
	Actor* message;
	bool pause;


	//HUD: info al usuario
	//Elementos de interfaz
	Pad* pad;
	Actor* buttonJump;
	Actor* buttonShoot;
	Text* textPoints;

	//cuidado: tiene que añadirse en el Space: peta, si no
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

	list<FriendMoguri*> friends;

	bool controlContinue = false; //encargado de poner/quitar pausa
	bool controlPeck = false;  //cambiado space x picar suelo
	int controlMoveY = 0;
	int controlMoveX = 0;

	Actor* backgroundCollectibles;
	list<Chocography*> chocographies;          //revisar CHOCOGRAFIAS
	int collected = 0;
	Text* textCollected;

	//ampliación: elemento para salvar punto de reinicio partida
	Tile* salvar; 
	bool savedPoint = false;
	int savedX = 0;
	int savedY = 0;

	//añadir cronometro al juego: Uint32 == 
	Uint32 totalTime = 120000;
	Uint32 initTime;
	Uint32 passedTime; //para que sea temporizador, no cronómetro
	Uint32 pauseTime;
	Uint32 waitingTime = 3000; //3 segundos de pausa si se toca al moguri, de momento
	bool activeTimer = true;
	bool temporalPause = false;
	Uint32 pauseInit = 0;

	Text* textPauseMessage; //para que el mguri anuncie que le ha ayudado y comience la pausa
	bool showPauseMessage = false;
	Text* textTimer;

	void updateTimer();

	//para mostrar mensaje/pantalla de que se acabó en tiempo y, por tanto, la partida:
	bool showGameOver;

};
