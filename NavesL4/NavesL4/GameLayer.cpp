#include "GameLayer.h"
#include <algorithm> //para std::clamp en el control de scroll

GameLayer::GameLayer(Game* game) : Layer(game) {
	//llama al constructor del padre : Layer(renderer)

	/* Variables pausa/ message: Normalmente esto lo hacemos en el método init() , 
	pero en este caso nos viene mejor hacerlo en el constructor
	para evitar que cuando se reinicie el juego aparezca siempre con los mismos valores,
	nos aportará algo más de control*/

	firstStart = true; //distingue pausa inicial para controlar cuándo se inicia el temporizador
	pause = true;
	message = new Actor("res/hotNCold_como_jugar.png", WIDTH * 0.5, HEIGHT * 0.5,
		WIDTH, HEIGHT, game);
	//message: apunta al “mensaje actual”, pero puede cambiarse en cualquier momento, instanciando otro actor

	textMoguriPauseMessage = new Text("hola", WIDTH * 0.5, HEIGHT * 0.5, game);  //mensaje colisión con moguri
	textMoguriPauseMessage->content = ""; // vacía por defecto

	//textChocoDistance = new Text("", WIDTH * 0.20, HEIGHT * 0.08, game);
	//textChocoDistance->content = "Distancia: ???";   funciona mal

	//posición aleatoria, pero evita nullppinter si x alguna razón se llama antes de que esté listo player: 
	//textChocoHint = new Text("hola", WIDTH * 0.5, HEIGHT * 0.5, game);
	//textChocoHint->content = "";  //hasta que no se muestren en updateChoc() coordenadas aleatorias+vacía


	init();
}


