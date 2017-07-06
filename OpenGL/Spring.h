#pragma once

#include "PhysicsObject.h"
#include "RigidBody.h"

class Spring : public PhysicsObject
{
public:
	Spring(RigidBody* body1, vec2 contact1, RigidBody* body2, vec2 contact2, float length, float strength, float damping = 0.1f);
	~Spring();

	RigidBody* m_pBody1;
	RigidBody* m_pBody2;

	vec2 m_v2Contact1;
	vec2 m_v2Contact2;

	float m_fRestLength;
	float m_fStrength;
	float m_fDamping;

	virtual void draw();
	virtual void update(float deltaTime);
	
	virtual bool collideWithPlane(PhysicsObject*) { return false; };
	virtual bool collideWithCircle(PhysicsObject*) { return false; };
	virtual bool collideWithBox(PhysicsObject*) { return false; };
};

