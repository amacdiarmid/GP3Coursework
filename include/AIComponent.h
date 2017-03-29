#pragma once

#include "Common.h"
#include "Components\BaseComponent.h"
#include "physicsComponent.h"

class AIComponent : public Component
{
public:
	AIComponent(GameObject *tempOwner, physicsComponent* TempPhys, GameObject* TempTarget);
	~AIComponent();

	void update(mat4 MVPMat) override;
	void FireMissile();
	void assignMissile(Object* tempObject, Shader* tempShader, Texture* tempTexture, int shapeID, BulletPhys* TempPhys, AudioClip* TempExp, AudioClip* TempFire) { missileObject = tempObject, missileShader = tempShader, missileTexture = tempTexture, missileShapeID = shapeID, bulPhys = TempPhys, FireAudio = TempFire, ExpAudio = TempExp; };


private:
	btRigidBody * phys;
	GameObject* target;

	float Speed = 10;

	//missile cound 
	int missileCount = 0;
	int FireForce = 100;
	float missileFireRate = 3000;
	bool CanFire = true;
	int TickOnFire;
	int missileMass = 1;
	int fireMissileDistance = 500;

	//missile 
	Object* missileObject;
	Shader* missileShader;
	Texture* missileTexture;
	int missileShapeID;
	AudioClip* ExpAudio;
	AudioClip* FireAudio;
	BulletPhys* bulPhys;
};