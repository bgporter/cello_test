/*
  ==============================================================================

    IpcDemoProperties.h
    Created: 18 Feb 2024 6:22:42pm
    Author:  Brett Porter

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct IpcDemoProperties : public cello::Object
{
    IpcDemoProperties(bool isServer_) : cello::Object("root", nullptr)
    {
        isServer = isServer_;
    }

    MAKE_VALUE_MEMBER(bool, isServer, true);
};
