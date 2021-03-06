/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

// ndn-congestion-topo-plugin.cpp
#include "ns3/node.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"
//#include "ns3/ndnSIM/helper/inrpp-strategy-choice-helper.hpp"
#include "ns3/ndnSIM/helper/inrpp-stack-helper.hpp"
//#include "ns3/ndnSIM/model/inrpp-l3-protocol.hpp"
#include "ns3/ndnSIM/NFD/daemon/fw/inrpp-forwarder.hpp"
#include "ns3/ndnSIM/NFD/daemon/face/inrpp-link-service.hpp"

NS_LOG_COMPONENT_DEFINE ("ndn.inrpp_topology");
using ndn::Interest;
using ndn::Data;
using nfd::Face;

namespace ns3 {

/**
 * This scenario simulates a grid topology (using topology reader module)
 *
 *   /------\                                                    /------\
 *   | Src1 |<--+                                            +-->| Dst1 |
 *   \------/    \                                          /    \------/
 *                \                                        /
 *                 +-->/------\   "bottleneck"  /------\<-+
 *                     | Rtr1 |<===============>| Rtr2 |
 *                 +-->\------/                 \------/<-+
 *                /                                        \
 *   /------\    /                                          \    /------\
 *   | Src2 |<--+                                            +-->| Dst2 |
 *   \------/                                                    \------/
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-congestion-topo-plugin
 */


/*static void OutInterests(uint32_t val, const Interest& interest, const Face& face)
{
	NS_LOG_LOGIC("Node " << val << " out interest "<<interest.getName().toUri() << " in face " << face.getId());

}

static void InInterests(uint32_t val, const Interest& interest, const Face& face)
{
	NS_LOG_LOGIC("Node " << val << " in interest "<<interest.getName().toUri() << " in face " << face.getId());

}

void OutData(uint32_t val, const Data& data, const Face& face)
{
	NS_LOG_LOGIC("Node " << val << " out data packet "<< data.getName().toUri() << " in face " << face.getId());

}

static void InData(uint32_t val, const Data& data, const Face& face)
{
	NS_LOG_LOGIC("Node " << val << " in data packet "<< data.getName().toUri() << " in face " << face.getId());

}

static void ChangeState(uint32_t val, nfd::face::InrppState state, const Face& face)
{
	NS_LOG_LOGIC("Node " << val << " changed state to "<< (int)state << " in face " << face.getId());

}*/

class PcapWriter {
public:
  PcapWriter(const std::string& file)
  {
    PcapHelper helper;
    m_pcap = helper.CreateFile(file, std::ios::out, PcapHelper::DLT_PPP);
  }

