*******************************************************************************

			Minibot Framework

*******************************************************************************	

The Minibot framework, is a minimalist frame work for provideing a connction to
an irc server and abstracting away all of the message formating involved in the 
irc protocol.

------ircio------

This is an abstract class that documents the interface used by minibot for
accesing the the platform dependant io system functions.

------posix_ircio------

This is a Platform dependant subclass of the ircio interface implemented using 
posix/berkley sockets and pthreads. Per the ircio interface it provides a means
for submitting strings to be transmitted over a socket and accepting a class 
for callback notifications when the socket receives data.  When the object is 
created a pthread is spawned to loop over the socket and block on it waiting for
data.  When data is received it passes it to the registerd objects.

------ircNotifyClient------

This is an abstract class that defines the interface used as callbacks by the 
ircInterface class to notify them of messages and events on the irc server

------ircInterface------

This class handles the abstraction of the irc protocol. It does this Four ways.
The first way is by making connection and disconnection transparent by handling 
handshakes by itself.  second, it maitains the connection by automatically 
handling PING messages. Third it abstracts message formats into similiarly named
function calls that take the necessary substrings to construct the message as
arguments. And lastly it process all messsages and events on the server into 
the helper classes in ircNotifyClasses.h/.cpp.  When such messages are received
it passes a copy of the message to each registerd client.

------ircUser------

this is one of the Helper classes.  It contains a nick, an id number and useful
information such as whether or not he is authenticated with the server's nick 
serve. Once created this class is imutable except by the ircUserDB and 
ircInterface classes

------ircMessage------

This helper class wraps up the component parts of a message, the message, the
user of origin, and the channel of orgin.  The channel remains an empty string
for private messages.  Once this class is instantiated it is immutable.

------ircEvent------

This helper class wraps up all messages that are not actuall messages, such as
errors, joins, parts, nick changes, etc. it provide methods for createing the 
various event types and extracting the contents.

***After I actually started implementing this it looks more and more like it 
   should be a virtual class with static methods for creating specific 
   subclasses for each particlualr type of event. It's pretty messy.

------ircUserDB------

***This Class is a work in progress

This class provides a database of all users currently on the server accessable
either by nick, or by id.  It also keeps track of known users registered with 
nickserv.  It provides persistant id numbers for users registered with nickserv
and generates temporary ids for all other nicks.  This class is solely 
responsible for createing and destroying ircUser objects as well as keeping 
their contents up-to-date. Right now this class is entirely implemented in the
application, however it is encapsulated such that if it becomes necessary it 
can be implemented entirely as a database. (subclasses for build options?)




