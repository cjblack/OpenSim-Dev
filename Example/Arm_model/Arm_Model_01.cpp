//
//  Arm_Model_01.cpp
//  
//
//  Created by Christopher Black on 7/24/15.
//
//

#include <stdio.h>
#include <OpenSim/OpenSim.h>
#include <curl/curl.h>
#include <stddef.h>
#include <time.h>
#include <sys/stat.h>
#include <Python/Python.h>


using namespace OpenSim;
using namespace SimTK;
using namespace std;




int main()
{
    
//Timing//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    
    time_t start = time(0);
    time_t rawtime;
    tm* timeinfo;
    char moYear[80];
    char dayNo[80];
    char hrMnSc[80];
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(moYear,80,"%b_%G/",timeinfo);
    strftime(dayNo,80,"%a_%d/",timeinfo);
    strftime(hrMnSc,80,"%b_%G",timeinfo);
    
//Create Files//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    
  // ** Setup Directories ** \\
    
    // Empty for now, but will keep build folder light and organized for simulations/analysis
    string dirname = "../Analysis/";
    string year = dirname + moYear;
    string day = year + dayNo;
    int result01 = mkdir(year.c_str(),0777);
    int result02 = mkdir(day.c_str(),0777);
    
    // Generate new file for each simulation
    struct stat statInfo;
    int counter = 0;
    string modelFolder;
    do
    {
        counter++;
        stringstream namestream;
        namestream << day << "Model_No_" << counter << "/";
        modelFolder = namestream.str();
        
    }while(stat(modelFolder.c_str(),&statInfo) == 0);
    
    int result03 = mkdir(modelFolder.c_str(),0777);
    string dfiles = modelFolder + "DataFiles";
    int result04 = mkdir(dfiles.c_str(),0777);
    
    
//Create Model//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    
    Model model;
    model.setName("Arm_Model01.osim");
    
    
    
//Initial Parameters//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    
    
    
    
//Reference Info//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // ** Ground Ref ** \\
    
        Ground& ground = model.updGround();
        ground.addDisplayGeometry("ground.vtp");
    
//Add Body//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    
// Taking some parameters from Rehbinder 2001 and Dumas 2006...add references*
    
    // ** Ulna ** \\
        //Params
            double ulnMass = 0.546, ulnLength = 0.27;
            Vec3 ulnMassCenter(0, -0.284, -0.033);
            Inertia ulnInertia = ulnMass*Inertia::brick(ulnLength, ulnLength/8, 0);
    
        //Build
            OpenSim::Body *ulna = new OpenSim::Body("ulna", ulnMass, ulnMassCenter, ulnInertia);
            ulna->addDisplayGeometry("rulna.vtp");
    
    
    
    // ** Radius ** \\
        //Params
            double radMass = 0.5464, radLength = 0.24;
            Vec3 radMassCenter(0, -0.284, 0.033);
            Inertia radInertia = radMass*Inertia::brick(radLength, radLength/8, 0);
    
        //Build
            OpenSim::Body *radius = new OpenSim::Body("radius", radMass, radMassCenter, radInertia);
            radius->addDisplayGeometry("rradius.vtp");
    
    
    // ** Humerus ** \\
        //Params
            double humMass = 0.5464, humLength = 0.24;
            Vec3 humMassCenter(0.005, -0.118, -0.007);
            Inertia humInertia = humMass*Inertia::brick(humLength, humLength/8, 0);
    
        //Build
            OpenSim::Body *humerus = new OpenSim::Body("humerus", humMass, humMassCenter, humInertia);
            humerus->addDisplayGeometry("rhumerus.vtp");
    
    
    
    // ** Add Bodies ** \\
    
    model.addBody(ulna);
    model.addBody(radius);
    model.addBody(humerus);
    
    
    
    

//Create Joints//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    
//Embed Python//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    Py_Initialize();
    FILE *fp = fopen("../hello.py", "r+");
    
    
    PyRun_SimpleFile(fp, "hello.py");
    
    Py_Exit(0);
    
    
}
