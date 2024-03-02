/*
    Copyright (c) 2024 Brett g Porter
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

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
    MainComponent (IpcDemoProperties& rootProps);

    void paint (juce::Graphics&) override;
    /**
     * @brief Left button = start a new line in this sketch
     * Right button = clear this sketch
     */
    void mouseDown (const juce::MouseEvent& e) override;

    /**
     * @brief Add points to the current line in this sketch as we drag.
     */
    void mouseDrag (const juce::MouseEvent& e) override;

private:
    /**
     * @brief Add a point to the sketch, scaling all points into the range (0.0..1.0)
     *
     * @param x
     * @param y
     * @param isStart true if the mouse button was just clicked, starting a new line
     * in the sketch
     */
    void addPoint (int x, int y, bool isStart);
    void drawSketch (juce::Graphics& g, const SketchProperties& sketch, juce::Colour color) const;

    struct Sketch
    {
        std::unique_ptr<SketchProperties> sketchProperties;
        std::unique_ptr<cello::IpcClient> ipcClient;
        std::unique_ptr<cello::IpcClientProperties> ipcClientProperties;
    };

    /**
     * @brief Helper function to create all the objects needed for a single
     * sketch:
     * - the SketchProperties object that manages point data
     * - the IpcClient object that connects a peer tree in another process to the
     *   SketchProperties object in this process
     * - the IpcClientProperties object we can use to monitor that IpcClient.
     *
     * @param sketch
     * @param update
     * @param options
     * @param name
     * @return bool
     */
    bool makeSketch (Sketch& sketch, cello::IpcClient::UpdateType update, cello::IpcClient::ConnectOptions options,
                     const juce::String& name);

    IpcDemoProperties demoProps;
    DrawingProperties drawingProps;

    Sketch serverSketch;
    Sketch clientSketch;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
