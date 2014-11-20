#include "CWindow.h"
#include "CGUIAction.h"
#include "StructsGUI.h" // contains widget includes
#include "../../IO/IOFunctions.h"
#include "../../Data/Globals.h"
#include "../../Data/Enums.h"
#include "../../Tools/debug.h"
#include "../../Tools/Log.h"
#include "../../Tools/StringWrapper.h"

CWindow::CWindow()
{
	m_pTextBox = nullptr;
	m_pButton = nullptr;
	m_pMenu = nullptr;
	m_pSelectBox = nullptr;
}

CWindow::~CWindow()
{

}

bool	CWindow::Init(const WindowType &type)
{
	m_pTextBox = DD_NEW _textBoxWrapper();
	m_pButton = DD_NEW _buttonWrapper();
	m_pMenu = DD_NEW _menuWrapper();
	m_pSelectBox = DD_NEW _selectBoxWrapper();
	m_pWidgetsClean = DD_NEW _GUITypeIdWrapper();

	m_pressedWidgetID = -1;
	m_activeSelectBoxID = -1;
	m_activeSelectBoxIndex = -1;
	m_activeTextBoxID = -1;
	m_activeTextBoxIndex = -1;

	if (type == WindowType::inGame)
	{
		if (!InitInGameWindow())
		{
			Clean();
			return	false;
		}
	}
	else if (type == WindowType::mainMenu)
	{
		if (!InitMainMenuWindow())
		{
			Clean();
			return	false;
		}
	}
	else if (type == WindowType::options)
	{
		if (!InitOptionsWindow())
		{
			Clean();
			return	false;
		}
	}
	else if (type == WindowType::mapEditor)
	{
		if (!InitMapEditorWindow())
		{
			Clean();
			return	false;
		}
	}
	else if (type == WindowType::mapEditorImage)
	{
		if (!InitMapEditorImageSelectionWindow())
		{
			Clean();
			return	false;
		}
	}

	return	true;
}

void	CWindow::Clean()
{
	if (m_pTextBox != nullptr)
	{
		for (size_t i = 0; i < m_pTextBox->textBox.size(); i++)
		{
			m_pTextBox->textBox[i].VClean();
		}
	}

	if (m_pButton != nullptr)
	{
		for (size_t i = 0; i < m_pButton->button.size(); i++)
		{
			m_pButton->button[i].VClean();
		}
	}

	if (m_pMenu != nullptr)
	{
		for (size_t i = 0; i < m_pMenu->menu.size(); i++)
		{
			m_pMenu->menu[i].VClean();
		}
	}

	if (m_pSelectBox != nullptr)
	{
		for (size_t i = 0; i < m_pSelectBox->selectBox.size(); i++)
		{
			m_pSelectBox->selectBox[i].VClean();
		}
	}

	DD_DELETE(m_pTextBox);
	DD_DELETE(m_pButton);
	DD_DELETE(m_pMenu);
	DD_DELETE(m_pSelectBox);
	DD_DELETE(m_pWidgetsClean);
}

bool	CWindow::InitInGameWindow()
{
	return	true;
}

bool	CWindow::InitMainMenuWindow()
{
	CButton	newGame{ "New game", g_windowWidth / 2 - 96, g_windowHeight - 144 - 64 * 4, 192, 64 };
	newGame.SetOnLeftRelease(WidgetAction::enterNewGame);

	CButton mapEditor{ "Map editor", g_windowWidth / 2 - 96, g_windowHeight - 144 - 64 * 3, 192, 64 };
	mapEditor.SetOnLeftRelease(WidgetAction::enterMapEditor);

	CButton quit{ "Quit", g_windowWidth / 2 - 96, g_windowHeight - 144 - 64 * 2, 192, 64 };
	quit.SetOnLeftRelease(WidgetAction::quitGame);

	m_pButton->button.push_back(newGame);
	m_pButton->button.push_back(mapEditor);
	m_pButton->button.push_back(quit);

	return	true;
}

