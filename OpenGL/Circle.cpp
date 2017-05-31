

#include <aie/Gizmos.h>

#include "Circle.h"


Circle::Circle(glm::vec2 posn, glm::vec2 vel)
	:RigidBody(PhysicsObjectType::CIRCLE, posn, vel)
{
}


Circle::~Circle()
{
}

void Circle::draw() {
	Gizmos::add2DCircle(m_v2Position, 1, 32, glm::vec4(1, 0, 0, 1));
}

void Circle::update(float deltaTime) {

	// Acceleration
	m_v2Velocity += (s_v2Gravity * deltaTime);


	// Collision



	// Motion
	m_v2Position += (m_v2Velocity * deltaTime);
	m_fRotation += (m_fSpin * deltaTime);
}

void Circle::collideWithPlane(PhysicsObject*) {}
void Circle::collideWithCircle(PhysicsObject*) {}
void Circle::collideWithBox(PhysicsObject*) {}