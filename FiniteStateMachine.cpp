#include "FiniteStateMachine.h"

FiniteStateMachine::FiniteStateMachine()
{
	currentStateID = UNDEFINED;
}

FiniteStateMachine::~FiniteStateMachine()
{
	std::map<DWORD, State*>::iterator iter = states.begin();
	while (iter != states.end())
	{
		State* state = iter->second;
		if (state != NULL) delete state;
		iter++;
	}
	states.clear();
}

void FiniteStateMachine::addStateTransition(DWORD stateID, DWORD inputEvent, DWORD outputStateID)
{
	// 동일한 State가 이미 존재하는지를 알아본다.
	std::map<DWORD, State*>::iterator iter = states.find(stateID);
	if (iter == states.end())
	{ // 새 상태를 만든다.
		states[stateID] = new State(stateID);
	}

	// 상태 전이 정보를 추가한다.
	states[stateID]->addTransition(inputEvent, outputStateID);
}

void FiniteStateMachine::deleteTransition(DWORD stateID, DWORD inputEvent)
{
	std::map<DWORD, State*>::iterator iter = states.find(stateID);
	if (iter == states.end()) return; //상태가 없음.

	State * state = iter->second;
	// State의 전이 정보를 지운다.
	state->deleteTransition(inputEvent);
	if (state->getNumTransitions() == 0)
	{ // 더이상 전이가 없다면 상태를 지운다.
		delete state;
		states.erase(iter);
	}
}

void FiniteStateMachine::setCurrentStateID(DWORD stateID)
{
	std::map<DWORD, State*>::iterator iter = states.find(stateID);
	if (iter == states.end()) return; //stateID에 해당하는 상태가 없음.

	currentStateID = stateID;
}

void FiniteStateMachine::issueEvent(int inputEvent)
{
	// 입력 이벤트에 따라서 상태전이를 수행하여 현재 상태를 바꿈.
	if (currentStateID == UNDEFINED) return; // 현재 상태가 정의되어 있지 않음.

	DWORD outputStateID = states[currentStateID]->getOutputStateID(inputEvent);
	if (outputStateID == UNDEFINED) return; //상태전이하지 못했음.
	currentStateID = outputStateID; //현재 상태를 바꿈.
}
