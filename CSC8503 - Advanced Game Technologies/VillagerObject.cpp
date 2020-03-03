#include "VillagerObject.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Common/Maths.h"
#include "GooseObject.h"

using namespace NCL::CSC8503;

VillagerObject::VillagerObject() :
	GameObject("Villager") {
	setupStateMachine();
	grid = new NavigationGrid("CourseworkGrid.txt");
}

void VillagerObject::setupStateMachine() {
	stateMachine = new StateMachine();
	State* standState = new GenericState(VillagerObject::standFunc, this);
	State* gooseMoveState = new GenericState(VillagerObject::GooseMoveFunc, this);
	State* itemMoveState = new GenericState(VillagerObject::ItemMoveFunc, this);
	State* spawnMoveState = new GenericState(VillagerObject::SpawnMoveFunc, this);
	stateMachine->AddState(standState);
	stateMachine->AddState(gooseMoveState);
	stateMachine->AddState(itemMoveState);
	stateMachine->AddState(spawnMoveState);

	stateMachine->AddTransition((StateTransition*) new ObjectTransition(
		VillagerObject::standMoveFunc, this, standState, gooseMoveState));
	stateMachine->AddTransition((StateTransition*) new ObjectTransition(
		VillagerObject::gooseItemFunc, this, gooseMoveState, itemMoveState));
	stateMachine->AddTransition((StateTransition*) new ObjectTransition(
		VillagerObject::itemGooseFunc, this, itemMoveState, gooseMoveState));
	stateMachine->AddTransition((StateTransition*) new ObjectTransition(
		VillagerObject::toSpawnFunc, this, gooseMoveState, spawnMoveState));
	stateMachine->AddTransition((StateTransition*) new ObjectTransition(
		VillagerObject::toSpawnFunc, this, itemMoveState, spawnMoveState));
	stateMachine->AddTransition((StateTransition*) new ObjectTransition(
		VillagerObject::toStandFunc, this, spawnMoveState, standState));
}

VillagerObject::~VillagerObject() {
	delete stateMachine;
	delete grid;
}

void VillagerObject::rotate() {
	Vector3 newDir = physicsObject->GetForce();
	if (Vector2(newDir.x, newDir.z).Length() == 0)
		return;

	float angle = RadiansToDegrees(atan2f(newDir.x, newDir.z));
	if (angle > 180)
		angle -= 180;
	Quaternion newQuat = Quaternion::EulerAnglesToQuaternion(0, angle, 0);

	transform.SetLocalOrientation(
		Quaternion::Slerp(transform.GetLocalOrientation(),
		newQuat, 0.33f));
}

void VillagerObject::moveTo(Vector3 pos) {

	waypoints.clear();
	NavigationPath path;

	Vector3 thisPos = transform.GetWorldPosition();
	if (!grid->FindPath(thisPos, pos, path))
		return;

	path.PopWaypoint(Vector3());
	Vector3 waypoint;

	waypoints.emplace_back(thisPos);
	while (path.PopWaypoint(waypoint)) {
		if (path.numWaypoints() != 0)
			waypoints.emplace_back(waypoint);
	}
	waypoints.emplace_back(pos);

	Vector3 dir = (waypoints[1] - thisPos);
	dir.y = 0;
	dir.Normalise();
	physicsObject->AddForce(dir * 28.0f);
}

void VillagerObject::Update(float dt) {
	stateMachine->Update();
}

void VillagerObject::OnCollisionBegin(GameObject* other) {
	if (other->GetName() == "Goose") {
		if (item != nullptr && item == ((GooseObject*)other)->getHeldItem()) {
			retrievedItem = true;
			((GooseObject*)other)->drop();
		}
	}
	else if (other->GetName() == "Item") {
		if (item == (GameObject*)other)
			retrievedItem = true;
	}
}

//---------- States ----------//

StateFunc VillagerObject::standFunc = [](void* data, float dt) {
	VillagerObject* parent = (VillagerObject*)data;

	parent->GetTransform().SetWorldPosition(parent->spawnPos);

	vector<GameObject*>::const_iterator first;
	vector<GameObject*>::const_iterator last;
	parent->getWorld()->GetObjectIterators(first, last);
	for (auto i = first; i != last; ++i) {
		if ((*i)->GetName() == "Goose") {

			Vector3 dist = (*i)->GetTransform().GetWorldPosition();
			dist -= parent->getSpawnPos();
			
			if (dist.Length() < 20.0f)
				parent->setItem((ItemObject*)((GooseObject*)* i)->getHeldItem());

		}
	}

};

StateFunc VillagerObject::GooseMoveFunc = [](void* data, float dt) {
	VillagerObject* parent = (VillagerObject*)data;
	GameObject* goose = parent->getItem()->getHeldBy();
	if (goose == nullptr) return;
	
	parent->moveTo(goose->GetTransform().GetWorldPosition());
	parent->rotate();
};

StateFunc VillagerObject::ItemMoveFunc = [](void* data, float dt) {
	VillagerObject* parent = (VillagerObject*)data;
	
	parent->moveTo(parent->getItem()->GetTransform().GetWorldPosition());
	parent->rotate();
};

StateFunc VillagerObject::SpawnMoveFunc = [](void* data, float dt) {
	VillagerObject* parent = (VillagerObject*)data;

	if (parent->retrievedItem) {
		parent->setRetrievedItem(false);
		ItemObject* item = parent->getItem();
		item->GetTransform().SetWorldPosition(item->getSpawnPos());
		parent->setItem(nullptr);
	}

	parent->moveTo(parent->spawnPos);
	parent->rotate();
};

//---------- Transitions ----------//

TransitionFunc VillagerObject::standMoveFunc = [](void* data) {
	VillagerObject* parent = (VillagerObject*)data;
	return parent->getItem() != nullptr;
};

TransitionFunc VillagerObject::gooseItemFunc = [](void* data) {
	return ((VillagerObject*)data)->getItem()->getHeldBy() == nullptr;
};

TransitionFunc VillagerObject::itemGooseFunc = [](void* data) {
	return ((VillagerObject*)data)->getItem()->getHeldBy() != nullptr;
};

TransitionFunc VillagerObject::toSpawnFunc = [](void* data) {
	return ((VillagerObject*)data)->retrievedItem;
};

TransitionFunc VillagerObject::toStandFunc = [](void* data) {
	VillagerObject* parent = (VillagerObject*)data;
	return (parent->GetTransform().GetWorldPosition() - parent->spawnPos).Length() < 2;
};