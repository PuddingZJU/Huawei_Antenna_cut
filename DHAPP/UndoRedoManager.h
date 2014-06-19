#ifndef __ES_DHAPP_UNDOREDOMANAGER_H
#define __ES_DHAPP_UNDOREDOMANAGER_H

#include <deque>

class CommandInterface;
class UndoRedoManager
{
public:
	UndoRedoManager(size_t maxSteps = 100);
	~UndoRedoManager(void);

	void AddCommand(CommandInterface* cmd);
	void Undo(int steps = 1);
	void Redo(int steps = 1);
	bool CanUndo() const;
	bool CanRedo() const;
private:
	typedef std::deque<CommandInterface*> CommandQueue;
	size_t m_maxSteps;
	CommandQueue m_cmds;
	CommandQueue::iterator m_currPos;
};

#endif