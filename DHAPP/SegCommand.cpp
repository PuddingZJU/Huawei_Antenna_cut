#include "SegCommand.h"

SegCommand::SegCommand(OperatorName Name,ImageWin * Win,Segmentation * seg)
{
	this->currentOperator._imgWin = Win;
	this->currentOperator._operatorName = Name;
	m_seg = seg;
}

SegCommand::~SegCommand(void)
{
}

void SegCommand::Do()
{
	switch(currentOperator._operatorName)
	{
	case ON_Stroke:
		m_seg->ImageWin_SeedItMapMinusOne(currentOperator._imgWin);
		currentOperator._imgWin->update();
		break;
	case ON_Excute:
		
		break;
	case ON_Reset:

		break;
	default:
		break;
	}
}
void SegCommand::Undo()
{
	//操作撤销
	switch(currentOperator._operatorName)
	{
	case ON_Stroke:
		 m_seg->ImageWin_SeedItMapPlusOne(currentOperator._imgWin);
		currentOperator._imgWin->update();
		break;
	case ON_Excute:
		break;
	case ON_Reset:

		break;
	default:
		break;
	}
}
