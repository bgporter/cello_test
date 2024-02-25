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

#include "MainComponent.h"

MainComponent::MainComponent (IpcDemoProperties& rootProps)
: demoProps (rootProps)
, drawingProps (demoProps)
{
    using Update  = cello::IpcClient::UpdateType;
    using Options = cello::IpcClient::ConnectOptions;

    // we need to behave differently depending on whether we're the server app
    // or the client app. The server app *always* creates the required pipes.
    const Options options { demoProps.isServer ? Options::createOrFail : Options::mustExist };

    // ..and we also change which of the sketches sends/receives data depending
    // on the client/server state.
    const Update serverUpdate { demoProps.isServer ? Update::send : Update::receive };
    makeSketch (serverSketch, serverUpdate, options, "serverSketch");

    const Update clientUpdate { demoProps.isServer ? Update::receive : Update::send };
    makeSketch (clientSketch, clientUpdate, options, "clientSketch");

    setSize (600, 400);
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (demoProps.isServer ? juce::Colours::white : juce::Colours::lightgrey);

    drawSketch (g, *clientSketch.sketchProperties, juce::Colours::red);
    drawSketch (g, *serverSketch.sketchProperties, juce::Colours::black);
}

void MainComponent::drawSketch (juce::Graphics& g, const SketchProperties& sketch, juce::Colour color) const
{
    juce::Path path;
    for (int i { 0 }; i < sketch.getNumChildren (); ++i)
    {
        Pt pt { sketch[i] };
        if (pt.lineStart)
            path.startNewSubPath (pt.x, pt.y);
        else
            path.lineTo (pt.x, pt.y);
    }

    // we add this bounding rectangle so that the scaleToFit call always scales to
    // fit the bounds of the window, not to the smallest bounding box of the current
    // set of points.
    path.addRectangle (0.f, 0.f, 1.f, 1.f);

    const auto bounds { getLocalBounds ().toFloat () };
    path.scaleToFit (0.f, 0.f, bounds.getWidth (), bounds.getHeight (), false);

    g.setColour (color);
    g.strokePath (path, juce::PathStrokeType { 3.f, juce::PathStrokeType::JointStyle::curved,
                                               juce::PathStrokeType::EndCapStyle::rounded });
}

void MainComponent::mouseDown (const juce::MouseEvent& e)
{
    if (e.mods.isRightButtonDown ())
    {
        // clear 'my' sketch on right click.
        auto& mySketch { demoProps.isServer ? serverSketch : clientSketch };
        juce::ValueTree tree { *mySketch.sketchProperties };
        tree.removeAllChildren (mySketch.sketchProperties->getUndoManager ());
        return;
    }

    addPoint (e.x, e.y, true);
}

void MainComponent::mouseDrag (const juce::MouseEvent& e)
{
    if (e.mods.isRightButtonDown ())
        return;
    addPoint (e.x, e.y, false);
}

void MainComponent::addPoint (int x, int y, bool isStart)
{
    auto& mySketch { demoProps.isServer ? serverSketch : clientSketch };
    const auto bounds { getLocalBounds ().toFloat () };
    mySketch.sketchProperties->addPoint (x / bounds.getWidth (), y / bounds.getHeight (), isStart);
}

bool MainComponent::makeSketch (Sketch& sketch, cello::IpcClient::UpdateType update,
                                cello::IpcClient::ConnectOptions options, const juce::String& name)
{
    // The SketchProperties object contains the list of points to draw. Add it as a
    // child of the `drawingProps` object.
    sketch.sketchProperties = std::make_unique<SketchProperties> (name, drawingProps);

    // ...and whenever a point is added or removed from that list, repaint this component.
    sketch.sketchProperties->onChildAdded   = [this] (juce::ValueTree&, int, int) { repaint (); };
    sketch.sketchProperties->onChildRemoved = [this] (juce::ValueTree&, int, int) { repaint (); };

    // The IpcClient connects to the sketch data and either sends out local changes to it,
    // or updates it with changes from the other process.
    sketch.ipcClient = std::make_unique<cello::IpcClient> (*(sketch.sketchProperties), name, -1, update, &demoProps);
    // the IPC client properties object lets us monitor the state of the IPC connection
    sketch.ipcClientProperties = std::make_unique<cello::IpcClientProperties> (name, &demoProps);

    // finally, tell the IPC client to connect so it may be used.
    return sketch.ipcClient->connect (options);
}
