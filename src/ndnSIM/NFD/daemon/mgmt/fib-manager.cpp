/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014-2017,  Regents of the University of California,
 *                           Arizona Board of Regents,
 *                           Colorado State University,
 *                           University Pierre & Marie Curie, Sorbonne University,
 *                           Washington University in St. Louis,
 *                           Beijing Institute of Technology,
 *                           The University of Memphis.
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fib-manager.hpp"
#include "fw/face-table.hpp"

#include <ndn-cxx/lp/tags.hpp>
#include <ndn-cxx/mgmt/nfd/fib-entry.hpp>

#include <boost/range/adaptor/transformed.hpp>

namespace nfd {

NFD_LOG_INIT("FibManager");

FibManager::FibManager(Fib& fib,
        			 //  Fib& dfib,
                       const FaceTable& faceTable,
                       Dispatcher& dispatcher,
                       CommandAuthenticator& authenticator)
  : NfdManagerBase(dispatcher, authenticator, "fib")
  , m_fib(fib)
 // , m_dfib(dfib)
  , m_faceTable(faceTable)
{
  registerCommandHandler<ndn::nfd::FibAddNextHopCommand>("add-nexthop",
    bind(&FibManager::addNextHop, this, _2, _3, _4, _5));
  registerCommandHandler<ndn::nfd::FibRemoveNextHopCommand>("remove-nexthop",
    bind(&FibManager::removeNextHop, this, _2, _3, _4, _5));
  //registerCommandHandler<ndn::nfd::FibAddNextHopCommand>("add-detour-nexthop",
  //  bind(&FibManager::addDetourNextHop, this, _2, _3, _4, _5));

  registerStatusDatasetHandler("list", bind(&FibManager::listEntries, this, _1, _2, _3));
}

/*void
FibManager::addDetourNextHop(const Name& topPrefix, const Interest& interest,
                       ControlParameters parameters,
                       const ndn::mgmt::CommandContinuation& done)
{
  setFaceForSelfRegistration(interest, parameters);

  const Name& prefix = parameters.getName();
  FaceId faceId = parameters.getFaceId();
  uint64_t cost = parameters.getCost();

  NFD_LOG_TRACE("add-detour-nexthop prefix: " << prefix
                << " faceid: " << faceId
                << " cost: " << cost);

  Face* face = m_faceTable.get(faceId);
  if (face != nullptr) {
    fib::Entry* entry = m_dfib.insert(prefix).first;
    entry->addNextHop(*face, cost);

    NFD_LOG_DEBUG("add-detour-nexthop result: OK"
                  << " prefix:" << prefix
                  << " faceid: " << faceId
                  << " cost: " << cost);

    return done(ControlResponse(200, "Success").setBody(parameters.wireEncode()));
  }
  else {
    NFD_LOG_INFO("add-detour-nexthop result: FAIL reason: unknown-faceid: " << faceId);
    return done(ControlResponse(410, "Face not found"));
  }
}*/

void
FibManager::addNextHop(const Name& topPrefix, const Interest& interest,
                       ControlParameters parameters,
                       const ndn::mgmt::CommandContinuation& done)
{
  setFaceForSelfRegistration(interest, parameters);

  const Name& prefix = parameters.getName();
  FaceId faceId = parameters.getFaceId();
  uint64_t cost = parameters.getCost();

  NFD_LOG_TRACE("add-nexthop prefix: " << prefix
                << " faceid: " << faceId
                << " cost: " << cost);

  Face* face = m_faceTable.get(faceId);
  if (face != nullptr) {
    fib::Entry* entry = m_fib.insert(prefix).first;
    entry->addNextHop(*face, cost);

    NFD_LOG_DEBUG("add-nexthop result: OK"
                  << " prefix:" << prefix
                  << " faceid: " << faceId
                  << " cost: " << cost);

    return done(ControlResponse(200, "Success").setBody(parameters.wireEncode()));
  }
  else {
    NFD_LOG_INFO("add-nexthop result: FAIL reason: unknown-faceid: " << faceId);
    return done(ControlResponse(410, "Face not found"));
  }
}

void
FibManager::removeNextHop(const Name& topPrefix, const Interest& interest,
                          ControlParameters parameters,
                          const ndn::mgmt::CommandContinuation& done)
{
  setFaceForSelfRegistration(interest, parameters);

  NFD_LOG_TRACE("remove-nexthop prefix: " << parameters.getName()
                << " faceid: " << parameters.getFaceId());

  Face* face = m_faceTable.get(parameters.getFaceId());
  if (face != nullptr) {
    fib::Entry* entry = m_fib.findExactMatch(parameters.getName());
    if (entry != nullptr) {
      entry->removeNextHop(*face);
      NFD_LOG_DEBUG("remove-nexthop result: OK prefix: " << parameters.getName()
                    << " faceid: " << parameters.getFaceId());

      if (!entry->hasNextHops()) {
        m_fib.erase(*entry);
      }
    }
    else {
      NFD_LOG_DEBUG("remove-nexthop result: OK");
    }
  }
  else {
    NFD_LOG_DEBUG("remove-nexthop result: OK");
  }

  done(ControlResponse(200, "Success").setBody(parameters.wireEncode()));
}

void
FibManager::listEntries(const Name& topPrefix, const Interest& interest,
                        ndn::mgmt::StatusDatasetContext& context)
{
  for (const auto& entry : m_fib) {
    const auto& nexthops = entry.getNextHops() |
                           boost::adaptors::transformed([] (const fib::NextHop& nh) {
                             return ndn::nfd::NextHopRecord()
                                 .setFaceId(nh.getFace().getId())
                                 .setCost(nh.getCost());
                           });
    context.append(ndn::nfd::FibEntry()
                   .setPrefix(entry.getPrefix())
                   .setNextHopRecords(std::begin(nexthops), std::end(nexthops))
                   .wireEncode());
  }
  context.end();
}

void
FibManager::setFaceForSelfRegistration(const Interest& request, ControlParameters& parameters)
{
  bool isSelfRegistration = (parameters.getFaceId() == 0);
  if (isSelfRegistration) {
    shared_ptr<lp::IncomingFaceIdTag> incomingFaceIdTag = request.getTag<lp::IncomingFaceIdTag>();
    // NDNLPv2 says "application MUST be prepared to receive a packet without IncomingFaceId field",
    // but it's fine to assert IncomingFaceId is available, because InternalFace lives inside NFD
    // and is initialized synchronously with IncomingFaceId field enabled.
    BOOST_ASSERT(incomingFaceIdTag != nullptr);
    parameters.setFaceId(*incomingFaceIdTag);
  }
}

} // namespace nfd
