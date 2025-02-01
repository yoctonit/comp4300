[COMP4300 - Game Programming - Lecture 16 - Saving, Tools, Drag and Drop](https://youtu.be/02ITGTalU5w?si=wipDrV--5nLnhXDz)

Game Saving
* A vital part of any game is the ability to save the player's progress at some point
* Games can be saved in a number of ways:
	* File Save, Quick Save, Checkpoint, etc.
* Saved games can be stored in a number of ways as well, such as:
	* Local File, Cloud Save, Temp (RAM), Password
* When you allow players to save their progress is a game design decision
* Save/Load whenever they like can lead to much easier gameplay (quick save/load)
* Save after or mid-level (checkpoint)
* Saving only at specific locations (town/camp) can be inconvenient for the player

Saving Between Levels
* The easiest way to implement game save is between levels, when no entities are currently active in the game world
* In simple games (without quests, inventories, items, status, etc.) we can just save the level number that we are on

Displaying Progress
* Users progress must be conveyed somehow so that their progress can continue
* Simplest way: display level number (SMB)
* Another common way is to display progress as an 'overworld map' which conveys a deeper sense of immersion/progress
* RPG game may save at a location

Continue
* If you don't want to (or can't) implement a full save for your game
* Let the player continue from a given spot
* Restart level? World? Etc.
* Very common in early video games when arbitrary save game functionality may not gave been possible

Save Points
* Save points make it easier to implement game saving/loading by limiting the places where players can save/load
* Typically static locations away from other entities and dangerous situations
* Allow us to load game without worrying about entity storage

Checkpoints
* Level-based games may want a progress checkpoint placed within the level
* This reduces frustration by letting players save progress mid-level
* Doesn't allow players to make game easier or reduce RNG like quick save
 
Checkpoint Implementation
* Checkpoints are easy to implement
* If level has a finite number of checkpoints, just store the checkpoint number along with each level
* When player enters a level, spawn them at the position of the checkpoint stored
* Reset checkpoints when level completed

What Data to Save
* Depends on saving functionality
* Save After Level, Store:
	* Current levels completed, inventory, weapons
* Save at Save Point
	* Quests completed, party stats, save point used
* Quick Save:
	* Entity game state, each entity's components

Loading Saved Game
* How to implement loading a saved game depends on what data was saved
* If level/items stored, just recreate the game before that level starts
* If quick save, may have to load all entities back so that game resumes from QS point

Loading Ex: Overworld View
* Saved game stores levels completed, current Mario status/inventory (SMB3)
* Game is loaded, Mario appears on the overworld map with completed maps shown and inventory available
* Game load may require quit to main menu

Loading Ex: Quick Save
* In many FPS games, press a button at any time to instantly save
* When quick load happens, all entities and game progress resumes from when the button was pressed
* Players can abuse this to get past difficult areas/know the outcome of RNG etc.

