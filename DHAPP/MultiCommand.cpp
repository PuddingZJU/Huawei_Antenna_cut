#include "MultiCommand.h"

#include <assert.h>
#include <functional>

MultiCommand::MultiCommand(void)
{
}

MultiCommand::~MultiCommand()
{
	std::for_each(m_cmds.begin(), m_cmds.end(), DeleteContPtr());
}

void MultiCommand::Do()
{
	std::for_each(m_cmds.begin(), m_cmds.end(), std::mem_fun(&CommandInterface::Do));
}

void MultiCommand::Undo()
{
	std::for_each(m_cmds.rbegin(), m_cmds.rend(), std::mem_fun(&CommandInterface::Undo));
}

void MultiCommand::AddCommand(CommandInterface* cmd)
{
	assert(cmd);
	m_cmds.push_back(cmd);
}

bool MultiCommand::IsEmpty() const
{
	return m_cmds.empty();
}