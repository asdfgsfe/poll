1.singnal(SIGPIPE, SIG_IGN);
2.singnal(SIGCHLD, SIG_IGN);
3.int idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);
4.int listenfd = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP)
5.struct sockaddr_in servaddr; 初始化地址在bind的时候用
6.setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) 设置地址重复利用
7.bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr))
8.listen(listenfd, SOMAXCONN)
9.准备poll部分
	//struct pollfd {
		//int   fd;         /* file descriptor */
	        //short events;     /* requested events */
		//short revents;    /* returned events */
	//};

	struct pollfd pfd;
	pfd.fd = listenfd;
	pfd.events = POLLIN;
	std::vector<struct pollfd> pollfds;
	pollfds.emplace_back(pfd);
	while (1)
	{
		//int poll(struct pollfd *fds, nfds_t nfds, int timeout);
		int nready = ::poll(pollfds.data(), pollfds.size(), -1);
		if (nready == -1)
		{
			if (error == EINTR)
			{
				continue;
			}
			exit(1);
		}
		if (nready == 0)
		{
			continue;
		}
		if (pollfds[0].revents & POLLIN)
		{
			peerlen = sizeof(peeraddr);
			int connfd = ::accept4(pollfds[0].fd, (struct sockaddr*)&peeraddr, &peerlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
			if (connfd == -1)
			{
				if (error == EMFILE)
				{
					idlefd = ::accept(pollfds[0].fd);
					close(idlefd);
					illefd = open("/dev/null", ...);
				}
			}
			struct pollfd connpfd;
			connpfd.fd = connfd;
			connpfd.events |= POLLIN;
			pollfds.emplace_back(std::move(connpfd));
			--nready;
		}
		for (int pfd = 1; i < pollfd.size() && nready > 0; ++pfd)
		{
			--nready;
			if (pfd[i].revents & POLLIN)
			{
				//处理读事件
			}
			if (pfd[i].revents & POLLOUT)
			{
				//处理写事件
			}
		}
	}
