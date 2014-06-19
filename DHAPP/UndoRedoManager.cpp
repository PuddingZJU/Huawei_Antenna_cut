#include "UndoRedoManager.h"

#include <assert.h>
#include <algorithm>

#include "CommandInterface.h"

UndoRedoManager::UndoRedoManager(size_t maxSteps /*= 100*/) : m_maxSteps(maxSteps)
{
	assert(m_maxSteps > 0);
	m_currPos = m_cmds.begin();
}

UndoRedoManager::~UndoRedoManager()
{
	std::for_each(m_cmds.begin(),m_cmds.end(),DeleteContPtr());
}

void UndoRedoManager::AddCommand(CommandInterface* cmd)
{
	assert(cmd);

	std::for_each(m_cmds.begin(), m_currPos, DeleteContPtr());
	m_cmds.erase(m_cmds.begin(), m_currPos);

	while (m_cmds.size() >= m_maxSteps)
	{
		delete m_cmds.back();
		m_cmds.pop_back();
	}

	m_cmds.push_front(cmd);
	m_currPos = m_cmds.begin();
}

void UndoRedoManager::Undo(int steps /* = 1 */)
{
	int i = 0;
	while (m_currPos != m_cmds.end() && ++i <= steps)
	{
		(*m_currPos++)->Undo();
	}
}

void UndoRedoManager::Redo(int steps /* = 1 */)
{
	int i = 0;
	while (m_currPos != m_cmds.begin() && ++i <= steps)
	{
		(*--m_currPos)->Do();
	}
}

bool UndoRedoManager::CanUndo() const
{
	return m_currPos != m_cmds.end();
}

bool UndoRedoManager::CanRedo() const
{
	return m_currPos != m_cmds.begin();
}