void GameLayer::init() {

	//iniciar y mostrar el cronómetro del juego: se reinicia en otro lugar para que empiece tras cómo jugar""
	//initTime = SDL_GetTicks();  //muestra "Tiempo: mm:ss"
	pauseTime = 0;

	textTimer = new Text("hola", WIDTH * 0.20, HEIGHT * 0.04, game);
	textTimer->content = "02:00";                    //ya funciona el temporizador

	pad = new Pad(WIDTH * 0.15, HEIGHT * 0.80, game);
	//se crean los 2 botones a partir de actor xq son muy simples:
	buttonJump = new Actor("res/boton_salto.png", WIDTH * 0.9, HEIGHT * 0.55, 100, 100, game);
	buttonShoot = new Actor("res/boton_disparo.png", WIDTH * 0.75, HEIGHT * 0.83, 100, 100, game);  //MODIFICAR

	space = new Space(0); //Pongo la gravedad en 0, pera retirar la gravedad vertical
	scrollX = 0;
	scrollY = 0;  //añadir scrollY para ampliación
	tiles.clear();																					//REVISAR

	//audioBackground = new Audio("res/Ukule_Chocobo_IX.mp3", true);  
	//audioBackground->play();

	//mostrar chocografías:      
	collected = 0;
	textCollected = new Text("hola", WIDTH * 0.67, HEIGHT * 0.04, game);
	textCollected->content = to_string(collected);
	
	background = new Background("res/fondo_hierba.png", WIDTH * 0.5, HEIGHT * 0.5, 0, game);  //CAMBIO A VELOCIDAD 0 PARA PROBAR ESTÁTICO
	//icono temporizador:
	backgroundTimer = new Actor("res/timer_icon.png",
		WIDTH * 0.1, HEIGHT * 0.05, 45, 48, game);
	//recolectables: chocografías:
	backgroundCollectibles = new Actor("res/chocography_icon.png",
		WIDTH * 0.6, HEIGHT * 0.05, 25, 25, game);
																							//REVISAR
	enemies.clear(); // Vaciar por si reiniciamos el juego
	chocographies.clear(); // Vaciar por si reiniciamos el juego

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
			mapWidth = line.length() * 40; // Ancho del mapa en pixels               !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
		case 'E': {
			AnnoyingEnemy* annoyingEnemy = new AnnoyingEnemy(x, y, game);
			// modificación para empezar a contar desde el suelo.
			annoyingEnemy->y = annoyingEnemy->y - annoyingEnemy->height / 2;
			enemies.push_back(annoyingEnemy);
			//space->addDynamicActor(annoyingEnemy);  //control a mano
			break;
		}
		case 'F': {
			FriendMoguri* friendMoguri = new FriendMoguri(x, y, game);
			// modificación para empezar a contar desde el suelo:
			friendMoguri->y = friendMoguri->y - friendMoguri->height / 2;
			friends.push_back(friendMoguri);
			space->addDynamicActor(friendMoguri);
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
		case 'C': {
			Chocography* chocography = new Chocography(x, y, game);  //no tienen imagen
			// modificación para empezar a contar desde el suelo.
			chocography->y = chocography->y - chocography->height / 2;
			chocographies.push_back(chocography);
			//space->addStaticActor(chocography);  //chocos son actores lógicos, no físicos !!
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

		/*inicio del trmporizador : tras haberse eliminado el mensaje "cómo jugar", pero solo tras iniciar partida
		(no mirar ni reiniciar en pausas durante la partida)*/
		if (firstStart) {
			initTime = SDL_GetTicks(); //se inicial el temporizador
			pauseTime = 0;
			activeTimer = true;
			firstStart = false;
		}
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

	//para controlar los límites del jugador antes de actualizar su posición:
	limitPlayerPosition();

	space->update();
	background->update();
	player->update();

	//control de las posiciones de las chocografías:
	//updateChocographies();

	for (auto const& enemy : enemies) {
		enemy->update(player);                                        
	}

	for (auto const& friendMoguri : friends) {
		friendMoguri->update();                                   
	}

	// Colisiones: ENEMIGO MOLESTO
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {

			//CUANDO CHOCOBO CHOQUE CON ENEMIGO MOLESTO: TENDRÁ QUE BAJAR LA VELOCIDAD DEL CHOCOBO
			//(DECIDIR SI PUEDE HABER + DE 1)
			/*player->loseLife();
			if (player->lifes <= 0) {
				init();
				return;
			}*/
		}
	}
	//COLISIÓN CON CHOCOGRAPHY: tiene que incrementarse el contador de chocos + marcarse como encontrada
	if (controlPeck) {  //se mira primero si player está picando, luego si está sobre choco
		for (auto const& choco : chocographies) {
			if (!choco->isEncontrada() && player->isOverlap(choco)) {
				choco->picar();  //encontrada
				collected++;     //incremento contador
				textCollected->content = to_string(collected); //actualizar texto encontradas
				break; //solo una choco por picotazo
			}
		}
		controlPeck = false; // Solo una vez por pulsación
	}


	// Colisiones: COLISIÓN CON PERSONAJE AMIGO (MOGURI): PAUSA EL CONTADOR DE TIEMPO DURANTE UNOS SEGUNDOS
	////COLISIÓN CON PERSONAJE AMIGO (MOGURI): PAUSA EL CONTADOR DE TIEMPO DURANTE UNOS SEGUNDOS
	for (auto const& friendMoguri : friends) {
		if (player->isOverlap(friendMoguri)) {
			if (!temporalPause) {
				temporalPause = true;
				//parar el movimiento del moguri¿? o poner otra animacion: REVISAR !!
				activeTimer = false;
				pauseInit = SDL_GetTicks();
				pauseTime += waitingTime;
				//mostrar mensaje del moguri:
				textMoguriPauseMessage->content = "¡Amigo Moguri te ayuda! Tiempo detenido.";
				showMoguriPauseMessage = true;
			}
		}
	}
	//actualizar el cronómetro:
	updateTimer();

	cout << "update GameLayer" << endl;
	
}

void GameLayer::updateTimer() {
	//si el temporizador está pausado por la ayuda del moguri:
	if (temporalPause) {
		if (SDL_GetTicks() - pauseInit >= waitingTime) {  //mirar si ha terminado la pausa
			temporalPause = false;
			activeTimer = true;
			showMoguriPauseMessage = false;
		}
	}
	//cuando no hay pausa activa:
	if (activeTimer) {
		passedTime = SDL_GetTicks() - initTime - pauseTime;
		//se calcula tiempo restante:
		int timeLeft = totalTime - passedTime;
		if (timeLeft <= 0) { //si se agoó el tiempo
			timeLeft = 0;
			activeTimer = false;
			//AÑADIR LÓGICA PARA TERMINAR EL JUEGO:									REVISARRRRRRRRRR
			showGameOver = true;
			pause = true; //se pausa el juego de fondo, además de mostrar el mensaje
			//game->state = "timeout";   //cambiado por el cambio de layer
			
			game->layer = game->gameOverLayer;
		}

		int sec = timeLeft / 1000;
		int min = sec / 60;
		sec = sec % 60;

		stringstream ss;  //formato para mostrar el temporizador== MM:ss (algo como StringBuilder)
		ss << "";
		if (min < 10) ss << "0";
		ss << min << ":";
		if (sec < 10) ss << "0";
		ss << sec;
		//mostrar en la pantalla
		textTimer->content = ss.str();
	}
}

void GameLayer::updateChocographies() {
	float minDistance = 99999.0f;  //valor alto para que cubra mucha superficie de búsqueda
	Chocography* closestChoco = nullptr;

	for (auto const& choco : chocographies) {
		if (choco->isEncontrada()) continue;
		//calcular posición respecto del jugador:
		float dx = player->x - choco->x;
		float dy = player->y - choco->y;
		float dist = sqrt(dx * dx + dy * dy);
		//elegir la que tenga menor distancia:
		if (dist < minDistance) {
			minDistance = dist;
			closestChoco = choco;
		}
	}

	//para actualizar mensajes de pistas:
	if (closestChoco != nullptr) {
		string hint;
		if (minDistance < 30) {
			hint = "KHUÉ???";
		}
		else if (minDistance < 80) {
			hint = "khué??";
		}
		else if (minDistance < 150) {
			hint = "khué";
		}
		else {
			hint = "";
		}
		//actualizar y mostrar cuando haya pista:
		if (!hint.empty()) {
			textChocoHint->content = hint;
			textChocoHint->x = player->x - scrollX;
			//ubicar mensaje de pista encima del jugador:
			textChocoHint->y = player->y - player->height * 0.7 - scrollY;

			showHotColdHint = true;
			hintStartTime = SDL_GetTicks();  //para controlar el tiempo que dura la pista
		}
	}
	else {
		textChocoHint->content = "";
	}
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
	int rightLimit = 65000;//BACKGROUND_WIDTH;// -WINDOW_WIDTH;  // 600 - 480 = 120
	int bottomLimit = 65000; // BACKGROUND_HEIGHT;// -WINDOW_HEIGHT;  // 400 - 320 = 80

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
	//if (scrollX < leftLimit) scrollX = leftLimit;
	//if (scrollY < topLimit) scrollY = topLimit;
	//if (scrollX > rightLimit) scrollX = rightLimit;
	//if (scrollY > bottomLimit) scrollY = bottomLimit;
}

void GameLayer::limitPlayerPosition() {

	const int MARGIN = 8;

	int leftLimit = MARGIN;
	int topLimit = 0;
	int rightLimit = 1840 - player->width;
	int bottomLimit = 320 - player->height;

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
	//aunque sean transparentes: situal chocos para poder localizarlas
	for (auto const& choco : chocographies) {
		//choco->draw(scrollX, scrollY);
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

	for (auto const& friendMoguri : friends) {
		friendMoguri->draw(scrollX, scrollY);
	}

	for (auto const& chocography : chocographies) {
		chocography->draw(scrollX, scrollY);
	}

	//mostrar temporizador:
	textTimer->draw();
	//mostrar icono:
	backgroundTimer->draw();
	//mensaje de pista sobre la chocografía más cercana:
	if (textChocoHint != nullptr) {
		//textChocoHint->draw();  //tiene que moverse con el jugador
	}

	//textChocoHint->draw(); 
	//actualizar contador distancia a choco + cercana:
	//textChocoDistance->draw();

	//mostrar o no mensaje del moguri al empezar la pausa (tras colisión):
	if (showMoguriPauseMessage) {
		textMoguriPauseMessage->draw();
	}

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