#pragma once
#include "../CSC8503Common/GameObject.h"
#include "..//CSC8503Common/StateMachine.h"
#include "..//CSC8503Common/State.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "ItemObject.h"

namespace NCL {
	namespace CSC8503 {

		class VillagerObject : public GameObject {

		public:
			VillagerObject();
			virtual ~VillagerObject();

			void setRetrievedItem(bool b) { retrievedItem = b; }
			bool getRetrievedItem() const { return retrievedItem; }

			void setSpawnPos(Vector3 p) { spawnPos = p; }
			Vector3 getSpawnPos() const { return spawnPos; }

			void setItem(ItemObject* i) { item = i; }
			ItemObject* getItem() const { return item; }

			void setStateMachine(StateMachine* s) { stateMachine = s; }
			StateMachine* getStateMachine() const { return stateMachine; }

			vector<Vector3> getWaypoints() const { return waypoints; }

			virtual void Update(float dt);
			virtual void OnCollisionBegin(GameObject* other) override;

			static StateFunc standFunc;
			static StateFunc GooseMoveFunc;
			static StateFunc ItemMoveFunc;
			static StateFunc SpawnMoveFunc;
			static TransitionFunc standMoveFunc;
			static TransitionFunc gooseItemFunc;
			static TransitionFunc itemGooseFunc;
			static TransitionFunc toSpawnFunc;
			static TransitionFunc toStandFunc;

		protected:
			bool retrievedItem;
			Vector3 spawnPos;
			ItemObject* item;
			StateMachine* stateMachine;

			NavigationGrid* grid;
			vector<Vector3> waypoints;

			void setupStateMachine();
			void rotate();
			void moveTo(Vector3 pos);
			

		};

	}
}