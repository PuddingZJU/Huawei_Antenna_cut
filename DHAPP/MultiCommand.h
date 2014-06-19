#ifndef __ES_DHAPP_MULTICOMMAND_H
#define __ES_DHAPP_MULTICOMMAND_H

#include "CommandInterface.h"

#include <vector>

class MultiCommand :
	public CommandInterface
{
public:
	MultiCommand(void);
	virtual ~MultiCommand(void);

	virtual void Do();
	virtual void Undo();

	void AddCommand(CommandInterface* cmd);
	bool IsEmpty() const;

private:
	typedef std::vector<CommandInterface*> CommandVec;
	CommandVec m_cmds;
};

#endif