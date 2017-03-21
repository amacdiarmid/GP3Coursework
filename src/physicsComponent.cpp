#include "physicsComponent.h"



physicsComponent::physicsComponent()
{
	type = "physics component";
}


physicsComponent::physicsComponent(GameObject *tempOwner, btRigidBody* tempRidgidBody)
{
	type = "physics component";
	owner = tempOwner;
	ridgidBody = tempRidgidBody;
}

physicsComponent::~physicsComponent()
{

}

void physicsComponent::update(mat4 MVPMat)
{
	MVP = MVPMat;

	btTransform trans;
	ridgidBody->getMotionState()->getWorldTransform(trans);

	btVector3 BTpos = trans.getOrigin();
	vec3 vecPos = vec3(BTpos.x(), BTpos.y(), BTpos.z());
	
	btQuaternion BTrot = ridgidBody->getOrientation();
	double ysqr = BTrot.y() * BTrot.y();

	// roll (x-axis rotation)
	double t0 = +2.0 * (BTrot.w() * BTrot.x() + BTrot.y() * BTrot.z());
	double t1 = +1.0 - 2.0 * (BTrot.x() * BTrot.x() + ysqr);
	double roll = std::atan2(t0, t1);

	// pitch (y-axis rotation)
	double t2 = +2.0 * (BTrot.w() * BTrot.y() - BTrot.z() * BTrot.x());
	t2 = t2 > 1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;
	double pitch = std::asin(t2);

	// yaw (z-axis rotation)
	double t3 = +2.0 * (BTrot.w() * BTrot.z() + BTrot.x() * BTrot.y());
	double t4 = +1.0 - 2.0 * (ysqr + BTrot.z() * BTrot.z());
	double yaw = std::atan2(t3, t4);

	vec3 vecRot = vec3(roll, pitch, yaw);
	
	owner->setPosition(vecPos);
	//owner->setRotation(vecRot);
}

string physicsComponent::getType()
{
	return string();
}
