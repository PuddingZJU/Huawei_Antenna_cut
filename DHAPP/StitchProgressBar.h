#ifndef __STITCHPROGRESSBAR_H__
#define __STITCHPROGRESSBAR_H__

class PanoramaOptionParam;
class PanoramaThread;
class StitchProgressBar
{
	friend class PanoramaOptionParam;
public:
	static StitchProgressBar* getInstance();
	void advanceValue(int value);

	int getPenctangeNumber(double percentage) 
	{
		return (_maximum - _minimum)*percentage;
	}
	void setRange(int min, int max)
	{
		_minimum = min;
		_maximum = max;
		_current_value = min;
	}

private:
	StitchProgressBar(PanoramaThread* thread, int min, int max);
	StitchProgressBar(const StitchProgressBar&);
	static StitchProgressBar *instance;

private:
	/* only used in PanoramaOptionParam */
	static void createInstance(PanoramaThread* thread, int min, int max);
	static void deleleteInstance();

private:
	int _maximum, _minimum;
	int _current_value;
	PanoramaThread* _running_thread;
};

#endif