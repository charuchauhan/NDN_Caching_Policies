// ndn-simple.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {

int
main(int argc, char* argv[])
{

  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault ("ns3::QueueBase::MaxSize", StringValue ("30p"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Creating nodes
  NodeContainer nodes;
  nodes.Create(7);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  p2p.Install(nodes.Get(0), nodes.Get(1));
  p2p.Install(nodes.Get(1), nodes.Get(2));
 p2p.Install(nodes.Get(2), nodes.Get(3));
 p2p.Install(nodes.Get(3), nodes.Get(4));
 p2p.Install(nodes.Get(4), nodes.Get(5));
 p2p.Install(nodes.Get(5), nodes.Get(6));


  // Install NDN stack on all nodes
  /*ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();*/
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.setCsSize(10); // allow just 2 entries to be cached
  ndnHelper.setPolicy("nfd::cs::lru");
  ndnHelper.InstallAll();
  // Choosing forwarding strategy
  ndn::StrategyChoiceHelper::InstallAll("/prefix", "/localhost/nfd/strategy/multicast");

  // Installing applications

  // Consumer
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerBatches");
  // Consumer will request /prefix/0, /prefix/1, ...
  consumerHelper.SetPrefix("/prefix");
  consumerHelper.SetAttribute("Batches", StringValue("1s 1 10s 1")); // 10 interests a second
  consumerHelper.Install(nodes.Get(0));                        // first node

consumerHelper.SetAttribute("Batches", StringValue("11s 1"));
  consumerHelper.Install(nodes.Get(1));
  
  consumerHelper.SetAttribute("Batches", StringValue("11s 1"));
  consumerHelper.Install(nodes.Get(2));
  
  consumerHelper.SetAttribute("Batches", StringValue("11s 1"));
  consumerHelper.Install(nodes.Get(3));
  consumerHelper.SetAttribute("Batches", StringValue("11s 1"));
  consumerHelper.Install(nodes.Get(4));
  consumerHelper.SetAttribute("Batches", StringValue("11s 1"));
  consumerHelper.Install(nodes.Get(5));
  // Producer
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerHelper.SetPrefix("/prefix");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(nodes.Get(6)); // last node
	
  

  Simulator::Stop(Seconds(20.0));
  ndn::L3RateTracer::InstallAll("L3_rate_linear_trace.txt", Seconds(0.5));
  L2RateTracer::InstallAll("L2_rate_linear_trace.txt", Seconds(0.5));
 ndn::CsTracer::InstallAll("cs-hit-linear_trace.txt", Seconds(0.5));
 ndn::AppDelayTracer::InstallAll("app-delays-linear-trace.txt");
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
