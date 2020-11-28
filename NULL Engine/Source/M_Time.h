#pragma once

#include "Module.h" 

class M_Time : public Module
{
public:
	M_Time(bool is_Active = true);
	~M_Time();

	UPDATE_STATUS Update(float dt);
	bool CleanUp();

	uint GetFrameCount() const;
	float GetRealTime() const;
	float GetRealDT() const;
	float GetGameTime() const;
	float GetGameDt() const;
	float GetTimeScale() const;
	void ResetGameTimer();

public:

	float game_time = 0.0f;
	float game_dt = 0.0f;
	float game_time_scale = 1.0f;

	float real_time = 0.0f;
	float real_dt = 0.0f;

	float started_play_time = 0.0f;

	bool game_paused = false;

	uint fps = 0;
};