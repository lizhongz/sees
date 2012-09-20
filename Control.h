#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <cstdio>
#include "NavigationEngine.h"
#include "env_detection.h"

class Control
{
public:
	Control();

	~Control();

	void schedule();
	
	// Open navigation engine
	void open_nvigt_eng();
	
	// Close navigation engine
	void close_nvigt_eng();

	// Start to navigate
	int start_nvigt();
	
	// Stop navigating
	void stop_nvigt();

private:
	NavigationEngine *p_nvigt_eng; // Navigation engine
	EnvDetection	 *p_env_det;
	FILE *p_log_file;
};

#endif // _CONTROL_H_