bool	CWindow::InitMapEditorWindow()
{
	//select box

	CSelectBox mapSize(g_windowWidth / 2 - 128, 64, 256, 256, 3, 160, 222, 186, 10, 10, 10, FontSize::font22);
	mapSize.SetName("mapFileSize");
	mapSize.AddItem({ "Small", "Medium", "Big" });
	mapSize.SetOnLeftRelease({ { WidgetAction::setMapEditorFileSize, true }, { WidgetAction::selectBoxClicked, true } });
	mapSize.SetOnMouseWheel(WidgetAction::scrollSelectBox);

	m_pSelectBox->selectBox.push_back(mapSize);

	CSelectBox	allMaps(g_windowWidth / 2 - 128, 128, 256, 256, 10, 160, 222, 186, 10, 10, 10, FontSize::font16);
	allMaps.SetName("allMaps");
	
	std::vector<std::string> fileNameItems;

	if (!DD::IO::LoadFileNames(fileNameItems, "data\\maps"))
	{
		allMaps.VClean();
		return	false;
	}

	for (size_t i = 0; i < fileNameItems.size(); i++)
	{
		allMaps.AddItem(fileNameItems[i]);
	}

	allMaps.SetOnLeftRelease({ { WidgetAction::selectBoxClicked, true }, { WidgetAction::loadMapEditorFile, true } });
	allMaps.SetOnMouseWheel(WidgetAction::scrollSelectBox);

	m_pSelectBox->selectBox.push_back(allMaps);

	//test
	CSelectBox testSelect(g_windowWidth / 2 - 160, 128, 400, 60,2, 160, 222, 186, 10, 10, 10, FontSize::font16);
	testSelect.SetName("TestSelect");
	testSelect.AddItem({ "Bla", "Bla1", "Bla2", "Bla3", "Bla4", "Bla5" });
	testSelect.SetOnMouseWheel(WidgetAction::scrollSelectBox);
	m_pSelectBox->selectBox.push_back(testSelect);

	// text box

	const	int	text_width = 300;
	const	int	text_height = 200;

	CTextBox fileName{ "File name: ", g_windowWidth / 2 - text_width / 2, g_windowHeight / 2 - text_height / 2, text_width, text_height, 160, 222, 186, 10, 10, 10,20 };
	fileName.SetName(TXT_FILE_NAME);
	fileName.SetOnLeftRelease(WidgetAction::textInput);
	fileName.SetOnEnterReleased({ WidgetAction::setMapEditorFileName, WidgetAction::openChild });

	if (!SetWidgetChild(&fileName, "mapFileSize", WidgetType::selectBox))
		return	false;

	m_pTextBox->textBox.push_back(fileName);

	// menus

	// file menu

	CMenu	Menu(MenuPosition::MenuPos_vertUp, 420, g_windowHeight, 192, 44);
	Menu.SetName("FileMenuMapEditor");

	CButton btnNew("New file");
	
	if (!SetWidgetChild(&btnNew, TXT_FILE_NAME, WidgetType::textBox))
		return	false;
	
	btnNew.SetOnLeftRelease(WidgetAction::openChild);
	
	CButton btnSave("Save file");
	btnSave.SetOnLeftRelease(WidgetAction::saveMapEditorFile);

	CButton btnLoad("Load file");

	if (!SetWidgetChild(&btnLoad, "allMaps", WidgetType::selectBox))
		return	false;

	btnLoad.SetOnLeftRelease(WidgetAction::openChild);
	//btnLoad.SetOnLeftRelease(WidgetAction::loadMapEditorFile);

	CButton	btnTest("Test"); // test

	Menu.InsertButton(btnNew);
	Menu.InsertButton(btnSave);
	Menu.InsertButton(btnLoad);
	Menu.InsertButton(btnTest); // test

	Menu.CalculateMenuSize();

	m_pMenu->menu.push_back(Menu);

	// test menu, depends on file menu being first inside m_pMenu->menu
	CMenu MenuTest(MenuPosition::MenuPos_horizLeft, m_pMenu->menu[0].GetButton(3)->GetX(), m_pMenu->menu[0].GetButton(3)->GetY(), 192, 44);
	MenuTest.SetName("MenuTest");

	CButton	btnTest1("Test1");
	CButton	btnTest2("Test2");

	if (!SetWidgetChild(&btnTest2, "TestSelect", WidgetType::selectBox))
		return	false;
	btnTest2.SetOnLeftRelease(WidgetAction::openChild);

	MenuTest.InsertButton(btnTest1);
	MenuTest.InsertButton(btnTest2);

	MenuTest.CalculateMenuSize();

	m_pMenu->menu.push_back(MenuTest);

	if (!SetWidgetChild(m_pMenu->menu[0].GetButton(3), "MenuTest", WidgetType::menu))
		return	false;
	m_pMenu->menu[0].GetButton(3)->SetOnLeftRelease(WidgetAction::openChild);

	// image selection menu

	CMenu	ImageMenu(MenuPosition::MenuPos_vertUp, 220, g_windowHeight, 192, 44);
	ImageMenu.SetName("ImageWindowMapEditor");

	CButton	btnAllTiles("Tiles");
	btnAllTiles.SetOnLeftRelease(WidgetAction::enterMapEditorAllTiles);

	CButton	btnAllMonsters("Monsters");
	btnAllMonsters.SetOnLeftRelease(WidgetAction::enterMapEditorAllMonsters);

	ImageMenu.InsertButton(btnAllTiles);
	ImageMenu.InsertButton(btnAllMonsters);

	ImageMenu.CalculateMenuSize();

	m_pMenu->menu.push_back(ImageMenu);

	// buttons

	const int button_width = 192;
	const int button_height = 44;
	const int button_start_pos_x = 20;

	CButton returnToMenu{ "Return", button_start_pos_x, g_windowHeight, button_width, button_height };
	returnToMenu.SetOnLeftRelease(WidgetAction::enterMainMenu);

	CButton tileList{ "Select tiles", button_start_pos_x + button_width + 8, g_windowHeight, button_width, button_height };
	if (!SetWidgetChild(&tileList, "ImageWindowMapEditor", WidgetType::menu))
		return	false;
	tileList.SetOnLeftRelease(WidgetAction::openChild);

	// if some changes are done to x,y update the menu x,y too
	CButton file{ "File", button_start_pos_x + 2 * button_width + 2 * 8, g_windowHeight, button_width, button_height };
	if (!SetWidgetChild(&file, "FileMenuMapEditor", WidgetType::menu))
		return	false;
	file.SetOnLeftRelease(WidgetAction::openChild);//openButtonMenu);

	m_pButton->button.push_back(returnToMenu);
	m_pButton->button.push_back(tileList);
	m_pButton->button.push_back(file);

	return	true;
}

