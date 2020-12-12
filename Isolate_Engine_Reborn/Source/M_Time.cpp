#include "M_Time.h"
#include "Application.h"
#include "Globals.h"


M_Time::M_Time(bool is_Active) : Module("TimeManager", is_Active)
{
}

M_Time::~M_Time() {}


UPDATE_STATUS M_Time::Update(float dt)
{
	fps++;

	real_dt = App->GetDt();
	real_time += real_dt;

	return UPDATE_STATUS::CONTINUE;
}

bool M_Time::CleanUp()
{
	return true;
}

uint M_Time::GetFrameCount() const
{
	return fps;
}

void M_Time::ResetGameTimer()
{
	game_time = 0.0f;
	game_dt = 0.0f;
}

float M_Time::GetRealTime() const
{
	return real_time;
}

float M_Time::GetRealDT() const
{
	return real_dt;
}

float M_Time::GetGameTime() const
{
	return game_time;
}

float M_Time::GetGameDt() const
{
	return game_dt;
}

float M_Time::GetTimeScale() const
{
	return game_time_scale;
}
