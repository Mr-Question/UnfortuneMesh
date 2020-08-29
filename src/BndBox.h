// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <Point.h>

struct BndBox
{
  union
  {
    struct
    {
      Point Min;
      Point Max;
    } Corners;

    Point Data[2];
  };

  // =======================================================================
  // function : Constructor
  // purpose  :
  // =======================================================================
  BndBox(const Point& theMinCorner = Point{},
         const Point& theMaxCorner = Point{})
    : Corners {theMinCorner, theMaxCorner}
  {
  }
};
