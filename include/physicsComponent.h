#pragma once

#include "Common.h"
#include "Components\BaseComponent.h"
#include "Object.h"
#include <btBulletDynamicsCommon.h>
#include "GameObject.h"

class physicsComponent :public Component
{
public:
	physicsComponent();
	physicsComponent(GameObject *tempOwner, btRigidBody* tempRidgidBody);
	~physicsComponent();

	void update(mat4 MVPMat) override;
	string getType() override;

	btRigidBody* getRB() { return ridgidBody; };

private:
	btRigidBody* ridgidBody;
};

