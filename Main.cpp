#include "Precompiled.h"
#include "Link.h"
#include "Dispatcher.h"

int main(int argc, char* argv[])
{
	Dispatcher disp;

	Link link;
	if (link.init() == false)
		return 1;

	int fd = link.open(9999, &disp);
	if (fd == -1)
		return 1;

	link.run();

	getchar();

	link.stop();
	link.close();

	return 0;
}