bool	CWindow::InitMapEditorImageSelectionWindow()
{
	const int button_width = 192;
	const int button_height = 44;
	const int button_start_pos_x = 20;

	CButton returnToMapEditor{ "Return", button_start_pos_x, g_windowHeight, button_width, button_height };
	returnToMapEditor.SetOnLeftRelease(WidgetAction::enterMapEditorFromTiles);

	CButton selectTile{ "Select image", button_start_pos_x + 8 + button_width, g_windowHeight, button_width, button_height };
	selectTile.SetOnLeftRelease(WidgetAction::selectTile);

	m_pButton->button.push_back(returnToMapEditor);
	m_pButton->button.push_back(selectTile);

	return	true;
}

bool	CWindow::InitOptionsWindow()
{
	return	true;
}

bool	CWindow::ProcessInput(const _mouseEvents &mouseEvent, const _keyboardEvents &key, const int mouseX, const int mouseY, const int mouseX1, const int mouseY1, const int mouseWheelY, CGUIAction *guiAction)
{
	_widgetResponse response{};

	for (size_t i = 0; i < m_pSelectBox->selectBox.size(); i++)
	{
		if (m_pSelectBox->selectBox[i].IsVisible())
		{
			if (mouseEvent.mouse == MouseEvents::LeftButton)
			{
				if (mouseEvent.state == InputState::pressed)
				{
					response = m_pSelectBox->selectBox[i].OnMouseLeftPress(mouseX, mouseY);
				}
				else if (mouseEvent.state == InputState::released)
				{
					response = m_pSelectBox->selectBox[i].OnMouseLeftRelease(mouseX, mouseY);
				}
			}
			else if (mouseEvent.mouse == MouseEvents::RightButton)
			{
				if (mouseEvent.state == InputState::pressed)
				{
					response = m_pSelectBox->selectBox[i].OnMouseRightPress(mouseX, mouseY);
				}
				else if (mouseEvent.state == InputState::released)
				{
					response = m_pSelectBox->selectBox[i].OnMouseRightRelease(mouseX, mouseY);
				}
			}
			
			if (mouseWheelY != 0)
			{
				response = m_pSelectBox->selectBox[i].OnMouseWheel(mouseWheelY, mouseX1, mouseY1);
			}

			int tResponse = UpdateActions(response, mouseEvent.state, guiAction);

			if (tResponse == -1) // error
				return	false;
			else if (tResponse == 1) // update happened
				return	true;
		}
	}

	for (size_t i = 0; i < m_pTextBox->textBox.size(); i++)
	{
		bool	mouse = false;
		bool	keyboard = false;

		if (m_pTextBox->textBox[i].IsVisible())
		{
			if (mouseEvent.mouse == MouseEvents::LeftButton)
			{
				if (mouseEvent.state == InputState::pressed)
				{
					response = m_pTextBox->textBox[i].OnMouseLeftPress(mouseX, mouseY);
					mouse = true;
				}
				else if (mouseEvent.state == InputState::released)
				{
					response = m_pTextBox->textBox[i].OnMouseLeftRelease(mouseX, mouseY);
					mouse = true;
				}
			}
			else if (mouseEvent.mouse == MouseEvents::RightButton)
			{
				if (mouseEvent.state == InputState::pressed)
				{
					response = m_pTextBox->textBox[i].OnMouseRightPress(mouseX, mouseY);
					mouse = true;
				}
				else if (mouseEvent.state == InputState::released)
				{
					response = m_pTextBox->textBox[i].OnMouseRightRelease(mouseX, mouseY);
					mouse = true;
				}
			}
			else if (key.key == KeyboardEvents::Enter)
			{
				if (key.state == InputState::pressed)
				{
					response = m_pTextBox->textBox[i].OnEnterPressed(m_activeTextBoxID);
					keyboard = true;
				}
				else if (key.state == InputState::released)
				{
					response = m_pTextBox->textBox[i].OnEnterReleased(m_activeTextBoxID);
					keyboard = true;
				}
			}

			int tResponse{ 0 };
			
			if (mouse)
			{
				tResponse = UpdateActions(response, mouseEvent.state, guiAction);
			}
			else if (keyboard)
			{
				tResponse = UpdateActions(response, key.state, guiAction);
			}

			if (tResponse == -1) // error
				return	false;
			else if (tResponse == 1) // update happened
				return	true;
		}
	}

	// menu buttons have a higher priority over regular buttons, so if a click happens on them we return, no need to check other widgets
	for (size_t i = 0; i < m_pMenu->menu.size(); i++)
	{
		if (m_pMenu->menu[i].IsShown())
		{
			if (mouseEvent.mouse == MouseEvents::LeftButton)
			{
				if (mouseEvent.state == InputState::pressed)
				{
					response = m_pMenu->menu[i].OnMouseLeftPress(mouseX, mouseY);
				}
				else if (mouseEvent.state == InputState::released)
				{
					response = m_pMenu->menu[i].OnMouseLeftRelease(mouseX, mouseY);
				}
			}
			else if (mouseEvent.mouse == MouseEvents::RightButton)
			{
				if (mouseEvent.state == InputState::pressed)
				{
					response = m_pMenu->menu[i].OnMouseRightPress(mouseX, mouseY);
				}
				else if (mouseEvent.state == InputState::released)
				{
					response = m_pMenu->menu[i].OnMouseRightRelease(mouseX, mouseY);
				}
			}

			int tResponse = UpdateActions(response, mouseEvent.state, guiAction);

			if (tResponse == -1) // error
				return	false;
			else if (tResponse == 1) // update happened
				return	true;
		}
	}

	for (size_t i = 0; i < m_pButton->button.size(); i++)
	{
		// check the hover function. maybe its better if it doesn't have a time component
		//widgetActionMovement = m_vButtons[i].MouseHover(mouseX1,mouseY1,)

		if (mouseEvent.mouse == MouseEvents::LeftButton)
		{
			if (mouseEvent.state == InputState::pressed && m_pButton->button[i].IsSetLeftPress())
			{
				response = m_pButton->button[i].OnMouseLeftPress(mouseX, mouseY);
			}
			else if (mouseEvent.state == InputState::released && m_pButton->button[i].IsSetLeftRelease())
			{
				response = m_pButton->button[i].OnMouseLeftRelease(mouseX, mouseY);
			}
		}
		else if (mouseEvent.mouse == MouseEvents::RightButton)
		{
			if (mouseEvent.state == InputState::pressed && m_pButton->button[i].IsSetRightPress())
			{
				response = m_pButton->button[i].OnMouseRightPress(mouseX, mouseY);
			}
			else if (mouseEvent.state == InputState::released && m_pButton->button[i].IsSetRightRelease())
			{
				response = m_pButton->button[i].OnMouseRightRelease(mouseX, mouseY);
			}
		}

		int tResponse = UpdateActions(response, mouseEvent.state, guiAction);

		if (tResponse == -1) // error
			return	false;
		else if (tResponse == 1) // update happened
			return	true;
	}

	// all buttons have been checked, no action has been returned
	if (mouseEvent.state == InputState::released)  // check if a release click happened
	{
		m_pressedWidgetID = -1; // reset the id for a new press event 
	}

	return	true;
}

