#include "StitchProgressBar.h"
#include "MainWindow.h"
#include "PanoramaOptionParam.h"

StitchProgressBar* StitchProgressBar::instance = NULL;

StitchProgressBar* StitchProgressBar::getInstance()
{
	return instance;
}

StitchProgressBar::StitchProgressBar(PanoramaThread* thread, int min, int max)
: _running_thread(thread)
{
	setRange(min, max);
}

void StitchProgressBar::advanceValue(int value)
{
	_running_thread->set_bar_value(_current_value += value);
}

void StitchProgressBar::createInstance(PanoramaThread* thread, int min, int max)
{
	if(instance != NULL) deleleteInstance();
	instance = new StitchProgressBar(thread, min, max);
}

void StitchProgressBar::deleleteInstance()
{
	if(instance != NULL) {
		delete instance;
		instance = NULL;
	}	
}
