#pragma once

#include <glm\glm\glm.hpp>

#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody(PhysicsObjectType objectType, glm::vec2 posn, glm::vec2 vel, bool isAwake = true);
	~RigidBody();

	glm::vec2 m_v2LinearVelocity;
	
	float m_fAngularVelocity;
	float m_fMass;
	float m_fMoment;

	bool m_bIsKinematic;
	bool m_bIsAwake;

	glm::mat4& getRotation();
	glm::vec2  toWorld(vec2 local);

	//virtual void draw();
	virtual void update(float deltaTime);
	
	//virtual void collideWithPlane(PhysicsObject*);
	//virtual void collideWithCircle(PhysicsObject*);
	//virtual void collideWithBox(PhysicsObject*);
	
	virtual void resolveCollision(RigidBody* other, vec2 contact, vec2* direction = NULL);
	virtual void applyForce(glm::vec2 force, glm::vec2 posn = glm::vec2());


	int tag;

private:
	bool m_bIsRotationDirty;
	glm::mat4 m_m4Rotation;

	static float sm_fSleepSpeed;
	static int sm_iSleepDelay;

	int m_iSleepCounter;

};

