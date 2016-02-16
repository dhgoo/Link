#include "Precompiled.h"
#include "Link.h"
#include "Dispatcher.h"

int main(int argc, char* argv[])
{
	Dispatcher disp;

	Link link;
	if (link.open(9999, &disp) == false)
		return 1;

	link.run();

	getchar();

	link.stop();

	return 0;
}

