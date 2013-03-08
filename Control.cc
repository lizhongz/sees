#include <iostream>
#include "Control.h"
#include "log.h"

using namespace std;

Control::Control() : p_nvigt_eng(NULL)
{
	// Open log function
	p_log_file = fopen("app.log", "w");
	// Output2FILE::Stream() = p_log_file;
        FILELog::ReportingLevel() = FILELog::FromString("DEBUG1");
}

Control::~Control()
{
	// Close log file
	fclose(p_log_file);
}

void Control::schedule()
{
	string cmd;

	while(1)
	{
		// Wait for inputing command
		cout << "Command: ";
		cin >> cmd;

		if(cmd.compare("opnv") == 0)
		{
			// Open navigationg function
			open_nvigt_eng();
		}	
		else if(cmd.compare("clnv") == 0)
		{
			// Close navigationg function
			close_nvigt_eng();
		}
		else if(cmd.compare("stnv") == 0)
		{
			while(1)
			{
				// Setup and start to navigate
				if(start_nvigt() != 0)
				{
					FILE_LOG(logINFO) << "Ctrl: Cannot navigation";
				}
			}
		}
		else if(cmd.compare("spnv") == 0)
		{
			// Stop navigationg function
			stop_nvigt();
		}
		else if(cmd.compare("q") == 0)
		{
			break;
		}
	}
}

void Control::open_nvigt_eng()
{
	if(p_nvigt_eng != NULL)
	{
		// If navigation engine was opened
		FILE_LOG(logINFO) << "Ctrl: Navigation engine was already opened";
		return;
	}

	// Create navigation engine
	p_nvigt_eng = new NavigationEngine();

	FILE_LOG(logINFO) << "Ctrl: Navigation engine was opened";
}
     
void Control::close_nvigt_eng()
{
	if(p_nvigt_eng == NULL)
	{
		// If navigation engine is not open
		FILE_LOG(logINFO) << "Ctrl: Navigation engine has not been opened";
		return;
	}

	// Delete navigation engine
	delete p_nvigt_eng;
	p_nvigt_eng = NULL;

	FILE_LOG(logINFO) << "Ctrl: Navigaton engine was stopped";
}
     
int Control::start_nvigt()
{
	if(p_nvigt_eng == NULL)
	{
		// If navigation engine has not been opened
		FILE_LOG(logINFO) << "Ctrl: Navigation engine has not been opened";
		return -1;
	}

	if(p_nvigt_eng->is_running())
	{
		// Navigation was already started
		FILE_LOG(logINFO) << "Ctrl: Navigaton engine was aready opened";
		return -1;
	}

	string srcName;
	string destName;

	// Get source and destination name
	//cout << "Source name: ";
	//cin >> srcName;
	//cout << "Destination name: ";
	//cin >> destName;

	if(p_nvigt_eng == NULL)
	{
		// If navigation engine is not opened
		return -1;
	}

	// Set up navigation route
	if(p_nvigt_eng->setup_route("ISIMA GATE", 
		"TRAM Station") == 0)
	{
		// Begin to navigate
		p_nvigt_eng->start();
	}
	else
	{
		return -1;
	}
	
	return 0;
}

     
void Control::stop_nvigt()
{
	if(p_nvigt_eng == NULL)
	{
		// If navigation engine is not open
		FILE_LOG(logINFO) << "Ctrl: Navigation engine has not been created";
		return;
	}

	// Stop navigation
	p_nvigt_eng->stop();	
}

