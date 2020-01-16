// MIT License
// Copyright (c) 2020 Oleg Agashin
// See file LICENSE for detail.

#pragma once

#include <stddef.h>

struct Point;

struct Node
{
  const Point* Pos;
  size_t       Id;
};
