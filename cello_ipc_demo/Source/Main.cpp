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
#include <JuceHeader.h>

#include "DrawingProperties.h"
#include "IpcDemoProperties.h"

//==============================================================================
class cello_ipc_demoApplication : public juce::JUCEApplication
{
public:
    //==============================================================================
    cello_ipc_demoApplication () {}

    const juce::String getApplicationName () override { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion () override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed () override { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        const juce::String clientCmd { "--client" };
        auto isServer { !commandLine.containsWholeWordIgnoreCase (clientCmd) };

        rootProperties = std::make_unique<IpcDemoProperties> (isServer);

        mainWindow.reset (new MainWindow (getApplicationName (), *(rootProperties.get ())));
        if (isServer)
        {
            // If this app is the first to launch, it becomes the server, and will
            // launch a second copy of itself as a child process, passing the command
            // line flag "--client" to tell that instance that it's to run in client
            // mode.
            auto myExecutable { juce::File::getSpecialLocation (juce::File::currentExecutableFile) };
            if (myExecutable.existsAsFile ())
            {
                auto cmdLine { myExecutable.getFullPathName () };
                cmdLine << " " << clientCmd;
                auto ok { clientProcess.start (cmdLine) };
                jassert (ok);
            }
        }
    }

    void shutdown () override
    {
        // Add your application's shutdown code here..
        if (clientProcess.isRunning ())
            clientProcess.kill ();
        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit () override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to
        // close.
        quit ();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name, IpcDemoProperties& rootProps)
        : DocumentWindow (name,
                          juce::Desktop::getInstance ().getDefaultLookAndFeel ().findColour (
                              juce::ResizableWindow::backgroundColourId),
                          DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent (rootProps), true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
#else
            setResizable (true, true);
            centreWithSize (getWidth (), getHeight ());
#endif

            setVisible (true);
        }

        void closeButtonPressed () override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance ()->systemRequestedQuit ();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;

    std::unique_ptr<IpcDemoProperties> rootProperties;
    std::unique_ptr<DrawingProperties> drawingProperties;
    juce::ChildProcess clientProcess;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (cello_ipc_demoApplication)
