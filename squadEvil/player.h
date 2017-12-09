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
	void setPosition(float x, float y) { avatar.setPosition(x, y); };
	void setPosition(Vector2f pos) { avatar.setPosition(pos); };
	void drawCorners(RenderWindow &);
	void setMap(std::vector <std::vector < int>> &);

	/* STATISTICS */
	

	virtual const int getGold() { return currentStats.gold; };
	virtual const float getHp() { return currentStats.HP; };
	virtual const float getMana() { return currentStats.MANA; };
	virtual const float getExp() { return currentStats.EXP; };
	virtual const int getMaxHp() { return currentStats.max_hp; };
	virtual const int getMaxMana() { return currentStats.max_mana; };

	/* SKILLS */
	virtual skills *getSkill(int number) = 0;

	/* OPERATORS */
	int operator++(int) { currentStats.gold++; return currentStats.gold; };
	int operator-=(int k) { currentStats.HP = (currentStats.HP - k < 0 ? 0 : currentStats.HP - k); return currentStats.HP; }

	/* SIGNALS */
	int sigLadder();
	void sigLadderReset() { ladderTime.restart(); };
	void stopGravity() { gravityState = false; };
	void startGravity() { gravityState = true; };
	void jump();
	const bool isJumping() { return isJump; };

	void death(Vector2f checkpoint) { currentStats.HP = currentStats.max_hp; setPosition(checkpoint); }
	void hitEffect();
	void hudEffect(RenderWindow &);

	enum dir
	{
		BACK = 0, STOP = 1, FORWARD = 2
	} currentState, currentDir;
	float currentSpeed;
	const float defaultSpeed;
	void move(dir);

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
	Texture avatarTex;
	Texture avatarBackTex;
	int texWidth, texHeight;
	std::vector <std::vector <int>> map;

	
	
	int skillBar[3];

	Vector2f corners[4];
	void setCorners();

	/* Virtuals */
	virtual void regen() = 0;


	/* Counters */
	Clock hitColorTimer;
	Clock ladderTime;
	

	int hitColorCounter;

	/* Physics */
	void gravity();
	bool gravityState;
	float gravitation;

	void moving();
	float prevSpeed;

	void jumping();
	bool isJump;
	int jumpingCounter;
	/************/

};
