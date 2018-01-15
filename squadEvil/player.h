#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include "HUD.h"
using namespace sf;
class skills;

class player
{
public:
	player();
	~player();
	void update();

	/* GETTERS */
	Sprite &getSprite();
	const Sprite &getConstSprite();
	Vector2f getPosition();
	Vector2f getCenter();
	/* SETTERS */
	void setPosition(float x, float y) { avatar.setPosition((x < 0 ? 0 : x), (y < 0 ? 0 : y)); };
	void setPosition(Vector2f pos) { avatar.setPosition(pos); };
	void setMap(std::vector <std::vector < int>> &);

	/* STATISTICS */
	virtual const int getGold() { return currentStats.gold; };
	virtual const float getHp() { return currentStats.HP; };
	virtual const float getMana() { return currentStats.MANA; };
	virtual const float getExp() { return currentStats.EXP; };
	virtual const int getMaxHp() { return currentStats.max_hp; };
	virtual const int getMaxMana() { return currentStats.max_mana; };
	
	/* TEXTURE STATES */
	enum states
	{
		Attack, Climb, Dead,
		Glide, Idle, Jump,
		Jump_Attack, Jump_Throw,
		Run, Slide, Throw, COUNT
	} cTextureState;

	/* SKILLS */
	virtual skills *getSkill(int number) = 0;

	/* OPERATORS */
	int operator++(int) { currentStats.gold++; return currentStats.gold; };
	int operator-=(int k) { currentStats.HP = (currentStats.HP - k < 0 ? 0 : currentStats.HP - k); return currentStats.HP; }

	/* FLAGS */
	bool fEndOfLevel;
	//int flagLadder();
	//void flagLadderReset() { ladderTime.restart(); };

	/* Jumping */
	void startJump() { isJumping = true; startingPositionY = avatar.getPosition().y; }; // Secured on game::mainLoop Key::Space
	void endJump() { isJumping = false; };
	const bool getJumpingVariable() { return isJumping; }; 
	float startingPositionY;
	/* Its just helper variable, everything is in physics::gravity */

	void death(Vector2f checkpoint) { currentStats.HP = currentStats.max_hp; setPosition(checkpoint); }
	void hitEffect();
	void hudEffect(RenderWindow &);
	
	/* MOVE */
	enum dir
	{
		BACK = 0, STOP = 1, FORWARD = 2
	} currentDirState, currentMoveDir;

	/* Points for physics */
	enum class physicsPoints
	{
		LEFT_TOP, RIGHT_TOP, 
		RIGHT_BOT, LEFT_BOT,
		CENTER, COUNT
	};
	Vector2f getCorner(physicsPoints a) { return corners[(int)a]; };
	void drawCorners(RenderWindow &);

protected:
	struct basicStats
	{
		int max_hp = 100, max_mana = 100;
		float HP = max_hp, MANA = max_mana, EXP = 0;
		int gold = 0, level = 1;
		float regHp = 5, regMana = 5;
	} currentStats;

	Clock regTimer;

private:
	HUD hud;
	Sprite avatar;
	Texture girlTex[11][9];
	//int texWidth, texHeight;
	//std::vector <std::vector <int>> map;
	int skillBar[3];
	Vector2f corners[5]; // [4] - center
	Vector2f lastControlPoint;


	/* Methods */
	void setCorners();
	void checkTexState();


	/* Virtuals */
	virtual void regen() = 0;


	/* Counters */
	Clock hitColorTimer;
	Clock avatarFramesTimer;

	int hitColorCounter;

	/* Physics */

	//void jumping();
	bool isJumping;
	/************/

};