int		CWindow::UpdateActions(_widgetResponse &response, const InputState &inputState, CGUIAction *guiAction)
{
	guiAction->AddMovement(response.movementAction);
	guiAction->AddWheel(response.wheelAction);

	response.wheelAction.clear();
	response.movementAction.clear();

	if (response.pressAction.size() > 0) // do we need to update the widgetActions
	{
		if (response.waitForPress) // do we need to wait for a press event 
		{
			if (inputState == InputState::released) // if a release event happened
			{
				if (m_pressedWidgetID == response.id) // check to see if a press event happened on the same widget where the release event happened
				{
					// if true we return the action and reset the m_pressedWidgetID
					 guiAction->AddPressRelease(response.pressAction);

					if (!ReadResponse(response))
						return	-1;

					m_pressedWidgetID = -1;

					return 1;
				}
				// else press event happened on a different widget, we do not want to return an action
				m_pressedWidgetID = -1;
				guiAction->AddPressRelease(WidgetAction::failed);
				return 1;
			}
			else if (inputState == InputState::pressed) // if a press event happened and we are waiting for a press
			{
				m_pressedWidgetID = response.id; // set the pressedWidgetID so that we can check if a release event happens on the same widget
			}
		}

		// if we are not waiting for press 
		// return the action ( in case of pressed event, we just return wait event, no harm done )
		guiAction->AddPressRelease(response.pressAction);

		if (!ReadResponse(response))
			return	-1;

		return 1;
	}
	// we have not updated a widgetAction
	return	0;
}

bool	CWindow::ReadResponse(const _widgetResponse &response)
{
	m_parentID = response.id;

	if (response.childID > -1 && response.childType != WidgetType::nothing)
	{
		m_childWidgetID = response.childID;
		m_childWidgetType = response.childType;
	}
	else 
	{
		for (size_t i = 0; i < response.pressAction.size(); i++)
		{
			if (response.pressAction[i] == WidgetAction::openChild || response.pressAction[i] == WidgetAction::openButtonMenu)
			{
				// error because the game wants to open a child widget and the parent doesn't contain child info required for opening it
				// probably SetWidgetChild hasn't been called on the parent in Init function

				Log("CWindow::ReadResponse failed, probably because there was no SetWidgetChild call. ParentID: ", response.id);

				return	false;
			}
		}
	}

	if (response.type != WidgetType::nothing)
	{
		m_parentWidgetType = response.type;
	}

	return	true;
}

bool	CWindow::IsValidMapEditorSetTile(const int mouseX1, const int mouseY1)
{
	for (size_t i = 0; i < m_pMenu->menu.size(); i++) // check if a movement happened over a menu
	{
		if (m_pMenu->menu[i].IsShown() && m_pMenu->menu[i].MouseWithinMenu(mouseX1, mouseY1))
		{
			return	false;
		}
	}

	// no movement over a menu, check if it's over the map
	if (mouseX1 > 0 && mouseX1 < g_windowWidth && mouseY1 > 0 && mouseY1 < g_windowHeight)
		return true;

	return	false;
}

_buttonWrapper*	CWindow::GetButtons()
{
	return	m_pButton;
}


_menuWrapper*	CWindow::GetAllButtonMenu()
{
	return	m_pMenu;
}

