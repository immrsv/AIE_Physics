
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

glm::vec4 pointColour(float x, float y) {
	if (x < 0)
		if (y < 0) return glm::vec4(1, 1, 1, 0); // -1, -1, BLACK
		else return glm::vec4(1, 1, 0, 1); // -1, 1, YELLOW
	else
		if (y < 0) return glm::vec4(1, 0, 1, 1); // 1, -1, 
		else return glm::vec4(0, 1, 1, 1); // 1, 1

	return glm::vec4(1, 1, 1, 1);
}

void Box::draw() {
	glm::vec4 color = m_bIsAwake ? glm::vec4(1, 0, 0, 1) : glm::vec4(0.4, 0, 0, 1);

	glm::mat4 transform = glm::eulerAngleZ(m_fRotation);

	Gizmos::add2DAABBFilled(m_v2Position, m_v2Size, color, &transform);

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

	if (!circle) throw "Box::collideWithCircle() :: Invalid Argument: obj is not a Circle";

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

	Box* box = dynamic_cast<Box*>(obj);

	if (!box) throw "Box::collideWithBox() :: Invalid Argument: obj is not a Box";


	glm::vec2 boxPos = box->m_v2Position - m_v2Position;
	{
		glm::vec2 norm;
		glm::vec2 contact;
		float pen = 0;
		int numContacts = 0;

		bool thatHitThis = checkBoxCorners(box, contact, numContacts, pen, norm);
		bool thisHitThat = box->checkBoxCorners(this, contact, numContacts, pen, norm);
		
		if (thisHitThat)
			norm = -norm;

		if (pen > 0)
		{
			resolveCollision(box, contact / float(numContacts), &norm);
			float numDynamic = 0.0f + (m_bIsKinematic ? 0 : 1) + (box->m_bIsKinematic ? 0 : 1);
			if (numDynamic > 0)
			{
				glm::vec2 contactForce = norm * pen / numDynamic;
				if (!m_bIsKinematic)
					m_v2Position -= contactForce;

				if (!box->m_bIsKinematic)
					box->m_v2Position += contactForce;
			}
		}
	}
}

bool checkEdgePenetration(float checkPenetration, vec2 checkNormal, float &edgePenetration, vec2 &edgeNormal) {
	if (checkPenetration > 0 && (checkPenetration < edgePenetration || edgePenetration == 0))
	{
		edgeNormal = checkNormal;
		edgePenetration = checkPenetration;
		return true;
	}
	return false;
}

bool Box::checkBoxCorners(Box* box, vec2& contact, int& numContacts, float& penetration, vec2& edgeNormal) {

	glm::mat4 transform = glm::eulerAngleZ(m_fRotation);
	vec2 localX = glm::vec2(transform * glm::vec4(1, 0, 0, 0)); // Rotated X axis
	vec2 localY = glm::vec2(transform * glm::vec4(0, 1, 0, 0)); // Rotated Y axis

	glm::mat4 boxTransform = glm::eulerAngleZ(box->m_fRotation);
	vec2 boxLocalX = glm::vec2(boxTransform * glm::vec4(1, 0, 0, 0)); // Rotated X axis
	vec2 boxLocalY = glm::vec2(boxTransform * glm::vec4(0, 1, 0, 0)); // Rotated Y axis

	vec2 min, max;

	int numLocalContacts = 0;
	vec2 localContact;

	bool first = true;

	// For each box corner
	for (float x = -box->m_v2Size.x; x <= box->m_v2Size.x; x += 2 * box->m_v2Size.x)
	for (float y = -box->m_v2Size.y; y <= box->m_v2Size.y; y += 2 * box->m_v2Size.y)
	{

		vec2 boxOffset = x*boxLocalX + y*boxLocalY; // position in other rotated-box space
		glm::vec2 boxCorner = box->m_v2Position + boxOffset; // position in worldspace

		// position in this local box space
		glm::vec2 localOffset(glm::dot(boxCorner - m_v2Position, localX), glm::dot(boxCorner - m_v2Position, localY)); // position in our box's space
			
		if (first || localOffset.x < min.x) min.x = localOffset.x;
		if (first || localOffset.x > max.x) max.x = localOffset.x;
		if (first || localOffset.y < min.y) min.y = localOffset.y;
		if (first || localOffset.y > max.y) max.y = localOffset.y;


		if (localOffset.x >= -m_v2Size.x && localOffset.x <= m_v2Size.x && localOffset.y >= -m_v2Size.y && localOffset.y <= m_v2Size.y)
		{
			numLocalContacts++;
			localContact += localOffset;
		}

		first = false;

	}
	

	if (max.x < -m_v2Size.x || min.x >m_v2Size.x || max.y <-m_v2Size.y || min.y > m_v2Size.y)
		return false;

	if (numLocalContacts == 0)
		return false;

	contact += m_v2Position + (localContact.x*localX + localContact.y*localY) / (float)numLocalContacts;
	numContacts++;

	bool hasPenetrated = false;

	hasPenetrated |= checkEdgePenetration(m_v2Size.x - min.x, localX, penetration, edgeNormal); // Right Side
	hasPenetrated |= checkEdgePenetration(m_v2Size.x + max.x, -localX, penetration, edgeNormal); // Right Side
	hasPenetrated |= checkEdgePenetration(m_v2Size.y - min.y, localY, penetration, edgeNormal); // Top Side
	hasPenetrated |= checkEdgePenetration(m_v2Size.y + max.y, -localY, penetration, edgeNormal); // Bottom Side

	return hasPenetrated;
}

