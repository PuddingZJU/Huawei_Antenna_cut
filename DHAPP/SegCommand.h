#pragma once
#include "commandinterface.h"
#include "Segmentation.h"
#include "ImageWin.h"
#include <QDebug>
enum OperatorName
{
	ON_Reset,
	ON_Stroke,
	ON_Excute,
	ON_Count
};
struct SegmentOperator
{
	OperatorName _operatorName;
	ImageWin * _imgWin;
};
class Segmentation;
class SegCommand :
	public CommandInterface
{
public:
	SegCommand(OperatorName Name,ImageWin * Win,Segmentation * seg);
	~SegCommand(void);

	void Do();
	void Undo();
private:
	SegmentOperator currentOperator;
	Segmentation * m_seg;
};
