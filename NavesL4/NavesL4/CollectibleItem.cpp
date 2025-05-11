#include "CollectibleItem.h"

CollectibleItem::CollectibleItem(float x, float y, Game* game)
	: Actor("res/icono_recolectable.png", x, y, 25, 25, game) {

	aMoving = new Animation("res/recolectable.png", width, height,
		256, 32, 6, 8, true, game);
	animation = aMoving;

	collected = false;
}

void CollectibleItem::update() {
	animation->update();
}

void CollectibleItem::markAsCollected() {
	collected = true;
}

bool CollectibleItem::isCollected() {
	return collected;
}

void CollectibleItem::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}
