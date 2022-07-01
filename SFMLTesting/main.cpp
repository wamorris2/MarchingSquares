#include "MarchingSquares.h"

int main()
{
	MarchingSquares ms;

	while (ms.running())
	{
		ms.update();
		ms.render();
	}
	
	return 0;
}