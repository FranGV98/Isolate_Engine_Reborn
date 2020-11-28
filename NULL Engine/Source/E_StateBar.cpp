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

	if (ImGui::Button("Play"))
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

