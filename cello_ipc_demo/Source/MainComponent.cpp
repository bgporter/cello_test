#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(IpcDemoProperties &rootProps)
    : demoProps(rootProps), drawingProps(demoProps), serverSketchProps("server", drawingProps),
      clientSketchProps("client", drawingProps)
{
    serverSketchProps.onChildAdded = [this](juce::ValueTree &child, int oldIndex, int newIndex) { repaint(); };
    serverSketchProps.onChildRemoved = [this](juce::ValueTree &child, int oldIndex, int newIndex) { repaint(); };
    clientSketchProps.onChildAdded = [this](juce::ValueTree &child, int oldIndex, int newIndex) { repaint(); };
    clientSketchProps.onChildRemoved = [this](juce::ValueTree &child, int oldIndex, int newIndex) { repaint(); };

    const cello::IpcClient::ConnectOptions options{demoProps.isServer ? cello::IpcClient::ConnectOptions::createOrFail
                                                                      : cello::IpcClient::ConnectOptions::mustExist};

    cello::IpcClient::UpdateType update = static_cast<cello::IpcClient::UpdateType>(
        cello::IpcClient::UpdateType::send | cello::IpcClient::UpdateType::receive);

    //   cello::IpcClient::UpdateType::fullUpdateOnConnect;

    // create an IPC client for this end and connect appropriately
    ipc = std::make_unique<cello::IpcClient>(drawingProps, "drawingDemo", -1, update, &demoProps);
    if (ipc != nullptr)
    {
        ipcProperties = std::make_unique<cello::IpcClientProperties>(&demoProps);
        ipcProperties->connected.onPropertyChange([this](juce::Identifier) {
            DBG("IPC connection is" << (ipcProperties->connected ? "" : " NOT") << " connected.");
        });
        auto connectedOk{ipc->connect(options)};
        juce::String msg{demoProps.isServer ? "Server" : "Client"};
        msg << " connection: " << (connectedOk ? "OK" : "ERROR");
        DBG(msg);
    }

    setSize(600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::white);

    drawSketch(g, clientSketchProps, juce::Colours::red);
    drawSketch(g, serverSketchProps, juce::Colours::black);
}

void MainComponent::drawSketch(juce::Graphics &g, const SketchProperties &sketch, juce::Colour color)
{
    juce::Path path;
    for (int i{0}; i < sketch.getNumChildren(); ++i)
    {
        Pt pt{sketch[i]};
        if (pt.lineStart)
            path.startNewSubPath(pt.x, pt.y);
        else
            path.lineTo(pt.x, pt.y);
    }

    path.addRectangle(0.f, 0.f, 1.f, 1.f);

    const auto bounds{getLocalBounds().toFloat()};
    path.scaleToFit(0.f, 0.f, bounds.getWidth(), bounds.getHeight(), false);

    g.setColour(color);
    g.strokePath(path, juce::PathStrokeType{2.f, juce::PathStrokeType::JointStyle::curved,
                                            juce::PathStrokeType::EndCapStyle::rounded});
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::mouseDown(const juce::MouseEvent &e)
{
    if (e.mods.isRightButtonDown())
    {
        // clear 'my' sketch on right click.
        auto &mySketch{demoProps.isServer ? serverSketchProps : clientSketchProps};
        juce::ValueTree tree{mySketch};
        tree.removeAllChildren(mySketch.getUndoManager());
        return;
    }

    addPoint(e.x, e.y, true);
}

void MainComponent::mouseDrag(const juce::MouseEvent &e)
{
    if (e.mods.isRightButtonDown())
        return;
    addPoint(e.x, e.y, false);
}

void MainComponent::addPoint(int x, int y, bool isStart)
{
    auto &mySketch{demoProps.isServer ? serverSketchProps : clientSketchProps};
    auto bounds{getLocalBounds().toFloat()};
    mySketch.addPoint(x / bounds.getWidth(), y / bounds.getHeight(), isStart);
}
