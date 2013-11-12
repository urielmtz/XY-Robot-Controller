// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/* 
 * Copyright (C) 2011 The University of Sheffield
 * Author: Uriel Martinez-Hernandez
 * email: cop10um@sheffield.ac.uk
 *
*/

/**
 * @file xytable.cpp
 * @brief this file contains the implementation of the main code.
 * @include xytable.cpp
 */

#include "xytable.h"

/* Default constructor */
XYTable::XYTable()
{
	port = "/dev/ttyUSB0";
	strcpy(settings.CommChannel, port);
	settings.SerialParams.baudrate = 9600;
	settings.SerialParams.paritymode = "odd";
	settings.SerialParams.databits = 8;
	settings.SerialParams.stopbits = 1;

	initCommands();
}

/* Optional constructor */
XYTable::XYTable(char *port_name, int baudrate_conf, char *parity_mode, int databits_conf, int stopbits_conf)
{
	port = port_name;
	strcpy(settings.CommChannel, port);
	settings.SerialParams.baudrate = baudrate_conf;
	settings.SerialParams.paritymode = parity_mode;
	settings.SerialParams.databits = databits_conf;
	settings.SerialParams.stopbits = stopbits_conf;

	initCommands();
}

/* Initializes attributes */
void XYTable::initCommands()
{
	servoONCommand = new Bottle;
	servoOFFCommand = new Bottle;
	whereCommand = new Bottle;
	manualCommand = new Bottle;
	moveCommand = new Bottle;
	emgrstCommand = new Bottle;

	servoONCommand->addString("@SERVO ON \r");
	servoOFFCommand->addString("@SERVO OFF \r");
	whereCommand->addString("@?WHRXY \r");
	manualCommand->addString("@MANUAL \r");
	emgrstCommand->addString("@EMGRST \r");
}

/* Destructor */
XYTable::~XYTable()
{
	delete servoONCommand;
	delete servoOFFCommand;
	delete whereCommand;
	delete manualCommand;
	delete moveCommand;
	delete emgrstCommand;
}

/* Starts the servo */
bool XYTable::xyServoOn()
{
	if( commPort.send(*servoONCommand) )
		return true;
	else
		return false;
}

/* Stops the servo */
bool XYTable::xyServoOff()
{
	if( commPort.send(*servoOFFCommand) )
		return true;
	else
		return false;
}

/* Starts the communication */
bool XYTable::xyOpenPort()
{
	if( commPort.open(settings) )
		return true;
	else
		return false;
}

/* Stops the communication */
bool XYTable::xyClosePort()
{
	if( commPort.close() )
		return true;
	else
		return false;
}

/* Movement of the robot with X,Y position and configurable speed */
bool XYTable::xyMove(double x_p, double y_p, int speed)
{
	char *command = new char[40];
	char *s_xp = new char[10];
	char *s_yp = new char[10];
	char *s_speed = new char[10];

	if( x_p < 0.0 || x_p > 650.0 )
	{
		cout << endl << "Error!, XMin:0.0 XMax:650.0" << endl;
		return false;
	}

	if( y_p < 0.0 || y_p > 300.0 )
	{
		cout << endl << "Error!, YMin:0.0 YMax:300.0" << endl;
		return false;
	}

	strcpy(s_xp,"");
	strcpy(s_yp,"");
	sprintf(s_xp, "%.1f", x_p);
	sprintf(s_yp, "%.1f", y_p);
	sprintf(s_speed, "%d", speed);	
	strcpy(command,"@");
	strcat(command, "MOVE P, ");
	strcat(command, s_xp);
	strcat(command, " ");
	strcat(command, s_yp);
	strcat(command, " 0 0 0 0, S=");
	strcat(command, s_speed);
	strcat(command, " \r");
	moveCommand->addString(command);
	commPort.send(*moveCommand);
	moveCommand->clear();
	strcpy(s_xp,"");
	strcpy(s_yp,"");

	return true;
}

