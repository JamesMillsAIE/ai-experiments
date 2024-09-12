#ifdef _DEBUG
#include <crtdbg.h>
#endif // _DEBUG

#include <cstdlib>

#include "AiApplication.h"

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	AiApplication* app = new AiApplication;

	app->Run();

	delete app;

	return EXIT_SUCCESS;
}