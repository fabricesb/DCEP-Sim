/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DCEP_HELPER_H
#define DCEP_HELPER_H


#include "ns3/node-container.h"
#include "ns3/ptr.h"
#include "ns3/object-factory.h"
#include "ns3/dcep.h"


namespace ns3 {

    
    class DcepHelper
    {
    public:
        DcepHelper(void);
        
        
        void Install (Ptr<Node> node) const;
        void Install (NodeContainer c) const;
        
    private:
        
        /*
         * do initialization stuff here
         */
        void Initialize(void);
        
       // static void CreateAndAggregateComponentFromTypeId(Ptr<Node> node,
        //const std::string typeId);
        
        
        
    };

}

#endif /* DCEP_HELPER_H */

