#ifndef _H_OMNITHREADEDCLASS_
#define _H_OMNITHREADEDCLASS_
class OmniThreadedClass
{
	public:
		enum Mode
		{
			MODE_A,
			MODE_B,
			MODE_C,
			MODE_D
			
		};
		
		//run functions
		virtual void runA()=0;
		virtual void runB()=0;
		virtual void runC()=0;
		virtual void runD()=0;

};
#endif
