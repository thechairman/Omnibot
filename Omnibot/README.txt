*******************************************************************************
		
		Omnibot Core

*******************************************************************************

THis is the core functionality of the omnibot, including command interprating 
and the plugin frame work.  The actual irc interface is outside the scope of 
this file as that is all handled in the MiniBot frame work

------main.cpp------

This file contains the bot's main function.  It establishes the Minibot irc 
connection instantiats the omnibot.  Afterwards it repeatedly blocks on stdin 
waiting for a quit message because I wasn't quite sure how else to keep main 
from exiting while the bot was running. everything but the initialization is 
event driven from other threads.

-----Omnibot-----

This class handles all of omnibot's high level functions including various 
commands for the program as a whole such as joining/leaving channels, loading/
unloading plugins etc.  It also handles connection errors.

------PluginFactory------

This is an abstract singleton class accessed by the PluginFactory::instance()
function. It proveds a uniform interface for implementation specific subclasses
and abstracts them away for the plugin manager.  This is done for future support
of both cross platform builds and dynamic class loading, which is platform 
specific

------LinuxPluginFactory------

In the future this will provide dynamic class loading of plugins on the linux 
platform

------PluginManager------

This class manages the loading and unloading of plugins as well as permissons and 
utilites for those plugins.

------PluginUtils------

This provides a filtered interface a the plugin to interact with the outside 
world. The plugin must access all functions like the irc communications, 
interplugin communcations, etc through this object. PluginUtils instances are
created by the Plugin manager based on the recorded permissions for a given plugin
and are passed to the plugin on load.  From the plugins perspective this class is
imutable.  This class *can deny* access to certian services based on a plugin's
permissions.

------PluginAttrs------

this class is used to represent the permissions configureation of a given plugin
like the mode bits on an inode or the security attributes structs in the win32 API

------OmniPlugin------

This class is an abstract class that defines the interface that an plugin must
implement. It also abstracts all the thread handling involved in the plugin's 
message processing away. It has a private thread instance and a private class
OmniPlugin::PluginRunner that implements OmniThreadedClass. On reciept of a 
message, the plugin manager will call the static funcion OmniPlugin::passMessage()
passing each of the loaded plugins and a copy of the ircMessage object as arguments
This function then wraps the two up in a PluginRunner instance and passes it to
the plugin's worker thread. The RunA() function in the Runner is called and passes
the message to the plugin.  Once the plugin finishes processing the message the 
PluginRunner instance deletes itself, and the thread idles waiting for the next task

***Need to come up with a better way for deleting PluginRunner instances when they
   finish. As it sits now if a thread is realeased with pending tasks, the ThreadPool
   will delete the only pointers to these tasks and leave orphaned objects in memory.
