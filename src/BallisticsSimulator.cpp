#include "Canvas.hpp"
#include "Tuple.hpp"
#include <fstream>
#include <iostream>

struct Environment {
    Tuple gravity;
    Tuple wind;

    Environment(Tuple g, Tuple w) : gravity(g), wind(w){};
};

struct Projectile {
    Tuple position;
    Tuple velocity;

    Projectile(Tuple pos, Tuple vel) : position(pos), velocity(vel){};

    void tick(Environment e, float dt) {
        this->position = this->position + this->velocity * dt;
        this->velocity = this->velocity + e.gravity * dt + e.wind * dt;
    }
};

void PrintVectorHelper(const Tuple& v) {
    std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
}

void RunSimulation(const float gravity, const float wind, const float startingHeight, const float startingXVelocity, const float deltaTime) {
    Environment e = Environment(Vector(0.0, gravity, 0.0), Vector(wind, 0.0, 0.0));
    Projectile p = Projectile(Vector(0.0, startingHeight, 0.0), Vector(startingXVelocity, 0.0, 0.0));

    std::cout << "Environment gravity: ";
    PrintVectorHelper(e.gravity);
    std::cout << "Environment wind: ";
    PrintVectorHelper(e.wind);
    std::cout << "\n";

    Canvas c = Canvas(900, 550);
    while (p.position.y > 0.0f) {
        //std::cout << "Projectile position: ";
        //PrintVectorHelper(p.position);
        //std::cout << "Projectile velocity: ";
        //PrintVectorHelper(p.velocity);
        //std::cout << "\n";

        uint32_t xCoord = static_cast<uint32_t>(p.position.x);
        uint32_t yCoord = 550 - static_cast<uint32_t>(p.position.y);
        if (xCoord < 900 && yCoord < 550) {
            c.pixels[yCoord][xCoord] = Color(1.0f, .5f, 0.0f);
        }

        p.tick(e, deltaTime);
    }
    std::ofstream imageFile("simulation.ppm", std::ios::out);
    //imageFile.open("simulation.ppm", std::ios::out);
    imageFile << c.GetPPMString();
    //imageFile.close();
}
