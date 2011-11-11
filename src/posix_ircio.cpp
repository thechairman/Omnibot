
void posix_ircio::open(std::string server, int port)
{
	//get a file descriptor
	socket = socket(AF_INET, SOCK_STREAM, O);
	if(socket < 0)
	{
		return false;
	}

	//look up the host
	hostent serverhost = gethostbyname(server.c_str());
	if(serverhost == NULL)
	{
		return false;
	}

	//setup the sock address struct
	sockaddr_in sockaddr;
	bzero((char*) &sockaddr, sizeof(sockaddr));

	sockaddr.sin_family = AF_NET;

	bcopy((char*)serverhost->h_addr,
			(char*)&sockaddr.sin_addr.s_addr, serverhost->h_length);

	sockaddr.sin_port = htons(port);

	//open trhe socket
	int result = connect(socket, &sockaddr, sizeof(sockaddr));
	if(result < 0){
		return false;
	}

	//start a p thread running listening on the socket
	isOpen = true;

	pthread_create(&listenerThread, NULL, (void*)listen, NULL);
	return true;

}

void posix_ircio::close()
{
	isOpen = false;
}

void posix_ircio::write(std::string str)
{
	int written =(int) write(socket, str.c_str(), str.size());

	if(written <0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool posix_ircio::read(std::string& temp)
{
	char buf[BUFSIZE];
	bzero(buf, BUFSIZE);
	//read on the socket
	int chars = (int)read(socket, buf, BUFSIZE);
	if(chars < 0)
		return false;
	 temp.assign(buf);
	return true;
}

void posix_ircio::listen(void* ptr)
{
	char buf[BUFSIZE];
	while(isOpen)
	{

		bzero(buf, BUFSIZE);
		//read on the socket
		int chars = (int)read(socket, buf, BUFSIZE);	
		if(chars < 0)
			continue;

		//this shouldn't be needed if
		//the buffer is zeroed
		//buf[chars] = '\0';
		
		std::string temp(buf);
		//call on receive
		onReceive(temp);

	}
}

void posix_ircio::onReceive(std::string msg){

	std::vector<ircioCallBack*>::iterator iter;
	for(iter = callbacks.begin(); iter < callbacks.end(); iter++)
	{
		(*iter).onMessage(msg);
	}
}

void posix_ircio::registerCallBack(ircioCallBack* callback)
{
	callbacks.push_back(callback);	
}
