/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "dcep-helper.h"
#include "ns3/dcep.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/string.h"
#include "ns3/node.h"
#include "ns3/node-list.h"
#include "ns3/object-factory.h"





namespace ns3 {

    NS_LOG_COMPONENT_DEFINE ("DcepHelper");
    
    DcepHelper::DcepHelper(){
        Initialize();
    }
    
    void
    DcepHelper::Initialize(){
        
    }
    
    
    void
    DcepHelper::Install (Ptr<Node> node) const{
        
        /*create the main dcep component on which the others are aggregated to.
         * this component is the one to be aggregated on the node object. 
        */
        ObjectFactory m_factory;
        m_factory.SetTypeId ("ns3::Dcep");
        Ptr<Application> m_dcep = m_factory.Create <Application> ();
       
        /*
         * Initialize the rest of the system
         */
        m_factory.SetTypeId ("ns3::Placement");
        Ptr<Object> c_placement = m_factory.Create <Object> ();
        m_dcep->AggregateObject (c_placement);
        //c_placement->AggregateObject(m_dcep);
        
        m_factory.SetTypeId ("ns3::CEPEngine");
        Ptr<Object> c_cepengine = m_factory.Create <Object> ();
        m_dcep->AggregateObject (c_cepengine);
        
        
        /*
         * the placement and cep engine will be working closely together
         * thus we connect them together using aggregation.
         
        
        c_cepengine->AggregateObject(c_placement);
        */
        
        
        m_factory.SetTypeId ("ns3::Communication");
        Ptr<Object> c_communication = m_factory.Create <Object> ();
        m_dcep->AggregateObject (c_communication);
        
        m_factory.SetTypeId ("ns3::Dispatcher");
        Ptr<Object> c_dispatcher = m_factory.Create <Object> ();
        m_dcep->AggregateObject (c_dispatcher);
        
        /*
         * configure pub-sub interaction between data sources, the sink and the
         * cep engine 
         */
        
        
        
        node->AddApplication (m_dcep);
        
        
    }
    
    /*
    void
    DcepHelper::CreateAndAggregateComponentFromTypeId (Ptr<Node> node, const std::string typeId)
    {
      ObjectFactory factory;
      factory.SetTypeId (typeId);
      Ptr<Object> component = factory.Create <Object> ();
      node->AggregateObject (component);
    }
    */
    
    void
    DcepHelper::Install (NodeContainer c) const{
            for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
            {
              Install (*i);
            }
    }


}

