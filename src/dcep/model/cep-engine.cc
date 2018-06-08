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

/*
 * Implementation of a CEP engine including models for:
 *  1. Event and Query models
 *  2. Detector and Producer models to process CEP events
 *  3. A forwarder model to forward composite events produced by the CEP engine
 */

#include "cep-engine.h"
#include "ns3/uinteger.h"
#include "ns3/names.h"
#include "ns3/log.h"
#include "ns3/config.h"
#include "placement.h"
#include "message-types.h"
#include "ns3/abort.h"
#include "ns3/placement.h"


namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(CEPEngine);
NS_LOG_COMPONENT_DEFINE ("Detector");

/**************** CEP CORE *******************
 * *************************************************
 * ***************************************************/
/* ... */
    TypeId
    CEPEngine::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::CEPEngine")
        .SetParent<Object> ()
        .AddConstructor<CEPEngine> ()
        .AddTraceSource ("Event",
                       "Final event.",
                       MakeTraceSourceAccessor (&CEPEngine::nevent))
        
        ;
        
        return tid;
    }
    
    
    CEPEngine::CEPEngine()
    {
        Ptr<Forwarder> forwarder = CreateObject<Forwarder>();
        Ptr<Detector> detector = CreateObject<Detector>();
        Ptr<Producer> producer = CreateObject<Producer>();
        AggregateObject(forwarder);
        AggregateObject(detector);
        AggregateObject(producer);
        
        
    }
    
    void
    CEPEngine::Configure()
    {
        GetObject<Forwarder>()->TraceConnectWithoutContext("new event", 
                MakeCallback(&CEPEngine::ForwardProducedEvent, this));
    }
    
    void
    CEPEngine::ForwardProducedEvent(Ptr<Event> e)
    {
        GetObject<Placement>()->ForwardProducedEvent(e);
    }
    
    void
    CEPEngine::ProcessCepEvent(Ptr<Event> e){
        
        GetObject<Detector>()->ProcessEvent(e);
    }
    
    
    void
    CEPEngine::GetOpsByInputEventType(std::string eventType, std::vector<Ptr<CepOperator>>& ops)
    {
        
        Ptr<CepOperator> op;
        
        std::vector<Ptr<CepOperator> >::iterator it;
        for( it = ops_queue.begin(); it != ops_queue.end(); ++it) 
        {

            op = (Ptr<CepOperator>) *it;
           // Ptr<EventPattern> ep = q->GetObject<EventPattern>();
            //Ptr<EventPattern> ep = q->ep;
            if(op->ExpectingEvent(eventType))
            {
                NS_LOG_INFO("found query expecting event type " << eventType);
                ops.push_back(op);
            }
        }
    }
    
    Ptr<Query> 
    CEPEngine::GetQuery(uint32_t id){
        
        Ptr<Query> q;
        
        std::vector<Ptr<Query> >::iterator it;
        for( it = queryPool.begin(); it != queryPool.end(); ++it) {

            q = (Ptr<Query>) *it;
            if(q->id == id)
            {
                return q;
            }
        }
        
        return NULL;
    }
    
    void
    CEPEngine::RecvQuery(Ptr<Query> q)
    {
        InstantiateQuery(q);
        StoreQuery(q);
    }
    
    void
    CEPEngine::InstantiateQuery(Ptr<Query> q){
        
        if(q->isAtomic)
        {
            /* instantiate atomic event */
        }
        else
        {
            Ptr<CepOperator> cepOp;
            if(q->op == "and")
            {
                cepOp = CreateObject<AndOperator>();
            }
            else if(q->op == "or")
            {
                cepOp = CreateObject<OrOperator>();
            }
            else
            {
                NS_ABORT_MSG ("UNKNOWN OPERATOR");
            }
            cepOp->Configure(q);
            this->ops_queue.push_back(cepOp);
        }
            
    }
    
    void
    CEPEngine::StoreQuery(Ptr<Query> q){
        queryPool.push_back(q);
    }
    
    
    /**************** DETECTOR ****************
     ***********************************************
     **********************************************/
    
    TypeId
    Detector::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::Detector")
        .SetParent<Object> ()
        .AddConstructor<Detector> ()
        ;
        
        return tid;
    }
    
    void
    Detector::ProcessEvent(Ptr<Event> e)
    {
        
        Ptr<CEPEngine> cep = GetObject<CEPEngine>();
        
        std::vector<Ptr<CepOperator>> ops;
        cep->GetOpsByInputEventType(e->type, ops);
        std::vector<Ptr<CepOperator>>::iterator it;
        for(it = ops.begin(); it != ops.end(); it++)
        {
            Ptr<CepOperator> op = (Ptr<CepOperator>) *it;
            
            bool proceed = false;
            std::vector<Ptr<Event> > returned;
            
            if(op->Evaluate(e, returned))
            {
                proceed = true;

            }
            
            if(proceed)
            {
                Ptr<Query> q = cep->GetQuery(op->queryId);
                
                Ptr<Producer> producer = GetObject<Producer>();
                producer->HandleNewEvent(q, returned);
            }
        }
        
    }
    
    
    TypeId
    CepOperator::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::CepOperator")
        .SetParent<Object> ()
        ;
        
        return tid;
    }
    
    TypeId
    AndOperator::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::AndOperator")
        .SetParent<CepOperator> ()
        ;
        
        return tid;
    }
    
    TypeId
    OrOperator::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::OrOperator")
        .SetParent<CepOperator> ()
        ;
        
        return tid;
    }
    
    void
    AndOperator::Configure(Ptr<Query> q)
    {
        this->queryId = q->id;
        this->event1 = q->inevent1;
        this->event2 = q->inevent2;
            
        Ptr<BufferManager> bufman = CreateObject<BufferManager>();
        
        bufman->consumption_policy = SELECTED_CONSUMPTION; //default
        bufman->selection_policy = SINGLE_SELECTION; //default
        bufman->configure(this);
        this->bufman = bufman; 
    }
    
    void
    OrOperator::Configure(Ptr<Query> q)
    {
        this->queryId = q->id;
        this->event1 = q->inevent1;
        this->event2 = q->inevent2;
            
        Ptr<BufferManager> bufman = CreateObject<BufferManager>();
        
        bufman->consumption_policy = SELECTED_CONSUMPTION; //default
        bufman->selection_policy = SINGLE_SELECTION; //default
        bufman->configure(this);
        this->bufman = bufman; 
    }
    
    bool
    AndOperator::Evaluate(Ptr<Event> e, std::vector<Ptr<Event> >& returned)
    {
        std::vector<Ptr<Event>> events1;
        std::vector<Ptr<Event>> events2;
        bufman->read_events(events1, events2);
        
        if((!events1.empty()) && (!events2.empty()))
        {
            if (e->type == events1.front()->type)
            {
                std::vector<Ptr<Event>>::iterator it = events2.begin();
                for (uint32_t i = 0; i < events2.size(); i++, it++)
                {
                     
                    if(e->m_seq == bufman->events2[i]->m_seq)
                    {
                        Ptr<Event> e1 = CreateObject<Event>();
                        Ptr<Event> e2 = CreateObject<Event>();
                        e->CopyEvent(e1);
                        events2[i]->CopyEvent(e2);
                        
                        bufman->events2.erase(it);
                        returned.push_back(e1);
                        returned.push_back(e2);
                        
                        
                        
                        return true;
                    }
                }
                
            }
            else
            {
                std::vector<Ptr<Event>>::iterator it = bufman->events1.begin();
                for (uint32_t i = 0; i < bufman->events1.size(); i++, it++)
                {
                    if(e->m_seq == bufman->events1[i]->m_seq)
                    {
                        Ptr<Event> e1 = CreateObject<Event>();
                        Ptr<Event> e2 = CreateObject<Event>();
                        e->CopyEvent(e1);
                        bufman->events1[i]->CopyEvent(e2);
                        
                        bufman->events1.erase(it);
                        returned.push_back(e1);
                        returned.push_back(e2);
                        return true;
                    }
                }
            }
            
        }
        bufman->put_event(e);//wait for event with corresponding sequence number
        return false;
    }
    
    bool
    OrOperator::Evaluate(Ptr<Event> e, std::vector<Ptr<Event> >& returned)
    {
        /* everything is a match*/
       returned.push_back(e);
        return true; 
    }
    
    bool
    AndOperator::ExpectingEvent(std::string eType)
    {
        if((event1 == eType) || (event2 == eType))
            return true;
        else 
            return false;
    }
    
    bool
    OrOperator::ExpectingEvent(std::string eType)
    {
        if((event1 == eType) || (event2 == eType))
            return true;
        else 
            return false;
    }
    
    
     
    /*********** BUFFER MANAGEMENT******************
     *****************************************************
     ************************************************************ */
    
    TypeId
    BufferManager::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::WindowManager")
        .SetParent<Object> ()
        .AddConstructor<CEPEngine> ()
        ;
        
        return tid;
    }
    
    void
    BufferManager::configure(Ptr<CepOperator> op)
    {
        /*
         * setup the buffers with their corresponding event types
         */
        
    }
    
    void
    BufferManager::read_events(std::vector<Ptr<Event> >& event1,
            std::vector<Ptr<Event> >& event2)
    {
        //apply selection policy
        switch(selection_policy)
        {
            case SINGLE_SELECTION:
                event1 = events1;
                event2 = events2;
                break;
                
            default:
                NS_LOG_INFO("Not applying any selection policy");
        }
        
    }
    
    void
    BufferManager::put_event(Ptr<Event> e)
    {
        if(events1.empty() && events2.empty())
        {
            events1.push_back(e);
        }
        else if ((events1.empty()) && (!events2.empty()))
        {
            if(events2.front()->type == e->type)
            {
                events2.push_back(e);
            }
            else
            {
                events1.push_back(e);
            }
        }
        else if ((!events1.empty()) && (events2.empty()))
        {
            if(events1.front()->type == e->type)
            {
                events1.push_back(e);
            }
            else
            {
                events2.push_back(e);
            }
        }
        else
        {
            NS_LOG_INFO("unknown type");
        }

    }
    
    void
    BufferManager::clean_up()
    {
        switch(consumption_policy)
        {
            case SELECTED_CONSUMPTION:
                NS_LOG_INFO("Applying consumption policy " << SELECTED_CONSUMPTION);
                events1.clear();
                events2.clear();
                break;
                
            default:
                NS_LOG_INFO("Not applying any consumption policy");
                
        }
        
    }
      
    /***************************PRODUCER **************
     * ***************************************************
     * *************************************************************/
    
    TypeId
    Producer::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::Producer")
        .SetParent<Object> ()
        .AddConstructor<Producer> ()
        ;
        
        return tid;
    }
    
    void
    Producer::HandleNewEvent(Ptr<Query> q, std::vector<Ptr<Event> > events){
        if(q->actionType == NOTIFICATION)
        {
            
            Ptr<Event> new_event = CreateObject<Event>();
            uint64_t delay = 0;
            uint32_t hops = 0;
            for(std::vector<Ptr<Event>>::iterator it = events.begin();
                    it != events.end(); it++)
            {
                Ptr<Event> e = *it;
                delay = std::max(delay, e->delay);
                hops = hops + e->hopsCount;
            }
            
            new_event->type = q->eventType;
            
            new_event->delay = delay; 
            new_event->hopsCount = hops;
            
            
            if(q->isFinal)
            {
                new_event->event_class = FINAL_EVENT;
            }
            else
            {
                new_event->event_class = COMPOSITE_EVENT;
            }
            
            new_event->m_seq = events.back()->m_seq;
            
            Ptr<Forwarder> forwarder = GetObject<Forwarder>();
            forwarder->ForwardNewEvent(new_event);
        }
    }

    
    
    /*************************** FORWARDER **************
     * ***************************************************
     * *************************************************************/
    
    TypeId
    Forwarder::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::Forwarder")
        .SetParent<Object> ()
        .AddConstructor<Forwarder> ()
        .AddTraceSource ("new event",
                       "a new event is produced by the CEP engine.",
                       MakeTraceSourceAccessor (&Forwarder::new_event))
        ;
        
        return tid;
    }
    
    Forwarder::Forwarder()
    {}
    
    void
    Forwarder::Configure()
    {}
    
    void
    Forwarder::ForwardNewEvent(Ptr<Event> event)
    {
        new_event(event);
    }
    
    
    TypeId
    Query::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::Query")
        .SetParent<Object> ()
        .AddConstructor<Query> ()
        ;
        
        return tid;
    }
    
    
    /*********** EVENT PATTERN IMPLEMENTATION ******************
     *****************************************************
     ************************************************************ */
    
    TypeId
    EventPattern::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::EventPattern")
        .SetParent<Object> ()
        .AddConstructor<EventPattern> ()
        ;
        return tid;
    }
    
    
    /************** EVENT **************
     * ***********************************************
     * *************************************************/
    TypeId
    Event::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::Event")
        .SetParent<Object> ()
        .AddConstructor<Event> ()
        ;
        
        return tid;
    }
    
    Event::Event(Ptr<Event> e)
    {
        type = e->type;
        event_class = e->event_class;
        delay = e->delay;
        hopsCount = e->hopsCount;
        e->m_seq = m_seq;
    }
    
    Event::Event()
    {}
    
    void 
    Event::operator=(Ptr<Event> e)
    {
        this->type = e->type;
        
    }
    uint32_t
    Event::getSize()
    { 
        return (type.size()+sizeof(int64_t)+sizeof(uint32_t));
    }
    
    SerializedEvent*
    Event::serialize()
    {
        
        SerializedEvent *message = new SerializedEvent();
        message->type = this->type;
        message->event_class = this->event_class;
        message->size = sizeof(SerializedEvent);
        message->delay = this->delay;
        message->hopsCount = this->hopsCount;
        message->prevHopsCount = this->prevHopsCount;
        message->m_seq = this->m_seq;
        NS_LOG_INFO("serialized type " << message->type);
        
        return message;
       
    }
    
    void
    Event::deserialize(uint8_t *buffer, uint32_t size)
    {
        SerializedEvent *message = new SerializedEvent;
        
        memcpy(message, buffer, size);
        this->type = message->type;
        this->delay = message->delay;
        this->m_seq = message->m_seq;
        this->hopsCount = message->hopsCount;
        this->prevHopsCount = message->prevHopsCount;
        this->event_class = message->event_class;
    }
    
    void
    Event::CopyEvent(Ptr<Event> e)
    {
        e->event_class = event_class;
        e->hopsCount = hopsCount;
        e->m_seq = m_seq;
        e->prevHopsCount = prevHopsCount;
        e->delay = delay;
    }
    
    
    
    /************** QUERY **************
     * ***********************************************
     * *************************************************/
    Query::Query()
    {
    }
    Query::Query(Ptr<Query> q)
    {
        this->actionType = q->actionType;
        this->eventType = q->eventType;
        this->id = q->id;
        this->isAtomic = q->isAtomic;
        this->isFinal = q->isFinal;
        this->output_dest = q->output_dest;
        this->assigned = q->assigned;
        this->currentHost = q->currentHost;
    }
    
    

    uint32_t 
    Query::getSerializedSize()
    {
        uint32_t size = eventType.size()+inevent1.size()+inevent2.size()+
                (sizeof(uint32_t)*3)+sizeof(bool);
        return size;
    }
    
    SerializedQuery*
    Query::serialize()
    {
        SerializedQuery * message = new SerializedQuery();
        
        
        output_dest.Serialize(message->output_dest);
        inputStream1_address.Serialize(message->inputStream1_address);
        inputStream2_address.Serialize(message->inputStream2_address);
        currentHost.Serialize(message->currentHost);
        message->actionType = this->actionType;
        message->eventType = this->eventType;
        message->q_id = this->id;
        message->isFinal = this->isFinal;
        message->isAtomic = this->isAtomic;
        message->inevent1 = this->inevent1;
        message->inevent2 = this->inevent2;
        message->op = this->op;
        message->assigned = this->assigned;
        
        message->parent_output = this->parent_output;
        message->size = sizeof(SerializedQuery);
        
        return message;
    }
    
    void
    Query::deserialize(uint8_t *buffer, uint32_t size)
    {
        NS_LOG_INFO ("1");
       SerializedQuery *message = new SerializedQuery();
        memcpy(message, buffer, size);
        NS_LOG_INFO("DESERIALIZED MESSAGE " << message->eventType);
        this->actionType = message->actionType;
        NS_LOG_INFO ("1");
        this->id = message->q_id;
        NS_LOG_INFO ("1");
        this->isFinal = message->isFinal;
        NS_LOG_INFO ("1");
        this->isAtomic = message->isAtomic;
        NS_LOG_INFO ("1");
        this->eventType = message->eventType;
        NS_LOG_INFO ("1");
        
        this->output_dest = Ipv4Address::Deserialize(message->output_dest);
        this->inputStream1_address = Ipv4Address::Deserialize(message->inputStream1_address);
        this->inputStream2_address = Ipv4Address::Deserialize(message->inputStream2_address);
        this->currentHost = Ipv4Address::Deserialize(message->currentHost);
        NS_LOG_INFO ("1");
        this->inevent1 = message->inevent1;
        this->inevent2 = message->inevent2;
        this->parent_output = message->parent_output;
        NS_LOG_INFO ("1");
        this->op = message->op;
        this->assigned = message->assigned;
        NS_LOG_INFO ("1");
    }
    
}
