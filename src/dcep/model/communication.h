/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

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


#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include "ns3/object.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/ipv4-address.h"
#include "ns3/traced-callback.h"
#include "ns3/queue.h"
#include "ns3/packet.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"

namespace ns3 {

/* ... */
    class Event;
    class Packet;
    class Query;
    
//template<typename Item> class DropTailQueue;
class Communication : public Object
    {
    public:
        Communication();
        ~Communication();
        
        static TypeId GetTypeId (void);
        /*create udp server and client applications to use for communication
         Might need to customize them a bit.*/
        void Configure (void);
        
        void setNode(Ptr<Node> node);
        void setPort(uint16_t);
        
      /**
   * \brief Handle a packet reception.
   *
   * This function is called by lower layers.
   *
   * \param socket the socket the packet was received to.
   */
        void HandleRead (Ptr<Socket> socket);

       // void ScheduleSend(Ipv4Address peerAddress, const uint8_t *, uint32_t size, uint16_t msg_type);
        void ScheduleSend(Ptr<Packet> p, Ipv4Address addr);
        Ipv4Address GetLocalAddress();  
        Ipv4Address GetSinkAddress();
    
    private:
        
        void send(void);
        Ptr<Queue> m_sendQueue;
        EventId m_packetSendEvent;
        uint32_t backoffTime;
        uint16_t m_port; 
        uint32_t numRetransmissions;
        Ipv4Address m_sinkAddress;
        Ipv4Address host_address;
        Ptr<Socket> m_socket; 
        Ptr<Node> disnode;
        uint32_t m_sent; 
     
    };
   
    
}

#endif /* COMMUNICATION_H */

