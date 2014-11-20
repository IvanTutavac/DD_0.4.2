#pragma once

enum class MsgToRender
{
	nothing,
	initInGame,
	initMainMenu,
	initMapEditor,
	initMapEditorAllTiles,
	startNewMapInGame,
	startWeaponMainAnimation,
	setInGameHUDSpellData,
	setInGameHUDWeapAttData,
	createButtonMenu,
	closeButtonMenu,
	createTextBox,
	closeTextBox,
	createAllTextBox,
	createSelectBox,
	closeAllTextBox,
	closeAllSelectBox,
	updateTextBox, // message to render so that it deletes the active text box and creates a new one
	destroyWidgets // reads from GUI which widgets to destroy
};

enum class MsgToInput
{
	nothing,
	startTextInput,
	stopTextInput
};

enum class MsgToLogic
{
	nothing,
};
