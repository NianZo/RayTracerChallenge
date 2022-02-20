/*
 * Sphere.cpp
 *
 *  Created on: Sep 17, 2021
 *      Author: nic
 */

#include "Shape.hpp"
#include "Ray.hpp"

#include <cmath>

Tuple Shape::normal(const Tuple& p, const Intersection& i) const noexcept
{
    const Matrix<4> fullTransformInverse = getFullTransform().inverse();
    // This is technically a hack and messes with w; set it to 0 at the end;
    Tuple worldSpaceNormal = fullTransformInverse.transpose() * objectNormal(fullTransformInverse * p, i);
    worldSpaceNormal.w = 0.0F;
    return worldSpaceNormal.normalize();
}

std::vector<Intersection> Shape::intersect(const Ray& r) const noexcept
{
    return objectIntersect(r.transform(transform.inverse()));
}

Color Shape::shade(const Light& light, const Tuple& position, const Tuple& eyeVector, const bool inShadow) const noexcept
{
    const Light objectLight = {getFullTransform().inverse() * light.position, light.intensity};
    const Tuple objectPosition = getFullTransform().inverse() * position;
    return material.light(objectLight, objectPosition, eyeVector, normal(position), inShadow);
}

Matrix<4> Shape::getFullTransform() const noexcept
{
    return parent != nullptr ? parent->getFullTransform() * transform : transform;
}

// Implicitly assumes that p is on the sphere surface and is a valid point (w = 1)
Tuple Sphere::objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept
{
    return (p - Point(0, 0, 0));
}

std::vector<Intersection> Sphere::objectIntersect(const Ray& r) const noexcept
{
    // const Ray ray2 = r.transform(this->transform.inverse());
    const Tuple sphereToRay = r.origin - Point(0, 0, 0);

    const float a = r.direction.dot(r.direction);
    const float b = 2 * r.direction.dot(sphereToRay);
    const float c = sphereToRay.dot(sphereToRay) - 1;

    const float discriminant = b * b - 4 * a * c;

    std::vector<Intersection> intersections;
    if (discriminant >= 0)
    {
        intersections.emplace_back(Intersection((-b - sqrtf(discriminant)) / (2 * a), this));
        intersections.emplace_back(Intersection((-b + sqrtf(discriminant)) / (2 * a), this));
    }

    return intersections;
}

