/* -------------------------------------------------------------------------- *
 *                    OpenSim:  psim.cpp                                      *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2014 Stanford University and the Authors                *
 * Author(s): Chris Dembia                                                    *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

// TODO this is private #include <OpenSim/version.h>
#include <OpenSim/Common/LoadOpenSimLibrary.h>
#include <OpenSim/Common/IO.h>
#include <PSim/PSimTool.h>

#include <iostream>

using namespace std;
using namespace OpenSim;

static void PrintUsage(const char *aProgName, ostream &os);

int main(int argc,char **argv)
{
	// PARSE COMMAND LINE
	int i;
	string option = "";
	string setupFileName = "";

	if (argc < 2) {
		PrintUsage(argv[0], cout);
		return -1;
	}

	// Load libraries first
	LoadOpenSimLibraries(argc, argv);

	// Parse other command-line options
	for (i = 1; i < argc; i++) {
		option = argv[i];

		// PRINT THE USAGE OPTIONS
        if (    option=="-help"  || option=="-h" ||
                option=="-Help"  || option=="-H" ||
                option=="-usage" || option=="-u" ||
                option=="-Usage" || option=="-U") {
			PrintUsage(argv[0], cout);
			return EXIT_SUCCESS;
 
		}

		// PRINT A DEFAULT SETUP FILE FOR THIS INVESTIGATION
        else if (option=="-PrintSetup" || option=="-PS") {

			PSimTool tool;
			tool.setName("default");
			Object::setSerializeAllDefaults(true);
			tool.print("default_Setup_PSim.xml");
			Object::setSerializeAllDefaults(false);
            cout << "Created file default_Setup_PSim.xml with default setup" <<
                endl;
			return EXIT_SUCCESS;

		}

		// IDENTIFY SETUP FILE
        else if( option=="-Setup" || option=="-S") {
			if((i+1)<argc) setupFileName = argv[i+1];
			break;
		}

		// PRINT PROPERTY INFO
        else if (option=="-PropertyInfo" || option=="-PI") {
			if ( i + 1 >= argc) {
				Object::PrintPropertyInfo(cout, "");
			}
            else {
				char *compoundName = argv[i+1];
				if (compoundName[0] == '-') {
					Object::PrintPropertyInfo(cout, "");
				}
                else {
					Object::PrintPropertyInfo(cout, compoundName);
				}
			}
			return EXIT_SUCCESS;
		}
	}

	// ERROR CHECK
	if (setupFileName == "") {
		cout << "\n\n";
        cout << argv[0] << ": ERROR- A setup file must be specified." << endl;
		PrintUsage(argv[0], cout);
		return -1;
	}

	// CONSTRUCT
	PSimTool tool(setupFileName);
    tool.print(tool.get_results_dir() + "./psim_setup.xml");

	// RUN
	PSimParameterValueSet solution = tool.solve();
    solution.print(tool.get_results_dir() + "./solution.xml");

	return 0;
}


//_____________________________________________________________________________
/**
 * Print the usage for this application
 */
void PrintUsage(const char *aProgName, ostream &os)
{
	string progName = IO::GetFileNameFromURI(aProgName);
	os << "\n\n" << progName << ":\n"; // TODO << GetVersionAndDate() << "\n\n";
	os << "Option              Argument         Action / Notes\n";
	os << "------              --------         --------------\n";
	os << "-Help, -H                            Print the command-line options for "<<progName<<".\n";
	os << "-PrintSetup, -PS                     Print a default setup file for psim (default_Setup_PSim.xml).\n";
	os << "-Setup, -S          SetupFileName    Specify the name of the XML setup file for the CMC investigation.\n";
	os << "-PropertyInfo, -PI                   Print help information for properties in setup files.\n";
	os << "-Library, -L        LibraryName      Specifiy a library to load. Do not include the extension (e.g., .lib or .dll).\n";
	os << "                                     To load more than one library, repeat the -Library command-line option.\n\n";

}

