
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
	m_fMoment = 4.0f * m_fMass * m_v2Size.x * m_v2Size.y / 12.0f;
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

	glm::mat4 transform = glm::eulerAngleZ(m_fRotation);

	Gizmos::add2DAABBFilled(m_v2Position, m_v2Size, glm::vec4(1,0,0,1), &transform);

	vec2 localX = glm::vec2(transform * glm::vec4(1, 0, 0, 0)); // Rotated X axis
	vec2 localY = glm::vec2(transform * glm::vec4(0, 1, 0, 0)); // Rotated Y axis

	for (float x = -m_v2Size.x; x <= m_v2Size.x; x += 2 * m_v2Size.x) {
		for (float y = -m_v2Size.y; y <= m_v2Size.y; y += 2 * m_v2Size.y) {

			// find world position
			vec2 offset = x * localX + y * localY;
			vec2 corner = m_v2Position + offset;
			Gizmos::add2DCircle(corner , 0.1f, 16, pointColour(x,y));
		}
	}
	
}


void Box::collideWithPlane(PhysicsObject* obj) {

	Plane* plane = dynamic_cast<Plane*>(obj);

	if (!plane) throw "Box::collideWithPlane() :: Invalid Argument: obj is not a Plane";


	glm::mat4 transform = glm::eulerAngleZ(m_fRotation);
	vec2 localX = glm::vec2(transform * glm::vec4(1, 0, 0, 0));
	vec2 localY = glm::vec2(transform * glm::vec4(0, 1, 0, 0));


	int numContacts = 0;
	vec2 contact(0, 0);
	float contactV = 0;
	float radius = glm::length(m_v2Size);

	float comFromPlane = glm::dot(m_v2Position - plane->m_v2Position, plane->m_v2Normal);
	float penetration = 0;
	
	//if (radius < std::fabsf(comFromPlane)) return; // No Contact

	vec2 offset;

	// For each corner
	for (float x = -m_v2Size.x; x <= m_v2Size.x; x += 2 * m_v2Size.x) {
		for (float y = -m_v2Size.y; y <= m_v2Size.y; y += 2 * m_v2Size.y) {

			// find world position
			vec2 offset = x * localX + y * localY;
			vec2 corner = m_v2Position + offset;


			// find distance from plane and point-relative velocity
			float distFromPlane = glm::dot(corner - plane->m_v2Position, plane->m_v2Normal);
			vec2 tangentialVelocity = radius * m_fAngularVelocity * glm::normalize(vec2(-offset.y,offset.x));
			//float impactVelocity = glm::dot(m_v2LinearVelocity + (m_fAngularVelocity * offset * vec2(-y,x)), plane->m_v2Normal);
			float impactVelocity = glm::dot(m_v2LinearVelocity + tangentialVelocity, plane->m_v2Normal);

			if ((distFromPlane > 0 && comFromPlane < 0 && impactVelocity > 0) // if inside going outward
				|| (distFromPlane < 0 && comFromPlane > 0 && impactVelocity < 0)) { // or outside going inward

				// register collision
				numContacts++;
				contact += offset;
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

		vec2 localContact = (contact / (float)numContacts);// -m_v2Position;

		float r = glm::dot(localContact, vec2(plane->m_v2Normal.y, -plane->m_v2Normal.x));

		float effectiveMass = 1.0f / (1.0f / m_fMass + (r*r) / m_fMoment);

		applyForce(accel * effectiveMass, localContact);
		m_v2Position -= plane->m_v2Normal * penetration;
	}

}

void Box::collideWithCircle(PhysicsObject* obj) {
	Circle* circle = dynamic_cast<Circle*>(obj);

	if (!circle) throw "Box::collideWithPlane() :: Invalid Argument: obj is not a Circle";

	glm::mat4 transform = glm::eulerAngleZ(m_fRotation);
	vec2 localX = glm::vec2(transform * glm::vec4(1, 0, 0, 1)); // Rotated X axis
	vec2 localY = glm::vec2(transform * glm::vec4(0, 1, 0, 1)); // Rotated Y axis

	glm::vec2 circlePos = circle->m_v2Position - m_v2Position; // Circle position in rotated-Box space
	float w2 = m_v2Size.x, 
		  h2 = m_v2Size.y;
	int numContacts = 0;

	// contact is in our box coordinates
	glm::vec2 contact(0, 0);

	// check the four corners to see if any of them are inside the circle
	for (float x = -m_v2Size.x; x <= m_v2Size.x; x += 2*m_v2Size.x) {
		for (float y = -m_v2Size.y; y <= m_v2Size.y; y += 2*m_v2Size.y) {

			// find world position
			vec2 offset = x * localX + y * localY;
			//vec2 corner = m_v2Position + offset;
			vec2 distance = offset - circlePos;

			
			if (glm::length2(distance) < circle->m_fRadius*circle->m_fRadius)
			{
				numContacts++;
				contact += offset;
			}
		}
	}

	// Check 4 planes
	glm::vec2* direction = NULL;

	// get the local position of the circle centre
	glm::vec2 localPos = vec2( glm::dot(localX, circlePos), glm::dot(localY, circlePos));
	if (localPos.y < m_v2Size.y && localPos.y > -m_v2Size.y) // Centre inside height
	{
		if (localPos.x > 0 && localPos.x < m_v2Size.x + circle->m_fRadius) // Centre right of Box->Centre and within radius
		{
			numContacts++;
			contact += m_v2Size.x * localX + localPos.y * localY; // Contact back in world space!
			direction = new glm::vec2(localX);
		}
		if (localPos.x < 0 && localPos.x > -(m_v2Size.x + circle->m_fRadius)) // Centre left of box centre and within radius
		{
			numContacts++;
			contact += -m_v2Size.x * localX + localPos.y * localY; // Contact back in world space!
			direction = new glm::vec2(-localX);
		}
	}
	if (localPos.x < m_v2Size.x && localPos.x > -m_v2Size.x) // Centre Inside width
	{
		if (localPos.y > 0 && localPos.y < m_v2Size.y + circle->m_fRadius)
		{
			numContacts++;
			contact += localPos.x * localX + m_v2Size.y * localY; // Contact back in world space!
			direction = new glm::vec2(localY);
		}
		if (localPos.y < 0 && localPos.y > -(m_v2Size.y + circle->m_fRadius))
		{
			numContacts++;
			contact += localPos.x * localX + -m_v2Size.y * localY; // Contact back in world space!
			direction = new glm::vec2(-localY);
		}
	}

	if (numContacts > 0)
	{
		// average, and convert back into world coords
		contact = m_v2Position + (contact / numContacts);
		resolveCollision(circle, contact, direction);
	}

	delete direction;

}

void Box::collideWithBox(PhysicsObject* obj) {
}

