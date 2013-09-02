/**********************************************************************
 *
 *    FILE:            StateSet.cpp
 *
 *    DESCRIPTION:    Read/Write osg::StateSet in binary format to disk.
 *
 *    CREATED BY:        Auto generated by iveGenerated
 *                    and later modified by Rune Schmidt Jensen.
 *
 *    HISTORY:        Created 17.3.2003
 *
 *    Copyright 2003 VR-C
 **********************************************************************/

#include "Exception.h"
#include "StateSet.h"
#include "Object.h"

#include <osg/StateAttribute>

using namespace ive;

void StateSet::write(DataOutputStream* out){
    // Write StateSet's identification.
    out->writeInt(IVESTATESET);
    // If the osg class is inherited by any other class we should also write this to file.
    osg::Object*  obj = dynamic_cast<osg::Object*>(this);
    if(obj){
         ((ive::Object*)(obj))->write(out);
    }
    else
        out_THROW_EXCEPTION("StateSet::write(): Could not cast this osg::StateSet to an osg::Object.");

    // Write render bin bin details.
    switch(getRenderBinMode()){
        case osg::StateSet::INHERIT_RENDERBIN_DETAILS:        out->writeChar((char)0);break;
        case osg::StateSet::USE_RENDERBIN_DETAILS:            out->writeChar((char)1);break;
        case osg::StateSet::OVERRIDE_RENDERBIN_DETAILS:        out->writeChar((char)2);break;
        default: out_THROW_EXCEPTION("Unknown RenderBinMode in StateSet::write()");
    }
    out->writeInt(getBinNumber());
    out->writeString(getBinName());


    StateSet::ModeList ml = getModeList();
    StateSet::AttributeList al = getAttributeList();

    // Write stateset modes, this could for instance be GL_NORMALIZE, GL_LIGHT, etc.
    out->writeInt(ml.size());
    for(StateSet::ModeList::const_iterator mitr=ml.begin(); mitr!=ml.end(); ++mitr){
        out->writeInt((unsigned int)mitr->first);
        out->writeInt((unsigned int)mitr->second);
    }

    // Write stateset attributes, this could for instance be alphafunctions, materials, etc.
    out->writeInt(al.size());
    for(StateSet::AttributeList::iterator aitr=al.begin(); aitr!=al.end(); ++aitr){
        out->writeStateAttribute(aitr->second.first.get());
        out->writeInt(aitr->second.second);
    }

    StateSet::TextureModeList tml = getTextureModeList();
    StateSet::TextureAttributeList tal = getTextureAttributeList();

    // Write texture stateset modes, this could for instance be TEXGEN ON, TEXTURECUBEMAP ON, etc.
    int nUnits = tml.size();
    out->writeInt(nUnits);
        int unit;
    for(unit=0;unit<nUnits;unit++){
        ml = tml[unit];
        out->writeInt(ml.size());
        for(StateSet::ModeList::const_iterator mitr=ml.begin(); mitr!=ml.end(); ++mitr){
            out->writeInt((unsigned int)mitr->first);
            out->writeInt((unsigned int)mitr->second);
        }
    }

    // Write texture attributes, this could for instance texture2D, texturecubemap
    nUnits = tal.size();
    out->writeInt(nUnits);
    for(unit=0;unit<nUnits;unit++){
        al = tal[unit];
        out->writeInt(al.size());
        for(StateSet::AttributeList::iterator aitr=al.begin(); aitr!=al.end(); ++aitr){
            out->writeStateAttribute(aitr->second.first.get());
            out->writeInt(aitr->second.second);
        }
    }

    if ( out->getVersion() >= VERSION_0010 )
    {
        // Write stateset uniforms
        StateSet::UniformList ul = getUniformList();
        out->writeInt(ul.size());
        for(StateSet::UniformList::iterator uitr=ul.begin(); uitr!=ul.end(); ++uitr)
        {
            out->writeUniform(uitr->second.first.get());
            out->writeInt(uitr->second.second);
        }
    }
}

void StateSet::read(DataInputStream* in){
    // Read StateSet's identification.
    int id = in->peekInt();
    if(id == IVESTATESET){
        // Code to read StateSet's properties.
        id = in->readInt();

        // If the osg class is inherited by any other class we should also read this from file.
        osg::Object*  obj = dynamic_cast<osg::Object*>(this);
        if(obj){
            ((ive::Object*)(obj))->read(in);;
        }
        else
            in_THROW_EXCEPTION("StateSet::read(): Could not cast this osg::StateSet to an osg::Object.");

        // Read render bin details.
        char c = in->readChar();
        int num = in->readInt();
        std::string name = in->readString();
        switch((int)c){
            case 0:
                setRenderBinDetails(num, name, osg::StateSet::INHERIT_RENDERBIN_DETAILS);
                break;
            case 1:
                setRenderBinDetails(num, name, osg::StateSet::USE_RENDERBIN_DETAILS);
                break;
            case 2:
                setRenderBinDetails(num, name, osg::StateSet::OVERRIDE_RENDERBIN_DETAILS);
                break;
            case 3:
                setRenderBinDetails(num, name, osg::StateSet::USE_RENDERBIN_DETAILS);
                break;
            default: in_THROW_EXCEPTION("Unknown RenderBinMode in StateSet::read()");
        }

        // Read stateset modes.
        int size = in->readInt();
        int i;
        for(i=0;i<size;i++){
            int mode = in->readInt();
            int value = in->readInt();
            setMode((osg::StateAttribute::GLMode)mode, (osg::StateAttribute::GLModeValue)value);
        }

        // Read state attributes
        size = in->readInt();
        for(i=0;i<size;i++){
            osg::StateAttribute* attribute = in->readStateAttribute();
            setAttribute(attribute, (osg::StateAttribute::OverrideValue)in->readInt());
        }

        // Read texture stateset mode.
        int nUnits = in->readInt();
        int unit;
        for(unit=0;unit<nUnits;unit++){
            size = in->readInt();
            for(i=0;i<size;i++){
                int mode = in->readInt();
                int value = in->readInt();
                setTextureMode(unit,(osg::StateAttribute::GLMode)mode, (osg::StateAttribute::GLModeValue)value);
            }
        }

        // Read texture attributes.
        nUnits = in->readInt();
        for(unit=0;unit<nUnits;unit++)
        {
            size = in->readInt();
            for(i=0;i<size;i++)
            {
                osg::StateAttribute* attribute = in->readStateAttribute();
                setTextureAttribute(unit, attribute, (osg::StateAttribute::OverrideValue)in->readInt());
            }
        }

        if ( in->getVersion() >= VERSION_0010 )
        {
            // Read uniforms
            size = in->readInt();
            for(i=0;i<size;i++)
            {
                osg::Uniform* uniform = in->readUniform();
                addUniform(uniform, (osg::StateAttribute::OverrideValue)in->readInt());
            }
        }
    }
    else{
        in_THROW_EXCEPTION("StateSet::read(): Expected StateSet identification");
    }
}
