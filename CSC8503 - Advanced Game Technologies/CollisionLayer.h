#pragma once

#define HIGH_BIT 

class CollisionLayer {

public:
	CollisionLayer();
	CollisionLayer(unsigned int id, unsigned int collidingLayers);
	CollisionLayer(const CollisionLayer& src);
	~CollisionLayer();

	int getId() const { return id; }
	int getCollidingLayers() const { return collidingLayers; }

	void setLayer(CollisionLayer other, bool colliding);
	void setLayers(int collidingLayers);

	bool isColliding(CollisionLayer other);

	static CollisionLayer defaultLayer;


protected:
	static unsigned int currentId;

	unsigned int id;
	unsigned int collidingLayers;

};