#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#pragma once

#include "Common.h"
#include "Components\Components.h"
#include "Shader.h"
#include "Object.h"
#include <map>
#include "ViewFrustumCulling.h"
#include "Scene.h"
#include "Mesh.h"

class GameObject
{
public:
	GameObject();
	//create world object
	GameObject(string tempName);
	//greate mesh object
	GameObject(string tempName, GameObject *tempParent, Object *tempModel, Texture *tempTexture, Shader *tempShader);
	//create player
	GameObject(string tempName, GameObject *tempParent, PlayerController *tempInput);
	//create node
	GameObject(string tempName, GameObject *tempParent);
	//create texture
	GameObject(string tempName, Texture *tempTexture);
	
	~GameObject();

	//in .cpp
	void update(mat4 VPMat);
	void render(Fustrum *fustrum);
	void addComponent(Component *newComp);
	void addChild(GameObject *tempChild);
	void changePosition(vec3 tempPos);
	GameObject *findChild(string com);
	void printChildern();
	void printComponents();

	//getters
	vec3 getLocalPos(){ return localPos; };
	vec3 getWorldPos();
	vec3 getScale(){ return size; };
	vec3 getRotation(){ return rotation; };
	Object *getModel(){ return model; };
	Shader *getShader(){ return shader; };
	Texture *getTexture(){ return texture; };
	GameObject *getChild(string tempName) { return childrenList[tempName]; };
	string getName(){ return name; };
	map<string, GameObject*> *getChildrenMap(){ return &childrenList; };
	map<string, Component*> *getCompMap(){ return &componentsList; };
	bool getActive(){ return active; };
	bool getWorld(){ return world; };
	PlayerController *getInput(){ return input; };
	mat4 getModelMatrix(){ return modelMatrix; }
	static Scene *getCurScene(){ return curScene; };
	MeshData getMesh(){ return mesh; };
	bool getForceRender(){ return forceRender; }

	//setters
	void setScale(vec3 tempScale){ size = tempScale; };
	void setRotation(vec3 tempRotation){ rotation.x = tempRotation.x; rotation.y = tempRotation.y; rotation.z = tempRotation.z; };
	void setModel(Object* tempModel) { model = tempModel; };
	void setShader(Shader* tempShader){ shader = tempShader; };
	void setTexture(Texture* tempTexture){ texture = tempTexture; };
	void setPosition(vec3 TempPos){ localPos = TempPos; };
	void setName(string tempName){ name = tempName; };
	void setParent(GameObject* tempParent){ parent = tempParent; };
	void setWord(bool tempBool){ world = tempBool; };
	void setActive(bool tempBool){ active = tempBool; };
	void setInput(PlayerController *tempPlayer){ input = tempPlayer; };
	void setScene(Scene *tempScene){ curScene = tempScene; };
	void setForceRender(bool tempBool){ forceRender = tempBool; }


private:
	string name;
	GameObject *parent;
	bool world;
	bool active;
	map<string, GameObject*> childrenList;
	map<string, Component*> componentsList;
	Object *model;
	Texture *texture;
	Shader *shader;
	PlayerController *input;
	mat4 MVP;
	mat4 modelMatrix;
	MeshData mesh;
	bool forceRender = false;

	//the distance from the parent world position
	vec3 localPos = vec3(0.0f, 0.0f, 0.0f);
	//scale
	vec3 size = vec3(1.0f, 1.0f, 1.0f);
	//rotation
	vec3 rotation = vec3(0.0f, 0.0f, 0.0f);

	//current scene
	static Scene *curScene;

};


#endif