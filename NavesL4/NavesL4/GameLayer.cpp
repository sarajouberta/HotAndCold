#include "GameLayer.h"
#include <algorithm> //para std::clamp en el control de scroll

GameLayer::GameLayer(Game* game) : Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	
	//para probar gamepad:
	gamePad = SDL_GameControllerOpen(0);

	/* Variables pausa/ message: Normalmente esto lo hacemos en el método init() , 
	pero en este caso nos viene mejor hacerlo en el constructor
	para evitar que cuando se reinicie el juego aparezca siempre con los mismos valores,
	nos aportará algo más de control*/
	pause = true;
	message = new Actor("res/mensaje_como_jugar.png", WIDTH * 0.5, HEIGHT * 0.5,
		WIDTH, HEIGHT, game);
	//message: apunta al “mensaje actual”, pero puede cambiarse en cualquier momento, instanciando otro actor
	init();
}


void GameLayer::init() {
	pad = new Pad(WIDTH * 0.15, HEIGHT * 0.80, game);
	//se crean los 2 botones a partir de actor xq son muy simples:
	buttonJump = new Actor("res/boton_salto.png", WIDTH * 0.9, HEIGHT * 0.55, 100, 100, game);
	buttonShoot = new Actor("res/boton_disparo.png", WIDTH * 0.75, HEIGHT * 0.83, 100, 100, game);

	space = new Space(0); //Pongo la gravedad en 0, pera retirar la gravedad vertical
	scrollX = 0;
	scrollY = 0;  //añadir scrollY para ampliación
	tiles.clear();

	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	points = 0;
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points);

	collected = 0;
	textCollected = new Text("hola", WIDTH * 0.67, HEIGHT * 0.04, game);
	textCollected->content = to_string(collected);

	
	background = new Background("res/fondo_hierba.png", WIDTH * 0.5, HEIGHT * 0.5, 0, game);  //CAMBIO A VELOCIDAD 0 PARA PROBAR ESTÁTICO
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);
	//ampliación: mostrar contador recoletables
	backgroundCollectibles = new Actor("res/icono_recolectable.png",
		WIDTH * 0.6, HEIGHT * 0.05, 25, 25, game);

	enemies.clear(); // Vaciar por si reiniciamos el juego
	projectiles.clear(); // Vaciar por si reiniciamos el juego
	collectibleItems.clear(); // Vaciar por si reiniciamos el juego
	destructibleTiles.clear(); // Vaciar por si reiniciamos el juego
	//Ampliación: monstruo "saltable"
	jumpableMonsters.clear(); 

	//c++: siempre hacer el cast explícito

	//cargar currentLevel, ya no hardcodeado
	loadMap("res/" + to_string(game->currentLevel) + ".txt");   //comentado para probar con un mapa de prueba !!!!!!!
	//loadMap("res/prueba.txt");
}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	int lineNums = 0;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			lineNums++;
			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
			}

			cout << character << endl;
		}
	}
	mapHeight = lineNums * 32;   //ampli scroll y
	streamFile.close();
}

