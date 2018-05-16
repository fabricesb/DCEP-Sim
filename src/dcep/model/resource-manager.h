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

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "ns3/object.h"
#include "ns3/traced-callback.h"
#include "placement.h"
#include "ns3/olsr-routing-protocol.h"
#include "ns3/ipv4.h"

namespace ns3
{
    class MonitoredRoute
    {
        public:
            MonitoredRoute(Ipv4Address adr, Ipv4Address nextAddr);
            Ipv4Address adr;
            uint32_t hops;
            Ipv4Address nextAddr;
            std::vector<std::string> etypes;
          //  uint32_t threshhold;
    };

    class ResourceManager: public Object
    {
        public:
            static TypeId GetTypeId (void);

            ResourceManager ();

            void Configure (void);
            void getRoute(olsr::RoutingTableEntry &);
            
        private:

            Ptr<olsr::RoutingProtocol> olsr_routing_protocol;

    };
}
#endif /* RESOURCE_MANAGER_H */

