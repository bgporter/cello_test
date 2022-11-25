/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>

//==============================================================================
int main(int argc, char *argv[])
{

    juce::StringArray argList{argv, argc};

    // ..your code goes here!
    TestSuite::runAllTests(argList.joinIntoString(" "));

    return 0;
}