void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
	case 'J': {
		JumpableMonster* jumpableMonster = new JumpableMonster (x, y, game);
		// modificación para empezar a contar desde el suelo.
		jumpableMonster->y = jumpableMonster->y - jumpableMonster->height / 2;
		jumpableMonsters.push_back(jumpableMonster);

		//línea clave: meter todo lo que hay en el mapa en espacio fuerzas físicas()
		space->addStaticActor(jumpableMonster);
		break;
	}
	case 'A': {
		salvar = new Tile("res/punto_salvado.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		salvar->y = salvar->y - salvar->height / 2;

		//línea clave: aunque no se mueva, meter todo lo que hay en el mapa en espacio fuerzas físicas()
		space->addDynamicActor(salvar);
		break;
	}
		case 'C': {
			cup = new Tile("res/copa.png", x, y, game);
			// modificación para empezar a contar desde el suelo.
			cup->y = cup->y - cup->height / 2;

			//línea clave: aunque no se mueva, meter todo lo que hay en el mapa en espacio fuerzas físicas()para q no se olvide al copy/paste)
			space->addDynamicActor(cup); // Realmente no hace falta
			break;
		}
		//ampliación: añadir coleccionables a espacio de fuerzas físicas:
		case 'R': {
			CollectibleItem* collectible = new CollectibleItem(x, y, game);
			// modificación para empezar a contar desde el suelo.
			collectible->y = collectible->y - collectible->height / 2;
			collectibleItems.push_back(collectible);
			space->addStaticActor(collectible);
			break;
		}
		//ampliación: añadir tiles destructibles a espacio de fuerzas físicas:
		case 'W': {
			DestructibleTile* destructibleTile = new DestructibleTile(x, y, game);
			// modificación para empezar a contar desde el suelo.
			destructibleTile->y = destructibleTile->y - destructibleTile->height / 2;
			destructibleTiles.push_back(destructibleTile);
			space->addStaticActor(destructibleTile);
			break;
		}
		case 'E': {
			Enemy* enemy = new Enemy(x, y, game);
			// modificación para empezar a contar desde el suelo.
			enemy->y = enemy->y - enemy->height / 2;
			enemies.push_back(enemy);
			space->addDynamicActor(enemy);
			break;
		}
		case '1': {
			player = new Player(x, y, game);
			if (savedPoint) {
				player->x = savedX;
				player->y = savedY;
			}
			// modificación para empezar a contar desde el suelo.
			player->y = player->y - player->height / 2;
			space->addDynamicActor(player);
			break;

		}
		case '#': {
			Tile* tile = new Tile("res/arbusto.png", x, y, game);
			// modificación para empezar a contar desde el suelo.
			tile->y = tile->y - tile->height / 2;
			tiles.push_back(tile);
			space->addStaticActor(tile);
			break;
		}
		case 'H': {
			Tile* tile = new Tile("res/hierba_chocografia.png", x, y, game);         //PRUEBAS TILES DE CAMUFLAJE PARA LA CHOCOGRAFÍA
			// modificación para empezar a contar desde el suelo.
			tile->y = tile->y - tile->height / 2;
			tiles.push_back(tile);
			space->addStaticActor(tile);
			break;
		}
		case 'G': {
			Tile* tile = new Tile("res/grass_tile.png", x, y, game);          //se camufla mejor la H
			// modificación para empezar a contar desde el suelo.
			tile->y = tile->y - tile->height / 2;
			tiles.push_back(tile);
			space->addStaticActor(tile);
			break;
		}
	}
}

/* NOTA CLASE: NO se asocian directamente a las teclas: paso previo: variables de control
* de esta forma, podemos añadir otro método de control(ratón, por ej)
* primero pensar qué se va a poder hacecr, lo las teclas, directammente, al hacer nuestro juego
* (controles del juego son los mismos, lo q cambia es el mecanismo de control )
*/
void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	//para soportar mecanismos de control dif: uno u otro, ahora
	while (SDL_PollEvent(&event)) {
		// Cambio automático de input
		if (event.type == SDL_KEYDOWN) {
			game->input = game->inputKeyboard;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			game->input = game->inputMouse;
		}
		// Procesar teclas
		if (game->input == game->inputKeyboard) {
			keysToControls(event);
		}
		if (game->input == game->inputMouse) {
			mouseToControls(event);
		}

	}
	//procesar controles
	if (controlContinue) {  //retira pausa del juego si lee controlContinue==true
		pause = false;
		controlContinue = false;
	}


	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		//player->jump(); //Desactivo el salto en el eje Y
		player->moveY(-1); //Reactivo el movimiento vertical hacia arriba
	}
	else {
		player->moveY(0); //Reactivo movimiento vertical hacia abajo
	}
}

