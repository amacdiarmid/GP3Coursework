#pragma once

#include <btBulletDynamicsCommon.h>
//#include "GameObject.h"
//#include "SpaceScene.h"

class BulletPhys
{
public:
	BulletPhys();
	~BulletPhys();
	void CreateGroundPlane();
	btRigidBody* CreatePhysBox(btVector3 StartPos, float TempMass, int ID);
	int CreateBoxShape(btVector3 size);
	int CreateSphereShape(double size);
	void updatePhysics();
	btRigidBody* getRidgidBody(int ID);
	btVector3 getPosition(btRigidBody* body);
	btDiscreteDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; }


private:
	//init objects
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};