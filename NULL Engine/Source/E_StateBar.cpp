#include "M_Editor.h"

#include "E_StateBar.h"

E_StateBar::E_StateBar() : E_Panel("StateBar")
{

}

E_StateBar::~E_StateBar()
{

}

bool E_StateBar::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin("StateBar");
	 //Transform Buttons
	if (ImGui::Button("Move"))
	{

	}

	ImGui::SameLine();
	if (ImGui::Button("Rotate"))
	{

	}

	ImGui::SameLine();
	if (ImGui::Button("Scale"))
	{

	}
	ImGui::SameLine(200);

	//Time Buttons 
	if (not_running)
	{
		if (ImGui::Button("Play"))
		{
			not_running = false;
		}
	}
	else
	{
		if (ImGui::Button("Stop"))
		{
			not_running = true;
		}
	}
	ImGui::SameLine();
	
	if (ImGui::Button("Pause"))
	{

	}
	ImGui::SameLine();
	if (ImGui::Button("NextFrame"))
	{

	}

	ImGui::End();

	return ret;
}

bool E_StateBar::CleanUp()
{
	bool ret = true;

	return ret;
}

