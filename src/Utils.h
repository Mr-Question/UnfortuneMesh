// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <Point.h>
#include <Node.h>
#include <HalfEdge.h>
#include <TrianglesIterator.h>

#include <deque>
#include <string>
#include <chrono>

#include <fstream>
#include <iostream>
#include <algorithm>

struct Utils
{
  // =======================================================================
  // function : DecimalSeparator
  // purpose  :
  // =======================================================================
  template<typename CharT>
  class DecimalSeparator : public std::numpunct<CharT>
  {
  public:
    DecimalSeparator(CharT Separator)
    : m_Separator(Separator)
    {
    }

  protected:
    CharT do_decimal_point()const
    {
      return m_Separator;
    }

  private:
    CharT m_Separator;
  };

  // =======================================================================
  // function : GenerateXY
  // purpose  :
  // =======================================================================
  static void GenerateXY(
    const int          theDimSize,
    std::deque<Point>& thePoints)
  {
    for (int i = 0; i < theDimSize; ++i)
    {
      const double aX = static_cast<double>(i);
      for (int j = 0; j < theDimSize; ++j)
      {
        const double aY = static_cast<double>(j);
        const Point aPoint {aX, aY};

        thePoints.push_back (aPoint);
      }
    }
  }

  // =======================================================================
  // function : ReadXY
  // purpose  :
  // =======================================================================
  static void ReadXY(
    const std::string& theFileName,
    std::deque<Point>& thePoints)
  {
    Point aPoint;
    std::ifstream aFile (theFileName);
    while ((aFile >> aPoint.Data[0]) &&
           (aFile >> aPoint.Data[1]))
    {
      thePoints.push_back (aPoint);
    }
    aFile.close();
  }

  // =======================================================================
  // function : WriteObj
  // purpose  :
  // =======================================================================
  static void WriteObj(
    const std::string&       theFileName,
    const std::deque<Point>& thePoints,
    std::deque<HalfEdge>&    theMesh)
  {
    std::ofstream aFile (theFileName);
    aFile.imbue(std::locale(aFile.getloc(), new DecimalSeparator<char>(',')));
    std::for_each (thePoints.begin(), thePoints.end(), [&aFile](
      const Point& thePoint)
    {
      aFile << "v ";
      for (int i = 0; i < 2; ++i)
      {
        aFile << thePoint.Data[i] << " ";
      }
      aFile << 0 << std::endl;
    });

    for (TrianglesIterator aIt (theMesh); !aIt; ++aIt)
    {
      aFile << "f ";

      const HalfEdge* aHalfEdge = aIt.operator -> ();
      for (int i = 0; i < 3; ++i)
      {
        aFile << aHalfEdge->NodePtr->Id + 1 << " ";
        aHalfEdge = aHalfEdge->NextPtr;
      }
      aFile << std::endl;
    }

    aFile.close();
  }

  // =======================================================================
  // function : ShowTime
  // purpose  :
  // =======================================================================
  static std::chrono::system_clock::time_point ShowTime(
    const std::string&                           theMsg,
    const std::chrono::system_clock::time_point& theCheckPoint)
  {
    std::chrono::system_clock::time_point aEventPoint = std::chrono::high_resolution_clock::now ();
    std::cout << theMsg << " " << std::chrono::duration_cast<std::chrono::microseconds> (
      aEventPoint - theCheckPoint).count() / 1000. << " ms" << std::endl;
    std::cout.flush ();
    return aEventPoint;
  }
};
