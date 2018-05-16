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

#include "dcep.h"
#include "ns3/node.h"
#include "ns3/uinteger.h"
#include "ns3/object.h"
#include "ns3/log.h"
#include "ns3/random-variable-stream.h"
#include "placement.h"
#include "communication.h"
#include "cep-engine.h"
#include "common.h"
#include "ns3/simulator.h"
#include "ns3/config.h"
#include "ns3/string.h"
#include "src/core/model/object-base.h"

#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(Dcep);
NS_LOG_COMPONENT_DEFINE ("Dcep");


    TypeId
    Dcep::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::Dcep")
        .SetParent<Application> ()
        .SetGroupName("Applications")
        .AddConstructor<Dcep> ()
        .AddAttribute ("placement policy", "The placement mechanism policy being used",
                        StringValue("centralized"),
                        MakeStringAccessor (&Dcep::placementPolicy),
                        MakeStringChecker())
        .AddAttribute ("IsGenerator",
                       "This attribute is used to configure the current node as a "
                        "datasource",
                       BooleanValue (false),
                       MakeBooleanAccessor (&Dcep::datasource_node),
                       MakeBooleanChecker ())
        .AddAttribute ("IsSink",
                       "This attribute is used to configure the current node as a "
                        "Sink",
                       BooleanValue (false),
                       MakeBooleanAccessor (&Dcep::sink_node),
                       MakeBooleanChecker ())
        .AddAttribute ("SinkAddress",
                       "The destination Address of the outbound packets",
                       Ipv4AddressValue (),
                       MakeIpv4AddressAccessor (&Dcep::m_sinkAddress),
                       MakeIpv4AddressChecker ())
        .AddAttribute ("event code", "The descriptor for the type of event generated"
        "by a datasource",
                      UintegerValue (0),
                      MakeUintegerAccessor (&Dcep::event_code),
                      MakeUintegerChecker<uint16_t> ())
        .AddAttribute ("number of events", "The number of events to be generated"
        "by a datasource",
                      UintegerValue (0),
                      MakeUintegerAccessor (&Dcep::events_load),
                      MakeUintegerChecker<uint32_t> ())
        .AddAttribute ("routing protocol", "The routing protocol being used",
                        StringValue("olsr"),
                        MakeStringAccessor (&Dcep::routing_protocol),
                        MakeStringChecker())
        .AddTraceSource ("RxFinalEvent",
                       "a new final event has been detected.",
                       MakeTraceSourceAccessor (&Dcep::RxFinalEvent))
        .AddTraceSource ("RxFinalEventHops",
                       "",
                       MakeTraceSourceAccessor (&Dcep::RxFinalEventHops))
        .AddTraceSource ("RxFinalEventDelay",
                       "",
                       MakeTraceSourceAccessor (&Dcep::RxFinalEventDelay))
        
        ;
        
        return tid;
    }
    
     
    Dcep::Dcep()
    {
        
    }
    
    uint16_t
    Dcep::getEventCode()
    {
        return this->event_code;
    }
    
    void
    Dcep::StartApplication (void)
    {
        
        
        NS_LOG_FUNCTION (this);
        NS_LOG_INFO("starting dcep application");
        
        
        Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();
       uint32_t mrand = x->GetInteger (1,30);
       mrand = mrand;
       std::cout << "generated number: " << mrand << std::endl;

        Ptr<Sink> sink = CreateObject<Sink>();
        Ptr<DataSource> datasource = CreateObject<DataSource>();

        AggregateObject (sink);
        AggregateObject (datasource);
        
        Ptr<Placement> c_placement = CreateObject<Placement> ();
        
        AggregateObject (c_placement);
       
        Ptr<CEPEngine> c_cepengine = CreateObject<CEPEngine> ();
        AggregateObject (c_cepengine);
        
        Ptr<Communication> c_communication = CreateObject<Communication> ();
        AggregateObject (c_communication);
        
        c_placement->configure();
        c_cepengine->Configure();
        datasource->Configure();
        
        c_communication->setNode(GetNode());
        c_communication->setPort(m_cepPort);
        c_communication->SetAttribute("SinkAddress", Ipv4AddressValue (m_sinkAddress));
        c_communication->Configure();
        
        
        if(sink_node)
        {
            Simulator::Schedule (Seconds (150.0), &Sink::BuildAndSendQuery, sink);
        }
          
    }
    
    uint32_t
    Dcep::getNumEvents()
    {
        return this->events_load;
    }
    
    bool
    Dcep::isGenerator()
    {
        return this->datasource_node;
    }
    
    void
    Dcep::StopApplication (void)
    {
        
        NS_LOG_FUNCTION (this);
       
    }
    
    void
    Dcep::SendPacket(Ptr<Packet> p, Ipv4Address addr)
    {
        NS_LOG_INFO ("DCEP: Sending packet to destination " << addr);
        GetObject<Communication>()->ScheduleSend(p, addr);
    }
    
    void
    Dcep::DispatchQuery(Ptr<Query> q)
    {
        NS_LOG_FUNCTION(this);
        NS_LOG_INFO("DCEP: received query to dispatch");
        
        GetObject<Placement>()->RecvQuery(q);
    }
    
    void
    Dcep::DispatchAtomicEvent(Ptr<Event> e)
    {
        NS_LOG_INFO ("DCEP: Dispatching atomic event");
        GetObject<Placement>()->ForwardProducedEvent(e);
    }
    
    void
    Dcep::ActivateDatasource(Ptr<Query> q)
    {
        GetObject<DataSource>()->GenerateAtomicEvents();
    }
    
    void 
    Dcep::SendFinalEventToSink(Ptr<Event> event)
    {
        this->RxFinalEvent(1);
        this->RxFinalEventDelay(event->delay);
        this->RxFinalEventHops(event->hopsCount);
        
        GetObject<Sink>()->receiveFinalEvent(event);
    }
    
    void
    Dcep::sendRemoteMsg(const uint8_t *data, uint32_t size, Ipv4Address dst, uint16_t msg_type)
    {
        NS_LOG_INFO ("DCEP: SENDING REMOTE MSG");
        Ptr<Communication> c_communication = GetObject<Communication>();    
        c_communication->ScheduleSend(dst,data,size,msg_type);
    }
    
    void
    Dcep::rcvRemoteMsg(uint8_t* data, uint32_t size, uint16_t msg_type, uint64_t delay)
    {
        
        Ptr<Placement> p = GetObject<Placement>();
        
        switch(msg_type)
        {
            case EVENT: /*handle event*/
            {
                NS_LOG_INFO ("DCEP: RECEIVED EVENT MESSAGE");   
                Ptr<Event> event = CreateObject<Event>();
                
                event->deserialize(data, size);
                /* setting link delay from source to this node*/
                event->delay = delay;
                
                p->RcvCepEvent(event);
                break; 
            }
                
                
            case QUERY: /* handle query*/
            {
                Ptr<Query> q = CreateObject<Query>();
                q->deserialize(data, size);
                p->RecvQuery(q);
                break;
            }
                
            default:
                NS_LOG_INFO("dcep: unrecognized remote message");
                
        }
    }
    
    
    /*
     * #######################################################################
     * ####################### SINK ##########################################
     */
    
    TypeId
    Sink::GetTypeId (void)
    {
      static TypeId tid = TypeId ("ns3::Sink")
        .SetParent<Object> ()
        .AddConstructor<Sink> ()
        .AddTraceSource("new-query",
              "A newly generated query",
              MakeTraceSourceAccessor (&Sink::nquery),
              ""
              )
      
      ;
      return tid;
    }

    Sink::Sink ()
    {
      NS_LOG_FUNCTION (this);
      

    }

    Sink::~Sink ()
    {
      NS_LOG_FUNCTION (this);
    }
    
    void
    Sink::receiveFinalEvent(Ptr<Event> e)
    {
        std::cout << "COMPLEX EVENT NOTIFIED HOPSCOUNT " << e->hopsCount << " DELAY " << e->delay <<  std::endl;
        NS_LOG_INFO("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ COMPLEX EVENT $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ");
        
    }

    void
    Sink::BuildAndSendQuery(){

       uint32_t query_counter = 1;

        /**
         * create and configure the query
         * 
         */
       Ptr<Dcep> dcep = GetObject<Dcep> ();
       
       Ptr<Query> q1 = CreateObject<Query> ();
       
        q1->actionType = NOTIFICATION;

        q1->id = query_counter++;

        q1->isFinal = false;
        q1->isAtomic = true;
        q1->eventType = "A";
        q1->output_dest = Ipv4Address::GetAny();
        q1->inevent1 = "A";
        q1->inevent2 = "";
        q1->op = "true";
        q1->assigned = false;
        q1->currentHost.Set("0.0.0.0");
        q1->parent_output = "AorB";
        NS_LOG_INFO ("Setup query " << q1->eventType);
        dcep->DispatchQuery(q1);
        
        /***************************************************************/
        
        Ptr<Query> q2 = CreateObject<Query> ();
        q2->actionType = NOTIFICATION;

        q2->id = query_counter++;

        q2->isFinal = false;
        q2->isAtomic = true;
        q2->eventType = "B";
        q2->output_dest = Ipv4Address::GetAny();
        q2->inevent1 = "B";
        q2->inevent2 = "";
        q2->op = "true";
        q2->assigned = false;
        q2->currentHost.Set("0.0.0.0");
        q2->parent_output = "AorB";
        NS_LOG_INFO ("Setup query " << q2->eventType);
        dcep->DispatchQuery(q2);
        
        Ptr<Query> q3 = CreateObject<Query> ();
        q3->actionType = NOTIFICATION;

        q3->id = query_counter++;

        q3->isFinal = true;
        q3->isAtomic = false;
        q3->eventType = "AorB";
        q3->output_dest = Ipv4Address::GetAny();
        q3->inevent1 = "A";
        q3->inevent2 = "B";
        q3->op = "or";
        q3->assigned = false;
        q3->currentHost.Set("0.0.0.0");
        NS_LOG_INFO ("Setup query " << q3->eventType);
        dcep->DispatchQuery(q3);
        

    }
    
    
    
    /*
     * ########################################################
     * ####################### DATASOURCE #########################
     */
    
    
    
    TypeId
    DataSource::GetTypeId (void)
    {
      static TypeId tid = TypeId ("ns3::Datasource")
        .SetParent<Object> ()
        .AddConstructor<DataSource> ()
        .AddTraceSource ("Event",
                       "New CEP event from data source.",
                       MakeTraceSourceAccessor (&DataSource::nevent))
      
      ;
      return tid;
    }

    DataSource::DataSource ()
    : counter(0) 
    {
      NS_LOG_FUNCTION (this);
      
    }
    
    void
    DataSource::Configure()
    {
      Ptr<Dcep> dcep = GetObject<Dcep>();
      UintegerValue ecode, nevents;
      
      dcep->GetAttribute("event code", ecode);
      dcep->GetAttribute("number of events", nevents);
      eventCode = ecode.Get();
      numEvents = nevents.Get();
    }

    DataSource::~DataSource ()
    {
      NS_LOG_FUNCTION (this);
    }

    void
    DataSource::GenerateAtomicEvents(){
        
            Ptr<Dcep> dcep = GetObject<Dcep>();
            
            NS_LOG_INFO ("Starting to generate events of type " << m_eventType );
            
            
            switch(eventCode)
            {
                case 1:
                    m_eventType = "A";
                    break;
                case 2:
                    m_eventType = "B";
                    break;
                case 3:
                    m_eventType = "C";
                    break;
                case 4:
                    m_eventType = "D";
                    break;
                case 5: 
                    m_eventType = "E";
                    break;
                case 6:
                    m_eventType = "F";
                    break;
                case 7:
                    m_eventType = "G";
                    break;
                case 8:
                    m_eventType = "H";
                    break;
                default:
                    m_eventType = " ";
                     
            }
            
            if(m_eventType != " ")
            {
               counter++;
                Ptr<Event> e = CreateObject<Event>();
                NS_LOG_INFO("creating event of type " << m_eventType);
                e->type = m_eventType;
                e->event_class = ATOMIC_EVENT;
                e->delay = 0; //initializing delay
                e->m_seq = counter;
                e->hopsCount = 0;
                e->prevHopsCount = 0;
                NS_LOG_INFO("Event number  " << e->m_seq);
                dcep->DispatchAtomicEvent(e);
                
                NS_LOG_INFO("counter " << counter);
                if(counter < numEvents)
                {
                    Simulator::Schedule (MilliSeconds (100), &DataSource::GenerateAtomicEvents, this);
                }
                    
              
            }
            
    }
    
}
