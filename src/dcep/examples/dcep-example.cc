/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
#include <cstring>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/dcep-app-helper.h"
#include <fstream>
#include<cstring>
#include "ns3/csma-module.h"
#include "ns3/olsr-helper.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("DcepExample");

NetDeviceContainer SetupWirelessNetwork (NodeContainer& n) 
{
    std::string phyMode ("DsssRate1Mbps");
    WifiHelper wifi;

    YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
    // set it to zero; otherwise, gain will be added
    wifiPhy.Set ("RxGain", DoubleValue (-10) ); 
    // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
    wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO); 

    YansWifiChannelHelper wifiChannel;
    wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
    wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
    wifiChannel.AddPropagationLoss ("ns3::RangePropagationLossModel",
			"MaxRange", StringValue("410.0"));
    wifiPhy.SetChannel (wifiChannel.Create ());
    
    
    // Add an upper mac and disable rate control
    WifiMacHelper wifiMac;
    wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
    wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                  "DataMode",StringValue (phyMode),
                                  "ControlMode",StringValue (phyMode));
    
    // Set it to adhoc mode
    wifiMac.SetType ("ns3::AdhocWifiMac");
    return  wifi.Install (wifiPhy, wifiMac, n);
}

int 
main (int argc, char *argv[])
{

    double distance = 1000; 
    uint32_t gridWidth = 5;
    std::string placementPolicy ("centralized");
    uint32_t numberOfEvents = 1;
    
    
    LogComponentEnable ("Placement", LOG_LEVEL_INFO);
    LogComponentEnable ("Dcep", LOG_LEVEL_INFO);
    LogComponentEnable ("Detector", LOG_LEVEL_INFO);
    LogComponentEnable ("Communication", LOG_LEVEL_INFO);
    LogComponentEnable ("dcepstate", LOG_LEVEL_INFO);

    //bool useV6 = false;
    
    //Address datasourceAddress;

    CommandLine cmd;
    cmd.AddValue ("placement policy", "Sender node number", placementPolicy);
    cmd.AddValue ("NumberOfEvents", "the number of events to be generated by each datasource", numberOfEvents);
    
    cmd.Parse (argc, argv);

  //
  // Explicitly create the nodes required by the topology (shown above).
  //
    uint32_t numNodes = gridWidth*gridWidth;
    NS_LOG_INFO ("Create nodes.");
    NodeContainer n;
    n.Create (numNodes);


    NS_LOG_INFO ("Create channels.");
    /*
     * 
     */
    NetDeviceContainer devices = SetupWirelessNetwork(n);
    MobilityHelper mobility;
    mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
            "MinX", DoubleValue (0.0),
            "MinY", DoubleValue (0.0),
            "DeltaX", DoubleValue (distance),
            "DeltaY", DoubleValue (distance),
            "GridWidth", UintegerValue (gridWidth),
            "LayoutType", StringValue ("RowFirst"));
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (n);

    OlsrHelper olsr;

    Ipv4StaticRoutingHelper staticRouting;

    Ipv4ListRoutingHelper list;
    list.Add (staticRouting, 0);
    list.Add (olsr, 10);

    InternetStackHelper internet;
    internet.SetRoutingHelper (list); 
    internet.Install (n);

    Ipv4AddressHelper ipv4;
    NS_LOG_INFO ("Assign IP Addresses.");
    ipv4.SetBase ("10.0.0.0", "255.255.255.0");
    Ipv4InterfaceContainer iface = ipv4.Assign (devices);

    Ipv4Address sinkAddress = Ipv4Address::ConvertFrom(Address (iface.GetAddress (0)));
    DcepAppHelper dcepApphelper;
    ApplicationContainer dcepApps = dcepApphelper.Install (n);
    
    uint32_t eventCode = 1;
    for(uint32_t i = 0; i < numNodes; i++)
    {
        dcepApps.Get(i)->SetAttribute("SinkAddress", Ipv4AddressValue (sinkAddress));
        dcepApps.Get(i)->SetAttribute("placement policy", StringValue(placementPolicy));
        
        if(i == 0)
        {
            /* sink node*/
            dcepApps.Get(i)->SetAttribute("IsSink", BooleanValue(true));
        }
        else if ((i == (numNodes-1)) || (i == (numNodes-2)))
        {
            NS_LOG_INFO ("Generator");
            dcepApps.Get(i)->SetAttribute("IsGenerator", BooleanValue(true));
            dcepApps.Get(i)->SetAttribute("event code", UintegerValue (eventCode++));
            dcepApps.Get(i)->SetAttribute("number of events", UintegerValue (numberOfEvents));
        }
    }
    
      dcepApps.Start (Seconds (120.0));
      dcepApps.Stop (Seconds (1300.0));

      Simulator::Stop(Seconds(1335.0));
      Simulator::Run ();
      Simulator::Destroy ();

      return 0;
}


