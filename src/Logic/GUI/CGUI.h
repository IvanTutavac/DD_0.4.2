#pragma once
#include <vector>

class CWindow;

namespace GUI
{
	namespace WINDOW
	{
		enum WindowIndex : int
		{
			WindowIndex_nothing = -1,
			WindowIndex_mainMenu = 0,
			WindowIndex_inGame = 1,
			WindowIndex_mapEditor = 2,
			WindowIndex_mapEditorImageSelection = 3,
			WindowIndex_options = 4
		};
	}
}

class CMessage;

class CGUI
{
private:

	std::vector<CWindow*> m_pVWindow;

	int		 m_currentWindowIndex;

public:

	CGUI();
	~CGUI();

	bool	Init();
	void	Clean();

public:

	CWindow *GetMapEditorWindow();
	CWindow *GetMapEditorImageSelectionWindow();
	CWindow *GetMainMenuWindow();
	CWindow *GetInGameWindow();

	// set the index of the window being used 
	// no checks done for index !
	// also, reset the state of widgets on the window being replaced
	void	ChangeCurrentWindow(GUI::WINDOW::WindowIndex index);

	// returns nullptr if there's no currentWindow selected ( index was wrong probably ), writes to log
	CWindow* GetCurrentWindow();

	void	Update(double deltaTime); 

	// updates message
	// returns false on error
	bool		OpenChildWidget(CMessage *message);
};
