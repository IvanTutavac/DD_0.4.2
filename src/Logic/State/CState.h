#pragma once

enum class GameState;

class CState
{
	GameState	m_gameState;

public:

	CState();
	~CState();

	bool	Init();
	void	Clean();

public:

	bool	IsGameStateInGame() const;
	void	SetGameStateInGame();

	bool	IsGameStateMainMenu() const;
	void	SetGameStateMainMenu();

	bool	IsGameStateMainOptions() const;
	void	SetGameStateMainOptions();

	bool	IsGameStateMapEditor() const;
	void	SetGameStateMapEditor();
};