void GameLayer::update() {
	/* Hemos declarado que el juego se inicia con pause=true, por lo que comienza pausado 
	  y con un mensaje de explicación.*/
	if (pause) {  //para q el juego se detenga si pausa
		return;
	}

	// Nivel superado: colisión especial: se acaba el juego
	//detectar cuando el player y la cup chocan
	if (cup->isOverlap(player)) {
		savedPoint = false;  //para que el siguiente nivel empiece sin punto de salvado
		game->currentLevel++; //sube de nivel (cuidado: son finitos)
		if (game->currentLevel > game->finalLevel) {
			game->currentLevel = 0;
		}
		//para mostrar mensaje cuando se supera nivel:
		message = new Actor("res/mensaje_ganar.png", WIDTH * 0.5, HEIGHT * 0.5,
			WIDTH, HEIGHT, game);
		pause = true;

		init();   //reinicia juego
		//también podría hacerse layer de que se acabó el juego (opcional)
	}

	//ampliación: segunda colisión especial: con punto de guardado
	if (salvar->isOverlap(player)) {
		savedPoint = true;
		savedX = player->x;
		savedY = player->y;
	}

	//para controlar los límites del jugador antes de actualizar su posición:
	limitPlayerPosition();

	space->update();
	background->update();
	player->update();

	//ampli: se añaden los elementos recolectables
	for (auto const& coll : collectibleItems) {
		coll->update();
	}

	for (auto const& enemy : enemies) {
		//enemy->update();                                        TEMPORALMENTE COMENTADO PARA PROBAR ANIMACIONES PLAYER
	}
	for (auto const& projectile : projectiles) {
		projectile->update();
	}

	/*
	// Colisiones
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			player->loseLife();
			if (player->lifes <= 0) {
				init();
				return;
			}
		}
	}

	// Colisiones , Enemy - Projectile , player - elementos recoletables, player - tile destructible, player-jumpableMonster
	
	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	list<CollectibleItem*> deleteCollectibles;  //ampli plataformas: el recolectables
	list<DestructibleTile*> deleteDestructibleTiles;  //ampli plataformas: tiles destructibles
	list<JumpableMonster*> deleteJumpableMonsters;  //ampli plataformas: tiles destructibles

	for (auto const& projectile : projectiles) {
		if (projectile->isInRender(scrollX) == false || projectile->vx == 0) {

			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}

	//ampliación: colisiones de player con elemento recolectable:
	for (auto const& collectible : collectibleItems) {
		if (player->isOverlap(collectible) && !collectible->isCollected()) {
			//objeto se añade al contador de recolectados			 
			collected++;  // aumentar contador elementos recolectados
			textCollected->content = to_string(collected);
			collectible->markAsCollected();
			return;
		}
	}

	//ampliación: colisiones de player con destructibleTile: 
	for (auto const& destructibleTile : destructibleTiles) {
		if (player->isOverlap(destructibleTile) && !destructibleTile->isSteppedOn()) {
			destructibleTile->markAsSteppedOn();
			return;
		}
	}
	
	//ampliación: jugador salta sobre enemigo "saltable": 
	for (auto const& jumpableMonster : jumpableMonsters) {
		if (!jumpableMonster->isJumpedOn() && (player->y + player->height / 2) //condiciones para que el jugador esté sobre el enemigo "saltable"
			== (jumpableMonster->y - jumpableMonster->height / 2)
			&& (player->x >= (jumpableMonster->x - jumpableMonster->width) 
				&& player->x <= (jumpableMonster->x + jumpableMonster->width))) {
			jumpableMonster->jumpOnIt();
			points++;  //se incrementa punto al eliminar monstruo "saltable"
			textPoints->content = to_string(points);
			return;
		}
	}
	
    //colisión enemy-projectile:
	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
				enemy->impacted();
				points++;
				textPoints->content = to_string(points);
			}
		}
	}
	//enemigos no se eliminan hasta que termine su animación de morir
	for (auto const& enemy : enemies) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();

			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
		}
	}
	//eliminar elementos recolectables recogidos tras salir de renderizado
	for (auto const& coll : collectibleItems) {
		if (coll->isCollected()) {  //si ya se cogió
			bool pInList = std::find(deleteCollectibles.begin(),
				deleteCollectibles.end(),
				coll) != deleteCollectibles.end();

			if (!pInList) {
				deleteCollectibles.push_back(coll);
			}
		}
	}
	//eliminar tiles destructibles salir de renderizado
	for (auto const& desTile : destructibleTiles) {
		if (desTile->isSteppedOn() && desTile->timer == 0) {  //si ya fue pisada y pasó su tiempo
			bool pInList = std::find(deleteDestructibleTiles.begin(),
				deleteDestructibleTiles.end(),
				desTile) != deleteDestructibleTiles.end();

			if (!pInList) {
				deleteDestructibleTiles.push_back(desTile);
			}
		}
	}

	//eliminar enemigos "saltados" tras salir de renderizado
	for (auto const& jumped : jumpableMonsters) {
		if (jumped->isJumpedOn()) {  //si ya saltó jugador sobre él
			bool pInList = std::find(deleteJumpableMonsters.begin(),
				deleteJumpableMonsters.end(),
				jumped) != deleteJumpableMonsters.end();

			if (!pInList) {
				deleteJumpableMonsters.push_back(jumped);
			}
		}
	}

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();

	//ampli: se eliminan también los elementos recolectables
	for (auto const& delCollectible : deleteCollectibles) {
		collectibleItems.remove(delCollectible);
		space->removeStaticActor(delCollectible);
	}
	deleteCollectibles.clear();

	//ampli: se eliminan los tiles destructibles
	for (auto const& delTile : deleteDestructibleTiles) {
		destructibleTiles.remove(delTile);
		space->removeStaticActor(delTile);
	}
	deleteDestructibleTiles.clear();

	//ampli: se eliminan enemigos "saltados"
	for (auto const& jumpable : deleteJumpableMonsters) {
		jumpableMonsters.remove(jumpable);
		space->removeStaticActor(jumpable);
	}
	deleteJumpableMonsters.clear();


	cout << "update GameLayer" << endl;
	*/
}
/*Para hacer que el jugador pueda hacer scroll para ver las partes del mapa que no caben en la pantalla:
* - los límites no son relativos: el tamaño del mapa es fijo (600x400)
* - la "cámara" jugador tiene que adaptarse al movimiento, pero sin pasar los límites en 4 direcciones
* - se mantiene el márgen en el que pa pantalla no se mueve, con 0.3/0.7 (0.5 en ambos==jugador siempre centrado)
* - std::clamp manual(tengo C++14): limitar máx y mín oara que player no se salga de límites
*/
void GameLayer::calculateScroll() {
	// Límites de la cámara
	int leftLimit = 0;
	int topLimit = 0;
	int rightLimit = BACKGROUND_WIDTH - WINDOW_WIDTH;  // 600 - 480 = 120
	int bottomLimit = BACKGROUND_HEIGHT - WINDOW_HEIGHT;  // 400 - 320 = 80

	// Limitar el desplazamiento horizontal (scrollX)
	if (player->x - scrollX < WINDOW_WIDTH * 0.3) {  // Si el jugador está cerca del borde izquierdo
		scrollX = player->x - WINDOW_WIDTH * 0.3;
	}
	if (player->x - scrollX > WINDOW_WIDTH * 0.7) {  // Si el jugador está cerca del borde derecho
		scrollX = player->x - WINDOW_WIDTH * 0.7;
	}

	// Limitar el desplazamiento vertical (scrollY)
	if (player->y - scrollY < WINDOW_HEIGHT * 0.3) {  // Si el jugador está cerca del borde superior
		scrollY = player->y - WINDOW_HEIGHT * 0.3;
	}
	if (player->y - scrollY > WINDOW_HEIGHT * 0.7) {  // Si el jugador está cerca del borde inferior
		scrollY = player->y - WINDOW_HEIGHT * 0.7;
	}

	// Limitar el desplazamiento para que no se salga del fondo (600x400)
	if (scrollX < leftLimit) scrollX = leftLimit;
	if (scrollY < topLimit) scrollY = topLimit;
	if (scrollX > rightLimit) scrollX = rightLimit;
	if (scrollY > bottomLimit) scrollY = bottomLimit;
}

