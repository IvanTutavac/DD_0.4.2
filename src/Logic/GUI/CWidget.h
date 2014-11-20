#pragma once

#include <utility>

enum class WidgetAction;
enum class WidgetType;
struct _widgetResponse;
struct _widgetActionData;

// class that GUI objects can inherit

class CWidget
{
protected:

	int		m_x, m_y, m_w, m_h;
	double  m_time;
	double  m_timeReset;
	bool	m_hoverEventFired;
	bool	m_hoverEventTimerStarted;
	int		m_id;
	WidgetType m_type; // e.g. can be used to know if a child has been accessed from a menu, if yes close the menu ( will be written to response )
	int		m_parentID; // m_id of the CMenu that contains the button, or if it's a CMenu, it's the id of the parent
	int		m_childID; // m_id of a widget that should be opened after a set event happens on a parent widget
	WidgetType m_childType; // so that we know the childs widget type ( textBox, button, menu .... )
	// childID and childType are used in a _widgetResponse to update CWindow data

	int		m_openedChildID; // if a button inside a menu has been clicked and an additional menu needs to be opened, the parent will have this value updated with the child menu id

	bool			m_visible; // true if visible, false if hidden, default is true
	bool			m_defaultVisible; // used when changing windows so that one knows if a widget was hidden or shown from the beginning or after an event happened on some other widget

	char	*m_name;

	_widgetActionData	*m_pAction;

	

	// returns true if mouseX and mouseY is within widget
	bool		MouseWithinWidget(const int mouseX, const int mouseY) const;

	void		UpdateWidgetOnMouseResponse(_widgetResponse &response);

public:

	CWidget();
	virtual ~CWidget();

	// if a widget overrides this method it must call CWidget::Clean() from it !
	virtual	void	VClean();

public:

	int				GetX();
	int				GetY();
	int				GetW();
	int				GetH();

	// if the event has been enabled check for the condition
	// if true, reduce time by deltaTime. After it reaches 0 reset it and return the action that needs to be taken care of
	// if false after the action has been taken care of, return the ofMouseHover WidgetAction
	// if there's no action to be taken care of it will return ButtonAction::nothing
	_widgetResponse	OnMouseHover(const int mouseX, const int mouseY, const double deltaTime);

	// Window will return an action depending whether a click happened on the widget
	virtual	_widgetResponse	OnMouseLeftPress(const int mouseX, const int mouseY);
	// if press event is needed, the release action will be returned only if the press event happened on the same widget
	virtual	_widgetResponse	OnMouseLeftRelease(const int mouseX, const int mouseY);
	virtual	_widgetResponse	OnMouseRightPress(const int mouseX, const int mouseY);
	virtual	_widgetResponse	OnMouseRightRelease(const int mouseX, const int mouseY);

	// Set events

	// An event can have multiple actions ( for example, opening multiple widgets upon leftPress event )

	void	SetOnLeftPress(const WidgetAction &a);
	// if we do not need a press event before we can use the release event, set pressNeeded as false
	void	SetOnLeftRelease(const WidgetAction &a, bool pressNeeded = true);
	void	SetOnLeftRelease(const std::initializer_list<std::pair<const WidgetAction &,bool>> &values);
	void	SetOnRightPress(const WidgetAction &a);
	// if we do not need a press event before we can use the release event, set pressNeeded as false
	void	SetOnRightRelease(const WidgetAction &a, bool pressNeeded = true);

	// also set time required for the event to activate
	void	SetOnMouseHover(const WidgetAction &OnEntering, const WidgetAction &OnLeaving, const double t);

	bool	IsSetLeftPress();
	bool	IsSetLeftRelease();
	bool	IsSetRightPress();
	bool	IsSetRightRelease();

	bool	IsSetHover();

	int		GetWidgetID();

	void	SetChildID(const int value_);
	int		GetChildID();

	int		GetOpenedChildID() const;
	void	SetOpenedChildID(const int value_);

	int		GetParentID() const;
	void	SetParentID(const int value_);

	void	SetChildWidgetType(const WidgetType &value_);
	WidgetType GetChildWidgetType();

	void	SetName(const char *name_);
	char*	GetName();
	
	// returns 1 if the widget name is equal to name_
	// returns 0 if not equal
	// returns -1 if the argument or the widget name is equal to nullptr
	int		CompareName(const char *name_);

	bool	IsVisible();
	void	SetHidden();
	void	SetShown();

	// returns true if the widget has been visible from the start
	// returns false otherwise
	bool	IsDefaultVisible();

	// resets to default state ( hidden or visible )
	// does not call VReset, child widgets will call VReset from VClear and Parent::VReset from VReset
	virtual void	VClear();
	// hide the widget
	virtual void	VReset();
};
