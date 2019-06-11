#pragma once
#include <d2d1.h>
#include "FiniteStateMachine.h"
#include <vector>
#include "Bullet.h"

// 평면의 크기. 좌우로 [-PLANE_SIZE,PLANE_SIZE], 앞뒤로도 [-PLANE_SIZE,PLANE_SIZE].
#define PLANE_SIZE_X 640
#define PLANE_SIZE_Y 480


class Character
{
public:
	enum StateID {
		STATE_UNDEFINED = 0,
		STATE_MOVE,
		STATE_ATTACK,

	};

	enum EventID {
		EVENT_UNDEFINED = 0,
		EVENT_FINDTARGET,
		EVENT_LOSTTARGET,


	};



private:

	D2D1_POINT_2F position;
	D2D1_POINT_2F destPosition;

	float speed;
	float orientation;
	FiniteStateMachine* stateMachine;
	bool moveUp;


public:
	Character(DWORD stateTransitions[][3], int numTransitions);//, MeshMD2* _model);
	~Character();

	void update(float mouseX, float mouseY, float timeDelta,std::vector<Bullet>* bullets);

	void setPosition(D2D1_POINT_2F pos) { position = pos; }
	D2D1_POINT_2F getPosition() { return position; }
	void setDestPosition(D2D1_POINT_2F dest);
	D2D1_POINT_2F getDestPositon() { return destPosition; }
	float getOrientation() { return orientation; }

private:
	void issueEvent(DWORD event);
	void moveTo(float timeDelta);

	void updateAI(float mouseX, float mouseY, float timeDelta, std::vector<Bullet>* bullets);

	void actionStand(float timeDelta);
	void actionMove(float timeDelta);
	void actionFollow(float timeDelta);
	void actionAttack(float timeDelta);
	void actionRunaway(float timeDelta);
};


double GetLengthBetween(D2D1_POINT_2F p1, D2D1_POINT_2F p2);

struct D2DVECTOR
{
	float x;
	float y;
};
