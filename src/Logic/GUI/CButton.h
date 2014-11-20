#pragma once
#include "CWidget.h"
#include "../../Data/Const.h"

enum class GUIAction;

class CButton : public CWidget
{
private:

	char	m_text[BUTTON_TEXT_SIZE];

public:

	CButton() = delete;

	// does not take check character size ( 32 is limit )
	// used for a stand alone button
	CButton(const char *text,const int x,const int y,const int w,const int h,bool visible_ = true);

	// does not take check character size ( 32 is limit )
	// used for a button inside a menu
	CButton(const char *text);

	~CButton();

public:

	// does not take care character size ( 32 is limit )
	void	SetText(const char *text);
	const char*	GetText();

	void	SetPosition(const int x, const int y, const int w, const int h);

	void			VClean() override;

	// reset text, calls parent VClear()
	void			VClear() override;
	// reset state to default, calls VClear 
	void			VReset() override;
};
