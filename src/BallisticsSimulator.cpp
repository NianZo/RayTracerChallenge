#include "Tuple.hpp"
#include <iostream>


struct Environment
{
	Tuple gravity;
	Tuple wind;

	Environment(Tuple g, Tuple w) : gravity(g), wind(w) {};
};

struct Projectile
{
	Tuple position;
	Tuple velocity;

	Projectile(Tuple pos, Tuple vel) : position(pos), velocity(vel) {};

	void tick(Environment e, float dt)
	{
		this->position = this->position + this->velocity * dt;
		this->velocity = this->velocity +  e.gravity * dt + e.wind * dt;
	}
};

void PrintVectorHelper(const Tuple& v)
{
	std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
}

void RunSimulation(const float gravity, const float wind, const float startingHeight, const float startingXVelocity, const float deltaTime)
{
	Environment e = Environment(Vector(0.0, gravity, 0.0), Vector(wind, 0.0, 0.0));
	Projectile p = Projectile(Vector(0.0, startingHeight, 0.0), Vector(startingXVelocity, 0.0, 0.0));

	std::cout << "Environment gravity: ";
	PrintVectorHelper(e.gravity);
	std::cout << "Environment wind: ";
	PrintVectorHelper(e.wind);
	std::cout << "\n";

	while (p.position.y > 0.0)
	{
		std::cout << "Projectile position: ";
		PrintVectorHelper(p.position);
		std::cout << "Projectile velocity: ";
		PrintVectorHelper(p.velocity);
		std::cout << "\n";

		p.tick(e, deltaTime);
	}
}