int		CWindow::GetWidgetID(const char *widgetName_, const WidgetType &widgetType_)
{
	int foundID = -1;

	if (widgetType_ == WidgetType::nothing || widgetType_ == WidgetType::widget || widgetName_ == nullptr)
		foundID = -2;
	else if (widgetType_ == WidgetType::textBox)
	{
		if (m_pTextBox == nullptr)
			return	-3;

		for (size_t i = 0; i < m_pTextBox->textBox.size(); i++)
		{
			foundID = m_pTextBox->textBox[i].CompareName(widgetName_);

			if (foundID == 1)
				return	m_pTextBox->textBox[i].GetWidgetID();
		}
	}
	else if (widgetType_ == WidgetType::button)
	{
		if (m_pButton == nullptr)
			return	-3;

		for (size_t i = 0; i < m_pButton->button.size(); i++)
		{
			foundID = m_pButton->button[i].CompareName(widgetName_);

			if (foundID == 1)
				return	m_pButton->button[i].GetWidgetID();
		}
	}
	else if (widgetType_ == WidgetType::menu)
	{
		if (m_pMenu == nullptr)
			return	-3;

		for (size_t i = 0; i < m_pMenu->menu.size(); i++)
		{
			foundID = m_pMenu->menu[i].CompareName(widgetName_);

			if (foundID == 1)
				return	m_pMenu->menu[i].GetWidgetID();
		}
	}
	else if (widgetType_ == WidgetType::selectBox)
	{
		if (m_pSelectBox == nullptr)
			return	-3;

		for (size_t i = 0; i < m_pSelectBox->selectBox.size(); i++)
		{
			foundID = m_pSelectBox->selectBox[i].CompareName(widgetName_);

			if (foundID == 1)
				return	m_pSelectBox->selectBox[i].GetWidgetID();
		}
	}

	return	-1;
}

bool	CWindow::SetWidgetChild(CWidget *parent,const char *childName, const WidgetType &childType)
{
	int id = GetWidgetID(childName, childType);

	if (id == -3)
	{
		Log("Widget of type WidgetType == nullptr on SetWidgetChild");
		return	false;
	}
	else if (id == -2)
	{
		Log("Invalid arguments on SetWidgetChild");
		return	false;
	}
	else if (id == -1)
	{
		Log("Child not found on SetWidgetChild");
		return	false;
	}

	parent->SetChildID(id);
	parent->SetChildWidgetType(childType);

	return	true;
}

int	CWindow::OpenMenu(int index_)
{
	auto *Menu = &m_pMenu->menu[index_];

	if (Menu->IsVisible()) // already visible ? close it
	{
		return	0;
	}

	Menu->SetShown();

	int	childMenuID = Menu->GetWidgetID();

	for (size_t i = 0; i < m_pMenu->menu.size(); i++)
	{
		auto *tmpMenu = &m_pMenu->menu[i];

		int limit = tmpMenu->GetButtons()->size();

		for (int j = 0; j < limit; j++)
		{
			int tmpId = tmpMenu->GetButtons()->at(j).GetChildID();

			if (tmpId == childMenuID)
			{
				tmpMenu->SetOpenedChildID(childMenuID); // the parent menu of the newly shown menu child will get the childs ID
				Menu->SetParentID(m_pMenu->menu[i].GetWidgetID());
			}
		}
	}

	return	1;
}

int	CWindow::OpenTextBox(int index_)
{
	if (m_pTextBox->textBox[index_].IsVisible())
		return	0;

	m_pTextBox->textBox[index_].SetShown();

	m_activeTextBoxID = m_pTextBox->textBox[index_].GetWidgetID();
	m_activeTextBoxIndex = index_;

	return	1;
}

int	CWindow::OpenSelectBox(int index_)
{
	if (m_pSelectBox->selectBox[index_].IsVisible())
		return	0;

	m_pSelectBox->selectBox[index_].SetShown();

	m_activeSelectBoxID = m_pSelectBox->selectBox[index_].GetWidgetID();
	m_activeSelectBoxIndex = index_;

	return	1;
}

int		CWindow::OpenChildWidget(WidgetType &type)
{
	int i_ = GetWidgetIndex(m_childWidgetID, m_childWidgetType); // get the widget array position

	if (i_ < 0) // no widget with m_childWidgetID, m_childWidgetType found
		return	-1;

	CWidget *child = GetWidget(i_, m_childWidgetType);

	bool	visible = child->IsVisible(); // to preserve widget visibility (HideAllMenuButtonsChild may hide the widget, so child->IsVisible will fail and the widget will be recreated
										  // to prevent that we use this temp variable

	if (m_parentWidgetType == WidgetType::menu) // click happened inside a menu ?
	{
		if (m_childWidgetType != WidgetType::menu) // if we are not opening a menu, close the menu parents
		{
			// close every parent and child widget

			HideAllMenuButtonsChild();
			HideAllMenuButtons();
		}
		else if (child != nullptr) // otvaramo neki izbornik
		{
			if (HideAllMenuButtons(child->GetWidgetID()) == 0)
				return	0;
		}

		// close every child
		if (child->IsVisible()) // child already opened ?
		{
			if (HideAllChildren(child, i_, m_childWidgetType))
				return	0; // child already opened, return 0 because we want to prevent re-opening
			else
				return	-1;
		}

		if (!visible)
			return	OpenWidget(type, i_); // open the new child widget
		else
			return	0;
	}
	else // other widgets
	{
		if (child != nullptr && child->IsVisible()) // is the child already visible ? (open) 
		{
			HideAllMenuButtonsChild();
			// close every child
			if (HideAllChildren(child, i_, m_childWidgetType))
				return	0; // child already opened, do not open a new one !
			else
				return	-1; // error
		}
		else // open a new child
		{
			HideAllMenuButtons(); // if any menu is open close it because only one menu may be opened with its children
			return	OpenWidget(type, i_); 
		}

	}

	return	-1;
}

