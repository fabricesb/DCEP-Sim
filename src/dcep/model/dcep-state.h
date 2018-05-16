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

#ifndef DCEP_STATE_H
#define DCEP_STATE_H

#include "ns3/object.h"
#include "ns3/ipv4-address.h"
#include "common.h"
namespace ns3
{
    class Query;
    class Event;
     
    class EventRoutingTableEntry : public Object{
    public:
        static TypeId GetTypeId (void);
 
       // void EventRoutingTableEntry(Ptr<EventRoutingTableEntry>);
        EventRoutingTableEntry();
        Ptr<Query> source_query;//the query used to produce the event corresponding to the current entry
        OperatorState state;
        bool monitoring;
        Ipv4Address current_processor;
        Ipv4Address next_hop;//main info used to determine where parent query should be placed
        uint32_t freezeAck_counter;
        std::vector<Ptr< Event> > freeze_queue;
        std::vector<Ipv4Address> dataSources;
        
    };
    
    class DcepState : public Object
    {
    public:
        static TypeId GetTypeId (void);
        DcepState ();
        void Configure ();
        
        bool IsExpected(Ptr<Event> e);
        bool IsActive(std::string eventType);
        Ipv4Address GetOuputDest(std::string eventType);
        Ipv4Address GetNextHop(std::string eventType);
        Ptr<Query> GetQuery(std::string eventType);
        OperatorState GetState(std::string eventType);
        Ipv4Address GetCurrentProcessor(std::string eType);
        
        
        void SetNextHop (std::string eventType, Ipv4Address adr);
        void SetCurrentProcessor (std::string eventType, Ipv4Address adr);
        void SetOutDest (std::string eventType, Ipv4Address adr);
        void CreateEventRoutingTableEntry (Ptr<Query> q);
        
        
    private:
        void HandlerLocalPlacement (std::string eType);
        
        void SetState (std::string eventType, OperatorState state);
        Ptr<EventRoutingTableEntry> lookUpEventRoutingTable(std::string eventType);
        std::vector<Ptr<EventRoutingTableEntry> > eventRoutingTable;
    };
}

#endif /* DCEP_STATE_H */