Password Save
* [NES Punchout Password](https://www.youtube.com/watch?v=ap1YL_kGGlg)

Game Tools
* As you make more games, you quickly learn that content creation becomes one of the most time consuming aspects of video game development
* Tools are programs that assist us with creating content for our games
* Many tools exist to help with general asset creation
	* Photoshop, Blender, PixelArt, Notepad. etc.
* For more specific asset creation (maps, levels) we need to construct our own tools to help us

Popular Game Engines
* Several popular game engines exist that come with tools for creating content
	* Unity, Unreal, GameMaker, etc.
* These game engines are more general, also help with game programming etc.
* They do a great job of illustrating the power of tools for game creation

Custom Game Tools
* If your game is not using a pre-existing engine, you must create your own tools
* Many existing games (older games) come with editors/tools for modeling or creating other custom content
* The features and power of these tools vary wildly from game to game

Tools Implementation
* Game tools are typically made along with a graphical user interface (GUI), which includes some sort of menu system
* The first step will be drag-and-drop

Drag and Drop
* Drag and drop typically refers to the ability to move an on-screen UI element from one place to another with the mouse
* In order to implement a basic level editing tool for our games, we can implement drag-and-drop via ECS
* Let's create a Draggable component

Drag and Drop Variant
* Literal drag and drop would implement:
	1. Mouse Down on entity = start drag
	2. As mouse moves, move entity
	3. Mouse Release = let go
* Kind of annoying in practice, instead let's implement 'pick up and put down'
	1. Mouse Down on entity = pick up
	2. As mouse moves, move entity
	3. Click to place final location and let go

Draggable Component
```
void sInput()
{
	if (left mouse button pressed)
	{
		auto e = closestDraggableEntityToMouseClickPosition();
		auto &d = e->getComponent<CDragable>().dragging;
		// if we are currently dragging, let go of the entity
		if (d) { d = false; }
		// otherwise if we clicked inside the entity, start dragging
		else if (mouse click is inside e's animation) { d = true; }
	}
}
```

Draggable System
```
void sDragAndDrop()
{
	for (e->hasComponent<CDraggable>() && e->getComponent<CDraggable>.dragging)
	{
		// set e position to the current mouse position
	}
}
```

```
// Mouse Actions in GameEngine.cpp
auto mpos = sf::Mouse::getPosition(m_window);
Vec2 pos(mpos.x, mpos.y);
if (event.type == sf::Event::MouseButtonPressed)
{
	switch (event.mouse.button)
	{
	case sf::Mouse::Left: {currentScene()->doAction(Action("LEFT_CLICK", "START", pos)); break;}
	case sf::Mouse::Middle: {currentScene()->doAction(Action("MIDDLE_CLICK", "START", pos)); break;}
	case sf::Mouse::Right: {currentScene()->doAction(Action("RIGHT_CLICK", "START", pos)); break;}
	default: break;
	}
}

if (event.type == sf::Event::MouseButtonReleased)
{
	switch (event.mouse.button)
	{
	case sf::Mouse::Left: {currentScene()->doAction(Action("LEFT_CLICK", "END", pos)); break;}
	case sf::Mouse::Middle: {currentScene()->doAction(Action("MIDDLE_CLICK", "END", pos)); break;}
	case sf::Mouse::Right: {currentScene()->doAction(Action("RIGHT_CLICK", "END", pos)); break;}
	default: break;
	}
}

if (event.type == sf::Event::MouseMoved)
{
	currentScene()->doAction(Action("MOUSE_MOVE", Vec2(event.mouseMove.x, event.mouseMove.y)));
}

// Also added Vec2 m_pos = Vec2(0, 0); member to Action class
```

```
// Live code of drag and drop
// in Scene_play.cpp in void Scene_Play::sDoAction(const Action& action)
// ...
if (action.name() == "MOUSE_MOVE")
{
	// std::cout << action.toString() << "\n";
	// we added in Scene_Play.h this variable
	m_mousePos = action.pos();
}

if (action.type() == "START")
{
	//...
	else if (action.name() == "LEFT_CLICK")
	{
		// detect the picking up of entities
		for (auto e : m_entityManager.getEntities())
		{
			Vec2 wPos = windowToWorld(m_mousePos);
			if (IsInside(wPos,e))
			{
				if (!e->hasComponent<CDraggable>()) { continue; }
			
				auto & dragging = e->getComponent<CDraggable>().dragging;
				
				dragging = !dragging;
			}
		}
		// std::cout << action.toString() << "\n";
	}
}

// in sRendering()
{
// ...
sf::CircleShape dot;
dot.setFillColor(sf::Color::Red);
dot.setRadius(8);
dot.setOrigin(8, 8);
Vec2 worldPos = windowToWorld(m_mousePos)
dot.setPosition(worldPos.x, worldPos.y);
m_game->widnow().draw(dot);
}

// in Scene_Play.h we added function
Vec2 windowToWorld(const Vec2 & window) const;

// in Scene_Play.cpp
Vec2 Scene_Play::windowToWorld(const Vec2 & window) const
{
	auto view = m_game->window().getView();
	float wx = view.getCenter().x - (m_game->window().getSize().x / 2);
	return Vec2(window.x + wx, window.y);
}

// add this function at the top of Schene_Play.cpp
bool IsInside(Vec2 pos, std::shared_ptr<Entity> e)
{
	Vec2 eSize = e->getComponent<CAnimation>().animation.getSize();
	Vec2 ePos = e->getComponent<CTransform>().pos;

	if (pos.x > ePos.x - eSize.x/2 && 
		pos.x < ePos.x + eSize.x/2 &&
		pos.y > ePos.y - eSize.y/2 &&
		pos.y < ePos.y + eSize.y/2)
	{
		std::cout << e->getComponent<CAnimation>().animation.name() << "\n";
		return true;
	}
	return false;
}

// Inside Components.h add:
class CDraggable : public Component
{
public:
	bool dragging = false;
};

// Inside Entity.h add CDraggable in typedef std::tuple<...

// Now in Scene_Play.h and Scene_Play.cpp
// add drag system declaration and definition
void Scene_Play::sDrag()
{
	for (auto e : m_entityManager.getEntities())
	{
		if (e->hasComponent<CDraggable>() && e->getComponent<CDraggable>().dragging)
		{
			Vec2 wPos = windowToWorld(m_mousePos);
			e->getComponent<CTransform>().pos = wPos;
		}
	}
}

// in void Scene_Play::update() call sDrag() system
// ...
if (!m_poused)
{
	sDrag();
	sMovement();
	// ...
}

// in lodadLevel add component CDraggable to tiles
tile->addComponent<CDraggable>();

```

