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
#ifndef COMMON_HEADERS_H
#define COMMON_HEADERS_H


namespace ns3 {

#define MAX_ATTR_SIZE sizeof(uint8_t)
#define NOTIFICATION 1
    /**
     * how are event buffer emptied
     */
#define SELECTED_CONSUMPTION 2 
#define ZERO_CONSUMPTION 3

    /**
     * buffer selection policy
     */
#define MULTIPLE_SELECTION 4
#define SINGLE_SELECTION 5 
#define ACTIVATE 0
    
    enum OperatorState {
        ACTIVE,
        DEACTIVATED,
        FREEZED,
        RETRANSMITING,
        RESUMING,
        TENTATIVE,
        REMOTE_ACTIVE,
        UNDEFINED,
        OUTPUT_CLOSED
    };
    enum event_class {
        ATOMIC_EVENT,
        COMPOSITE_EVENT,
        FINAL_EVENT,
        SYSTEM_EVENT
    };
    
    enum message_types {
        EVENT = 1,
        QUERY
    };
    
    
}
#endif /* COMMON_HEADERS_H */