/* Movement of the robot with X,Y position and speed of 2 */
bool XYTable::xyMove(double x_p, double y_p)
{
	char *command = new char[40];
	char *s_xp = new char[10];
	char *s_yp = new char[10];

	if( x_p < 0.0 || x_p > 650.0 )
	{
		cout << endl << "Error!, XMin:0.0 XMax:650.0" << endl;
		return false;
	}

	if( y_p < 0.0 || y_p > 300.0 )
	{
		cout << endl << "Error!, YMin:0.0 YMax:300.0" << endl;
		return false;
	}

	strcpy(s_xp,"");
	strcpy(s_yp,"");
	sprintf(s_xp, "%.1f", x_p);
	sprintf(s_yp, "%.1f", y_p);
	strcpy(command,"@");
	strcat(command, "MOVE P, ");
	strcat(command, s_xp);
	strcat(command, " ");
	strcat(command, s_yp);
	strcat(command, " 0 0 0 0, S=2 \r");
	moveCommand->addString(command);
	commPort.send(*moveCommand);
	moveCommand->clear();
	strcpy(s_xp,"");
	strcpy(s_yp,"");

	return false;
}

/* Resets the servo */
bool XYTable::xyReset()
{
	if( commPort.send(*emgrstCommand) )
		return true;
	else
		return false;
}

/* Manual operation */
bool XYTable::xyManual()
{
	if( commPort.send(*manualCommand) )
		return true;
	else
		return false;
}

/* Gets the position of the robot */
bool XYTable::xyWhere()
{
	if( commPort.send(*whereCommand) )
		return true;
	else
		return false;
}

/* Loads in memory the program */
bool XYTable::xyProgram(string path)
{
	string progPath;
	string parsPath;
	string line;
	string prog;
	string pars;

	if( !path.empty() || (path.length() > 0) )
	{
		splitFileName(path);
		toUpperCase(fileName);
	}
	else
	{
		pathName = "";
		fileName = "DATA";
	}

	progPath = pathName + "prog.txt";
	parsPath = pathName + "pars.txt";

	xyProgFile.open(progPath.c_str(), fstream::in);
	xyParsFile.open(parsPath.c_str(), fstream::in);

    if( !xyProgFile.is_open() || !xyParsFile.is_open() )
    {
        cout << "Failed to open!" << endl;
        return false;
    }

    while( !xyProgFile.eof() )
    {
        getline(xyProgFile, line);
		prog = prog + line + (char)13 + (char)10;
	}

    while( !xyParsFile.eof() )
    {
        getline(xyParsFile, line);
		pars = pars + line + (char)13 + (char)10;
	}

	xyProgFile.close();
	xyParsFile.close();

	progLength = prog.length();
	parsLength = pars.length();

	prog = "NAME=" + fileName + (char)13  + (char)10 + prog; 
	xyCommand("SYSTEM");
	xyCommand("WRITE PGM");
	xyData(prog);
	xyCommand("WRITE PNT");
	xyData(pars);

	return true;
}

/* Executes the program stored in memory */
bool XYTable::xyExecute()
{
	string swi_command = "SWI <" + fileName + ">";

	xyCommand("AUTO");
	xyCommand(swi_command);
	xyCommand("RUN");

	return true;
}

/* Gets the position of the robot in as double values */
void XYTable::getXYPosition(double &x_p, double &y_p)
{
	char *line = new char[60];
	string temp;
	double px;
	double py;

	if( commPort.send(*whereCommand) )
	{
		commPort.receiveLine(line,60);

		for( int i = 0; i < 6; i++ )
		{
			if( (int)line[i+7] != 32 )
				temp = temp + line[i+7];
		}
		px = atof(temp.c_str());

		temp = "";
		for( int i = 0; i < 6; i++ )
			temp = temp + line[i+15];

		py = atof(temp.c_str());
	}

	x_p = px;
	y_p = py;
}

/* Sends the data in prog.txt and pars.txt files */
bool XYTable::xyData(string s_data)
{
	Bottle command;
	char *f_data = new char[2*parsLength];

	strcpy(f_data, s_data.c_str());
	strcat(f_data, " \r\n");
	command.addString(f_data);
	commPort.send(command);
	return true;
}

/* Sends commands to the servo */
bool XYTable::xyCommand(string s_command)
{
	Bottle command;
	char *f_command = new char[100];

	strcpy(f_command, "@");
	strcat(f_command, s_command.c_str());
	strcat(f_command, " \r\n");
	command.addString(f_command);
	commPort.send(command);
	return true;
}

/* Splits the path and substracts the name of the folder where the files prog.txt and pars.txt are stored */
void XYTable::splitFileName(string path)
{
	size_t found;

	found = path.find_last_of("/\\");
	pathName = path + "/";
	fileName = path.substr(found+1);
}

/* Converts to uppercase the characters of a string */
void XYTable::toUpperCase(string &str)
{
	for( int i = 0; i < str.length(); i++ )
		str[ i ] = toupper(str[ i ]);
}

