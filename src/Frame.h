// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <Point.h>
#include <BndBox.h>

#include <deque>

class Frame
{
public:

  enum Type
  {
    Triangle,
    Rectangle,
    WidthCutTriangle,
    WidthCutRectangle,
    Grid
  };

  // =======================================================================
  // function : Add
  // purpose  :
  // =======================================================================
  static void Add(
    const BndBox&      theBox,
    std::deque<Point>& thePoints,
    const Type         theFrameType)
  {
    switch (theFrameType)
    {
    case Triangle:
      addFrameTriangle (theBox, thePoints);
      break;

    case Rectangle:
      addFrameRectangle (theBox, thePoints);
      break;

    case WidthCutTriangle:
      addFrameWidthCutTriangle (theBox, thePoints);
      break;

    case WidthCutRectangle:
      addFrameWidthCutRectangle (theBox, thePoints);
      break;

    case Grid:
      addFrameGrid (theBox, thePoints);
      break;

    default:
      break;
    }
  }

private:

  // =======================================================================
  // function : addFrameTriangle
  // purpose  :
  // =======================================================================
  static void addFrameTriangle(
    const BndBox&      theBox,
    std::deque<Point>& thePoints)
  {
    const double aDelta[2] = {
      (theBox.Corners.Max.Coord.X - theBox.Corners.Min.Coord.X),
      (theBox.Corners.Max.Coord.Y - theBox.Corners.Min.Coord.Y)
    };

    const double aDeltaMin = std::min (aDelta[0], aDelta[1]);
    const double aDeltaMax = std::max (aDelta[0], aDelta[1]);
    const double aDeltaSum = aDelta[0] + aDelta[1];

    thePoints.push_back (Point {
      (theBox.Corners.Max.Coord.X + theBox.Corners.Min.Coord.X) / 2.,
      (theBox.Corners.Max.Coord.Y + aDeltaMax)});

    thePoints.push_back (Point {
      (theBox.Corners.Min.Coord.X - aDeltaSum),
      (theBox.Corners.Min.Coord.Y - aDeltaMin)});

    thePoints.push_back (Point {
      (theBox.Corners.Max.Coord.X + aDeltaSum),
      (theBox.Corners.Min.Coord.Y - aDeltaMin)});
  }

  // =======================================================================
  // function : addFrameRectangle
  // purpose  :
  // =======================================================================
  static void addFrameRectangle(
    const BndBox&      theBox,
    std::deque<Point>& thePoints)
  {
    const double aDX = (theBox.Corners.Max.Coord.X - theBox.Corners.Min.Coord.X) / 2.;
    const double aDY = (theBox.Corners.Max.Coord.Y - theBox.Corners.Min.Coord.Y) / 2.;
    thePoints.push_back (Point {
      theBox.Corners.Min.Coord.X - aDX,
      theBox.Corners.Min.Coord.Y - aDY
    });

    thePoints.push_back (Point {
      theBox.Corners.Max.Coord.X + aDX,
      theBox.Corners.Min.Coord.Y - aDY
    });

    thePoints.push_back (Point {
      theBox.Corners.Min.Coord.X - aDX,
      theBox.Corners.Max.Coord.Y + aDY
    });

    thePoints.push_back (Point {
      theBox.Corners.Max.Coord.X + aDX,
      theBox.Corners.Max.Coord.Y + aDY
    });
  }

  // =======================================================================
  // function : addFrameWidthCutTriangle
  // purpose  :
  // =======================================================================
  static void addFrameWidthCutTriangle(
    const BndBox&      theBox,
    std::deque<Point>& thePoints)
  {
    const double aDY = (theBox.Corners.Max.Coord.Y - theBox.Corners.Min.Coord.Y) / 2.;
    thePoints.push_back (Point {
      theBox.Corners.Min.Coord.X,
      theBox.Corners.Min.Coord.Y - aDY
    });

    thePoints.push_back (Point {
      theBox.Corners.Max.Coord.X,
      theBox.Corners.Min.Coord.Y - aDY
    });

    thePoints.push_back (Point {
     (theBox.Corners.Min.Coord.X + theBox.Corners.Max.Coord.X) / 2.,
      theBox.Corners.Max.Coord.Y + aDY
    });
  }

  // =======================================================================
  // function : addFrameWidthCutRectangle
  // purpose  :
  // =======================================================================
  static void addFrameWidthCutRectangle(
    const BndBox&      theBox,
    std::deque<Point>& thePoints)
  {
    const double aDY = (theBox.Corners.Max.Coord.Y - theBox.Corners.Min.Coord.Y) / 2.;
    thePoints.push_back (Point {
      theBox.Corners.Min.Coord.X,
      theBox.Corners.Min.Coord.Y - aDY
    });

    thePoints.push_back (Point {
      theBox.Corners.Max.Coord.X,
      theBox.Corners.Min.Coord.Y - aDY
    });

    thePoints.push_back (Point {
      theBox.Corners.Min.Coord.X,
      theBox.Corners.Max.Coord.Y + aDY
    });

    thePoints.push_back (Point {
      theBox.Corners.Max.Coord.X,
      theBox.Corners.Max.Coord.Y + aDY
    });
  }

  // =======================================================================
  // function : addFrameGrid
  // purpose  :
  // =======================================================================
  static void addFrameGrid(
    const BndBox&      theBox,
    std::deque<Point>& thePoints)
  {
    const int aAuxSize = 10;

    const double aDelta[2] = {
      2. * (theBox.Corners.Max.Coord.X - theBox.Corners.Min.Coord.X) / (aAuxSize - 1),
      2. * (theBox.Corners.Max.Coord.Y - theBox.Corners.Min.Coord.Y) / (aAuxSize - 1)
    };

    const Point aStartPoint {
      (theBox.Corners.Max.Coord.X + theBox.Corners.Min.Coord.X) / 2. -
        (theBox.Corners.Max.Coord.X - theBox.Corners.Min.Coord.X),
      (theBox.Corners.Max.Coord.Y + theBox.Corners.Min.Coord.Y) / 2. -
        (theBox.Corners.Max.Coord.Y - theBox.Corners.Min.Coord.Y)
    };

    for (int j = 0; j < aAuxSize; ++j)
    {
      for (int i = 0; i < aAuxSize; ++i)
      {
        if (j < aAuxSize - 1)
        {
          if (i > 0 && i < aAuxSize - 1)
            continue;
        }

        const Point aPoint {
          aStartPoint.Coord.X + i * aDelta[0],
          aStartPoint.Coord.Y + j * aDelta[1]
        };

        thePoints.push_back (aPoint);
      }
    }
  }
};
