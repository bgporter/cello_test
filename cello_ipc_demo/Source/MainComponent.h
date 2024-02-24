#pragma once

#include <JuceHeader.h>

#include "DrawingProperties.h"
#include "IpcDemoProperties.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::Component
{
  public:
    //==============================================================================
    MainComponent(IpcDemoProperties &rootProps);
    ~MainComponent() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent &e) override;
    void mouseDrag(const juce::MouseEvent &e) override;

  private:
    void addPoint(int x, int y, bool isStart);
    void drawSketch(juce::Graphics &g, const SketchProperties &sketch, juce::Colour color);

    IpcDemoProperties demoProps;
    DrawingProperties drawingProps;
    SketchProperties serverSketchProps;
    SketchProperties clientSketchProps;

    std::unique_ptr<cello::IpcClient> ipc;
    std::unique_ptr<cello::IpcClientProperties> ipcProperties;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
