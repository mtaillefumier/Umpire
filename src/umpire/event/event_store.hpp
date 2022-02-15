//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016-20, Lawrence Livermore National Security, LLC and Umpire
// project contributors. See the COPYRIGHT file for details.
//
// SPDX-License-Identifier: (MIT)
//////////////////////////////////////////////////////////////////////////////
#ifndef UMPIRE_event_store_HPP
#define UMPIRE_event_store_HPP

#include <string>
#include <vector>

namespace umpire {
namespace event {

struct event;

class event_store {
 public:
  virtual void insert(event e) = 0;
  virtual void insert_direct(const std::string& s) = 0;

  virtual std::vector<event> get_events() = 0;
};

} // namespace event
} // namespace umpire

#endif // UMPIRE_event_store_HPP
