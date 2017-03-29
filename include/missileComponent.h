#pragma once

#include "Common.h"
#include "Components\BaseComponent.h"
#include "GameObject.h"
#include "AudioClip.h"

class missileComponent : public Component
{
public:
	missileComponent(GameObject *tempOwner, AudioClip* TempExp, AudioClip* TempFire);
	~missileComponent();

	void update(mat4 MVPMat) override;

	bool collideWithObject(GameObject* target);

private:
	AudioClip* explosion;
	AudioClip* fire;

	int spawnTime;
	int LifeSpawn = 60000;
};