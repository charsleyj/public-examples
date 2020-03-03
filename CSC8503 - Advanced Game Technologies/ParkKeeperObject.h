#pragma once
#include "..//CSC8503Common/GameObject.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "GooseObject.h"

namespace NCL {
	namespace CSC8503 {

		class ParkKeeperObject : public GameObject {

		public:
			ParkKeeperObject();
			virtual ~ParkKeeperObject();

			void setSpawnPos(Vector3 p) { spawnPos = p; }
			Vector3 getSpawnPos() const { return spawnPos; }

			GooseObject* getGoose() const { return goose; }

			vector<Vector3> getWaypoints() const { return waypoints; }

			virtual void Update(float dt);
			virtual void OnCollisionBegin(GameObject* other);

		protected:
			Vector3 spawnPos;
			GooseObject* goose;

			NavigationGrid* grid;
			vector<Vector3> waypoints;

			void moveTo(Vector3 pos);
			GooseObject* findGoose();
			void rotate();

		};

	}
}