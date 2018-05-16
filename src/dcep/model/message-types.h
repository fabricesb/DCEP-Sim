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

#ifndef MESSAGE_H
#define MESSAGE_H

#include "ns3/object-factory.h"
#include "common.h"
#include <stdint.h>


namespace ns3
{
    
    class DcepMessage
    {
    public:
        uint32_t size;
        uint16_t msg_type;
    };
    
    class SerializedQuery: public DcepMessage
    {
    public: 
        
        uint32_t q_id;
       // char event1[MAX_ATTR_SIZE];
       // char event2[MAX_ATTR_SIZE];
        std::string eventType;
        //char op[MAX_ATTR_SIZE];
        bool isFinal;
        bool isAtomic;
        uint32_t actionType;
        uint8_t output_dest[4];
        uint8_t inputStream1_address[4];
        uint8_t inputStream2_address[4];
        uint8_t currentHost[4];
        std::string inevent1;
        std::string inevent2;
        std::string parent_output;
        std::string op;
        bool assigned;
    };
    
    class SerializedEvent: public DcepMessage
    {
    public:
        std::string type;
        uint32_t event_class;
        uint64_t delay;
        uint64_t m_seq;
        uint32_t hopsCount;
        uint32_t prevHopsCount;
        
    };
    
    
}
#endif /* MESSAGE_H */

