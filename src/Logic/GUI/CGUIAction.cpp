#include "CGUIAction.h"
#include "EnumGui.h"

CGUIAction::CGUIAction()
{

}

CGUIAction::~CGUIAction()
{

}

void CGUIAction::AddPressRelease(const std::vector<WidgetAction> &action)
{
	for (size_t i = 0; i < action.size(); i++)
	{
		m_pressRelease.push_back(action[i]);
	}
}

void	CGUIAction::AddPressRelease(const WidgetAction &action)
{
	m_pressRelease.push_back(action);
}

void CGUIAction::AddWheel(const std::vector<WidgetAction> &action)
{
	for (size_t i = 0; i < action.size(); i++)
	{
		m_wheel.push_back(action[i]);
	}
}

void	CGUIAction::AddWheel(const WidgetAction &action)
{
	m_wheel.push_back(action);
}

void	CGUIAction::AddMovement(const std::vector<WidgetAction> &action)
{
	for (size_t i = 0; i < action.size(); i++)
	{
		m_movement.push_back(action[i]);
	}
}

void	CGUIAction::AddMovement(const WidgetAction &action)
{
	m_movement.push_back(action);
}

void	CGUIAction::Clear()
{
	m_pressRelease.clear();
	m_movement.clear();
}

bool	CGUIAction::IsPressRelease()
{
	if (m_pressRelease.size() > 0)
	{
		return	true;
	}

	return	false;
}

bool	CGUIAction::IsMovement()
{
	if (m_movement.size() > 0)
	{
		return	true;
	}

	return	false;
}

bool	CGUIAction::IsWheel()
{
	if (m_wheel.size() > 0)
	{
		return	true;
	}

	return	false;
}

WidgetAction	CGUIAction::PopPressReleaseAction()
{
	WidgetAction action{ WidgetAction::nothing };

	if (m_pressRelease.size() > 0)
	{
		action = m_pressRelease.back();

		m_pressRelease.pop_back();
	}

	return	action;
}

WidgetAction	CGUIAction::PopMovementAction()
{
	WidgetAction action{ WidgetAction::nothing };

	if (m_movement.size() > 0)
	{
		action = m_movement.back();

		m_movement.pop_back();
	}

	return	action;
}

WidgetAction	CGUIAction::PopWheelAction()
{
	WidgetAction action{ WidgetAction::nothing };

	if (m_wheel.size() > 0)
	{
		action = m_wheel.back();

		m_wheel.pop_back();
	}

	return	action;
}
