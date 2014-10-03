#include <zmq.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "times.h"
#include "pubsub.h"
#include "../modules/terminator.h"
#include "event.h"

int main(int argc, char *argv[]) {
	long pauseNanos = atol(argv[1]);
	char *address = argv[2];
	char *channel = argv[3];
	int msgLen = atoi(argv[4]);
	initTerminator();
	initEventStore("EVENTSTORE-PUB");
	initPub(address, channel);
	printf("Running with params: %d, %s, %s, %d\n", pauseNanos, address, channel, msgLen);

	long idCount = 0;
	char eData[msgLen];
	memset(eData, 'A', msgLen);
	while (killSignal == 0) {
		struct eventHeader eh = { .source = channel, .id = idCount, .created = currentTime() };
		struct event e = { .header = eh, .data = eData };
		pub(&e);
		e.header.published = currentTime();
		pause(pauseNanos);
		storeEvent(&e);
		idCount++;
	}

	finalizeEventStore();
	destroyPub();
	return 0;
}
