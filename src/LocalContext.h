// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <Context.h>

#include <Mesh.h>
#include <Frame.h>
#include <cmath>

struct LocalContext : public Context
{
  // =======================================================================
  // function : Constructor
  // purpose  :
  // =======================================================================
  LocalContext(
    Mesh&             theMesh,
    const double      theMinAngleThresholdDeg = 1.,
    const double      theMaxAngleThresholdDeg = 179.,
    const double      theScale                = 1.,
    const Frame::Type theFrameType            = Frame::Triangle)
    : Context (theMesh)
    , FrameType (theFrameType)
    , Scale     (theScale)
    , SinMinAngleThreshold (sin (theMinAngleThresholdDeg * M_PI / 180.))
    , CosMaxAngleThreshold (cos (theMaxAngleThresholdDeg * M_PI / 180.))
  {
  }

  Frame::Type FrameType;
  double      Scale;
  double      SinMinAngleThreshold;
  double      CosMaxAngleThreshold;
};
