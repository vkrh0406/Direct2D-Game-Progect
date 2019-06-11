#include "Character.h"


Character::Character(DWORD stateTransitions[][3], int numTransitions)//, MeshMD2* _model)
{
	//model = _model; //copy


	// ���ѻ��±�踦 �����.
	stateMachine = new FiniteStateMachine();
	for (int i = 0; i < numTransitions; i++)
	{
		stateMachine->addStateTransition(stateTransitions[i][0], stateTransitions[i][1], stateTransitions[i][2]);
	}
	stateMachine->setCurrentStateID(STATE_MOVE);

	// ĳ���� Ư���� �����Ѵ�.

		speed = 1.0f;
		//speed = (float)(80 + rand() % 3);



	// �ʱ�ȭ�Ѵ�.
	position = D2D1::Point2F(10.0f, 150.0f);
	orientation = 0.0f;
	setDestPosition(position); 
	moveUp = true;
}

Character::~Character()
{
	if (stateMachine != 0)
		delete stateMachine;
}


void Character::update(float mouseX, float mouseY, float timeDelta, std::vector<Bullet>* bullets)
{

		updateAI(mouseX, mouseY,timeDelta,bullets);
}


void Character::updateAI(float mouseX, float mouseY, float timeDelta, std::vector<Bullet>* bullets)
{
	switch (stateMachine->getCurrentStateID())
	{

	case STATE_MOVE:
		actionMove(timeDelta);

		if (position.y <= mouseY+100.0f && position.y >= mouseY-100.0f)
		{ // ���ü�� ����.
			D2D1_POINT_2F mousePositon = { mouseX,mouseY };
			setDestPosition(mousePositon); // ���ü�� ��ǥ�������� ������.
			issueEvent(Character::EVENT_FINDTARGET);
			break;
		}
		else
		{ //��ü�� ������ ���� ���
			if (moveUp)
			{
				if (position.y <= 80.0f)
				{
					moveUp = false;  //���� ���̻� ������ ������ȯ
					break;
				}
				position.y -= 2;

			}
			else
			{
				if (position.y >= 500)
				{
					moveUp = true; //�Ʒ��� ���̻� ������ ������ȯ
					break;
				}
				position.y += 2;
			}

		}

		break;

	case STATE_ATTACK:
		bullets->push_back(Bullet(position.x + 50.0f, position.y));

		
		if ( !(position.y <= mouseY + 100.0f && position.y >= mouseY - 100.0f))
		{ // ���ü�� �Ⱥ���.
			

			issueEvent(Character::EVENT_LOSTTARGET);
			break;
		}


		break;


	}

}

void Character::actionStand(float timeDelta) { }
void Character::actionMove(float timeDelta) { moveTo(timeDelta); }
void Character::actionFollow(float timeDelta) { moveTo(timeDelta); }
void Character::actionAttack(float timeDelta) { }
void Character::actionRunaway(float timeDelta) { }


void Character::issueEvent(DWORD event)
{
	stateMachine->issueEvent(event);


}

void Character::moveTo(float timeDelta)
{
	
}

void Character::setDestPosition(D2D1_POINT_2F dest)
{
	
}

double GetLengthBetween(D2D1_POINT_2F p1, D2D1_POINT_2F p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}