Tuple Plane::objectNormal([[maybe_unused]] const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept
{
    return Vector(0, 1, 0);
}

std::vector<Intersection> Plane::objectIntersect([[maybe_unused]] const Ray& r) const noexcept
{
    std::vector<Intersection> i;
    if (std::abs(r.direction.y) > TUPLE_EPSILON)
    {
        const float t = -r.origin.y / r.direction.y;
        i.emplace_back(Intersection(t, this));
    }
    return i;
}

Tuple Cube::objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept
{
    const float maxCoord = std::max(std::max(std::abs(p.x), std::abs(p.y)), std::abs(p.z));

    Tuple normal;
    if (maxCoord == std::abs(p.x))
    {
        normal = Vector(p.x, 0, 0);
    } else if (maxCoord == std::abs(p.y))
    {
        normal = Vector(0, p.y, 0);
    } else
    {
        normal = Vector(0, 0, p.z);
    }
    return normal;
}

std::vector<Intersection> Cube::objectIntersect(const Ray& r) const noexcept
{
    float xTMin = (-1.0F - r.origin.x) / r.direction.x;
    float xTMax = (1.0F - r.origin.x) / r.direction.x;
    if (xTMin > xTMax)
    {
        std::swap(xTMin, xTMax);
    }

    float yTMin = (-1.0F - r.origin.y) / r.direction.y;
    float yTMax = (1.0F - r.origin.y) / r.direction.y;
    if (yTMin > yTMax)
    {
        std::swap(yTMin, yTMax);
    }

    float zTMin = (-1.0F - r.origin.z) / r.direction.z;
    float zTMax = (1.0F - r.origin.z) / r.direction.z;
    if (zTMin > zTMax)
    {
        std::swap(zTMin, zTMax);
    }

    const float tMin = std::max(std::max(xTMin, yTMin), zTMin);
    const float tMax = std::min(std::min(xTMax, yTMax), zTMax);

    std::vector<Intersection> i;
    if (tMax > tMin)
    {
        i.emplace_back(Intersection(tMin, this));
        i.emplace_back(Intersection(tMax, this));
    }
    return i;
}

// Must have constructor definition in source file since infinity has an incomplete type
Cylinder::Cylinder() noexcept : minimum(-std::numeric_limits<float>::infinity()), maximum(std::numeric_limits<float>::infinity()){};

Tuple Cylinder::objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept
{
    const float dist = p.x * p.x + p.z * p.z;
    Tuple normal;

    if (dist < 1.0F && p.y >= maximum - TUPLE_EPSILON)
    {
        normal = Vector(0, 1, 0);
    } else if (dist < 1.0F && p.y <= minimum + TUPLE_EPSILON)
    {
        normal = Vector(0, -1, 0);
    } else
    {
        normal = Vector(p.x, 0, p.z);
    }

    return normal;
}

std::vector<Intersection> Cylinder::objectIntersect(const Ray& r) const noexcept
{
    std::vector<Intersection> i;

    // Calculate discriminant
    const float a = r.direction.x * r.direction.x + r.direction.z * r.direction.z;
    const float b = 2 * r.origin.x * r.direction.x + 2 * r.origin.z * r.direction.z;
    const float c = r.origin.x * r.origin.x + r.origin.z * r.origin.z - 1;
    const float discriminant = b * b - 4 * a * c;

    if (std::abs(a) > TUPLE_EPSILON && discriminant >= 0)
    {
        const float t0 = (-b - sqrtf(discriminant)) / (2 * a);
        const float t1 = (-b + sqrtf(discriminant)) / (2 * a);

        const float y0 = r.origin.y + t0 * r.direction.y;
        if (y0 > minimum && y0 < maximum)
        {
            i.emplace_back(Intersection(t0, this));
        }

        const float y1 = r.origin.y + t1 * r.direction.y;
        if (y1 > minimum && y1 < maximum)
        {
            i.emplace_back(Intersection(t1, this));
        }
    }

    if (closed)
    {
        const float tMin = (minimum - r.origin.y) / r.direction.y;
        if (std::pow(r.origin.x + tMin * r.direction.x, 2.0F) + std::pow(r.origin.z + tMin * r.direction.z, 2.0F) <= 1.0F)
        {
            i.emplace_back(Intersection(tMin, this));
        }

        const float tMax = (maximum - r.origin.y) / r.direction.y;
        if (std::pow(r.origin.x + tMax * r.direction.x, 2.0F) + std::pow(r.origin.z + tMax * r.direction.z, 2.0F) <= 1.0F)
        {
            i.emplace_back(Intersection(tMax, this));
        }
    }

    return i;
}

Cone::Cone() noexcept : minimum(-std::numeric_limits<float>::infinity()), maximum(std::numeric_limits<float>::infinity()){};

Tuple Cone::objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept
{
    const float dist = p.x * p.x + p.z * p.z;
    Tuple normal;

    if (closed && dist < maximum * maximum && p.y >= maximum - TUPLE_EPSILON)
    {
        normal = Vector(0, 1, 0);
    } else if (closed && dist < minimum * minimum && p.y <= minimum + TUPLE_EPSILON)
    {
        normal = Vector(0, -1, 0);
    } else
    {
        float y = sqrtf(p.x * p.x + p.z * p.z);
        y = p.y > 0 ? -y : y;
        normal = Vector(p.x, y, p.z);
    }

    return normal;
}

std::vector<Intersection> Cone::objectIntersect(const Ray& r) const noexcept
{
    std::vector<Intersection> i;

    // Calculate discriminant
    const float a = r.direction.x * r.direction.x - r.direction.y * r.direction.y + r.direction.z * r.direction.z;
    const float b = 2 * r.origin.x * r.direction.x - 2 * r.origin.y * r.direction.y + 2 * r.origin.z * r.direction.z;
    const float c = r.origin.x * r.origin.x - r.origin.y * r.origin.y + r.origin.z * r.origin.z;
    float discriminant = b * b - 4 * a * c;
    if (std::abs(discriminant) < MATRIX_EPSILON)
    {
        discriminant = 0.0F;
    }

    if (std::abs(a) > TUPLE_EPSILON && discriminant >= 0)
    {
        const float t0 = (-b - sqrtf(discriminant)) / (2 * a);
        const float t1 = (-b + sqrtf(discriminant)) / (2 * a);

        const float y0 = r.origin.y + t0 * r.direction.y;
        if (y0 > minimum && y0 < maximum)
        {
            i.emplace_back(Intersection(t0, this));
        }

        const float y1 = r.origin.y + t1 * r.direction.y;
        if (y1 > minimum && y1 < maximum)
        {
            i.emplace_back(Intersection(t1, this));
        }
    } else if (std::abs(a) < TUPLE_EPSILON)
    {
        const float t = -c / (2 * b);
        i.emplace_back(Intersection(t, this));
    }

    if (closed)
    {
        const float tMin = (minimum - r.origin.y) / r.direction.y;
        if (std::pow(r.origin.x + tMin * r.direction.x, 2.0F) + std::pow(r.origin.z + tMin * r.direction.z, 2.0F) <= minimum * minimum)
        {
            i.emplace_back(Intersection(tMin, this));
        }

        const float tMax = (maximum - r.origin.y) / r.direction.y;
        if (std::pow(r.origin.x + tMax * r.direction.x, 2.0F) + std::pow(r.origin.z + tMax * r.direction.z, 2.0F) <= maximum * maximum)
        {
            i.emplace_back(Intersection(tMax, this));
        }
    }

    return i;
}

Triangle::Triangle(const Tuple& v1, const Tuple& v2, const Tuple& v3) noexcept
{
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;

    edges[0] = vertices[1] - vertices[0];
    edges[1] = vertices[2] - vertices[0];

    normalVector = edges[1].cross(edges[0]).normalize();
}

Tuple Triangle::objectNormal([[maybe_unused]] const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept
{
    return normalVector;
}

std::vector<Intersection> Triangle::objectIntersect(const Ray& r) const noexcept
{

    const Tuple directionCrossE1 = r.direction.cross(edges[1]);
    const float determinant = edges[0].dot(directionCrossE1);
    if (std::abs(determinant) < TUPLE_EPSILON)
    {
        return {};
    }

    const float determinantInverse = 1.0F / determinant;
    const Tuple v0ToOrigin = r.origin - vertices[0];
    const float u = determinantInverse * v0ToOrigin.dot(directionCrossE1);
    if (u < 0.0F || u > 1.0F)
    {
        return {};
    }

    const Tuple originCrossE0 = v0ToOrigin.cross(edges[0]);
    const float v = determinantInverse * r.direction.dot(originCrossE0);
    if (v < 0.0F || (u + v) > 1.0F)
    {
        return {};
    }

    const float t = determinantInverse * edges[1].dot(originCrossE0);
    return {Intersection(t, this)};
}

SmoothTriangle::SmoothTriangle(const Tuple& v1, const Tuple& v2, const Tuple& v3, const Tuple& n1, const Tuple& n2, const Tuple& n3) noexcept
{
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;

    normals[0] = n1;
    normals[1] = n2;
    normals[2] = n3;

    edges[0] = vertices[1] - vertices[0];
    edges[1] = vertices[2] - vertices[0];
}

Tuple SmoothTriangle::objectNormal([[maybe_unused]] const Tuple& p, const Intersection& i) const noexcept
{
    Tuple interpolatedNormal = normals[0] * (1 - i.u - i.v) + normals[1] * i.u + normals[2] * i.v;
    return interpolatedNormal;
}

std::vector<Intersection> SmoothTriangle::objectIntersect(const Ray& r) const noexcept
{
    const Tuple directionCrossE1 = r.direction.cross(edges[1]);
    const float determinant = edges[0].dot(directionCrossE1);
    if (std::abs(determinant) < TUPLE_EPSILON)
    {
        return {};
    }

    const float determinantInverse = 1.0F / determinant;
    const Tuple v0ToOrigin = r.origin - vertices[0];
    const float u = determinantInverse * v0ToOrigin.dot(directionCrossE1);
    if (u < 0.0F || u > 1.0F)
    {
        return {};
    }

    const Tuple originCrossE0 = v0ToOrigin.cross(edges[0]);
    const float v = determinantInverse * r.direction.dot(originCrossE0);
    if (v < 0.0F || (u + v) > 1.0F)
    {
        return {};
    }

    const float t = determinantInverse * edges[1].dot(originCrossE0);
    return {Intersection(t, this, u, v)};
}

std::vector<std::reference_wrapper<const Shape>> Group::objects() const noexcept
{
    std::vector<std::reference_wrapper<const Shape>> objects;

    for (const Shape& group : groups)
    {
        objects.emplace_back(std::ref(group));
    }
    for (const Shape& sphere : spheres)
    {
        objects.emplace_back(std::ref(sphere));
    }
    for (const Shape& plane : planes)
    {
        objects.emplace_back(std::ref(plane));
    }
    for (const Shape& cube : cubes)
    {
        objects.emplace_back(std::ref(cube));
    }
    for (const Shape& cylinder : cylinders)
    {
        objects.emplace_back(std::ref(cylinder));
    }
    for (const Shape& cone : cones)
    {
        objects.emplace_back(std::ref(cone));
    }
    for (const Shape& triangle : triangles)
    {
        objects.emplace_back(std::ref(triangle));
    }
    for (const Shape& smoothTriangle : smoothTriangles)
    {
        objects.emplace_back(std::ref(smoothTriangle));
    }

    return objects;
}

std::vector<std::reference_wrapper<const Shape>> Group::allSubObjects() const noexcept
{
    std::vector<std::reference_wrapper<const Shape>> objects;

    for (const Shape& sphere : spheres)
    {
        objects.emplace_back(std::ref(sphere));
    }
    for (const Shape& plane : planes)
    {
        objects.emplace_back(std::ref(plane));
    }
    for (const Shape& cube : cubes)
    {
        objects.emplace_back(std::ref(cube));
    }
    for (const Shape& cylinder : cylinders)
    {
        objects.emplace_back(std::ref(cylinder));
    }
    for (const Shape& cone : cones)
    {
        objects.emplace_back(std::ref(cone));
    }
    for (const Shape& triangle : triangles)
    {
        objects.emplace_back(std::ref(triangle));
    }
    for (const Shape& smoothTriangle : smoothTriangles)
    {
        objects.emplace_back(std::ref(smoothTriangle));
    }
    for (const Shape& group : groups)
    {
        const std::vector<std::reference_wrapper<const Shape>> shapeIntersections = group.allSubObjects();
        objects.insert(objects.end(), shapeIntersections.begin(), shapeIntersections.end());
    }
    for (const Shape& csg : csgs)
    {
        const std::vector<std::reference_wrapper<const Shape>> shapeIntersections = csg.allSubObjects();
        objects.insert(objects.end(), shapeIntersections.begin(), shapeIntersections.end());
    }

    return objects;
}

Group& Group::addChild(const Group& c) noexcept
{
    groups.push_back(c);
    groups.back().parent = this;
    return groups.back();
}

Sphere& Group::addChild(const Sphere& c) noexcept
{
    spheres.push_back(c);
    spheres.back().parent = this;
    return spheres.back();
}

Plane& Group::addChild(const Plane& c) noexcept
{
    planes.push_back(c);
    planes.back().parent = this;
    return planes.back();
}
Cube& Group::addChild(const Cube& c) noexcept
{
    cubes.push_back(c);
    cubes.back().parent = this;
    return cubes.back();
}
Cylinder& Group::addChild(const Cylinder& c) noexcept
{
    cylinders.push_back(c);
    cylinders.back().parent = this;
    return cylinders.back();
}

Cone& Group::addChild(const Cone& c) noexcept
{
    cones.push_back(c);
    cones.back().parent = this;
    return cones.back();
}

Triangle& Group::addChild(const Triangle& t) noexcept
{
    triangles.push_back(t);
    triangles.back().parent = this;
    return triangles.back();
}

SmoothTriangle& Group::addChild(const SmoothTriangle& st) noexcept
{
    smoothTriangles.push_back(st);
    smoothTriangles.back().parent = this;
    return smoothTriangles.back();
}

CSG& Group::addChild(const CSG& csg) noexcept
{
    csgs.push_back(csg);
    csgs.back().parent = this;
    return csgs.back();
}

Tuple Group::objectNormal([[maybe_unused]] const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept
{
    return Vector(0, 0, 0); // this should never be called, so return a clearly invalid vector
}

std::vector<Intersection> Group::objectIntersect(const Ray& r) const noexcept
{
    std::vector<Intersection> intersections;

    for (const std::reference_wrapper<const Shape> shape : objects())
    {
        const std::vector<Intersection> shapeIntersections = shape.get().intersect(r);
        intersections.insert(intersections.end(), shapeIntersections.begin(), shapeIntersections.end());
    }
    return intersections;
}

std::vector<std::reference_wrapper<const Shape>> CSG::allSubObjects() const noexcept
{
    auto leftObjects = left->allSubObjects();
    auto rightObjects = right->allSubObjects();
    leftObjects.insert(leftObjects.end(), rightObjects.begin(), rightObjects.end());
    return leftObjects;
}

Tuple CSG::objectNormal([[maybe_unused]] const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept
{
    return {0, 0, 0, 0};
}

std::vector<Intersection> CSG::objectIntersect([[maybe_unused]] const Ray& r) const noexcept
{
    return {};
}

bool CSG::intersectionAllowed(int operation, bool lhit, bool inl, bool inr)
{
    bool allowed = false;
    if (operation == CSG::Operation::Union)
    {
        allowed = (lhit && !inr) || (!lhit && !inl);
    } else if (operation == CSG::Operation::Intersect)
    {
        allowed = (lhit && inr) || (!lhit && inl);
    } else if (operation == CSG::Operation::Difference)
    {
        allowed = (lhit && !inr) || (!lhit && inl);
    }

    return allowed;
}

std::vector<Intersection> CSG::filterIntersections(const std::vector<Intersection>& intersections) const noexcept
{
    bool inl = false;
    bool inr = false;
    std::vector<Intersection> filteredIntersections;

    for (const auto& intersection : intersections)
    {
        //		bool lhit = false;
        //		for (std::reference_wrapper<const Shape> subObject : left->allSubObjects())
        //		{
        //			if (subObject.get() == *intersection.object)
        //			{
        //				lhit = true;
        //				//break;
        //			}
        //		}
        auto leftSubObjects = left->allSubObjects();
        const bool lhit = any_of(leftSubObjects.cbegin(), leftSubObjects.cend(), [&](std::reference_wrapper<const Shape> s) { return s.get() == *intersection.object; });

        if (intersectionAllowed(operation, lhit, inl, inr))
        {
            filteredIntersections.push_back(intersection);
        }

        if (lhit)
        {
            inl = !inl;
        } else
        {
            inr = !inr;
        }
    }
    return filteredIntersections;
}

Sphere GlassSphere() noexcept
{
    Sphere s;
    s.material.transparency = 1.0F;
    s.material.refractiveIndex = 1.5F;
    return s;
}
