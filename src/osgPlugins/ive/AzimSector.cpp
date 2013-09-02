/**********************************************************************
 *
 *    FILE:            AzimSector.cpp
 *
 *    DESCRIPTION:    Read/Write osgSim::AzimSector in binary format to disk.
 *
 *    CREATED BY:        Auto generated by iveGenerator.exe
 *                    and later modified by Rune Schmidt Jensen.
 *
 *    HISTORY:        Created 9.9.2003
 *
 **********************************************************************/

#include "Exception.h"
#include "AzimSector.h"

using namespace ive;

void AzimSector::write(DataOutputStream* out){
    // Write AzimSector's identification.
    out->writeInt(IVEAZIMSECTOR);
    // Write AzimSector's properties.
    float minAzimuth, maxAzimuth, fadeAngle;
    getAzimuthRange(minAzimuth, maxAzimuth, fadeAngle);
    out->writeFloat(minAzimuth);
    out->writeFloat(maxAzimuth);
    out->writeFloat(fadeAngle);
}

void AzimSector::read(DataInputStream* in){
    // Peek on AzimSector's identification.
    int id = in->peekInt();
    if(id == IVEAZIMSECTOR){
        // Read AzimSector's identification.
        id = in->readInt();
        // Read AzimSector's properties
        float minAzimuth = in->readFloat();
        float maxAzimuth = in->readFloat();
        float fadeAngle = in->readFloat();
        setAzimuthRange(minAzimuth, maxAzimuth, fadeAngle);
    }
    else{
        in_THROW_EXCEPTION("AzimSector::read(): Expected AzimSector identification.");
    }
}
