// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <Point.h>

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
    const std::pair<Point, Point>& theBox,
    std::deque<Point>&             thePoints,
    const Type                     theFrameType)
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
    const std::pair<Point, Point>& theBox,
    std::deque<Point>&             thePoints)
  {
    const double aDelta[2] = {
      (theBox.second.Coord.X - theBox.first.Coord.X),
      (theBox.second.Coord.Y - theBox.first.Coord.Y)
    };

    const double aDeltaMin = std::min (aDelta[0], aDelta[1]);
    const double aDeltaMax = std::max (aDelta[0], aDelta[1]);
    const double aDeltaSum = aDelta[0] + aDelta[1];

    thePoints.push_back (Point {
      (theBox.second.Coord.X + theBox.first.Coord.X) / 2.,
      (theBox.second.Coord.Y + aDeltaMax)});

    thePoints.push_back (Point {
      (theBox.first.Coord.X - aDeltaSum),
      (theBox.first.Coord.Y - aDeltaMin)});

    thePoints.push_back (Point {
      (theBox.second.Coord.X + aDeltaSum),
      (theBox.first .Coord.Y - aDeltaMin)});
  }

  // =======================================================================
  // function : addFrameRectangle
  // purpose  :
  // =======================================================================
  static void addFrameRectangle(
    const std::pair<Point, Point>& theBox,
    std::deque<Point>&             thePoints)
  {
    const double aDX = (theBox.second.Coord.X - theBox.first.Coord.X) / 2.;
    const double aDY = (theBox.second.Coord.Y - theBox.first.Coord.Y) / 2.;
    thePoints.push_back (Point {
      theBox.first .Coord.X - aDX,
      theBox.first .Coord.Y - aDY
    });

    thePoints.push_back (Point {
      theBox.second.Coord.X + aDX,
      theBox.first .Coord.Y - aDY
    });

    thePoints.push_back (Point {
      theBox.first .Coord.X - aDX,
      theBox.second.Coord.Y + aDY
    });

    thePoints.push_back (Point {
      theBox.second.Coord.X + aDX,
      theBox.second.Coord.Y + aDY
    });
  }

  // =======================================================================
  // function : addFrameWidthCutTriangle
  // purpose  :
  // =======================================================================
  static void addFrameWidthCutTriangle(
    const std::pair<Point, Point>& theBox,
    std::deque<Point>&             thePoints)
  {
    const double aDY = (theBox.second.Coord.Y - theBox.first.Coord.Y) / 2.;
    thePoints.push_back (Point {
      theBox.first .Coord.X,
      theBox.first .Coord.Y - aDY
    });

    thePoints.push_back (Point {
      theBox.second.Coord.X,
      theBox.first .Coord.Y - aDY
    });

    thePoints.push_back (Point {
     (theBox.first .Coord.X + theBox.second.Coord.X) / 2.,
      theBox.second.Coord.Y + aDY
    });
  }

  // =======================================================================
  // function : addFrameWidthCutRectangle
  // purpose  :
  // =======================================================================
  static void addFrameWidthCutRectangle(
    const std::pair<Point, Point>& theBox,
    std::deque<Point>&             thePoints)
  {
    const double aDY = (theBox.second.Coord.Y - theBox.first.Coord.Y) / 2.;
    thePoints.push_back (Point {
      theBox.first .Coord.X,
      theBox.first .Coord.Y - aDY
    });

    thePoints.push_back (Point {
      theBox.second.Coord.X,
      theBox.first .Coord.Y - aDY
    });

    thePoints.push_back (Point {
      theBox.first .Coord.X,
      theBox.second.Coord.Y + aDY
    });

    thePoints.push_back (Point {
      theBox.second.Coord.X,
      theBox.second.Coord.Y + aDY
    });
  }

  // =======================================================================
  // function : addFrameGrid
  // purpose  :
  // =======================================================================
  static void addFrameGrid(
    const std::pair<Point, Point>& theBox,
    std::deque<Point>&             thePoints)
  {
    const int aAuxSize = 10;

    const double aDelta[2] = {
      2. * (theBox.second.Coord.X - theBox.first.Coord.X) / (aAuxSize - 1),
      2. * (theBox.second.Coord.Y - theBox.first.Coord.Y) / (aAuxSize - 1)
    };

    const Point aStartPoint {
      (theBox.second.Coord.X + theBox.first.Coord.X) / 2. - (theBox.second.Coord.X - theBox.first.Coord.X),
      (theBox.second.Coord.Y + theBox.first.Coord.Y) / 2. - (theBox.second.Coord.Y - theBox.first.Coord.Y)
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
