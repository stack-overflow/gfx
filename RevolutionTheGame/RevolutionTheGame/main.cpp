#include "SOSystem.h"
#include "Shader.h"
#include "Model.h"

#include "Utils.h"

#include "MathUtils.h"

int __cdecl main()
{
	SOSystem *system = new SOSystem();

	if (!system)
	{
		return 1;
	}

	bool result = system->Initialize();

	if (result)
	{
		system->Run();
	}

	delete system;

	return 0;
}
