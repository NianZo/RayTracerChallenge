/*
 * Sphere.hpp
 *
 *  Created on: Apr 25, 2021
 *      Author: nic
 */

#ifndef SRC_SPHERE_HPP_
#define SRC_SPHERE_HPP_

#include "Matrix.hpp"
#include "Material.hpp"

class Sphere
{
public:
	Matrix<4> transform;
	Material material;
	Sphere() {transform = IdentityMatrix();}

	bool operator==(const Sphere& other) const {return transform == other.transform && material == other.material;}
	Tuple normal(const Tuple& p) const;
};

class Intersection
{
public:
	// The actual fields must not be const or we can't sort a vector of hits based on intersection parameter 't'
	// However, object is fine because the pointer isn't const, the object it points to is const.
	float t;
	const Sphere* object;

	Intersection(const float tIn, const Sphere& objectIn) : t(tIn), object(&objectIn) {};
	bool operator==(const Intersection& other) const {return t == other.t && object == other.object;}
	bool operator<(const Intersection& other) const {return t < other.t;}
};

struct IntersectionDetails
{
	const Sphere& object;
	const Tuple point;
	const Tuple eyeVector;
	const Tuple normalVector;
	const float t;
	const bool inside;
};



#endif /* SRC_SPHERE_HPP_ */
