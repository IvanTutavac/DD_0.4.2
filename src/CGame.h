#pragma once

class CRenderManager;
class CInput;
class CLogic;
class ITimer;
class CMessage;
enum class RenderAPI;

class CGame
{
private:
	
	CRenderManager	*m_pRenderManager;
	CInput	*m_pInput;
	CLogic	*m_pLogic;
	CMessage *m_pMessage;
	ITimer	*m_pTimer;

	RenderAPI	m_API; // api used in game, set by LoadConfig

	double	m_seconds; // seconds passed from start, for now used for testing timer

private:

	bool	Render(int mouseX,int mouseY);
	bool	RenderInGame(int mouseX,int mouseY);
	bool	RenderMapEditor(int mouseX,int mouseY);
	bool	ReadMsgToRender();

	bool	Input();

	bool	LoadConfig();

public:

	CGame();
	~CGame();

	bool	Init();
	void	Clean();
	bool	RunGameLoop();
};
