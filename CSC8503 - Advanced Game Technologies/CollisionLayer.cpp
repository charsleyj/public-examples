#include "CollisionLayer.h"
#include <climits>

#define UINT_MAX_DIGIT (UINT_MAX / 2 + 1)

unsigned int CollisionLayer::currentId = 1;
CollisionLayer CollisionLayer::defaultLayer = CollisionLayer(1, UINT_MAX);

CollisionLayer::CollisionLayer() {
	if (currentId < UINT_MAX_DIGIT) {
		currentId *= 2;
		id = currentId;
	}
	collidingLayers = 0;
}

CollisionLayer::CollisionLayer(unsigned int id, unsigned int layers) :
	id(id),
	collidingLayers(layers) {
}

CollisionLayer::CollisionLayer(const CollisionLayer& src) :
	id(src.id),
	collidingLayers(src.collidingLayers) {
}

CollisionLayer::~CollisionLayer() {
}

void CollisionLayer::setLayer(CollisionLayer other, bool colliding) {
	collidingLayers = colliding ?
		collidingLayers | other.getId() :
		collidingLayers & ~other.getId();
}

void CollisionLayer::setLayers(int newLayers) {
	collidingLayers = newLayers;
}

bool CollisionLayer::isColliding(CollisionLayer other) {
	return (collidingLayers & other.getId()) != 0;
}

