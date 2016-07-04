/*********************** Licensing *******************************************************
*
*  Copyright 2016 @ Evandro  Coan
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 3 of the License, or ( at
*  your option ) any later version.
*
*  This program is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*****************************************************************************************
*/


/**
 * Preprocessor directive designed to cause the current source file to be included only once in a
 * single compilation. Thus, serves the same purpose as #include guards, but with several
 * advantages, including: less code, avoidance of name clashes, and sometimes improvement in
 * compilation speed. In main file this is enabled by default.
 */
#pragma once



/**
 * DO NOT REMOVE IT! Always move this into the main include linear order.
 * 
 * #define __SYS_NS	   System
 * #define __USING_SYS using namespace __SYS_NS
 */
__USING_SYS;



#include <utility/ostream.h>
#include <semaphore.h>

#include <cstdarg>
#include <cstdio>
#include <unistd.h>
#include <stdio.h>
#include <cstring>

#include <headers/lamps_project_debugger.h>
#include <headers/array_operations.h>
#include <headers/software_version.h>

#include <utility/malloc.h>
#include <utility/list.h>
#include <utility/vector.h>
#include <utility/list.h>
#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <uart.h>
#include <thread.h>
#include <mutex.h>
#include <semaphore.h>
#include <traits.h>

#include <classes/UserRegistry.cc>
#include <classes/LampConfiguration.cc>
#include <classes/Message.cc>
#include <interfaces/SmartObjectCommunication.h>
#include <interfaces/CommunicationStrategyObserver.h>
#include <classes/CommunicationSubject.cc>
#include <classes/Usb.cc>
#include <classes/Radio.cc>
#include <classes/Lamp.cc>
#include <interfaces/LampControlStrategy.h>
#include <classes/PwmHardware.cc>
#include <classes/UserBoard.cc>
#include <classes/LampBoard.cc>



