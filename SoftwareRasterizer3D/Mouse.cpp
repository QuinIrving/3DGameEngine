#include "Mouse.h"

int Mouse::GetXPos() const {
	return xPos;
}

int Mouse::GetYPos() const {
	return yPos;
}

std::pair<int, int> Mouse::GetPos() const {
	return std::pair<int, int>(xPos, yPos);
}

void Mouse::SetXPos(int newXPos) {
	xPos = newXPos;
}

void Mouse::SetYPos(int newYPos) {
	yPos = newYPos;
}

void Mouse::SetPos(int newXPos, int newYPos) {
	xPos = newXPos;
	yPos = newYPos;
}

void Mouse::AddXPos(int pos) {
	xPos += pos;
}

void Mouse::AddYPos(int pos) {
	yPos += pos;
}

void Mouse::AddPos(int x, int y) {
	xPos += x;
	yPos += y;
}

bool Mouse::IsLMBDown() const {
	return lmbIsDown;
}

bool Mouse::IsRMBDown() const {
	return rmbIsDown;
}

bool Mouse::IsMMBDown() const {
	return mmbIsDown;
}

void Mouse::UpdateLMBDown(InputStatus newState) {
	lmbIsDown = (bool) newState;
}

void Mouse::UpdateRMBDown(InputStatus newState) {
	rmbIsDown = (bool) newState;
}

void Mouse::UpdateMMBDown(InputStatus newState) {
	mmbIsDown = (bool) newState;
}

int Mouse::GetMWheelAccumulator() {
	return mwheelAccumulator;
}

void Mouse::SetAccumulator(int acc) {
	mwheelAccumulator = acc;
}

int Mouse::AddAccumulator(int acc) {
	mwheelAccumulator += acc;
	if (mwheelAccumulator > 120) {
		int val = mwheelAccumulator / 120;
		mwheelAccumulator %= 120;
		return val;
	}

	return 0;
}
