[COMP4300 - Game Programming - Lecture 01 - Course Introduction](https://youtu.be/Eoq12SNaWI8?si=1gnxWtS_oKb1_UYR)

Komentar: gledao sam verziju kursa 2023-09. Kada sam gledao tu verziju poceo sam da hvatam beleske, odnosno da prepisujem sa ekrana od lekcije 13. Sada sam se vratio ovom kursu jer nisam do kraja uradio zadatke, a hteo bih da ih uradim bolje. Medjutim, sada je izasla verzija kursa 2024-09. Mislim da ce biti vrlo slicna predhodnoj verziji, ali sada cu gledati ovu noviju verziju i hvatacu beleske od lekcije 1 zakljucno sa lekcijom 12. To je plan. Ali najvazniji plan je da uradim zadatke bolje. Mislim na zadatke iz predhodne verzije. Naime zadaci nisu podeljeni javno, nego sam se potrudio da kod prepisujem sa ekrana dok ga objasnjava. To je bio spor posao, ali uspeo sam. Pomoglo je i to sto su neki ljudi kacili na github svoja resenja zadataka, pa sam tu video neke delove koda koje nisam uspeo da prepisem sa ekrana, jer ih nije prikazivao nego ih je samo opisno pomenuo. Uglavnom ako su zadaci malo razliciti u novoj verziji (2024-09), necu ponovo prepisivati kod sa ekrana, nego cu se fokusirati na zadatke iz verzije 2023-09.

Intro to Game Programming

* This course is:
	* Programming that makes video games possible
	* Game Engine / Gameplay Programming
	* C++ / SFML / ImGui
	* Fun!
*  This course is not:
	* A game design course
	* A game development course
	* A game engine (Unity, Unreal) tutorial course

C++ Programming
*  This course has no quizzes
* This course has no exams
* This course has programming
* It is slightly more total work than most courses, but there is significant overlap between assignments and project
* Lots of programming, but very rewarding
* Many students have completed the course with no C++ knowledge beforehand
* We will have 2 full lectures on C++
* Assignment 1 lecture will also have a tutorial on how to set up our development environment for C++ / SFML / ImGui
* Don't be afraid! Modern C++ is awesome

Course Structure
* Lectures:
	* Tuesday / Thursday
	* No mandatory text, lectures are important
* Online Sessions / Help
	* I'm always available via email
	* I'm available on Discord (Dave's Courses) (please do not share discord info with public)
	* 31.08

Assignments / Project
* Assignments
	* 4 total assignments (+1 project related submission)
	* Assignments with 40% of course mark
	* Groups of up to 2, no contact with other groups
	* Each assignment will be to make a game with a given architecture / goal
* Course Project (60% of course mark)
	* Final project = make a full game
	* Project video trailer / final presentation
	* Projects will be in groups of up to 4 people

Cheating / Plagiarism
* All work is going to be group work
* Keep discussion of assignments and the project entirely within your own group
* We will be checking VERY CLOSELY for cheating, and punishing it strictly
* There are valid excuses to be late, miss work, miss class, etc...: but never to cheat

Anti Cheating Measures
* All assignment files will be personalized and sent directly to your email
	* Name, Student Number, email, etc...
	* Other steganographic information inserted
* Any files leaked to other groups or online will be traceable back to original students
* Large repository of past assignments to do code difference checking with
* We periodically search online / GitHub for code related to the course
	* Have had code removed under DMCA request
* Ensure any repository is made private
* You may not release any source code written for this course at any time for any reason (now or in the future)

Why You Shouldn't be Cheating
* 2 years ago, I had companies calling me asking for recommendations of good students to hire
* In 2024, my best students are struggling to get internships, co-ops, interviews, etc...
* Tech job market in St. John's has hone down significantly in past 1-2 years
* Use this course as a way to practice the skills that you will need to get a job. A piece of paper saying you graduated is no longer sufficient

Portfolio Building
* Even though you cannot release source code online, you can show off your work
* Final project will require a video presentation and game trailer video
* Feel free to share screen shots, videos, or compiled version of your game anywhere
* Course designed to build skills / portfolio

Course Goal is to Learn
* Game programming from scratch
* Learn the ECS programming architecture
* Game programming patterns
* Software design principles
* Implement several different games

Game Engines
* Many modern game engines exist
* Unity, Unreal, Godot, etc...
* Typically have a user interface to interact with, along with coding interface as well

Game Engine
* We will not make a full unity-like game engine with all of its features
* Our game engine will:
	* Have a C++ ECS programming architecture
	* Be able to program many 2D games very easily
	* Be able to incorporate different game 'scenes'
	* Make our own a level editor for the project
