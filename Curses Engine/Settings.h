#pragma once

class Settings
{
public:
	float GetMovePeriod() const { return movePeriods[snakeSpeedMode]; }
	int GetSnakeSpeedMode() const { return snakeSpeedMode; }
	bool GetWrappingMode() const { return wrapAround; }
	void ToggleWrappingMode() { wrapAround = true xor wrapAround; }
	void SetWrappingMode(bool mode) { wrapAround = mode; }
	void ToggleSnakeSpeedBwd() { if (--snakeSpeedMode == -1) snakeSpeedMode = 2; }
	void ToggleSnakeSpeedFwd() { if (++snakeSpeedMode == 3) snakeSpeedMode = 0; }
private:
	const float movePeriods[3] = { 0.12f, 0.10f, 0.08f };
	int snakeSpeedMode = 1;
	bool wrapAround = false;
};