void GameLayer::limitPlayerPosition() {
	const int MARGIN = 8;

	int leftLimit = MARGIN;
	int topLimit = 0;
	int rightLimit = 600 - player->width;
	int bottomLimit = 400 - player->height;

	if (player->x < leftLimit) player->x = leftLimit;
	if (player->x > rightLimit) player->x = rightLimit;
	if (player->y < topLimit) player->y = topLimit;
	if (player->y > bottomLimit) player->y = bottomLimit;
}



void GameLayer::draw() {
	calculateScroll();

	background->draw(scrollX, scrollY);
	for (auto const& tile : tiles) {
		tile->draw(scrollX, scrollY);
	}
	//for (auto const& desTile : destructibleTiles) {
		//desTile->draw(scrollX, scrollY);
	//}

	//for (auto const& projectile : projectiles) {
		//projectile->draw(scrollX, scrollY);
	//}
	//pintar Copa después de tiles, detrás del jugador
	//cup->draw(scrollX, scrollY);

	//ampli: poner punto de salvar partida
	//salvar->draw(scrollX, scrollY);

	//ampli: dibujar recolectables
	//for (auto const& coll : collectibleItems) {
		//coll->draw(scrollX, scrollY);
	//}
	//ampli: dibujar enemigos "saltables"
	//for (auto const& jumpable : jumpableMonsters) {
		//jumpable->draw(scrollX, scrollY);
	//}

	player->draw(scrollX, scrollY);
	for (auto const& enemy : enemies) {
		enemy->draw(scrollX, scrollY);
	}

	backgroundPoints->draw();
	textPoints->draw();
	//ampli: pintar marcador recolectables:
	backgroundCollectibles->draw();
	textCollected->draw();

	// HUD : solo se pinta si inputMouse
	if (game->input == game->inputMouse) {
		buttonJump->draw(); // NO TIENEN SCROLL, POSICION FIJA
		buttonShoot->draw(); // NO TIENEN SCROLL, POSICION FIJA
		pad->draw(); // NO TIENEN SCROLL, POSICION FIJA
	}
	if (pause) {  //meesage se dibuje en último lugar, solo si está en pausa.
		message->draw();
	}

	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_QUIT) {
		game->loopActive = false;
	}
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: //space para picar el suelo en busca de la chocografía
			controlPeck = true;
			player->peck(); //para que cambie al estado de picar
			break;
		}


	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlPeck = false;
			break;
		}
	}
}

