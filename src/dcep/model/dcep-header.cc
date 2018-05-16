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


#include "dcep-header.h"

namespace ns3 {

    DcepHeader::DcepHeader ():
    m_type (0)
    {
      // we must provide a public default constructor, 
      // implicit or explicit, but never private.
        qt = "";
    }
    DcepHeader::~DcepHeader ()
    {
        
    }

    TypeId
    DcepHeader::GetTypeId (void)
    {
      static TypeId tid = TypeId ("ns3::DcepHeader")
        .SetParent<Header> ()
        .AddConstructor<DcepHeader> ()
      ;
      return tid;
    }
    TypeId
    DcepHeader::GetInstanceTypeId (void) const
    {
      return GetTypeId ();
    }

    void
    DcepHeader::Print (std::ostream &os) const
    {
      // This method is invoked by the packet printing
      // routines to print the content of my header.
      //os << "data=" << m_data << std::endl;
      os << "content type = " << m_type;
    }
    uint32_t
    DcepHeader::GetSerializedSize (void) const
    {
      // we reserve 2 bytes for our header.
      return (sizeof(uint32_t)+sizeof(uint16_t));
    }
    void
    DcepHeader::Serialize (Buffer::Iterator start) const
    {
      // we can serialize two bytes at the start of the buffer.
      // we write them in network byte order.
      start.WriteHtonU16 (m_type);
      start.WriteHtonU32(size);
    }
    uint32_t
    DcepHeader::Deserialize (Buffer::Iterator start)
    {
      // we can deserialize two bytes from the start of the buffer.
      // we read them in network byte order and store them
      // in host byte order.
      m_type = start.ReadNtohU16 ();
      size = start.ReadNtohU32();
      // we return the number of bytes effectively read.
      return (sizeof(uint32_t)+sizeof(uint16_t));
    }
    
    void 
    DcepHeader::SetContentType (uint16_t data)
    {
      m_type = data;
    }
    uint16_t 
    DcepHeader::GetContentType (void) const
    {
      return m_type;
    }
    
    void
    DcepHeader::setContentSize(std::size_t s)
    {
        size = s + qt.size();
    }
    
    std::size_t
    DcepHeader::GetContentSize() const
    {
        return size;
    }
    
    
}