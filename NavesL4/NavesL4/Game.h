#pragma once

// Todas las librerias de SDL y otras utilidades
#include <iostream>
#include <string>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <map> 

// Valores generales
#define WIDTH 480
#define HEIGHT 320

#include "Layer.h"
//#include "GameOverLayer.h"  //esro me estaba provocando 600 errores de compilaci�n (ref cruzada)
//lo pongo en el .cpp tras a�adirlo como declaraci�n adelantada:
/* las declaraciones adelantadas sirven para decirle al compilador que la clase existe, sin tener q verla entera.
* evita errores de dependencias circulares, que eran mi caso, y reducen acoplamiento entre clases
* se usa para declarar punteros (como en este caso)pero evita a�adir archivos pesados como el .h
* (si se usan m�todos, entonces se necesita el #include del .h)
*/
class GameLayer;
class GameOverLayer;
class Layer;

class Game
{
public:
	Game();
	void scale();
	void loop();
	SDL_Texture* getTexture(string filename);
	map<string, SDL_Texture*> mapTextures; // map - cache
	TTF_Font* font;
	bool scaledToMax = false;
	float scaleLower = 1;
	SDL_Window* window; // ventana
	SDL_Renderer* renderer; // renderizador
	bool loopActive; // Juego activo
	
	//se a�ade capa men�:
	Layer* layer; /*Layer apuntar� a la que se quiera pintar en cada caso*/
	
	Layer* menuLayer;
	Layer* gameLayer;  //tira de clase base (usa m�todos base)

	int currentLevel = 0;
	int finalLevel = 2;

	//para que sporte diferentes mecanismos de control
	int input;
	int const inputKeyboard = 1;
	int const inputMouse = 2;
	int const inputGamePad = 3;

	int const stateMoving = 1;
	int const statePecking = 2;

	int const orientationRight = 1;
	int const orientationLeft = 2;
	int const orientationUp = 3;
	int const orientationDown = 4;

	//para acabar la partida cuando termine el temporizador:
	//string state = "playing";   //cambiado tras a�adir mejor una nueva layer de GameOver
	//refe a la layer de game over
	Layer* gameOverLayer;
	//�Cuidado! ya hab�a variable de tipo GameLayer*, solo q declarada como Layer* (pero instanciada como GameLayer())
	//GameLayer* gameLayer; //tambi�n, para poder reiniciar el juego desde el final de partida

};
