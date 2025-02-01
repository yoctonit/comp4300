[COMP4300 - Game Programming - Lecture 22 - Vertex Arrays and Particle Systems](https://youtu.be/hPeXrGTZ-8M?si=q2-hTL4B-LUVTrGi)

ESC and Particles
* Main ECS philosophy is that everything in our game is an Entity with Components
* ... except when it shouldn't be...
* It turns out that particle systems are a case where we would have objects in our game that are not necessarily Entities
* Let's explore why and how

Particle Systems
* It may be interesting to have a system in our game which is composed of particles
* Many small (similar looking) particles come together to form a complex system
* The behavior of a particle is usually simple
* The lifespan of a particle is usually short
* There can be MANY particles...
* Most modern game engines come with solutions for building particle systems
* User specifies the behavior and look of the particles, engine handles the details
* By varying parameters, users can get the desired effects from the particle system

Why not ECS for Particles
* It is possible to implement particles using our current ECS system, but inefficient
* Hundreds of thousands of particles may need to be created and destroyed per second
* Particles will usually not need all of the associated components of our ECS system
* Each particle stores minimal data

Particle System Architecture
* Memory Field: Contiguous block of memory allocated for particle storage
* Renderer: Define how particles drawn
* Initializer: Define initial particle data
* Emitter: Define how particles created
* Forces: Define physics of the particles
* Possibly others, depending on complexity

Drawing Particles Quickly
* So far in this course we have been drawing each Entity with its own draw() call
* Separate draw calls from the CPU to the GPU can be very time consuming
* Ideally we would want a way to send all data from CPU to GPU and tell the GPU to draw all the particles in one instruction

Vertex Arrays
* Vertex Arrays are a way to send a single GPU instruction to draw many things
* There are two parts to a vertex array draw
* Vertex Data: Position / color of vertices
* Primitive Type: Type of shapes to draw
* We will use a Vertex Array to draw all of our particles in one draw command
* [SFML Vertex Array Tutorial](https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php)

 Making a Particle System
* We will create a class called `ParticleSystem`
* This class will have the following:
	* Contiguous vector of Particle data: Velocity, Lifetime, Speed, Steering, etc.
	* Vertex Array position/color data
	* Some data about the game world/window
	* Functions that let us initialize, reset, update and draw the particles

```
// main.cpp
#include <SFML/Window.hpp>
#include <iostream>
#include "ParticleSystem.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920,1080), "Particle System");
	window.setFramerateLimit(60);
	
	ParticleSystem particles;
	particles.init(window.getSize());
	
	while (window.isOpen())
	{
		// Handle User Input
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed) { window.close(); }
			if (e.type == sf::Event::KeyPressed) { particles.resetParticles(); }
		}
		
		// Update the particle system
		particles.update();
		
		// Clear the window
		window.clear(sf::Color::Black);
		
		// Draw the particles
		particles.draw(window);
		
		window.display();
	}
}

// ParticleSystem.hpp
#pragme once

#include <SFML/Graphics.hpp>
#include <vector>

class ParticleSystem
{
	struct Particle
	{
		sf::Vector2f velocity;
		int lifetime = 0;
		// int torchIndex = 0;
	}
	
	std::vector<Particle> m_particles;
	sf::VertexArray       m_vertices;
	sf::Vector2u          m_windowSize;
	float                 m_size = 8;
	
	// sf::Vector2f          m_torches[3];
	
	void resetParticle(std::size_t index, bool first = false)
	{
		// float mx = m_torches[m_particles[index].torchIndex].x;
		// float my = m_torches[m_particles[index].torchIndex].y;
		
		// give the particle an initial position
		float mx = m_windowSize.x / 2;
		float my = m_windowSize.y / 2;
		
		m_vertices[4 * index + 0].position = sf::Vector2f(mx, my);
		m_vertices[4 * index + 1].position = sf::Vector2f(mx + m_size, my);
		m_vertices[4 * index + 2].position = sf::Vector2f(mx + m_size, my + m_size);
		m_vertices[4 * index + 3].position = sf::Vector2f(mx, my + m_size);

		// give the particle a color
		sf::Color color(255, 0, 255, 255); // purple
		// color = sf::Color(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
		color = sf::Color(128 + rand() % 128, 55, 55, rand() % 255);
		
		if (first) { color.a = 0; }
		
		m_vertices[4 * index + 0].color = color;
		m_vertices[4 * index + 1].color = color;
		m_vertices[4 * index + 2].color = color;
		m_vertices[4 * index + 3].color = color;

		// give the particle a velocity
		float rx = ((float)rand() / RAND_MAX) * 10.0f - 5.0f;
		float ry = ((float)rand() / RAND_MAX) * 10.0f - 5.0f;
		m_particles[index].velocity = sf::Vector2f(rx, ry);
		
		// give the particle a lifespan
		m_particles[index].lifetime = 30 + rand() % 60;
	}

public:
	ParticleSystem()
	{

	}

	void resetParticles(size_t count = 1024, float size = 8)
	{
		m_particles = std::vector<Particle>(count);
		m_vertices = sf::VertexArray(sf::Quads, count * 4);
		m_size = size;
		
		for (size_t p = 0; p < m_particles.size(); p++)
		{
			resetParticle(p, true);
		}
	}

	void init(sf::Vector2u windowSize)
	{
		m_windowSize = windowSize;
		resetParticles();
	}
	
	void update()
	{
		for (std::size_t i = 0; i < m_particles.size(); i++)
		{
			if (m_particles[i].lifetime == 0)
			{
				resetParticle(i);
			}
			m_particles[i].lifetime--;
			
			m_vertices[4 * i + 0].position += m_particles[i].velocity;
			m_vertices[4 * i + 1].position += m_particles[i].velocity;
			m_vertices[4 * i + 2].position += m_particles[i].velocity;
			m_vertices[4 * i + 3].position = += m_particles[i].velocity;
		}
	}
	
	void draw(sf::RenderWindow& window) const
	{
		window.draw(m_vertices);
	}

};
```
