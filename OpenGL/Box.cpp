
#include <aie\Gizmos.h>
#include <glm\glm\ext.hpp>

#include "Box.h"

#include "Circle.h"
#include "Plane.h"

Box::Box(vec2 posn, vec2 vel, vec2 size)
	:RigidBody(PhysicsObjectType::BOX, posn, vel),
	m_v2Size(size)
{
	m_fMoment = m_fMass * m_v2Size.x * m_v2Size.y / 12.0f;
}


Box::~Box()
{
}

void Box::draw() {

	Gizmos::add2DAABBFilled(m_v2Position, m_v2Size, glm::vec4(1,0,0,1), &m_m4Transform);
	
}

void Box::update(float deltaTime) {

	// Motion
	m_v2Position += (m_v2Velocity * deltaTime);
	m_fRotation += (m_fSpin * deltaTime);

	m_m4Transform = glm::eulerAngleZ(m_fRotation);
	
}

void Box::collideWithPlane(PhysicsObject* obj) {

}

void Box::collideWithCircle(PhysicsObject* obj) {
}

void Box::collideWithBox(PhysicsObject* obj) {
}

void Box::applyForce(glm::vec2 force, glm::vec2 posn) {

	glm::vec2 linearAccel = force / m_fMass;

	m_v2Velocity += linearAccel;
}