void GameLayer::mouseToControls(SDL_Event event) {
	// Modificación de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;
	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		controlContinue = true; //eliminar pausa con clic en la pantalla
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlPeck = true;
		}
		//
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = -1;
		}
	}
	// Cada vez que se mueve
	if (event.type == SDL_MOUSEMOTION) {
		//para el pad: imitar joystick real (sin clicks)
		if (pad->containsPoint(motionX, motionY)) {
			controlMoveX = pad->getOrientationX(motionX);
			// Rango de seguridad: -20 a 20 es igual que 0:  x si jugador quiere ponerse en 0, pero no lo consigue
			if (controlMoveX > -20 && controlMoveX < 20) {
				controlMoveX = 0;
			}
		/*
		* Código x si quiere implementarse que haya que hacer click
		* (guion plat3)
		*/
		}
		else {
			controlMoveX = 0;
		}
		//if: x si se sale el puntero del botón: tiene que parar
		if (buttonShoot->containsPoint(motionX, motionY) == false) {
			controlPeck = false;
		//si quieres q haya que darle cada vez q se dispara: poner a false en cuanto se pulse, para tner que reiniciar el pulsar (true es justo al pulsarse: y dispara))
		}
		//
		if (buttonJump->containsPoint(motionX, motionY) == false) {
			controlMoveY = 0;
		}
	}
	// Cada vez que levantan el click
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlPeck = false;
		}
		//
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = 0;
		}
	}
}

void GameLayer::gamePadToControls(SDL_Event event) {

	// Leer los botones
	bool buttonA = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_A);
	bool buttonB = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_B);
	// SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B
	// SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_Y
	cout << "botones:" << buttonA << "," << buttonB << endl;
	int stickX = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_LEFTX);
	cout << "stickX" << stickX << endl;

	// Retorna aproximadamente entre [-32800, 32800], el centro debería estar en 0
	// Si el mando tiene "holgura" el centro varia [-4000 , 4000]
	if (stickX > 4000) {
		controlMoveX = 1;
	}
	else if (stickX < -4000) {
		controlMoveX = -1;
	}
	else {
		controlMoveX = 0;
	}

	if (buttonA) {
		controlPeck = true;
	}
	else {
		controlPeck = false;
	}

	if (buttonB) {
		controlMoveY = -1; // Saltar
	}
	else {
		controlMoveY = 0;
	}
}

