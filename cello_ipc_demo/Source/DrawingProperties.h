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

/**
 * @class Pt
 * @brief a record for a single point in a drawing, giving the (x, y) position
 * and a boolean that's true for the first point in a new line, then false for
 * all points added by dragging. All points used in this app should be scaled
 * between the range of (0.0..1.0)
 */
struct Pt : public cello::Object
{
    Pt (float x_, float y_, bool start)
    : cello::Object ("pt", nullptr)
    {
        x         = x_;
        y         = y_;
        lineStart = start;
    }

    Pt (const juce::ValueTree& tree)
    : cello::Object ("pt", tree)
    {
    }
    MAKE_VALUE_MEMBER (float, x, {});
    MAKE_VALUE_MEMBER (float, y, {});
    MAKE_VALUE_MEMBER (bool, lineStart, false);
};

/**
 * @class SketchProperties
 * @brief Each process will have two 'sketches' -- its own, and one that's mirrored
 * from the other process. This class manages a list of `Pt` point objects (as
 * children of its ValueTree)
 */
struct SketchProperties : public cello::Object
{
    SketchProperties (const juce::String& name, cello::Object& parent)
    : cello::Object (name, parent)
    {
    }

    void addPoint (float x, float y, bool start)
    {
        Pt point { x, y, start };
        append (&point);
    }

    void clear () { data.removeAllChildren (getUndoManager ()); }
};

/**
 * @class DrawingProperties
 * @brief A container object that will hold the two sketches.
 */
struct DrawingProperties : public cello::Object
{
    DrawingProperties (cello::Object& parent)
    : cello::Object ("drawing", parent)
    {
    }
};
