********************************************************************************

		The Omnibot Threadding Framwork

********************************************************************************

As Omnibot is designed with with future cross platform buidling in mind, the 
threading frame work uses a some what complex class hierarchy.  All of the Omni
classes are primarily abstract pure virtual classes with the exection of some 
static functionality and enums. High level parts of the application can use these
abstract classes oblivious of the underlying implementations.  For each Class 
OmniX, there will be one more classes named pthread_X, posix_X, win32_x etc.
These classes contain the platform depenant threading implementation and will
be appropriately selected at build time. While platform dependant classes are 
welcome to refence one another and may in fact need to do so, Omni abstract
classes should only reference other Omni abstract classes

------OmniThreadPool------

This classs provides the interface for manage a pool of worker thereads.  It
handles the construction and destruction of the OmniPooledThread objects as 
well as serviceing requests and relaeses of pooled threads by the application's
higher layers. This Class is a singleton and is accessed by the 
OmniThreadPool::instance() method.  This returns the appropriate platform specifc
implementation subclass.

------OmniThreadedClass------

This is an interface that defines the call backs executed be the framework
it also has a mode type defined that allows selection of the callback to be 
executed

------OmniPooledThread------

This class is a thread object that accepts and queues tasks of the form 
(OmniThreadedClass, OmniThreadedClass::Mode). Creation and deletion of these 
objects is handled exclusively by the OmniThreadPool which ensures the 
appropriate implemetation specific sublcass is returned.

------OmniThread------

This class abstracts away the pooled nature of the threads in the framework.
It also defines Mode and ThreadStatus types.  

***Should have static methods to return implementation specific instances.

------pthread_PooledThread------

This pthread implementation of the pooled thread has a single thread that 
invokes the static callback function with the instance as an argument. Once
"started" the thread will begin proccessing tasks from the threads task queue
it uses a counting semaphore to count the number of tasks currrently in the queue
and blocks on it when the queue is empty.

-----pthread_ThreadPool-------

Creates and inits the pthread workers in a lazy fashion.  It also keeps track of 
which threads are currently being held by something and which are availible.  On 
requesting a thread it trys to find an idle one in the pool, failing that it creates
a new one and returns a pointer to it. When something is done with the thread it 
should be released at which point this class will clean it up and return it to the 
pool.

***should have a decay on threads in the pool and remove them if they have spent to
   time idle. That way the worker threads will be kept to a managable level, however
   pthreads are difficult to clean up without losing stack space

------pthread_Thread-----

A wrapper around the pthread OmniPooledThread implentation to hide the "pooled" 
nature of it.  These threads can be created and destroyed at when needed and it
takes care of all the pool realated overhead.
