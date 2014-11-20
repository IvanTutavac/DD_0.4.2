#include "CGUI.h"
#include "../../Message/CMessage.h"
#include "CWindow.h"
#include "CTextBox.h"
#include "StructsGUI.h"
#include "../../Tools/debug.h"
#include "../../Tools/Log.h"
#include "../../Tools/StringWrapper.h"
#include "../../Data/Enums.h"

CGUI::CGUI()
{

}

CGUI::~CGUI()
{

}

bool	CGUI::Init()
{
	m_currentWindowIndex = GUI::WINDOW::WindowIndex_nothing;

	// order matters ! ( becasue of GUI::WINDOW::WINDOWIndex )

	CWindow *MainMenu = DD_NEW CWindow;

	if (!MainMenu->Init(WindowType::mainMenu))
	{
		DD_DELETE(MainMenu);
		Log("CGUI::Init() failed, MainMenu window failed");
		return	false;
	}

	m_pVWindow.push_back(MainMenu);

	CWindow *InGame = DD_NEW CWindow;

	if (!InGame->Init(WindowType::inGame))
	{
		DD_DELETE(InGame);
		Log("CGUI::Init() failed, inGame window failed");
		return	false;
	}

	m_pVWindow.push_back(InGame);

	CWindow *MapEditor = DD_NEW CWindow;

	if (!MapEditor->Init(WindowType::mapEditor))
	{
		DD_DELETE(MapEditor);
		Log("CGUI::Init() failed, MapEditor window failed");
		return	false;
	}

	m_pVWindow.push_back(MapEditor);

	CWindow *MapEditorImage = DD_NEW CWindow;

	if (!MapEditorImage->Init(WindowType::mapEditorImage))
	{
		DD_DELETE(MapEditorImage);
		Log("CGUI::Init() failed, MapEditorFloor window failed");
		return	true;
	}

	m_pVWindow.push_back(MapEditorImage);

	CWindow *Options = DD_NEW CWindow;

	if (!Options->Init(WindowType::options))
	{
		DD_DELETE(Options);
		Log("CGUI::Init() failed, Options window failed");
		return	false;
	}

	m_pVWindow.push_back(Options);

	return	true;
}

void	CGUI::Clean()
{
	for (size_t i = 0; i < m_pVWindow.size(); i++)
	{
		if (m_pVWindow[i] != nullptr)
			m_pVWindow[i]->Clean();
		DD_DELETE(m_pVWindow[i]);
	}
}

CWindow*	CGUI::GetInGameWindow()
{
	return	m_pVWindow[GUI::WINDOW::WindowIndex_inGame];
}

CWindow*	CGUI::GetMainMenuWindow()
{
	return	m_pVWindow[GUI::WINDOW::WindowIndex_mainMenu];
}

CWindow*	CGUI::GetMapEditorWindow()
{
	return	m_pVWindow[GUI::WINDOW::WindowIndex_mapEditor];
}

CWindow*	CGUI::GetMapEditorImageSelectionWindow()
{
	return	m_pVWindow[GUI::WINDOW::WindowIndex_mapEditorImageSelection];
}

void	CGUI::Update(double deltaTime)
{

}

void	CGUI::ChangeCurrentWindow(GUI::WINDOW::WindowIndex index)
{
	if (m_currentWindowIndex != GUI::WINDOW::WindowIndex_nothing)
	{
		m_pVWindow[m_currentWindowIndex]->Reset();
	}

	m_currentWindowIndex = index;
}

CWindow* CGUI::GetCurrentWindow()
{
	if (m_currentWindowIndex >= (int)m_pVWindow.size() && m_currentWindowIndex < 0)
	{
		Log("CGUI::GetCurrentWindow() failed, index: ", m_currentWindowIndex);

		return	nullptr;
	}

	return	m_pVWindow[m_currentWindowIndex];
}

bool	CGUI::OpenChildWidget(CMessage *message)
{
	WidgetType type;

	int response = GetCurrentWindow()->OpenChildWidget(type);

	if (response == -1)
	{
		return	false;
	}
	else if (response == 0) // zero value means it's already being shown 
	{
		message->SetDestroyWidgets();
		return	true;
	}
	else if (response == 1) // widget creation
	{
		message->SetDestroyWidgets();

		if (type == WidgetType::textBox)
		{
			message->SetCreateTextBox();
			message->SetStartTextInput();

			return	true;
		}
		else if (type == WidgetType::menu)
		{
			message->SetCreateButtonMenu();
			return	true;
		}
		else if (type == WidgetType::selectBox)
		{
			message->SetCreateSelectBox();
			return	true;
		}
	}

	Log("CGUI::OpenChildWidget failed, wrong type value: ", static_cast<int>(type));
	return	false;
}
