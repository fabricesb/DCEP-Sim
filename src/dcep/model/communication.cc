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

#include "communication.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/type-id.h"
#include "ns3/dcep.h"
#include "dcep-header.h"
#include "seq-ts-header.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-header.h"
#include "ns3/log.h"
#include "cep-engine.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/names.h"
#include "ns3/uinteger.h"
#include "ns3/nstime.h"
#include "placement.h"
#include "common.h"
#include "message-types.h"
#include "ns3/socket-factory.h"
#include "src/internet/model/ipv4-header.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "ns3/abort.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(Communication);
NS_LOG_COMPONENT_DEFINE("Communication");


/* ... */
    TypeId
    Communication::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::Communication")
        .SetParent<Object> ()
        .AddConstructor<Communication> ()
        .AddAttribute ("SinkAddress",
                       "The destination Address of the outbound packets",
                       Ipv4AddressValue (),
                       MakeIpv4AddressAccessor (&Communication::m_sinkAddress),
                       MakeIpv4AddressChecker ())
        
        ;
        
        return tid;
    }
 
    Communication::Communication()
   // : m_lossCounter (0)
    {
        numRetransmissions = 0;
        m_sent=0;
        m_sendQueue = CreateObject<DropTailQueue> ();
        
    }
    
    
    Communication::~Communication()
    {
      //  m_lossCounter.~PacketLossCounter();
        this->m_sendQueue->DequeueAll();
    }
    
    void
    Communication::setNode(Ptr<Node> node)
    {
        this->disnode = node;
    }
    
    void
    Communication::setPort(uint16_t cepPort)
    {
        this->m_port = cepPort;
    }
    
    void
    Communication::Configure()
    {
        Ptr<Dcep> dcep = GetObject<Dcep>();
        Ipv4AddressValue adr;
        dcep->GetAttribute("SinkAddress", adr);
        m_sinkAddress = adr.Get();
        /* setup cep server socket */
        if (m_socket == 0)
        {
          TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
          m_socket = Socket::CreateSocket (disnode, tid);
          InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (),
                                                       m_port);
          m_socket->Bind (local);
          
        }
        
        if(this->GetAggregateIterator().HasNext())
        {
            Ptr<Ipv4> ipv4 = GetObject<Dcep>()->GetNode()->GetObject<Ipv4> (); // Get Ipv4 instance of the node
            host_address = ipv4->GetAddress (1, 0).GetLocal (); // Get Ipv4InterfaceAddress of xth interface
        }
       
        m_socket->SetRecvCallback (MakeCallback (&Communication::HandleRead, this));
        
        
    }
    
    
    void
    Communication::HandleRead(Ptr<Socket> socket)
    {
        NS_LOG_FUNCTION (this << socket);
        Ptr<Packet> packet, pcopy;
        Ptr<Dcep> dcep = GetObject<Dcep>();
        
        Address from;
        
        while ((packet = socket->RecvFrom (from)))
          {
            if (packet->GetSize () > 0)
              {
                NS_LOG_INFO("RECEIVED PACKET SIZE " << packet->GetSize());
                /* headers must be removed first before fetching the data*/
                SeqTsHeader seqTs;
                DcepHeader dcepHeader;
                Ipv4Header ipv4;
                pcopy = packet;
             //   
                
                packet->RemoveHeader(seqTs);
                    
                packet->RemoveHeader(ipv4);
                packet->RemoveHeader(dcepHeader);
                
                Time delay = Simulator::Now() - seqTs.GetTs();
                
                if (InetSocketAddress::IsMatchingType (from))
                {
                       NS_LOG_INFO ("At time " << Simulator::Now ().GetMilliSeconds()
                       << "s packet of type " << dcepHeader.GetContentType()
                       << " from "
                       << InetSocketAddress::ConvertFrom(from).GetIpv4 ()
                           << " destination was "
                           << ipv4.GetDestination()
                           << "local address "
                           << this->host_address
                               << "packet size "
                               << packet->GetSize()
                           << "delay "
                           << delay.GetMilliSeconds()
                               );
                      
                       uint8_t *buffer = new uint8_t[dcepHeader.GetContentSize()];
                       packet->CopyData(buffer, packet->GetSize ());
                       dcep->rcvRemoteMsg(buffer,dcepHeader.GetContentSize(),dcepHeader.GetContentType(), delay.GetMilliSeconds());
                  
                }
               }   
          }
    }
    
    void 
    Communication::ScheduleSend(Ipv4Address peerAddress, const uint8_t *data,
            uint32_t size, uint16_t msg_type)
    {
        DcepHeader dcepHeader;
        dcepHeader.SetContentType(msg_type);
        dcepHeader.setContentSize(size);
        
        Ptr<Packet> p = Create<Packet> (data, size);
        
        p->AddHeader (dcepHeader);
        ScheduleSend(p, peerAddress);
        
    }
    
    
    void Communication::ScheduleSend(Ptr<Packet> p, Ipv4Address addr)
    {
        Ipv4Header ipv4;
        ipv4.SetDestination(addr);
        ipv4.SetProtocol(123);
        
        p->AddHeader(ipv4);
        NS_LOG_INFO("about to send data size " << p->GetSize());
        Ptr<ns3::QueueItem> p_item = Create<ns3::QueueItem>(p);
        m_sendQueue->Enqueue(p_item);
        
        Simulator::Schedule (Seconds (0.0), &Communication::send, this);
        
    }
    
    void
    Communication::send()
    {
        /*we send a copy of the packet item*/
        NS_LOG_INFO ("COMMUNICATION @ " << host_address << " number sent " << m_sent);
        if(m_sendQueue->GetNPackets() > 0)
        {
            Ptr<Packet> p = m_sendQueue->Peek()->GetPacket(); 
            Ptr<Packet> pp = p->Copy();

            SeqTsHeader sth;
            sth.SetSeq(m_sent);

            pp->AddHeader(sth);

            DcepHeader dcepHeader;
            Ipv4Header ipv4;

            pp->RemoveHeader(sth);
            pp->RemoveHeader(ipv4);
            pp->RemoveHeader(dcepHeader);

            pp->AddHeader(dcepHeader);
            pp->AddHeader(ipv4);
            pp->AddHeader(sth);

            NS_LOG_INFO (" " << sth.GetTs().GetMicroSeconds() << " " << ipv4.GetDestination() << " " << dcepHeader.GetContentType());

            bool itemSent = false;

            m_socket->Connect (InetSocketAddress (Ipv4Address::ConvertFrom(ipv4.GetDestination()), m_port));
            if ((m_socket->Send (pp)) >= 0)
            {

                NS_LOG_INFO ("SUCCESSFUL TX from : " << host_address
                        << "packet size "
                        << pp->GetSize());
                itemSent = true;
                m_sent++;
            }
            else
            {
              NS_LOG_INFO ("Error " << m_socket->GetErrno());

            }

            Ptr<QueueItem> item = m_sendQueue->Remove();

            if (!itemSent) //we push it back at the rear of the queue
            {
                NS_LOG_INFO ("COMMUNICATION: Rescheduling item!");
                m_sendQueue->Enqueue(item);
            }

            if(!m_sendQueue->IsEmpty())//only schedule when there are more packet to send
            {
                NS_LOG_INFO ("SCHEDULING TRANSMISSION");
                Simulator::Schedule (Seconds(1), &Communication::send, this);
            }
        }
        else
        {
            NS_LOG_INFO ("SOMETHING IS WRONG SEND SHOULD BE ALWAYS CALLED WHEN THERE IS SOMETHING TO SEND");
        }
    }
    
    
    Ipv4Address
    Communication::GetSinkAddress()
    {
        return m_sinkAddress;
    }
    
    Ipv4Address
    Communication::GetLocalAddress() {
        return host_address;
    }
    
  
}

