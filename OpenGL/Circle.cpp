

#include <aie/Gizmos.h>
#include <iostream>

#include "Circle.h"
#include "Plane.h"


Circle::Circle(glm::vec2 posn, glm::vec2 vel, float radius)
	:RigidBody(PhysicsObjectType::CIRCLE, posn, vel),
	m_fRadius(radius)
{
	m_fMoment = 0.5f * m_fMass * m_fRadius * m_fRadius;
}


Circle::~Circle()
{
}

void Circle::draw() {
	glm::vec4 color = m_v4Color;

	if (!m_bIsAwake) color *= 0.5f;

	Gizmos::add2DCircle(m_v2Position, m_fRadius, 32, color);

	vec2 highlight = toWorld(vec2(0,m_fRadius / 2.0f));
	Gizmos::add2DCircle(highlight, m_fRadius/5.0f, 32, glm::vec4(0.5, 0.1, 0.5, 1));
}



bool Circle::collideWithPlane(PhysicsObject* obj) {

	Plane* plane = (Plane*)obj;

	// Height above plane
	float dist = glm::dot((m_v2Position - plane->m_v2Position), plane->m_v2Normal);

	float radialV = glm::dot(m_v2LinearVelocity, plane->m_v2Normal);


	if ((dist > 0 && dist < m_fRadius && radialV < 0) || (dist <0 && dist> -m_fRadius && radialV > 0)) {
		glm::vec2 deltaF = -m_fMass * (radialV * plane->m_v2Normal) * (1 + sm_fCoeffRestitution);
		applyForce(deltaF);
		return true;
	}
	return false;
}

bool Circle::collideWithCircle(PhysicsObject* obj) {
	Circle* circle = dynamic_cast<Circle*>(obj);


	glm::vec2 collisionNormal = glm::normalize(m_v2Position - circle->m_v2Position);

	// Height above circle
	float dist = glm::length(m_v2Position - circle->m_v2Position) - (m_fRadius + circle->m_fRadius);

	float radialV = glm::dot(m_v2LinearVelocity - circle->m_v2LinearVelocity, collisionNormal);


	if (dist < 0 && radialV < 0) {
		if (m_bIsAwake || circle->m_bIsAwake) {
			m_bIsAwake = true;
			circle->m_bIsAwake = true;
		}
		else
			return false; // Neither is awake

		glm::vec2 deltaF = -m_fMass * (radialV * collisionNormal) * (1 + sm_fCoeffRestitution);
		applyForce(0.5f * deltaF);
		circle->applyForce(-0.5f * deltaF);

		return true;
	}
	return false;
}

bool Circle::collideWithBox(PhysicsObject* obj) {
	return obj->collideWithCircle(this);
}

// pt in world space
bool Circle::isInside(glm::vec2 pt)
{
	pt -= m_v2Position;

	return glm::length(pt) < m_fRadius;
}