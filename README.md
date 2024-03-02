# cello_test
Unit test app for the [cello](https://github.com/bgporter/cello) juce module. 

## Setup

1. Clone this repo
2. After cloning, get the referenced submodules (JUCE, cello, and test_suite) by executing `git submodule update --init --recursive` 
3. Build and run the JUCE Projucer found in ` submodules/JUCE/extras/Projucer/Build/{yourOS}`
4. Load `cello_test.jucer` in the Projucer and save the project to create build/project files for your compiler. 
5. Build/run the `cello_test` application to run the unit tests. 

## IPC Test/Demo

Added Feb 2024, requires cello version 1.3.0 or later. 

1. Load `cello_ipc_demo/cello_ipc_demo.jucer` into Projucer and save the project to create build/project files for your compiler/OS.
2. Running the ensuing executable will launch a second copy of the application as a 'client' version of the app. Left click and drag in the window of either copy of the application to draw (lines drawn in the server app are in black, lines drawn in the client app display in red). The cello Interprocess Communication system will mirror drawings done in one app into the other's view. Right click in either app to clear its drawing. 
