/*
  ==============================================================================

    DrawingProperties.h
    Created: 18 Feb 2024 6:23:10pm
    Author:  Brett Porter

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/**
 * @class Pt
 * @brief a record for a single point in a drawing, giving the (x, y) position
 * and a boolean that's true for the first point in a new line, then false for
 * all points added by dragging.
 */
struct Pt : public cello::Object
{
    Pt(float x_, float y_, bool start) : cello::Object("pt", nullptr)
    {
        x = x_;
        y = y_;
        lineStart = start;
    }

    Pt(const juce::ValueTree &tree) : cello::Object("pt", tree)
    {
    }
    MAKE_VALUE_MEMBER(float, x, {});
    MAKE_VALUE_MEMBER(float, y, {});
    MAKE_VALUE_MEMBER(bool, lineStart, false);
};

/**
 * @class SketchProperties
 * @brief Each process will have two 'sketches' -- its own, and one that's mirrored
 * from the other process.
 */
struct SketchProperties : public cello::Object
{
    SketchProperties(const juce::String &name, cello::Object &parent) : cello::Object(name, parent)
    {
    }

    void addPoint(float x, float y, bool start)
    {
        Pt point{x, y, start};
        append(&point);
    }

    void clear()
    {
        data.removeAllChildren(getUndoManager());
    }
};

/**
 * @class DrawingProperties
 * @brief A container object that holds the two sketches.
 */
struct DrawingProperties : public cello::Object
{
    DrawingProperties(cello::Object &parent)
        : cello::Object("drawing", parent), serverSketch("server", *this), clientSketch("client", *this)
    {
    }

    SketchProperties serverSketch;
    SketchProperties clientSketch;
};
