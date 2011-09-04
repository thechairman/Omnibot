

class ircInterfaceClient{
	public:
		virtual void alertEvent(ircEvent e) = 0;
		virtual void alertMessage(ircMessage m) = 0;
}
