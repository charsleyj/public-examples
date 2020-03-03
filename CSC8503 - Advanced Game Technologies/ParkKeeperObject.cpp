#include "ParkKeeperObject.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Common/Maths.h"

using namespace NCL::CSC8503;

ParkKeeperObject::ParkKeeperObject() :
	GameObject("ParkKeeper"),
	goose(nullptr) {
    grid = new NavigationGrid("CourseworkGrid.txt");
}

ParkKeeperObject::~ParkKeeperObject() {
	delete grid;
}

void ParkKeeperObject::rotate() {
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

void ParkKeeperObject::Update(float dt) {
	goose = findGoose();
	moveTo(goose->GetTransform().GetWorldPosition());
	rotate();
}

void ParkKeeperObject::OnCollisionBegin(GameObject* other) {
	if (other->GetName() == "Goose")
		transform.SetWorldPosition(spawnPos);
}

GooseObject* ParkKeeperObject::findGoose() {
	vector<GameObject*>::const_iterator first;
	vector<GameObject*>::const_iterator last;
	world->GetObjectIterators(first, last);
	for (auto i = first; i != last; ++i) {
		if ((*i)->GetName() == "Goose") {
			return (GooseObject*)*i;
		}
	}
}

void ParkKeeperObject::moveTo(Vector3 pos) {

	waypoints.clear();
	NavigationPath path;

	Vector3 thisPos = transform.GetWorldPosition();
	if (!grid->FindPath(thisPos, pos, path))
		return;
	
	path.PopWaypoint(Vector3());
	Vector3 waypoint;

	waypoints.emplace_back(thisPos);
	while (path.PopWaypoint(waypoint)) {
		if(path.numWaypoints() != 0)
			waypoints.emplace_back(waypoint);
	}
	waypoints.emplace_back(pos);

	Vector3 dir = (waypoints[1] - thisPos);
	dir.y = 0;
	dir.Normalise();
	physicsObject->AddForce(dir * 28.0f);
}