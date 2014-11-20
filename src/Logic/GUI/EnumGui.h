#pragma once

// adding open actions to WidgetAction requires CWindow::ReadResponse to be updated !!!

enum class WidgetAction
{
	nothing,
	failed,
	enterNewGame,
	enterMainOptions,
	enterMapEditor,
	enterMapEditorFromTiles,
	quitGame,
	enterMainMenu,
	enterMapEditorAllTiles,
	enterMapEditorAllMonsters,
	selectTile,
	drawTile,
	openButtonMenu,
	saveMapEditorFile,
	loadMapEditorFile,
	openChild, // used when one wants to open a new widget when a certain event happens on a parent widget
	textInput, // this should be set on clicking a text box so that it can recieve input
	selectBoxClicked, // used to set the selectBoxIndex (active select box)
	setMapEditorFileName, // enter pressed on a text box for map editor file name
	setMapEditorFileSize,
	wait, // used to fire a release event if a press event happened on the same widget
	scrollSelectBox
};

enum class WidgetType
{
	nothing, widget, button, menu, textBox, selectBox
};

enum class WindowType
{
	nothing, mainMenu, options, inGame, mapEditor, mapEditorImage
};