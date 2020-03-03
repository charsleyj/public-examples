#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"
#include "../CSC8503Common/StateTransition.h"

namespace NCL {
	namespace CSC8503 {

		class GooseObject : public GameObject {

		public:
			GooseObject();
			virtual ~GooseObject();

			void setHeldItem(GameObject* o) { heldItem = o; }
			GameObject* getHeldItem() const { return heldItem; }

			void setRayLayer(CollisionLayer* layer) { rayLayer = layer; }
			CollisionLayer* getRayLayer() const { return rayLayer; }

			void setHeldLayer(CollisionLayer* layer) { heldLayer = layer; }
			CollisionLayer* getHeldLayer() const { return heldLayer; }

			void setItemLayer(CollisionLayer* layer) { itemLayer = layer; }
			CollisionLayer* getItemLayer() const { return itemLayer; }

			void setScorePtr(int* sp) { score = sp; }
			int getScore() const { return *score; }
			void incScore(int points) { *score += points; }
			void resetScore() { *score = 0; }

			void move();
			void rotate();
			void updateHeldItem();
			void pickup();
			void pickup(GameObject* obj);
			void drop();

			virtual void Update(float dt);
			virtual void OnCollisionBegin(GameObject* otherObject);
			virtual void OnCollisionEnd(GameObject* otherObject);

			static StateFunc playerMoveSFunc;
			static StateFunc autoMoveSFunc;

			Vector3 randomDir;
			float remAutoMoveTime;

		protected:
			StateMachine* stateMachine;
			GameObject* heldItem;
			CollisionLayer* rayLayer;
			CollisionLayer* heldLayer;
			CollisionLayer* itemLayer;

			Vector3 heldItemOffset = Vector3(0, 1, 2.5f);

			bool inWater;
			int* score;

			void setupStateMachine();
			bool canJump();
		};

	}
}
