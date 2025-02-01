[COMP4300 - Game Programming - Lecture 18 - Advanced Game Loops](https://youtu.be/x7shYL7UDDs?si=GdgpMcvGDSS6q7N3)

* Material for these slides is available in textbook: Game Programming Patterns
* http://gameprogrammingpatterns.com/game-loop.html

In the Beginning...
* You may recall simpler console programs you wrote while learning how to program
* Programs had the following structure:
	1. Run some initial code
	2. Print instruction for user to the screen
	3. Wait for input from the user
	4. Store the user input somehow
	5. Continue with program code

Simple Program Structure
```
int main()
{
	float input = 0;
	while (true)
	{
		cout << "Enter Distance in Feet\n";
		cin >> input;
		cout << "That is " << (input * 12) << " inches\n";
	}
}
```

```
// display some instructioins
while (true)
{
	// wait for user input (blocking)
	// do something interesting with input
	// display more instructions (or quit)
}
```

Graphical User Interfaces
* Even more modern GUIs follow the same basic structure, since nothing in the GUI changes unless user inputs values
* Example: Word Processor
	* Display current document
	* Wait for user to type or click
	* Handle the user input somehow
	* Repeat

```
// display initial GUI screen
while (true)
{
	// poll for user input events
	// update internal variables based on input
	// update user interface
}
```

Game Main Loops
* Unlike many other programs, games keep moving when the user doesn't input
* The game won't be paused just because the user isn't doing anything
* Clock keeps ticking, NPCs keep moving, animations keep animating
* Key: We don't WAIT for user input

```
// display initial GUI screen
while (true)
{
	// process user input if any exists
	// update internal game simulation by one step
	// render current game screen
}
```

```
// display initial GUI screen
while (true)
{
	processInput();
	update();
	render();
}

void update()
{
	sUserInput();             // processInput()
	m_entityManager.update(); // update()
	sAI();
	sMovement();
	sLifespan();
	sCollision();
	sAnimation();
	sRender();                // render()
}
```

Game Loop Speed
* When programs use blocking input, they can run the main loop at any speed since it's limited by user input speed
* With non-blocking input in a game loop, we now need to decide how fast the loop spins
* Each update() advances the game world by the same amount each call (example: speed 5, x += 5)
* Game world clock 'ticks' forward by 1
* Player's real-world clock ticks at a different speed
* How many game loop cycles in one second determines the game's 'frames per second' count

```
// display initial GUI screen
while (true)
{
	processInput();
	update();
	render();
}
// 60 loops per second = 60 render() calls = 60 FPS
```

Frames Per Second
* Graphically, more FPS = smoother play
* FPS is typically determined by two things:
	* How much work done in each loop
	* How fast is the underlying hardware
* Older games had the advantage of knowing the exact hardware it would run on, so they knew how 'fast' the game would run
* Modern computer games must run on a variety of hardware, so we must accommodate for that

Game Loops / Hardware
* The flexibility you gave with your game loop can depend on many factors
* OS: How often can you poll events?
* OS: Are hardware input events blocking?
* Monitor: Can't render more often than monitor refresh rate (Vsync)
* Networking: Type of net traffic can determine time outs, packet loss, etc. (not covered here)

Setting Fixed FPS Count
* Let's say we want to fix game to 60fps
* Note: we have cheated so far by letting SFML do this for us with build-in command:
	* `m_window.setFramerateLimit(60);`
* To do it manually, we must limit our game loop to run at most every 1000/60 ~ 16ms
* We will add a delay to the end of each loop

```
float MS_PER_FRAME = 1000.0 / 60.0;
while (true)
{
	float start = getCurrentTimeMS();
	processInput();
	update();
	render();
	float elapsed = getCurrentTimeMS - start;
	sleep(max(0, MS_PER_FRAME - elapsed));
}
```

* Adding the sleep() at the end of the loop ensures we don't update too quickly
* Unfortunately, there's no way to ensure that slow code doesn't make your game run too slowly
* In this case, must cut computation time to ensure the FPS target can be reached

Note: SFML
* `Sf::Window::setFrameLimit(limit)`
* Limits the framerate to a maximum
* If a limit is set, the window will use a small delay after each call to display() to ensure that the current frame lasted long enough to match the framerate limit. SFML will try to match the given limit as much as it can, but since it internally uses sf::sleep, whose precision depends on the underlying OS, the results may be a little unprecise as well (for example, you can get 65 FPS when requesting 60).

Quick Notes
* The following slides will show how to implement 'catching up' after lag frames
* In some cases we want to do this, and in other cases we actually don't want to
* These slides are to show you one way to implement different techniques in your game engine if this is your goal

Game Simulation Scaling
* The problem we face is as follows:
	1. Each update advances game time by some amount
	2. It takes some amount of real time to compute that
* If 2 takes longer than 1: the game slows down
* Example: If it takes longer than 16ms to update the game world by 16ms equivalent, we can't keep up
* If we could advance the game world by more than 16ms, we can update less frequently and keep up to date
* Idea: introduce a variable that we will use to update the game world simulation (variable time step)

```
float lastTime = getCurrentTimeMS();
while (true)
{
	float current = getCurrentTimeMS();
	float elapsed = current - lastTime;
	processInput();
	update(elapsed); // variable update time
	render();
	lastTime = current;
}
```

Variable Time Step
* Each frame we calculate how much real time has passed since the last game update
* When we update the game state, we use the real elapsed time as a scale variable
* Example: Bullet flying through the air
	* Each time step update it with velocity
	* Variable time: scale velocity with elapsed time
* Advantages
	* Game plays at consistent rate on diff hardware
	* Players with faster machines get smoother FPS
* Disadvantages
	* Non-deterministic effects based on variable FPS
	* Floating point errors produce diff results
	* Network desynchronization
	* Physics simulations yield diff results (collisions)

How Rendering Fits
* Whether or not we use variable time steps, rendering is not affected
* Rendering essentially just takes the current state of the game and draws it
* So instead of variably updating the game simulation, let's render at variable times
* This lets us go back fixed game time steps

Real-World Analogy
* You're tasked to make a cheeseburger every minute at a fast-foot place
* It takes you about 15 seconds to make a cheeseburger, so you have 45 sec rest/min
* One burger slips and falls, and it takes you 2 minutes to clean it up. You're now behind by 2
* Since you can make 4 burgers per minute, you catch up next minute by making 3 total
* Diagram at 42:00

```
float previous = getCurrentTimeMS();
float lag = 0.0f;
while (true)
{
	float current = getCurrentTimeMS();
	float elapsed = current - previous;
	previous = current;
	lag += elapsed;
	processInput();
	while (lag >= MS_PER_UPDATE)
	{
		update();
		lag -= MS_PER_UPDATE;
	}
	render();
}
```

What does this do?
* Lag measures 'how far behind' the game's clock is behind the real world clock (example: if one update or render took too long)
* Inner loop updates the game to 'catch up' until all of the lag is gone, with fixed step updates (which are accurate)
* Once we catch up, we render the scene
* Game simulates at a constant rate using fix time steps in a 'safe' way + 'catch up' for lagged frames
* Rendering may appear 'choppy' while the game catches up, but at least the simulation speed is fixed on diff hardware
* Note: We can only 'catch up' if 'lag frames' are infrequent

Rendering Decoupling
* Important: We have now removed the rendering synchronization from update()
* No longer call one render per update
* This means we could put rendering into another thread (for example)
* But some issues can arise from this, namely: rendering between updates

Rendering Between Updates
* Diagram at 47:45

Rendering / Interpolation
* If we render between updates, game world could be the same for 2 frames, which looks quite choppy / bad
* We can interpolate / do basic movement calculations inside rendering engine
	* render (lag / MS_PER_UPDATE)
* Physics may not be exact for this render, but the game world will be exact in future

Game Speed + Collision Issues
* Given our basic way of doing collision detection, an entity going fast enough could pass through a game tile / bullet
* We 'fixed' this in assignment 3 by setting a maximum speed for entities
* As long as the maximum speed is less than the thickness of smallest tile, works
* But what if we want to allow entities in our game to go at faster speeds than this allows?

Multiple Update Loops
* One simple solution is to increase the number of game ticks per second
* Example:
	* We want max player speed 100 pixels / frame
	* Tile size is fixed 32 pixels
	* Instead: fix player speed to 25, and update game world 4 times before rendering
* Trade CPU effort for simulation accuracy

Mario 64 Examples
* [Coin Falling Through Floor](https://www.youtube.com/watch?v=oQ7dXvMfr30)
* [Walls, Floors, & Ceilings](https://www.youtube.com/watch?v=UnU7DJXiMAQ)
* [Parallel Universes](https://www.youtube.com/watch?v=kpk2tdsPh0A)
