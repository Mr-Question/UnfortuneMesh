// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <Mesh.h>
#include <Frame.h>
#include <cmath>

struct Context
{
  // =======================================================================
  // function : Constructor
  // purpose  :
  // =======================================================================
  Context(
    Mesh&             theMesh,
    const double      theMinAngleThresholdDeg = 1.,
    const double      theMaxAngleThresholdDeg = 179.,
    const double      theScale                = 1.,
    const Frame::Type theFrameType            = Frame::Triangle)
    : Data      (theMesh)
    , Front     (nullptr)
    , FrameType (theFrameType)
    , Scale     (theScale)
    , SinMinAngleThreshold (sin (theMinAngleThresholdDeg * M_PI / 180.))
    , CosMaxAngleThreshold (cos (theMaxAngleThresholdDeg * M_PI / 180.))
  {
  }

  Mesh&       Data;
  HalfEdge*   Front;
  Frame::Type FrameType;
  double      Scale;
  double      SinMinAngleThreshold;
  double      CosMaxAngleThreshold;
};
