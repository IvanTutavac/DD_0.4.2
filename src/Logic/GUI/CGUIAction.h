#pragma once

#include <vector>

enum class WidgetAction;

// response from CWindow to CLogic
class CGUIAction 
{

private:

	std::vector<WidgetAction>	m_pressRelease;
	std::vector<WidgetAction>	m_movement;
	std::vector<WidgetAction>	m_wheel;

public:

	CGUIAction();
	~CGUIAction();


	void	AddPressRelease(const std::vector<WidgetAction> &action);
	void	AddPressRelease(const WidgetAction &action);

	void	AddMovement(const std::vector<WidgetAction> &action);
	void	AddMovement(const WidgetAction &action);

	void	AddWheel(const std::vector<WidgetAction> &action);
	void	AddWheel(const WidgetAction &action);

	// returns true if there's a press release action
	bool	IsPressRelease();

	// returns true if there's a mouse movement action
	bool	IsMovement();

	// returns true if there's a mouse wheel action
	bool	IsWheel();

	// returns last WidgetAction for press release and removes it from the object
	WidgetAction	PopPressReleaseAction();

	// returns last WidgetAction for movement and removes it from the object
	WidgetAction	PopMovementAction();

	WidgetAction	PopWheelAction();

	// empties data
	void	Clear();
};