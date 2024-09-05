#pragma once

#include <list>

using std::list;
using std::pair;

class Renderer2D;

namespace Debugging
{
	class IDebugHandler;

	enum EVerbosity
	{
		Basic,
		Advanced
	};

	class Debugger
	{
		friend class Game;

	private:
		typedef void(Debugger::* ListChange)(IDebugHandler*);

	public:
		static void ObserveDebugTool(IDebugHandler* handler);
		static void ForgetDebugTool(IDebugHandler* handler);
		static void SetVerbosity(EVerbosity verbosity);
		static bool IsDebuggingEnabled();

	private:
		static void Create();
		static void Destroy();

	private:
		static Debugger* m_instance;

	private:
		list<pair<ListChange, IDebugHandler*>> m_changes;
		list<IDebugHandler*> m_handlers;

		EVerbosity m_verbosity;
		bool m_isEnabled;

	private:
		Debugger();
		~Debugger();

	private:
		void Tick();
		void Render(Renderer2D* renderer);

		void AddHandler(IDebugHandler* handler);
		void RemoveHandler(IDebugHandler* handler);

	};
}