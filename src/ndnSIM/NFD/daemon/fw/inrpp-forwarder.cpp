/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014-2016,  Regents of the University of California,
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

#include "inrpp-forwarder.hpp"
#include "algorithm.hpp"
#include "core/logger.hpp"
#include "strategy.hpp"
#include "table/cleanup.hpp"
#include <ndn-cxx/lp/tags.hpp>
#include "face/null-face.hpp"
#include <boost/random/uniform_int_distribution.hpp>

namespace nfd {

NFD_LOG_INIT("InrppForwarder");

InrppForwarder::InrppForwarder() : Forwarder()
{
	NFD_LOG_DEBUG(this);
	m_delayGoal = 50;
}

InrppForwarder::~InrppForwarder() = default;

void
InrppForwarder::onOutgoingData(const Data& data, Face& inFace, Face& outFace)
{
	  if (outFace.getId() == face::INVALID_FACEID) {
	    NFD_LOG_WARN("onOutgoingData face=invalid data=" << data.getName());
	    return;
	  }
	  NFD_LOG_DEBUG("onOutgoingData face=" << outFace.getId() << " data=" << data.getName() << " scope= " << outFace.getScope());

	 // std::cout << this << " onOutgoingData face=" << outFace.getId() << " data=" << data.getName() << std::endl;
	  // /localhost scope control
	  bool isViolatingLocalhost = outFace.getScope() == ndn::nfd::FACE_SCOPE_NON_LOCAL &&
	                              scope_prefix::LOCALHOST.isPrefixOf(data.getName());
	  if (isViolatingLocalhost) {
	    NFD_LOG_DEBUG("onOutgoingData face=" << outFace.getId() <<
	                  " data=" << data.getName() << " violates /localhost");
	    // (drop)
	    return;
	  }

	  // TODO traffic manager

	  // send Data
	 // std::size_t found = data.getName().toUri().find("/prefix");
	  //Name localName("/localhost");
	 // if(localName.isPrefixOf(data.getName()))
	  if(outFace.getScope()==ndn::nfd::FACE_SCOPE_LOCAL)
	  {
		  NFD_LOG_DEBUG("Prefix outgoingdata face=" << outFace.getId() <<
		 	                  " data=" << data.getName());
		  outFace.sendData(data);
		  ++m_counters.nOutData;
	  } else if(outFace.getInrppState()!=face::InrppState::CLOSED_LOOP) {
		  //auto interest = make_shared<ndn::Interest>(data.getName());
		  NFD_LOG_DEBUG("Prefix outgoingdata face=" << outFace.getId() <<
		 	                  " data=" << data.getName() << " size=" <<   data.getContent().size());
		  m_outTable.insert(std::pair<FaceId,nameFace>(outFace.getId(),nameFace(data.getName(),inFace.getId())));

		  std::map<FaceId,uint32_t>::iterator it = m_bytes.find(outFace.getId());
		  if(it != m_bytes.end())
		  {
			  NFD_LOG_DEBUG("Bytes in the queue="<<it->second);
			  it->second+= static_cast<uint32_t>(data.getContent().size());
		  } else
		  {
			  m_bytes.insert(std::pair<FaceId,uint32_t>(outFace.getId(),static_cast<uint32_t>(data.getContent().size())));

		  }
		  //shared_ptr<Data> dataCopyWithoutTag = make_shared<Data>(data);
		  //dataCopyWithoutTag->removeTag<lp::HopCountTag>();
		  //NFD_LOG_DEBUG("NFD CACHE");
		  //m_cs.insert(*dataCopyWithoutTag);

	  }
	  //outFace.sendData(data);
	  //++m_counters.nOutData;
}

void
InrppForwarder::onIncomingData(Face& inFace, const Data& data)
{
  // receive Data
  NFD_LOG_DEBUG("onIncomingData face=" << inFace.getId() << " data=" << data.getName() << " "<<static_cast<int>(inFace.getInrppState()));
  data.setTag(make_shared<lp::IncomingFaceIdTag>(inFace.getId()));
  ++m_counters.nInData;

  if(checkCongestion(data)) inFace.setInrppState(face::InrppState::CONGESTED);
  NFD_LOG_DEBUG("onIncomingData face=" << inFace.getId() << " data=" << data.getName() << " "<<static_cast<int>(inFace.getInrppState()));

  // /localhost scope control
  bool isViolatingLocalhost = inFace.getScope() == ndn::nfd::FACE_SCOPE_NON_LOCAL &&
                              scope_prefix::LOCALHOST.isPrefixOf(data.getName());
  if (isViolatingLocalhost) {
    NFD_LOG_DEBUG("onIncomingData face=" << inFace.getId() <<
                  " data=" << data.getName() << " violates /localhost");
    // (drop)
    return;
  }

  // PIT match
  pit::DataMatchResult pitMatches = m_pit.findAllDataMatches(data);
  if (pitMatches.begin() == pitMatches.end()) {
    // goto Data unsolicited pipeline
    onDataUnsolicited(inFace, data);
    return;
  }

  shared_ptr<Data> dataCopyWithoutTag = make_shared<Data>(data);
  dataCopyWithoutTag->removeTag<lp::HopCountTag>();

  // CS insert
  if (m_csFromNdnSim == nullptr)
  {
	  NFD_LOG_DEBUG("NFD CACHE");
	  m_cs.insert(*dataCopyWithoutTag);
  }
  else
  {
	  NFD_LOG_DEBUG("NS3 CACHE " << m_csFromNdnSim->GetTypeId());
	  m_csFromNdnSim->Add(dataCopyWithoutTag);
  }

  std::set<Face*> pendingDownstreams;
  // foreach PitEntry
  auto now = time::steady_clock::now();
  for (const shared_ptr<pit::Entry>& pitEntry : pitMatches) {
    NFD_LOG_DEBUG("onIncomingData matching=" << pitEntry->getName());

    // cancel unsatisfy & straggler timer
    this->cancelUnsatisfyAndStragglerTimer(*pitEntry);

    // remember pending downstreams
    for (const pit::InRecord& inRecord : pitEntry->getInRecords()) {
      if (inRecord.getExpiry() > now) {
        pendingDownstreams.insert(&inRecord.getFace());
      }
    }

    // invoke PIT satisfy callback
    //beforeSatisfyInterest(*pitEntry, inFace, data);
    this->dispatchToStrategy(*pitEntry,
      [&] (fw::Strategy& strategy) { strategy.beforeSatisfyInterest(pitEntry, inFace, data); });

    // Dead Nonce List insert if necessary (for out-record of inFace)
    this->insertDeadNonceList(*pitEntry, true, data.getFreshnessPeriod(), &inFace);

    // mark PIT satisfied
    pitEntry->clearInRecords();
    pitEntry->deleteOutRecord(inFace);

    // set PIT straggler timer
    this->setStragglerTimer(pitEntry, true, data.getFreshnessPeriod());
  }

  // foreach pending downstream
  for (Face* pendingDownstream : pendingDownstreams) {
    if (pendingDownstream == &inFace) {
      continue;
    }
    // goto outgoing Data pipeline
    this->onOutgoingData(data,inFace, *pendingDownstream);
  }
}

bool
InrppForwarder::checkCongestion(const Data& data)
{
    shared_ptr<lp::CongestionMarkTag> congestionMarkTag = data.getTag<lp::CongestionMarkTag>();
    if (congestionMarkTag != nullptr) {
    		data.removeTag<lp::CongestionMarkTag>();
    		NFD_LOG_DEBUG("Congestion received");
    		return true;
    }
    return false;
}

bool
InrppForwarder::checkBackpressure(const Interest& interest)
{
    shared_ptr<lp::BackpressureMarkTag> backpressureMarkTag = interest.getTag<lp::BackpressureMarkTag>();
    if (backpressureMarkTag != nullptr) {
     	interest.removeTag<lp::BackpressureMarkTag>();
    		NFD_LOG_DEBUG("Backpressure received");
    		return true;
    }
    return false;
}

void
InrppForwarder::sendData(FaceId id)
{
    //NFD_LOG_DEBUG(this << " SendData face=" << id << " outTable " << m_outTable.size() << " cslimit="<< m_cs.getLimit() << " size="<<m_cs.size());
	std::map<FaceId,nameFace>::iterator it = m_outTable.find(id);

	//NFD_LOG_DEBUG("outTable size=" << m_outTable.size());
	if(it!=m_outTable.end())
	{

		nameFace name = it->second;
		NFD_LOG_DEBUG("Send=" << name.first << " " << m_cs.getLimit() << " " << m_cs.size());

	    //const ndn::Interest& interest(name.first);
		m_outTable.erase(it);

		m_cs.find(ndn::Interest(name.first),
		               bind(&InrppForwarder::onContentStoreHit, this,id, _1, _2),
		               bind(&InrppForwarder::onContentStoreMiss, this,id,name.second, _1));
		//m_cs.find(it->second);
		//NFD_LOG_DEBUG("outTable size=" << m_outTable.size());
		//NFD_LOG_DEBUG("outTable size=" << m_outTable.size());

		//NFD_LOG_DEBUG("outTable size=" << m_outTable.size());


	}

	uint64_t bps = m_faceTable.get(id)->getBps();
	std::map<FaceId,uint32_t>::iterator it2 = m_bytes.find(id);
	if(it2!=m_bytes.end())
	{
		if(it2->second>0)
		{
			NFD_LOG_DEBUG("outTable bytes time=" << it2->second << " " <<id);
			std::map<FaceId,double>::iterator it3 = m_queueTime.find(id);
			if(it3!=m_queueTime.end()){
				NFD_LOG_DEBUG("outTable queue time=" << it2->second << " " << bps << " "<< (double)it2->second*8/bps);
				it3->second = (double)it2->second*8/bps;
			}else{
				m_queueTime.insert(std::pair<FaceId,double>(id,(double)it2->second*8/bps));
			}
		}

	}

}

int
InrppForwarder::GetPackets(FaceId id)
{
	return m_outTable.count(id);
}

/*void
InrppForwarder::onOutgoingInterest(const shared_ptr<pit::Entry>& pitEntry, Face& outFace, const Interest& interest)
{
  NFD_LOG_DEBUG("onOutgoingInterest face=" << outFace.getId() <<
                " interest=" << pitEntry->getName());

  // insert out-record
  pitEntry->insertOrUpdateOutRecord(outFace, interest);

  // send Interest
  outFace.sendInterest(interest);
  ++m_counters.nOutInterests;
}*/

void
InrppForwarder::onContentStoreHit(FaceId id, const Interest& interest, const Data& data)
//InrppForwarder::onContentStoreHit( Face& outFace, const shared_ptr<pit::Entry>& pitEntry, const Interest& interest, const Data& data)
{
    NFD_LOG_DEBUG("onContentStoreHit face=" << id << " " << data.getName() <<" "<<data.getName().at(-1).toSequenceNumber());
	Face* outFace = m_faceTable.get(id);
	++m_counters.nOutData;

	std::map<FaceId,uint32_t>::iterator it = m_bytes.find(id);
	if(it != m_bytes.end())
	{
		NFD_LOG_DEBUG("Sojourn time "<< (double)it->second*8/outFace->getBps() << " " << (double)m_delayGoal/1000 << " "<<it->second<<" "<<outFace->getBps());
		if((double)it->second*8/outFace->getBps()>(double)m_delayGoal/1000){
			NFD_LOG_DEBUG("Congestion!!! "<<(double)it->second*8/outFace->getBps()<<" "<<(double)m_delayGoal/1000);
			data.setTag(make_shared<lp::CongestionMarkTag>(1));
		}
	    //interest->setTag(make_shared<lp::CongestionMarkTag>(1));
		NFD_LOG_DEBUG("Bytes in the queue="<<it->second << " " << id);
		it->second-= static_cast<uint32_t>(data.getContent().size());
		NFD_LOG_DEBUG("Bytes in the queue="<<it->second << " " << id);

	}

	outFace->sendData(data);

}

void
InrppForwarder::onContentStoreMiss(FaceId id, FaceId inFace, const Interest& interest)
//InrppForwarder::onContentStoreMiss( Face& inFace, const shared_ptr<pit::Entry>& pitEntry,const Interest& interest)
{
    NFD_LOG_DEBUG("onContentStoreMiss face=" << id <<" "<< interest.getName().at(-1).toSequenceNumber());
   // m_bytes.
    if(inFace!=NULL)
    {
	std::map<FaceId,uint32_t>::iterator it = m_bytes.find(inFace);
	if(it!=m_bytes.end())m_bytes.erase(it);

	//m_faceTable.get(inFace)->setInrppState(face::InrppState::CLOSED_LOOP);

	sendData(id);

	notifyUpstream(id,interest);
    }
}

void
InrppForwarder::onIncomingInterest(Face& inFace, const Interest& interest)
{
  // receive Interest
  NFD_LOG_DEBUG("onIncomingInterest face=" << inFace.getId() << " Nonce="<< interest.getNonce());
               // " interest=" << interest.getName().at(-1).toSequenceNumber());//
  if(checkBackpressure(interest)){
	  NFD_LOG_DEBUG("CLOSED_LOOP RECEIVED");
	  inFace.setInrppState(face::InrppState::CLOSED_LOOP);
	  m_cs.find(interest,
	  		               bind(&InrppForwarder::onContentStoreHit, this,inFace.getId(), _1, _2),
	  		               bind(&InrppForwarder::onContentStoreMiss, this,inFace.getId(),NULL, _1));
	  		//m_cs.find(it->second);
  }
  else Forwarder::onIncomingInterest(inFace,interest);
}

void
InrppForwarder::notifyUpstream(FaceId id,const Interest& interest)
{
	//shared_ptr<Interest> inter = make_shared<Interest>(interest);
	//Interest inter = Interest(interest);
	//inter.setNonce(0);
	//const Interest& inter2 = inter;
	//interest.setNonce(0);
	interest.setTag(make_shared<lp::BackpressureMarkTag>(1));

	m_faceTable.get(id)->sendInterest(interest);

}


} // namespace nfd
