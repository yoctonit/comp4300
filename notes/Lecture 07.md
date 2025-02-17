[COMP4300 - Game Programming - Lecture 07 - Assignment 2](https://youtu.be/-ugbLQlw_VM?si=tHpGrcjkDbjppqsE)

Komentar: Kroz ovu lekciju koja objasnjava Zadatak 2 sam zaustavljajuci video, prepisao osnovni kod koji je dat studentima i koji treba da dopune i dorade da rese zadatak. Kod se nalazi u folderu assignment2 u comp4300 projektu. Jedino sto sam umesto Makefile-a koristio CMakeLists.txt file.
Kod za verziju kursa 2024-09 se malo razlikuje od verzije 2023-09.
Jedna od razlika (mada ne velika) je implementacija Komponenti (Components.h), koja se prenosi i na implementaciju Entity klase i dalje u koriscenju Entity klase.
Druga razlika je implementacija EntityManager klase. Opet, moze se reci ne velika.

Engine Architecture Progression
* Assignment 1
	* Getting used to C++/API
	* All functionality in main.cpp
* Assignment 2
	* Separation of functionality into Classes
	* Wrote our own Vec2 class for 2D game math
	* Appearance of ECS classes
	* EntityManager handles all Entity data
	* ECS Systems functionality in Game.h functions

A2 Engine Architecture
```
GameEngine
  > Systems
  > EntityManager
    > Entity
      > Component
```

Components
* UML diagrams start at 6:00
* Pure Data
* Stored in Entity
* Each Component is its own class

Entity
* Entity = Game Object
* Stores Component Tuple
* Tag = Entity 'type'
* Active = Alive or Dead
* ID = Integer identifier

EntityManager
* Entity 'Factory' Class
* Delayed Entity Add()
	* No Iterator Invalidation
* Secondary map from tag->entity
	* Trade storage for convenience/run time
* Can do other bookkeeping like mem mgmt

Game
* Top-Level Game Object
* Holds all game data
* All game system functions
* All gameplay code

Vec2
* 2D Math Structure
	* see previous lecture
* Templated Class
	* T = (x, y) variable type

What can this engine do?
* Create game objects as Entity instances
* Add Component data to Entities
* Implement game play via Systems
	* Also, handle user input
* Pause game play/exit game
* Can initialize/load configuration from file

What are the limitations?
* Can only display one 'scene'
* Can NOT load texture/sounds assets
* Can NOT display textured animations
* Does not have any menu/interface

Game Scene
* A game can contain many different 'scenes' that have very different logic/controls
* Example: RPG Games
	* Menu/Text/Dialogue Scene
	* World-Map Scene
	* Combat/Battle Scene
* How can our architecture allow for different game states?
