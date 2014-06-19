#ifndef __ES_DHAPP_COMMANDINTERFACE_H
#define __ES_DHAPP_COMMANDINTERFACE_H

#include <algorithm>

class CommandInterface
{
public:
	CommandInterface(void){}
	virtual ~CommandInterface(void){}

	virtual void Do() = 0;
	virtual void Undo() = 0;
};

struct DeleteContPtr{
	template <typename T> 
	void operator()(T *&p) {
		delete p;
		p = NULL;
	}

	template <typename T,typename U>
	void operator()(std::pair<T,U*> &pr) {
		delete pr.second;
		pr.second = NULL;
	}
};

#endif