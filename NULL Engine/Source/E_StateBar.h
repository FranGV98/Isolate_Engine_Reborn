
#include "E_Panel.h"

class E_StateBar : public E_Panel
{
public:
	E_StateBar();
	 ~E_StateBar();

	bool	Draw(ImGuiIO& io) override;
	bool	CleanUp() override;
public:
	bool not_running;
};