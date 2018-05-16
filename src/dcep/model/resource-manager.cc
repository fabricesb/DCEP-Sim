/*
 * Copyright (C) 2018, Fabrice S. Bigirimana
 * Copyright (c) 2018, University of Oslo
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * 
 */


#include "resource-manager.h"
#include "ns3/log.h"
#include "ns3/string.h"
#include "dcep.h"
#include "cep-engine.h"
#include "ns3/boolean.h"
#include "dcep-state.h"
#include "ns3/communication.h"

namespace ns3
{
    NS_OBJECT_ENSURE_REGISTERED (ResourceManager);
    NS_LOG_COMPONENT_DEFINE ("ResourceManager");
    
    TypeId
    ResourceManager::GetTypeId(void)
    {
        static TypeId id = TypeId("ns3::ResourceManager")
        .SetParent<Object>()
        .AddConstructor<ResourceManager>();
        
        return id;
    }
    
    ResourceManager::ResourceManager()
    {}
    
    void
    ResourceManager::Configure(void)
    {
        Ptr<Dcep> dcep = GetObject<Dcep>();
        Ptr<Placement> p = GetObject<Placement>();

        StringValue s;
        dcep->GetAttribute("routing protocol", s);

        if (s.Get() == "olsr") 
        {

            NS_LOG_INFO("Current routing protocol is olsr");
            olsr_routing_protocol = dcep->GetNode()->GetObject<olsr::RoutingProtocol>();

        }
    }
    
    
    void
    ResourceManager::getRoute(olsr::RoutingTableEntry &rentry) {

        /*
         * gets the route to a specific destination on behalf
         * of a placement policy.
         * 
         */
        //NS_LOG_INFO("checking entry " << rentry.destAddr);

        if (rentry.destAddr == GetObject<Communication>()->GetLocalAddress()) {
            NS_LOG_INFO("DESTINATION IS LOCAL NODE, SOMEONE IS SCRUING UP! ABORTING...");
            NS_FATAL_ERROR_NO_MSG ();
            rentry.nextAddr = GetObject<Communication>()->GetLocalAddress();
            return;
        }

        if (olsr_routing_protocol) {
            std::vector<ns3::olsr::RoutingTableEntry>::iterator it;
            std::vector<ns3::olsr::RoutingTableEntry> olsr_entries = olsr_routing_protocol->GetRoutingTableEntries();

            bool found = false;
            for (it = olsr_entries.begin(); it != olsr_entries.end(); it++) {
                ns3::olsr::RoutingTableEntry entry = *it;

                //NS_LOG_INFO("current entry " << entry.destAddr );

                if (entry.destAddr.IsEqual(rentry.destAddr)) {
          //          NS_LOG_INFO("found entry, next hop is " << entry.nextAddr);
                    found = true;
                    //found an entry for the current destination
                    rentry.distance = entry.distance;
                    rentry.interface = entry.interface;
                    rentry.nextAddr.Set(entry.nextAddr.Get()); //the next hop
//                    if(this->localAddress.IsEqual(this->sinkAddress))
//                    std::cout << "Number of hops to destination: " << entry.distance << std::endl;
                    break;
                }
            }
            if (!found) {
               // NS_LOG_INFO("Couldn't find next hop to provided destination");
                rentry.nextAddr = Ipv4Address::GetAny();
            }
        }

    }
    
}
