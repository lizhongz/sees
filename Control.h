#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <cstdio>
#include "NavigationEngine.h"
#include "SeesClient.h"

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

	// Open sees client
	void open_sees_client();
	
	// Close sees client
	void close_sees_client();

	// Get navigation engine
	NavigationEngine * get_nvigt_eng() const;	
	

private:
	NavigationEngine *p_nvigt_eng; // Navigation engine

	SeesClient *p_client; // Pointer of SeesClient object

	FILE *p_log_file; // Log file's pointer
};

#endif // _CONTROL_H_
