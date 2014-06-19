#ifndef __ES_DHAPP_OPERATIONMODEMANAGER_H__
#define __ES_DHAPP_OPERATIONMODEMANAGER_H__

enum OperationMode
{
	INVALIDMODE = -1,
	ARROWMODE = 0,
	SELECTMODE,
	SEGMENTATIONMODE,
	ELEMENTPICKMODE,
	INPAINTINGMODE,

	//end of mode
	MODECOUNT
};

class QAction;
class QToolBar;
class QActionGroup;
class OperationModeManager
{
public:
	OperationModeManager(void);
	~OperationModeManager(void);

	void addOperationMode(OperationMode opermode, QAction* act);

	int mode() const;
	bool setMode(OperationMode opermode);

private:
	QActionGroup* actgroup_;
	QToolBar* toolbar_;

	QAction* actarray_[MODECOUNT];
};

#endif