#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"

#include "AnnoyingEnemy.h"
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
	//Actor* buttonJump;                                                              //REVISAR
	Actor* buttonPeck;

	//cuidado: tiene que añadirse en el Space: peta, si no
	Tile* cup; //elemento de final de nivel
	Space* space;
	int mapWidth;
	int mapHeight;
	list<Tile*> tiles;
	list<Tile*> backgroundTiles;

	Audio* audioBackground;
	Audio* audioChocoFound;
	Player* player;
	Background* background;   //RE VI SAR: CREO QUE YA NO SE USA
	
	list<AnnoyingEnemy*> enemies;

	list<FriendMoguri*> friends;

	bool controlContinue = false; //encargado de poner/quitar pausa
	bool controlPeck = false;  //cambiado space x picar suelo
	int controlMoveY = 0;
	int controlMoveX = 0;

	Actor* backgroundCollectibles;
	list<Chocography*> chocographies;     
	int collected = 0;
	Text* textCollected;

	//añadir cronometro al juego: Uint32 == 
	Uint32 getLevelTime(int level);
	Uint32 totalTime;    //¡¡ se asigna en método por nivel !!
	Uint32 initTime;
	Uint32 passedTime; //para que sea temporizador, no cronómetro
	Uint32 pauseTime;
	Uint32 waitingTime = 3000; //3 segundos de pausa si se toca al moguri, de momento
	bool activeTimer = true;
	bool temporalPause = false;
	Uint32 pauseInit = 0;

	/*bool para distinguir la pausa inicial(para que el temporizador no empiece hasta que desaparezca el mensaje
	de "cómo jugar"): */
	bool firstStart = true;

	Text* textMoguriPauseMessage; //para que el mguri anuncie que le ha ayudado y comience la pausa
	bool showMoguriPauseMessage = false;

	//para temporizador:
	Text* textTimer;
	void updateTimer();
	Actor* backgroundTimer;

	//para mostrar mensaje/pantalla de que se acabó en tiempo y, por tanto, la partida:
	bool showGameOver;

	//para mostrar mensaje/pantalla de que se se ganó la partida:
	bool showVictory;

	//para controlar posiciones y pistas sobre las chocografías:
	void updateChocographies();
	Text* textChocoHint;
	Uint32 hintStartTime = 0;  //para limitar que la pista se muestre solo un segundo
	bool showHotColdHint = false;

	Text* textChocoDistance;  //de momento nada, funciona fatal

	int totalChocos; //para mostrar cuántas hay que encontrar en el nivel

	//para controlar colisión con enemigo moelsto:
	bool isSlowed = false;
	int slowStartTime = 0;
	int slowDuration = 3000; //de momento, 3 segundos
	//variable para guardar la velocidad original, antes de ralentizar al chocobo
	float originalSpeed = 6.0f; //como runningSpeed: float valor 6 (runningSpeed)
	Text* textSlowMessage;

	//añado mensaje de que se ha encontrado una chocografía:
	Text* textChocoFoundMessage;
	Uint32 chocoFoundStartTime;
	bool showChocoFoundMessage = false;
	//control de layer entre niveles:
	bool showFinishedLevel = false;

};
