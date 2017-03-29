#pragma once

#include <btBulletDynamicsCommon.h>
#include "GameObject.h"
#include "missileComponent.h"
//#include "SpaceScene.h"

#define BIT(x) (1<<(x))
enum collisiontypes {
	COL_NOTHING = 0, //<Collide with nothing
	COL_AST = BIT(1), //<Collide with asteroids
	COL_ENEMY = BIT(2), //<Collide with enemy ship
	COL_ENEMYMISSILE = BIT(3), //<Collide with enemyMissile
	COL_PLAYER = BIT(4), //<Collide with player
	COL_PLAYERMISSILE = BIT(5), //<Collide with playerMissile
};

class BulletPhys
{
public:
	BulletPhys();
	~BulletPhys();
	void CreateGroundPlane();
	btRigidBody* CreatePhysBox(btVector3 StartPos, float TempMass, int ID, collisiontypes Group);
	int CreateBoxShape(btVector3 size);
	int CreateSphereShape(double size);
	void updatePhysics();
	btRigidBody* getRidgidBody(int ID);
	btVector3 getPosition(btRigidBody* body);
	btDiscreteDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; }
	void Remove(btRigidBody* tempBody);


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

	//collision set ups
	int astCollidesWith = COL_AST | COL_ENEMY | COL_PLAYER | COL_PLAYERMISSILE | COL_ENEMYMISSILE;
	int EnemyCollidesWith = COL_PLAYERMISSILE | COL_AST;
	int PlayerCollidesWith = COL_ENEMYMISSILE | COL_AST;
	int EnemyMissilesCollidesWith = COL_AST | COL_PLAYER;
	int PlayerMissilesCollidesWith = COL_AST | COL_ENEMY;
};