int		CWindow::OpenWidget(WidgetType &type, int index_)
{
	if (m_childWidgetType == WidgetType::textBox)
	{
		type = WidgetType::textBox;
		return	OpenTextBox(index_);
	}
	else if (m_childWidgetType == WidgetType::button)
	{
		type = WidgetType::button;
	}
	else if (m_childWidgetType == WidgetType::menu)
	{
		type = WidgetType::menu;
		return	OpenMenu(index_);
	}
	else if (m_childWidgetType == WidgetType::selectBox)
	{
		type = WidgetType::selectBox;
		return	OpenSelectBox(index_);
	}

	Log("OpenChildWidget: somehow the type is wrong !?");

	return	-1;
}

int		CWindow::GetWidgetIndex(const int widgetID, const WidgetType &type)
{
	if (type == WidgetType::button)
	{
		for (size_t i = 0; i < m_pButton->button.size(); i++)
		{
			if (widgetID == m_pButton->button[i].GetWidgetID())
			{
				return	i;
			}
		}
	}
	else if (type == WidgetType::menu)
	{
		for (size_t i = 0; i < m_pMenu->menu.size(); i++)
		{
			if (widgetID == m_pMenu->menu[i].GetWidgetID())
			{
				return	i;
			}
		}
	}
	else if (type == WidgetType::textBox)
	{
		for (size_t i = 0; i < m_pTextBox->textBox.size(); i++)
		{
			if (widgetID == m_pTextBox->textBox[i].GetWidgetID())
			{
				return	i;
			}
		}
	}
	else if (type == WidgetType::selectBox)
	{
		for (size_t i = 0; i < m_pSelectBox->selectBox.size(); i++)
		{
			if (widgetID == m_pSelectBox->selectBox[i].GetWidgetID())
			{
				return	i;
			}
		}
	}

	Log("GetWidgetIndex failed: there's no widget with the requested type and id value: ", widgetID);

	return	-1;
}

bool	CWindow::DeleteLastLetterFromTextBox()
{
	if (m_pTextBox == nullptr)
	{
		Log("DeleteLastLetterFromTextBox: textBox wrapper = nullptr");
		return	false;
	}

	if (m_activeTextBoxID < 0)
	{
		Log("DeleteLastLetterFromTextBox: wrong id: ", m_activeTextBoxID);
		return	false;
	}

	if (m_activeTextBoxIndex < 0 || m_activeTextBoxIndex >= (int)m_pTextBox->textBox.size())
	{
		Log("DeleteLastLetterFromTextBox: wrong idindex ", m_activeTextBoxIndex);
		return	false;
	}

	m_pTextBox->textBox[m_activeTextBoxIndex].DeleteLastChar();
	
	return	true;
}

bool	CWindow::ClearActiveTextBoxText()
{
	if (m_pTextBox == nullptr)
	{
		Log("ClearActiveTextBoxText: textBox wrapper = nullptr");
		return	false;
	}
	
	if (m_activeTextBoxID < 0)
	{
		Log("ClearActiveTextBoxText: wrong id: ", m_activeTextBoxID);
		return	false;
	}

	if (m_activeTextBoxIndex < 0 || m_activeTextBoxIndex >= (int)m_pTextBox->textBox.size())
	{
		Log("ClearActiveTextBoxText: wrong idindex ", m_activeTextBoxIndex);
		return	false;
	}

	m_pTextBox->textBox[m_activeTextBoxIndex].ClearText();
	
	return	true;
}

CTextBox*	CWindow::GetActiveTextBox()
{
	if (m_pTextBox == nullptr)
		return	nullptr;

	//if (m_activeTextBoxID < 0)
		//return	nullptr;

	if (m_activeTextBoxIndex < 0 || m_activeTextBoxIndex >= (int)m_pTextBox->textBox.size())
		return	nullptr;

	return	&m_pTextBox->textBox[m_activeTextBoxIndex];
}

CSelectBox*	CWindow::GetActiveSelectBox()
{
	if (m_pSelectBox == nullptr)
		return	nullptr;

	if (m_activeSelectBoxIndex < 0 || m_activeSelectBoxIndex >= (int)m_pSelectBox->selectBox.size())
		return	nullptr;

	return	&m_pSelectBox->selectBox[m_activeSelectBoxIndex];
}

void	CWindow::Reset()
{
	for (size_t i = 0; i < GetAllButtonMenu()->menu.size(); i++)
	{
		if (!GetAllButtonMenu()->menu[i].IsDefaultVisible())
		{
			GetAllButtonMenu()->menu[i].SetHidden();
		}
	}

	for (size_t i = 0; i < m_pTextBox->textBox.size(); i++)
	{
		if (m_pTextBox->textBox[i].IsVisible())
		{
			m_pTextBox->textBox[i].VReset();
		}
	}

	for (size_t i = 0; i < m_pSelectBox->selectBox.size(); i++)
	{
		if (m_pSelectBox->selectBox[i].IsVisible())
		{
			m_pSelectBox->selectBox[i].VReset();
		}
	}
}

CWidget*	CWindow::GetWidget(int index, const WidgetType &type)
{
	if (type == WidgetType::textBox)
	{
		return	&m_pTextBox->textBox[index];
	}
	else if (type == WidgetType::button)
	{
		return	&m_pButton->button[index];
	}
	else if (type == WidgetType::menu)
	{
		return	&m_pMenu->menu[index];
	}
	else if (type == WidgetType::selectBox)
	{
		return	&m_pSelectBox->selectBox[index];
	}

	return	nullptr;
}