  void
  TracePacket(Ptr<const Packet> packet)
  {
    static PppHeader pppHeader;
    pppHeader.SetProtocol(0x0077);

    m_pcap->Write(Simulator::Now(), pppHeader, packet);
  }

private:
  Ptr<PcapFileWrapper> m_pcap;
};


int
main(int argc, char* argv[])
{
  bool inrpp=true;
  CommandLine cmd;
  cmd.AddValue ("inrpp", "enable inrpp protocol", inrpp);
  cmd.Parse(argc, argv);

  AnnotatedTopologyReader topologyReader("", 25);
  topologyReader.SetFileName("src/ndnSIM/examples/topologies/topo-8-node.txt");
  topologyReader.Read();

  // Install NDN stack on all nodes
  if(inrpp){
	  ns3::ndn::InrppStackHelper ndnHelper;
	  ndnHelper.setCsSize(10000);
	  ndnHelper.setPolicy("nfd::cs::priority_fifo");
	  ndnHelper.SetDefaultRoutes(false);
	  ndnHelper.InstallAll();
  }
  else {
	  ns3::ndn::StackHelper ndnHelper;
	  ndnHelper.setCsSize(10000);
	  ndnHelper.setPolicy("nfd::cs::priority_fifo");
	  ndnHelper.SetDefaultRoutes(false);
	  ndnHelper.InstallAll();
  }

 // ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "10000");
 // ndnHelper.InstallAll();

  // Choosing forwarding strategy
  if(inrpp) ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/inrpp");
  else ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route2");

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  // Getting containers for the consumer/producer
  Ptr<Node> consumer1 = Names::Find<Node>("Src1");
  Ptr<Node> consumer2 = Names::Find<Node>("Src2");
  Ptr<Node> consumer3 = Names::Find<Node>("Src3");

  Ptr<Node> producer1 = Names::Find<Node>("Dst1");
  Ptr<Node> producer2 = Names::Find<Node>("Dst2");
  Ptr<Node> producer3 = Names::Find<Node>("Dst3");

  //Ptr<Node> router2 = Names::Find<Node>("Rtr2");

  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  consumerHelper.SetAttribute("Frequency", StringValue("500")); // 10 interests a second
  consumerHelper.SetAttribute("LifeTime", StringValue("100s")); // 10 interests a second
  consumerHelper.SetAttribute("RetxTimer", StringValue("100s"));
  consumerHelper.SetAttribute("MaxSeq",StringValue("6690"));
  // on the first consumer node install a Consumer application
  // that will express interests in /dst1 namespace
  consumerHelper.SetPrefix("/dst1");
  ApplicationContainer cons = consumerHelper.Install(consumer1);
  cons.Start(Seconds(1));     // start consumers at 0s, 1s, 2s, 3s
  cons.Stop(Seconds(50)); // stop consumers at 19s, 18s, 17s, 16s
  // on the second consumer node install a Consumer application
  // that will express interests in /dst2 namespace
  consumerHelper.SetPrefix("/dst2");
  ApplicationContainer cons2 = consumerHelper.Install(consumer2);
  cons2.Start(Seconds(2));     // start consumers at 0s, 1s, 2s, 3s
  cons2.Stop(Seconds(50)); // stop consumers at 19s, 18s, 17s, 16s

  // on the second consumer node install a Consumer application
  // that will express interests in /dst2 namespace
  consumerHelper.SetPrefix("/dst3");
  ApplicationContainer cons3 = consumerHelper.Install(consumer3);
  cons3.Start(Seconds(3));     // start consumers at 0s, 1s, 2s, 3s
  cons3.Stop(Seconds(50)); // stop consumers at 19s, 18s, 17s, 16s

  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetAttribute("PayloadSize", StringValue("1495"));

  // Register /dst1 prefix with global routing controller and
  // install producer that will satisfy Interests in /dst1 namespace
  ndnGlobalRoutingHelper.AddOrigins("/dst1", producer1);
  producerHelper.SetPrefix("/dst1");
  producerHelper.Install(producer1);

  // Register /dst2 prefix with global routing controller and
  // install producer that will satisfy Interests in /dst2 namespace
  ndnGlobalRoutingHelper.AddOrigins("/dst2", producer2);
  producerHelper.SetPrefix("/dst2");
  producerHelper.Install(producer2);

  // Register /dst2 prefix with global routing controller and
  // install producer that will satisfy Interests in /dst2 namespace
  ndnGlobalRoutingHelper.AddOrigins("/dst3", producer3);
  producerHelper.SetPrefix("/dst3");
  producerHelper.Install(producer3);

  // Calculate and install FIBs
  if(inrpp) ndn::GlobalRoutingHelper::CalculateRoutesWithDetour();
  else ndn::GlobalRoutingHelper::CalculateRoutes();

  NodeContainer nodes = topologyReader.GetNodes();
  /*for(uint32_t i = 0; i<nodes.GetN(); i++)
  {
	  Ptr<ndn::InrppL3Protocol> l3 = nodes.Get(i)->GetObject<ndn::InrppL3Protocol>();

	  NS_LOG_LOGIC("L3 "<<l3 << " " << i);

	  l3->TraceConnectWithoutContext("OutInterests",MakeBoundCallback(&OutInterests,nodes.Get(i)->GetId()));
	  l3->TraceConnectWithoutContext("InInterests", MakeBoundCallback(&InInterests,nodes.Get(i)->GetId()));
	  l3->TraceConnectWithoutContext("OutData", MakeBoundCallback(&OutData,nodes.Get(i)->GetId()));
	  l3->TraceConnectWithoutContext("InData", MakeBoundCallback(&InData,nodes.Get(i)->GetId()));
	  l3->TraceConnectWithoutContext("InrppState", MakeBoundCallback(&ChangeState,nodes.Get(i)->GetId()));
	  std::shared_ptr<nfd::InrppForwarder> fw = std::static_pointer_cast<nfd::InrppForwarder>(l3->getForwarder()); // or

	  nfd::FaceTable& faces = fw->getFaceTable();
	  for (nfd::FaceTable::const_iterator it = faces.begin(); it != faces.end(); ++it)
	  {
		  Face& f = *it;
		  nfd::face::InrppLinkService* l = static_cast<nfd::face::InrppLinkService*>(f.getLinkService());
		  NS_LOG_LOGIC("Faces "<< l);
		  l->afterReceiveInrppInterest.connect([this, weakFace](nfd::face::InrppState state) {
		      shared_ptr<Face> face = weakFace.lock();
		      if (face != nullptr) {
		        this->m_outInterests(interest, *face);
		      }
		    });
	  }
	  //l3->getForwarder()->GetObject<nfd::InrppForwarder>();
  }*/
  // setting default parameters for PointToPoint links and channels
    std::string folder;
	if(inrpp) folder = "results_inrpp/";
	else folder = "results_fifo2/";
	std::string ratesFile = folder + "rates.txt";
	std::string dropFile = folder + "drop.txt";
	std::string delayFile = folder + "delay.txt";

	L2RateTracer::InstallAll(dropFile, Seconds(0.1));
	ndn::L3RateTracer::InstallAll(ratesFile, Seconds(0.1));
	ndn::AppDelayTracer::InstallAll(delayFile);

  PcapWriter trace("ndn-congestion-topo-plugin2.pcap");



  Simulator::Stop(Seconds(50.0));

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
