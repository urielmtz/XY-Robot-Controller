// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/* 
 * Copyright (C) 2011 The University of Sheffield
 * Author: Uriel Martinez-Hernandez
 * email: cop10um@sheffield.ac.uk
 *
*/

/**
 * @file xytable.h
 * @class XYTable
 * @brief this file contains the definition of the main module code.
 * @see example.cpp
 */

#ifndef __XYTABLE_H__
#define __XYTABLE_H__

/** 
 * @ingroup Sheffield_modules
 *
 * \defgroup Sheffield_modules XYTable
 *
 * Addressing:
 * - documentation and coding standards
 *
 * Functionally, the module allows work with the XY robot VIP YAMAHA and driver RCX222
 *
 * \section tested_os_sec Tested OS
 *
 * Linux \n
 * Windows
 *
 * \section compiling Configuring CMakeLists
 * The path of SerialDeviceDriver.h has to be specified when the module is compiled. The easiest way is using ccmake in advanced mode and in CXX_FLAGS parameter
 * the header file has to be added which is inside your YARP path, i.e $YARP_ROOT/src/modules/serial
 *
 * \author Uriel Martinez-Hernandez
 * 
 * Copyright (C) 2011 The University of Sheffield
 * 
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <time.h>

#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>
#include <yarp/dev/DeviceDriver.h>
#include <SerialDeviceDriver.h>
#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <yarp/dev/all.h>


using namespace std;
using namespace yarp;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;

//!  XYTable class. 
/*!
    XYTable class contains methods to manage the XYTable robot as well as the methods to start the communication with the driver.
    Some of the current method are develop based on the sexy (Somatosensory Environment for an X & T robot) program done in MATLAB
    by Matt Evans and Stuart Wilson.
 */

class XYTable
{
	private:
        //! Establish the settings of the communication
		SerialDeviceDriverSettings settings;
        //! Object to establish the communication with the servo
		SerialDeviceDriver commPort;

        //! SERVO ON command
		Bottle *servoONCommand;
        //! SERVO OFF command
		Bottle *servoOFFCommand;
        //! WHERE command
		Bottle *whereCommand;
        //! MANUAL command
		Bottle *manualCommand;
        //! MOVE command
		Bottle *moveCommand;
        //! RESET command
		Bottle *emgrstCommand;

        //! Name of the communication port
		char *port;

        //! Path of the folder that contains prog.txt and pars.txt files
		string pathName;
        //! Name of the folder that contains prog.txt and pars.txt files
		string fileName;

        //! Temporary prog file to store the content of prog.txt file
		fstream xyProgFile;
        //! Temporary pars file to store the content of pars.txt file
		fstream xyParsFile;

        //! Lenght of the content of prog.txt file
		int progLength;
        //! Lenght of the content of pars.txt file
		int parsLength;

	protected:

	public:
		/**
		 * Default constructor.
         * Set the parameters.
         *
         * port_name = /dev/ttyUSB0
         * baudrate_conf = 9600
         * parity_mode = odd
         * databits_conf = 8
         * stopbits_conf = 1
         *
         * @see xytable.cpp
		 */
		XYTable();
		/**
		 * Optional Constructor
         *
         * @param port_name name of the communication port (i.e. COM1, /dev/ttyUSB0)
         * @param baudrate_conf baudrate configuration
         * @param parity_mode parity configuration
         * @param databits_conf databits configuration
         * @param stopbits_conf stopbits configuration
         * @see xytable.cpp
		 */
		XYTable(char *port_name, int baudrate_conf, char *parity_mode, int databits_conf, int stopbits_conf);
		/**
		 * Default destructor
		 */
		~XYTable();
		/**
		 * Initializes the attributes required for the communication
		 */
		void initCommands();
		/**
		 * Starts the servo, this allows the XYTable to execute some action
         *
         * @return true/false if the action was successful/unsuccessful respectively
		 */
		bool xyServoOn();
		/**
		 * Stops the servo, this disable the XYTable to execute some action
         *
         * @return true/false if the action was successful/unsuccessful respectively
         * @see xytable.cpp
		 */
		bool xyServoOff();
		/**
		 * Starts the communication with the servo
         *
         * @return true/false if the action was successful/unsuccessful respectively
         * @see xytable.cpp
		 */
		bool xyOpenPort();
		/**
		 * Closes the communication with the servo
         *
         * @return true/false if the action was successful/unsuccessful respectively
         * @see xytable.cpp
		 */
		bool xyClosePort();
		/**
		 * Define a movement in X and Y axes
         *
         * @param x_p X position
         * @param y_p Y position
         * @param speed speed of the movement
         * @return true/false if the action was successful/unsuccessful respectively
         * @see xytable.cpp
		 */
		bool xyMove(double x_p, double y_p, int speed);
		/**
		 * Define a movement in X and Y axes
         *
         * @param x_p X position
         * @param y_p Y position
         * @return true/false if the action was successful/unsuccessful respectively
         * @see xytable.cpp
         *
         * The speed of the movement is set to 2
		 */
		bool xyMove(double, double);
		/**
		 * Resets the system
         *
         * @return true/false if the action was successful/unsuccessful respectively
         * @see xytable.cpp
		 */
		bool xyReset();
		bool xyManual();
		/**
		 * Gets the position of the robot
         *
         * @return true/false if the action was successful/unsuccessful respectively
         * @see xytable.cpp
		 */
		bool xyWhere();
		/**
         * Loads in memory the files prog.txt and pars.txt
         *
         * @param path is the path where the files prog.txt and pars.txt are stored
         * @return true/false if the action was successful/unsuccessful respectively
         * @see xytable.cpp
		 */
		bool xyProgram(string path);
		/**
		 * Executes a program
         *
         * @return true/false if the action was successful/unsuccessful respectively
         * @see xytable.cpp
		 */
		bool xyExecute();
		/**
		 * Gets the position of the robot as double values, this way the values can be manipulated
         *
         * @param x_p X position
         * @param y_p Y position
         * @see xytable.cpp
		 */
		void getXYPosition(double &x_p, double &y_p);
		/**
		 * Sends the data in prog.txt and pars.txt files
         *
         * @param s_data data with points, speeds, etc
         * @return true/false if the action was successful/unsuccessful respectively
         * @see xytable.cpp
		 */
		bool xyData(string s_data);
		/**
		 * Sends commands to the servo, i.e. SERVO ON, SERVO OFF, WHERE
         *
         * @param s_command action to be executed
         * @return true/false if the action was successful/unsuccessful respectively
         * @see xytable.cpp
		 */
		bool xyCommand(string s_command);
		/**
		 * Splits the path and substracts the name of the folder where the files prog.txt and pars.txt are stored
         *
         * @param path is the path where the files prog.txt and pars.txt are stored
         * @see xytable.cpp
		 */
		void splitFileName(string path);
		/**
		 * Converts to uppercase the characters of a string
         *
         * @param str string to be converted
         * @see xytable.cpp
		 */
		void toUpperCase(string &);
};

#endif /*__XYTABLE_H__*/

