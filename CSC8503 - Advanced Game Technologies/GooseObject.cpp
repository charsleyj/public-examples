#include "GooseObject.h"
#include "../../Common/Window.h"
#include "../../Common/Maths.h"

using namespace NCL::CSC8503;

GooseObject::GooseObject() :
	GameObject("Goose"),
	inWater(false) {
	world = nullptr;
	setupStateMachine();
}

void GooseObject::setupStateMachine() {
	stateMachine = new StateMachine();
	State* playerState = new GenericState(playerMoveSFunc, this);
	State* autoState = new GenericState(autoMoveSFunc, this);
	stateMachine->AddState(playerState);
	stateMachine->AddState(autoState);
	stateMachine->AddTransition( new GenericTransition<float&, float>(
		GenericTransition<float&, float>::GreaterThanTransition,
		remAutoMoveTime, 0, playerState, autoState));
	stateMachine->AddTransition( new GenericTransition<float&, float>(
			GenericTransition<float&, float>::LessThanTransition,
			remAutoMoveTime, 0, autoState, playerState));
}

GooseObject::~GooseObject() {}

void GooseObject::move() {
	Matrix4 view = world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0));
	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	float force = 20.0f;
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
		physicsObject->AddForce(-rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
		physicsObject->AddForce(rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
		physicsObject->AddForce(fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
		physicsObject->AddForce(-fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P)) {
		std::cout << GetTransform().GetWorldPosition() << "\n";
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
		if (canJump())
			physicsObject->AddForce(Vector3(0, 1, 0) * 70 * 9.8f);
	}
}

void GooseObject::rotate() {
	Vector3 newDir = physicsObject->GetForce();
	if (Vector2(newDir.x, newDir.z).Length() == 0)
		return;

	float angle = RadiansToDegrees(atan2f(newDir.x, newDir.z));
	Quaternion newQuat = Quaternion::EulerAnglesToQuaternion(0, angle, 0);

	transform.SetLocalOrientation(
		Quaternion::Slerp(transform.GetLocalOrientation(),
		newQuat, 0.33f));
}

void GooseObject::updateHeldItem() {
	if (heldItem == nullptr)
		return;

	Vector3 itemPos =
		transform.GetWorldPosition() +
		(transform.GetWorldOrientation() *
		heldItemOffset);

	heldItem->GetTransform().SetLocalPosition(itemPos);
}

void GooseObject::pickup() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SHIFT) && heldItem == nullptr) {
		Ray itemCheck = Ray(transform.GetWorldPosition(),
			transform.GetWorldOrientation() * Vector3(0, 0, 1));
		     
		RayCollision collisionInfo;
		if (world->Raycast(itemCheck, collisionInfo, true) &&
			collisionInfo.rayDistance < 5 &&
			((GameObject*)collisionInfo.node)->GetName() == "Item") {
			
			pickup((GameObject*)collisionInfo.node);
		}
	}
	else if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SHIFT) && heldItem != nullptr) {
		drop();
	}
}

void GooseObject::pickup(GameObject* obj) {
	heldItem = obj;
	heldItem->setAsleep(false);
	heldItem->SetCollisionLayer(heldLayer);
}

void GooseObject::drop() {
	heldItem->setAsleep(true);
	heldItem->SetCollisionLayer(itemLayer);
	heldItem = nullptr;
}

void GooseObject::Update(float dt) {
	stateMachine->Update(dt);
}

void GooseObject::OnCollisionBegin(GameObject* otherObject) {
	if (otherObject->GetName() == "water")
		inWater = true;
	if (otherObject->GetName() == "ParkKeeper") {
		if(heldItem != nullptr) drop();
		*score -= 1;
		remAutoMoveTime = 2.0f;
		randomDir = Vector3(randomFloat(), 0, randomFloat()).Normalised();
	}
}

void GooseObject::OnCollisionEnd(GameObject* otherObject) {
	if (otherObject->GetName() == "water")
		inWater = false;
}

bool GooseObject::canJump() {
	
	Ray floorCheck = Ray(transform.GetWorldPosition(), Vector3(0, -1, 0));
	RayCollision collisionInfo;
	bool onFloor =
		world->Raycast(floorCheck, collisionInfo, true) &&
		collisionInfo.rayDistance < 1.15;

	return onFloor || inWater;
}

StateFunc GooseObject::playerMoveSFunc = [](void* data, float dt) {
	GooseObject* parent = (GooseObject*)data;
	parent->move();
	parent->rotate();
	parent->pickup();
	parent->updateHeldItem();
};

StateFunc GooseObject::autoMoveSFunc = [](void* data, float dt) {
	GooseObject* parent = (GooseObject*)data;
	parent->remAutoMoveTime -= dt;
	parent->GetPhysicsObject()->AddForce(parent->randomDir * 20.0f);
	parent->rotate();
};