* We could extend our game engine to 3D but we would spend half the course on rendering

Why not Unity / Unreal?
* This is a computer science class, we want to get our hands dirty in the details
* After this course, you will understand how existing game engines work and easily be able to use them, if not create your own
* Teaching an existing engine only teaches you syntax, with very little understanding
* If you get taught how to use a specific game engine and the company you go to doesn't use it, you have learned nothing
* If you get taught the underlying way that a game engine works, you will be able to learn any specific game engine easily and know even more about how it works

Introduction to C++ / SFML
* Introduction to C++
	* C++ Syntax / Semantics
	* Standard Template Library (STL)
* Simple and Fast Multimedia Library (SFML)
	* Basics of SFML
	* Windows, Rendering, Input Handling, etc...
* ImGui - Immediate Mode GUI

2D Game Engine Programming
* Game Engine Layout / Architecture
* Main Loop Structure / Tick Rate
* Game States & State Machines
* Asset Loading / Memory Management
* Sprites & Animations, Rendering
* Basic Shaders
* User Input Handling & Event Systems
* Data Oriented Design & Config Files
* Window / Menu / Drag & Drop Systems
* World View: Camera Systems / Viewports

ECS Game Engine Architecture
* Entities, Components, Systems (ECS)
* Architecture, Design, Implementation
* ECS Classes, Structure, Memory Management
* Systems for Gameplay Mechanics

Physics / Math for Games
* Vector Math / Class Implementation
* Game Object KInematics
	* Position, Velocity, Rotation
	* Acceleration, Gravity, Projectiles
* Collision Detection / Resolution
	* Circles, Rectangles
	* Bounding Boxes
* Mass, Momentum, Inertia

Gameplay Programming
* Basic Artificial Intelligence
	* NPC Behavior, Steering, Path-Finding
	* Entity Interactions / Dialogues
	* Difficulty Settings / Game Config
	* Game Events / Triggers
	* Items / Inventory / Weapons
	* Saving / Loading Games

Plan of Attack
1. Introduction to C++ /SFML
2. Assignment 1: Simple C++ /SFML
	* Simple game main loop
	* Drawing a Sprite to the screen
	* Playing a Sound when somethin happens
	* (Get used to C++ / SFML syntax and compiling)
3. Introduce ECS Game Engine Architecture
4. while (!courseOver)
	* Introduce new topic(s)
	* Assignment: make game using the new topics

Course Balance
* This course will maintain a balance of efficiency vs. usability vs. learnability
* The 'easiest' way of implementing somethin will be taught first, then if a more efficient way exists, taught later
* There are always trade-offs in any method and no 'best way' to do most things

ECS Game Programming
* Why use ECS for games?
* ECS = Entities, Components, Systems
* Entity: Anything object in the game
	* Player, Platform, Tile, Bullet, Enemy
* ECS is actually a software design paradigm
	* Can be used for all software, not just games
* Let's first look at Object-Oriented design

Object Oriented Programming
* Object-Oriented Programming uses the concept of Objects and Inheritance to implement specific functionality
* Object-Oriented Design (in games) is the process of strictly using OOP in order to design the architecture of our game engine and the entities within it
* OOP /OO Design helps us manage data / code
* Entity: any object in game
	* Each of them have (x, y) position
	* Entity used as base-class
	* Tile, Player, inherit from it

Entity-Component
* Entity: object in game
* Component: properties that can attach to Entities
* Component examples:
	* Position, Speed
	* Bounding Box
	* Health, Weapon, Damage

ECS Game Programming
* ECS uses Composition-based design
* Entity: Anything object in the game
	* Player, Platform, Tile, Bullet, Enemy
* Component: Properties attached to entities
	* Position, Texture, Animation, Health, Gravity
	* Components are PURE DATA
* Systems: Code / logic that drives behavior
	* Movement, Rendering, Sound, Physics

ECS Example Entity/components
* Player
	* Position, Speed, BBox, Sprite, Health, Gravity, Input
* Enemy
	* Position, Speed, BBox, Sprite, Health, AI
* Bullet
	* Position, Speed, Angle, BBox, Sprite, Damage, Lifespan
* Tile
	* Position, BBox, Sprite

ECS Example System
* Movement System
```
for (e: entities) { e.pos += e.velocity; }
```
* Collision System 
```
for (b: bullets)
	for (e: enemies) 
	  if (Physics::IsCollision(b,e)) {
	    e.health -= b.damage;
	    b.destroy();
	  }
```
* Rendering System
```
for (e: entities) { window.draw(e.sprite, e.pos); }
```

Engine Architecture
```
Game Engine
  > Scene
    > Systems
    > EntityManager
      > Entity
        > Component
```

 