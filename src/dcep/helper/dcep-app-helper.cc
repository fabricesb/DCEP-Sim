/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "dcep-app-helper.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/names.h"
#include "ns3/dcep.h"

namespace ns3 {

    
    
    
    
    DcepAppHelper::DcepAppHelper ()
    {
        m_factory.SetTypeId (Dcep::GetTypeId ());
    }
    void
    DcepAppHelper::SetAttribute (std::string name, const AttributeValue &value)
    {
      m_factory.Set (name, value);
    }

    ApplicationContainer
    DcepAppHelper::Install (NodeContainer c) const
    {
      ApplicationContainer apps;
      for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
        {
          Ptr<Node> node = *i;

          Ptr<Dcep> dcep = m_factory.Create<Dcep> ();
          node->AddApplication (dcep);
          apps.Add (dcep);

        }
      return apps;
    }
    
    Ptr<Application>
    DcepAppHelper::InstallPriv (Ptr<Node> node) const
    {
      Ptr<Application> app = m_factory.Create<Dcep> ();
      node->AddApplication (app);

      return app;
    }
    
    ApplicationContainer
    DcepAppHelper::Install (Ptr<Node> node) const
    {
      return ApplicationContainer (InstallPriv (node));
    }
}