CWidget*	CWindow::GetButtonInsideMenu(const int buttonID)
{
	for (size_t i = 0; i < m_pMenu->menu.size(); i++)
	{
		int index = m_pMenu->menu[i].GetButtonIndex(buttonID);
		
		if (index > -1)
		{
			return	m_pMenu->menu[i].GetButton(index);
		}
	}

	return	nullptr;
}

void	CWindow::HideAllMenuButtons()
{
	for (size_t i = 0; i < m_pMenu->menu.size(); i++)
	{
		if (m_pMenu->menu[i].IsVisible())
		{
			//HideAllChildren(&m_pMenu->menu[i], i, WidgetType::menu);
			m_pMenu->menu[i].Update(m_pWidgetsClean);

			m_pMenu->menu[i].VReset();
		}
	}
}

int		CWindow::HideAllMenuButtons(const int widgetID)
{
	// we have the child widget id passed (widgetID), we need the parent

	for (size_t j = 0; j < m_pMenu->menu.size(); j++)
	{
		if (m_pMenu->menu[j].IsParent(widgetID)) // found the parent
		{
			int id = m_pMenu->menu[j].GetOpenedChildID();

			int index = GetWidgetIndex(id, WidgetType::menu);

			if (index < 0)
				return	-1;

			CWidget *child = GetWidget(index, WidgetType::menu);

			bool	visible = child->IsVisible();

			HideAllChildren(child, index, WidgetType::menu);

			if (id == widgetID && visible)
				return	0;
			else
				return	1;
		}
	}

	return	-1;
}

void		CWindow::HideAllMenuButtonsChild()
{
	for (size_t i = 0; i < m_pMenu->menu.size(); i++) // search all the menus
	{
		//if (m_pMenu->menu[i].IsVisible()) // is the menu visible ?
		{
			int buttonN = m_pMenu->menu[i].GetButtonsSize();

			for (int j = 0; j < buttonN; j++) // search the buttons inside the menu
			{
				CWidget *tButton = &m_pMenu->menu[i].GetButtons()->at(j); // get the button

				int childID = tButton->GetChildID();
				WidgetType childType = tButton->GetChildWidgetType();

				if (childType != WidgetType::menu && childType != WidgetType::nothing && childID > -1) // is the child not a menu and if there is one
				{
					int btnChildIndex = GetWidgetIndex(childID, childType);
					CWidget *tBtnChild = GetWidget(btnChildIndex, childType); // get the child of the button

					if (tBtnChild != nullptr && tBtnChild->IsVisible()) // is the child visible
					{
						_GUITypeId tmp{};
						tmp.id = childID;
						tmp.type = childType;

						m_pWidgetsClean->GUITypeId.push_back(tmp); // close it

						tBtnChild->VReset();
					}
				}
			}
		}
	}

	return;

	_GUITypeIdWrapper tmp;
	m_pMenu->menu[0].Update(&tmp); // brojac treba

	for (size_t i = 0; i < tmp.GUITypeId.size(); i++) // search the buttons inside the menu
	{
		m_pWidgetsClean->GUITypeId.push_back(tmp.GUITypeId[i]);

		CWidget *tBtn = GetButtonInsideMenu(tmp.GUITypeId[i].id);

		int btnChildID = tBtn->GetChildID();
		WidgetType btnType = tBtn->GetChildWidgetType();

		if (btnType != WidgetType::menu && btnType != WidgetType::nothing && btnChildID > -1) // is the child not a menu and if there is one
		{
			int btnChildIndex = GetWidgetIndex(btnChildID, btnType);
			CWidget *tBtnChild = GetWidget(btnChildIndex, btnType);

			if (tBtnChild != nullptr && tBtnChild->IsVisible()) // is the child visible
			{
				_GUITypeId tmp1{};
				tmp1.id = btnChildID;
				tmp1.type = btnType;

				m_pWidgetsClean->GUITypeId.push_back(tmp1); // close it

				tBtnChild->VReset();
			}
		}
	}
}

bool		CWindow::HideAllChildren(CWidget *child_, const int index, const WidgetType type)
{
	if (child_ == nullptr)
	{
		child_ = GetWidget(index, type);

		if (child_ == nullptr)
		{
			Log("HideAllChildren failed, parentIndex: ", index);
			return	false;
		}
	}

	bool search = true;
	int	childID{ child_->GetWidgetID() };
	int childIndex{ index };
	WidgetType	childType{ type };

	CWidget *child = child_;

	while (search)
	{
		if (child == nullptr)
		{
			Log("HideAllChildren failed, childIndex: ", childIndex);
			Log("childType: ", static_cast<int>(childType));
			return	false;
		}
		else
		{
			if (child->IsVisible()) // child found
			{
				if (childType == WidgetType::menu) // for a menu we want buttons inside it
				{
					m_pMenu->menu[childIndex].Update(m_pWidgetsClean);
				}
				else
				{
					_GUITypeId tmp{};
					tmp.id = childID;
					tmp.type = childType;

					m_pWidgetsClean->GUITypeId.push_back(tmp); // render needs to remove these widgets
				}

				child->VClear(); // reset child data and the widget will be hidden
			}
			else // child not visible, do not search anymore
			{
				return	true;
			}
		}

		if (childType == WidgetType::menu && child->GetOpenedChildID() != -1)
		{
			childID = child->GetOpenedChildID();
			// no need to touch childType, WidgetType::menu will be preserved
		}
		else
		{
			childID = child->GetChildID();

			if (childID != -1)
			{
				childType = child->GetChildWidgetType();
			}
		}

		if (childID == -1)
		{
			search = false;
		}
		else // child found
		{
			childIndex = GetWidgetIndex(childID, childType);

			child = GetWidget(childIndex, childType);
		}
	}

	return	true;
}

