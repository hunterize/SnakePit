#pragma once
namespace SnakEngine
{
	enum class ScreenState 
	{
		NONE,
		RUNNING,
		EXIT_APP,
		CHANGING_NEXT,
		CHANGING_PREVIOUS
	};

	class IGameWorld;

	class IGameScreen
	{
		friend class CScreenList;
	public:
		IGameScreen();
		virtual ~IGameScreen();

		virtual int GetNextScreenIndex() const = 0;
		virtual int GetPreviousScreenIndex() const = 0;

		virtual void Create() = 0;
		virtual void Destroy() = 0;

		virtual void OnStart() = 0;
		virtual void OnExit() = 0;

		virtual void Update() = 0;
		virtual void Draw() = 0;

		int GetScreenIndex();
		ScreenState GetScreenState();
		void SetParentGame(IGameWorld* pGame);
		void SetRunning();

	protected:
		IGameWorld* m_pGame = nullptr;
		ScreenState m_eState = ScreenState::NONE;
		int m_iIndex = -1;
	};

}
