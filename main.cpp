// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#include <Launcher.h>
#include <Mesh.h>
#include <Utils.h>
#include <Context.h>

#include <iostream>

#include <deque>

namespace
{
  // =======================================================================
  // function : is_number
  // purpose  :
  // =======================================================================
  bool is_number(const std::string& s)
  {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
  }
}

// =======================================================================
// function : main
// purpose  :
// =======================================================================
int main(int theArgc, char* theArgv[])
{
  if (theArgc < 3)
    return 1;

  Mesh aMesh;
  Context aContext (aMesh);
  std::deque<Point> aPoints;

  if (is_number (theArgv[1]))
  {
    Utils::GenerateXY (std::atoi (theArgv[1]), aPoints);
  }
  else
  {
    Utils::ReadXY (theArgv[1], aPoints);
  }

  std::chrono::system_clock::time_point aCheckPoint = std::chrono::high_resolution_clock::now ();
  Launcher (aPoints, aContext);
  aCheckPoint = Utils::ShowTime ("Mesh", aCheckPoint);

  Utils::WriteObj (theArgv[2], aPoints, aMesh.HalfEdges);
  return 0;
}
