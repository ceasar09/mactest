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

static void do_accept(struct ev_loop* reactor, ev_io* w, int events) {
	if (listenFd == w->fd) {
		struct sockaddr_in addr;
		socklen_t addr_size = sizeof(addr);
		int conn = accept(w->fd, (struct sockaddr*) &addr, &addr_size);
		std::string r = common::address_to_string(&addr);
		fprintf(stderr, "accept %s\n", r.c_str());
		//close(conn);
		char buf[1024];
		int ret = recv(conn, buf, 1024, 0);
		cout << "ret is " << ret << endl;
		fprintf(stderr, "recv %s\n", buf);
	}
	else{
		char buf[1024];
		int ret = recv(w->fd, buf, 1024, 0);
		fprintf(stderr, "accept %s\n", buf);
	}
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
