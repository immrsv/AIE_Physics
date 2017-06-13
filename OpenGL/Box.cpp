
#include <aie\Gizmos.h>
#include <glm\glm\ext.hpp>
#include <iostream>

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

glm::vec4 pointColour(int x, int y) {
	if (x == -1)
		if (y == -1) return glm::vec4(1, 0, 0, 0); // -1, -1, BLACK
		else return glm::vec4(1, 1, 0, 1); // -1, 1, YELLOW
	else
		if (y == -1) return glm::vec4(1, 0, 1, 1); // 1, -1, 
		else return glm::vec4(0, 1, 1, 1); // 1, 1

	return glm::vec4(1, 1, 1, 1);
}

void Box::draw() {

	Gizmos::add2DAABBFilled(m_v2Position, m_v2Size, glm::vec4(1,0,0,1), &m_m4Transform);


	glm::vec2 rotatedCorners = glm::vec2(glm::vec4(m_v2Size, 0, 1) * m_m4Transform);

	for (int x = -1; x <= 1; x += 2) {
		for (int y = -1; y <= 1; y += 2) {
			glm::vec2 corner = glm::vec2(m_m4Transform * glm::vec4(m_v2Size*vec2( x,y), 0, 1));
			Gizmos::add2DCircle(m_v2Position + corner , 0.1f, 16, pointColour(x,y));
		}
	}
	
}

void Box::update(float deltaTime) {

	// Motion
	m_v2Position += (m_v2LinearVelocity * deltaTime);
	m_fRotation += (m_fAngularVelocity * deltaTime);

	m_m4Transform = glm::eulerAngleZ(m_fRotation);
	
}

void Box::collideWithPlane(PhysicsObject* obj) {

	Plane* plane = dynamic_cast<Plane*>(obj);

	if (!plane) throw "Box::collideWithPlane() :: Invalid Argument: obj is not a Plane";


	int numContacts = 0;
	vec2 contact(0, 0);
	float contactV = 0;
	float radius = glm::length(m_v2Size);

	float comFromPlane = glm::dot(m_v2Position - plane->m_v2Position, plane->m_v2Normal);
	float penetration = 0;
	
	//if (radius < std::fabsf(comFromPlane)) return; // No Contact

	vec2 offset;

	// For each corner
	for (int x = -1; x <= 1; x += 2) {
		for ( int y = -1; y <= 1; y += 2) {

			// find world position
			vec2 offset = glm::vec2(m_m4Transform * glm::vec4(m_v2Size*vec2(x, y), 0, 1));
			vec2 corner = m_v2Position + offset;


			// find distance from plane and point-relative velocity
			float distFromPlane = glm::dot(corner - plane->m_v2Position, plane->m_v2Normal);
			vec2 tangentialVelocity = radius * m_fAngularVelocity * glm::normalize(vec2(-y,x));
			float impactVelocity = glm::dot(m_v2LinearVelocity + (m_fAngularVelocity * offset * vec2(-y,x)), plane->m_v2Normal);
			//float impactVelocity = glm::dot(m_v2LinearVelocity + tangentialVelocity, plane->m_v2Normal);

			if ((distFromPlane > 0 && comFromPlane < 0 && impactVelocity > 0) // if inside going outward
				|| (distFromPlane < 0 && comFromPlane > 0 && impactVelocity < 0)) { // or outside going inward

				// register collision
				numContacts++;
				contact += corner;
				contactV += impactVelocity;

				if (comFromPlane >= 0) {
					if (penetration > distFromPlane)
						penetration = distFromPlane;
				}
				else
					if (penetration < distFromPlane)
						penetration = distFromPlane;
			}
		}
	}

	if (numContacts) {
		float collisionV = contactV / (float)numContacts;

		vec2 accel = -plane->m_v2Normal * ((1.0f + sm_fCoeffRestitution) * collisionV);

		vec2 localContact = (contact / (float)numContacts) - m_v2Position;

		float r = glm::dot(localContact, vec2(plane->m_v2Normal.y, -plane->m_v2Normal.x));

		float effectiveMass = 1.0f / (1.0f / m_fMass + (r*r) / m_fMoment);

		applyForce(accel * effectiveMass, localContact);
		m_v2Position -= plane->m_v2Normal * penetration;
	}

}

void Box::collideWithCircle(PhysicsObject* obj) {
	Circle* plane = dynamic_cast<Circle*>(obj);

	if (!plane) throw "Box::collideWithPlane() :: Invalid Argument: obj is not a Circle";
}

void Box::collideWithBox(PhysicsObject* obj) {
}

