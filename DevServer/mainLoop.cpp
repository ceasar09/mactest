#include <ev++.h>

//command line params
#include "config.h"
//command line params end
#include <iostream>
using namespace std;
#include "av_redis.h"
#include "av_app_server.h"
#include "av_sql.h"
#include "test.h"

int main_loop_init();

int main() {
	//params analysis

	//init redis server

	redis_init();

	cout << "redis init finished" << endl;

	app_server_init();

	mySql_init();

	main_loop_init();
}

int listenFd;
#include <errno.h>
#define BUFFER_SIZE 1024

void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents){
	char buffer[BUFFER_SIZE];
	ssize_t read;

	if(EV_ERROR & revents) {
		printf("error event in read");
		return;
	}
	cout << "read something" << endl;
	read = recv(watcher->fd, buffer, BUFFER_SIZE, 0);
	if(read < 0){
		printf("read error,errno:%d\n", errno);
		return;
	}
	if(read == 0) {
		printf("someone disconnected.errno:%d\n", errno);
		ev_io_stop(loop,watcher);
		free(watcher);
		return;
	} else {
		printf("get the message:%s\n",buffer);
	}
	cout << buffer << endl;
	send(watcher->fd, buffer, read, 0);
	bzero(buffer, read);
}

static void do_accept(struct ev_loop* loop, ev_io* watcher, int revents) {
	struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		int client_sd;
		cout << "socket accepted" << endl;
		struct ev_io *w_client = (struct ev_io*) malloc (sizeof(struct ev_io));
		if(EV_ERROR & revents){
			printf("error event in accept\n");
			return;
		}

		client_sd = accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);
		if (client_sd < 0) {
			printf("accept error\n");
			return;
		}
		printf("someone connected.\n");

		ev_io_init(w_client, read_cb, client_sd, EV_READ);
		ev_io_start(loop, w_client);
}

int main_loop_init() {
	struct ev_loop* reactor = ev_loop_new(EVFLAG_AUTO);
	listenFd = common::new_tcp_server(SERVE_PORT);
	ev_io w;
	ev_io_init(&w, do_accept, listenFd, EV_READ);
	ev_io_start(reactor, &w);
	ev_run(reactor, 0);
	close (listenFd);
	ev_loop_destroy(reactor);
}