_GUITypeIdWrapper*	CWindow::GetDestroyWidgetData()
{
	return	m_pWidgetsClean;
}

void	CWindow::ClearDestroyWidgetData()
{
	m_pWidgetsClean->GUITypeId.clear();
}

void	CWindow::StartTextInput()
{
	m_activeTextBoxID = m_parentID;
	
	int index = GetWidgetIndex(m_activeTextBoxID, WidgetType::textBox);

	m_activeTextBoxIndex = index;
}

void	CWindow::StopTextInput()
{
	m_activeTextBoxID = -1;
}

bool	CWindow::IsRecievingTextInput()
{
	if (m_activeTextBoxID < 0)
		return	false;
	return	true;
}

bool	CWindow::SetActiveSelectBox()
{
	m_activeSelectBoxID = m_parentID;

	int index = GetWidgetIndex(m_activeSelectBoxID, WidgetType::selectBox);

	if (index < 0)
	{
		Log("CWidget::SetActiveSelectBox failed, no widget with id:", m_parentID);
		return	false;
	}

	m_activeSelectBoxIndex = index;

	return	true;
}

void	CWindow::UnsetActiveSelectBox()
{
	m_activeSelectBoxID = -1;
}

int	CWindow::AddTextToTextBox(const char *text)
{
	if (text == nullptr)
	{
		Log("AddTextToTextBox: text = nullptr");
		return	-1;
	}

	if (m_pTextBox == nullptr)
	{
		Log("AddTextToTextBox: textBox wrapper = nullptr");
		return	-1;
	}

	if (m_activeTextBoxID < 0)
	{
		Log("AddTextToTextBox: wrong id: ", m_activeTextBoxID);
		return	-1;
	}

	if (m_activeTextBoxIndex < 0 || m_activeTextBoxIndex >= (int)m_pTextBox->textBox.size())
	{
		Log("AddTextToTextBox: wrong idindex ", m_activeTextBoxIndex);
		return	-1;
	}

	if (!m_pTextBox->textBox[m_activeTextBoxIndex].AddText(text))
		return	0;

	return	1;
}

bool	CWindow::CleanUpSelectBox()
{
	if (m_pSelectBox == nullptr)
	{
		Log("CleanUpSelectBox failed: select box = nullptr");
		return	false;
	}

	if (m_activeSelectBoxID < 0)
	{
		Log("CleanUpSelectBox failed, wrong id: ", m_activeSelectBoxID);
		return	false;
	}

	if (m_activeSelectBoxIndex < 0 || m_activeSelectBoxIndex >= (int)m_pSelectBox->selectBox.size())
	{
		Log("CleanUpSelectBox, wrong index: ", m_activeSelectBoxIndex);
		return	false;
	}

	_GUITypeId tmp;

	tmp.id = m_pSelectBox->selectBox[m_activeSelectBoxIndex].GetWidgetID();
	tmp.type = WidgetType::selectBox;

	m_pWidgetsClean->GUITypeId.push_back(tmp);

	m_pSelectBox->selectBox[m_activeSelectBoxIndex].VClear();

	m_activeSelectBoxID = -1;
	m_activeSelectBoxIndex = -1;

	return	true;
}

bool	CWindow::CleanUpTextBox()
{
	if (m_pTextBox == nullptr)
	{
		Log("CleanUpTextBox failed: text box = nullptr");
		return	false;
	}

	if (m_activeTextBoxID < 0)
	{
		Log("CleanUpTextBox: wrong id: ", m_activeTextBoxID);
		return	false;
	}

	if (m_activeTextBoxIndex < 0 || m_activeTextBoxIndex >= (int)m_pTextBox->textBox.size())
	{
		Log("CleanUpTextBox: wrong index ", m_activeTextBoxIndex);
		return	false;
	}

	SetTextBoxDestroyWidgetData();

	m_pTextBox->textBox[m_activeTextBoxIndex].VClear();

	m_activeTextBoxID = -1;
	m_activeTextBoxIndex = -1;

	return	true;
}

void	CWindow::SetTextBoxDestroyWidgetData()
{
	_GUITypeId tmp;

	tmp.id = m_pTextBox->textBox[m_activeTextBoxIndex].GetWidgetID();
	tmp.type = WidgetType::textBox;

	m_pWidgetsClean->GUITypeId.push_back(tmp);
}

void	CWindow::SetSelectBoxDestroyWidgetData()
{
	_GUITypeId	tmp;

	tmp.id = m_pSelectBox->selectBox[m_activeSelectBoxIndex].GetWidgetID();
	tmp.type = WidgetType::selectBox;

	m_pWidgetsClean->GUITypeId.push_back(tmp);
}

WidgetType	CWindow::ChildWidgetType()
{
	return	m_childWidgetType;
}

bool	CWindow::AddMapToAllMaps(const char* mapName)
{
	if (strlen(mapName) < 1)
	{
		return	true;
	}

	for (size_t i = 0; i < m_pSelectBox->selectBox.size(); i++)
	{
		if (m_pSelectBox->selectBox[i].CompareName("allMaps") == 1)
		{
			if (m_pSelectBox->selectBox[i].ItemExists(mapName) == false)
			{
				m_pSelectBox->selectBox[i].AddItem(mapName);
			}
			
			return	true;
		}
	}

	Log("CWindow::AddMapToAllMaps() failed");
	return	false;
}
