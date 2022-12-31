# cello_test
Unit test app for the [cello](https://github.com/bgporter/cello) juce module. 

## Setup

1. Clone this repo
2. After cloning, get the referenced submodules (JUCE, cello, and test_suite) by executing `git submodule update --init --recursive` 
3. Build and run the JUCE Projucer found in ` submodules/JUCE/extras/Projucer/Build/{yourOS}`
4. Load `cello_test.jucer` in the Projucer and save the project to create build/project files for your compiler. 
5. Build/run the `cello_test` application to run the unit tests. 