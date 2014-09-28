#include <zmq.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "times.h"
#include "terminator.h"
#include "event.h"

int MILLION = 1000000;

int main(int argc, char *argv[]) {
	setPauseLenNanos(argv[1]);
	setTerm();

	printf("Setting up ZeroMQ pubsub-system\n");
	int rc;
	void *context = zmq_ctx_new();
	void *publisher = zmq_socket(context, ZMQ_PUB);
	rc = zmq_bind(publisher, argv[2]);
	assert(rc == 0);
	printf("ZeroMQ up\n");
	printf("Pub bind to %s\n", argv[2]);

	struct eventHeader eh[MILLION];
	struct event e;
	long idCount = 0;
	char buffer[100];
	while (terminate() == 0) {
		strcpy(eh[idCount].pub, "pub1");
		eh[idCount].id = idCount;
		eh[idCount].created = currentNanos();
		e.header = eh[idCount];
		strcpy(e.data, dummyData);

		zmq_send(publisher, &e, sizeof(e), 0);

		eh[idCount].published = currentNanos();
		pause(currentNanos());
		if (idCount % MILLION == 0) {
			//async mongo header
		}
		printf("sent\n");
	}
	printf("ZeroMQ down\n");

	zmq_close(publisher);
	zmq_ctx_destroy(context);
	return 0;
}
