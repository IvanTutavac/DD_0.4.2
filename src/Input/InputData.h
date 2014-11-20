#pragma once

#include <string>

enum class InputState;
enum class UpDown;
enum class LeftRight;
enum class KeyboardEvents;
enum class MouseEvents;

struct _upDown
{
	InputState state; // nothing, pressed, released
	UpDown	upDown; // up or down
};

struct _leftRight
{
	InputState state; // nothing, pressed, released
	LeftRight leftRight; // left or right
};

struct _keyboardEvents
{
	InputState state; // nothing, pressed, released
	KeyboardEvents key; // keyboard input
};

struct _mouseEvents
{
	InputState state; // nothing, pressed, released
	MouseEvents mouse; // mouse input
};

struct _inputMessage
{
	int x; // press, release mouse event 
	int y; // press, release mouse event
	int x1; // mouse motion
	int y1; // mouse motion
	int y2; // mouse wheel, positive (up), negative (down) movement, if equal to 0 -> there was no movement

	char letter[8];
	bool written; // true means letter has been updated

	_upDown upDown;
	_leftRight leftRight;
	_keyboardEvents key;
	_mouseEvents mouse;
};

struct _mappedMouse
{
	MouseEvents m;
	std::string s; // string value of MouseEvent

	_mappedMouse(){}
	_mappedMouse(std::string s_, MouseEvents m_) : s(s_), m(m_){}
};

struct _mappedKey
{
	KeyboardEvents k;
	std::string s; // string value of KeyboardEvent

	_mappedKey(){}
	_mappedKey(std::string s_, KeyboardEvents k_) : s(s_), k(k_){}
};

struct _mappedUpDown
{
	UpDown uD;
	std::string s; // string value of UpDown

	_mappedUpDown(){}
	_mappedUpDown(std::string s_, UpDown uD_) : s(s_), uD(uD_){}
};

struct _mappedLeftRight
{
	LeftRight lR;
	std::string s; // string value of LeftRight

	_mappedLeftRight(){}
	_mappedLeftRight(std::string s_, LeftRight lR_) : s(s_), lR(lR_){}
};

