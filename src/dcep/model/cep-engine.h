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

#ifndef CEP_ENGINE_H
#define CEP_ENGINE_H

#include "ns3/object.h"
#include "ns3/traced-callback.h"
#include "ns3/ipv4-address.h"
namespace ns3 {

    class Event;
    class EventPattern;
    class CepOperator;
    class SerializedEvent;
    class SerializedQuery;
    
    class Window : public Object{
    public:
        static TypeId GetTypeId (void);
        std::vector<Event> buffer;
        
    };
    
     class Event : public Object{
    public:
        static TypeId GetTypeId (void);
        
        Event(Ptr<Event>);
        Event();
        void operator=(Ptr<Event>);
        SerializedEvent* serialize();
        void deserialize(uint8_t*, uint32_t);
        uint32_t getSize();
        void CopyEvent (Ptr<Event> e);
        
        std::string type; //the type of the event
        uint64_t m_seq;
        uint64_t delay;
        uint32_t event_class;
        int32_t hopsCount;
        int32_t prevHopsCount;
    };
    
    class EventPattern : public Object{
        
    public:
        static TypeId GetTypeId (void);
        std::vector<std::string> eventTypes;
        
        std::string op;
       // std::string temporalConstraintValue;
      
    };
//    
    class Query : public Object
    {
        
    public:
        static TypeId GetTypeId (void);
        
        Query(Ptr<Query> q);
        Query();
        uint32_t id;
        uint32_t actionType;
        std::string eventType;
        bool isAtomic;
        Ipv4Address output_dest;
        Ipv4Address inputStream1_address;
        Ipv4Address inputStream2_address;
        Ipv4Address currentHost;
        std::string inevent1;
        std::string inevent2;
        std::string parent_output;
        std::string op;
        /*
         * the event notification for the event of type above is the
         * one the sink is interested in.
         */ 
        bool isFinal;
        bool assigned;
        
        SerializedQuery* serialize();
        void deserialize(uint8_t *buffer, uint32_t);
        uint32_t getSerializedSize();
         
    };
    
class CEPEngine : public Object
    {
    public:
        static TypeId GetTypeId (void);
        CEPEngine();
        void Configure();
        void ProcessCepEvent(Ptr<Event> e);
        void GetOpsByInputEventType(std::string eventType, std::vector<Ptr<CepOperator> >& ops);
        
        /**
         * this method instantiates the query and 
         * stores it in the query pool
         * @param 
         * the query to instantiate
         */
        void RecvQuery(Ptr<Query>);
        TracedCallback< Ptr<Event> > nevent;
        
        
private:
    friend class Detector;
   
    
    void ForwardProducedEvent(Ptr<Event>);
    void InstantiateQuery(Ptr<Query> q);
    void StoreQuery(Ptr<Query> q);
    Ptr<Query> GetQuery(uint32_t id);
    std::vector<Ptr<Query> > queryPool;
    std::vector<Ptr<CepOperator> > ops_queue;
      
    };
    class Forwarder  : public Object
    {
    public:
        static TypeId GetTypeId (void);
        Forwarder();
        void Configure();
    private:
        friend class Producer;
        virtual void ForwardNewEvent(Ptr<Event> new_event);
        TracedCallback< Ptr<Event> > new_event;
        
    };
    
    class Detector  : public Object
    {
    public:
        static TypeId GetTypeId (void);
        void ProcessEvent(Ptr<Event> e);
       
    };
    
    class BufferManager : public Object{
    public:
        static TypeId GetTypeId (void);
        
        void configure(Ptr<CepOperator> op);
        void read_events(std::vector<Ptr<Event> >& event1, 
        std::vector<Ptr<Event> >& event2);
        void put_event(Ptr<Event>);
        void clean_up();
        uint32_t consumption_policy;
        uint32_t selection_policy;
        std::vector<Ptr<Event> > events1;
        std::vector<Ptr<Event> > events2;
        
    private:
        friend class CepOperator;
          
    };
    
    class CepOperator: public Object {
    public:
        static TypeId GetTypeId ();
        
        virtual void Configure (Ptr<Query>) = 0;
        virtual bool Evaluate(Ptr<Event> e, std::vector<Ptr<Event> >&) = 0; 
        virtual bool ExpectingEvent (std::string) = 0;
        uint32_t queryId;
    };
    
    class AndOperator: public CepOperator {
    public:
        static TypeId GetTypeId ();
        
        void Configure (Ptr<Query>);
        bool Evaluate (Ptr<Event> e, std::vector<Ptr<Event> >&); 
        bool ExpectingEvent (std::string);
        std::string event1;
        std::string event2;
        
    private:
        uint32_t queryId;
        //std::string first;
        Ptr<BufferManager> bufman;
        
    };
    
    class OrOperator: public CepOperator {
    public:
        static TypeId GetTypeId ();
        
        void Configure (Ptr<Query>);
        bool Evaluate(Ptr<Event> e, std::vector<Ptr<Event> >&); 
        bool ExpectingEvent (std::string);
        std::string event1;
        std::string event2;
    
    private:
        uint32_t queryId;
        //std::string first;
        Ptr<BufferManager> bufman;
    };
    
    class Producer  : public Object
    {
    public:
        static TypeId GetTypeId (void);
        
    private:
        friend class Detector;
        void HandleNewEvent(Ptr<Query> q, std::vector<Ptr<Event> >);
        
    };
    
}

#endif /* CEP_ENGINE_H */

