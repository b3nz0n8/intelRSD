/*!
 * @copyright
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file status/status_manager.hpp
 * @brief Module software status implementation
*/

#ifndef AGENT_STUBS_CHASSIS_STATUS_STATUS_MANAGER_HPP
#define AGENT_STUBS_CHASSIS_STATUS_STATUS_MANAGER_HPP

#include "agent-framework/status/status_manager.hpp"

/*! Agent namespace */
namespace agent {
/*! Chassis namespace */
namespace chassis {
/*! Status namespace */
namespace status {

using agent_framework::status::ModuleStatus;

/*! Status Manager implementation */
class StatusManager : public agent_framework::status::StatusManager {
public:
    /*! Default constructor */
    StatusManager();

    /*! Copy constructor */
    StatusManager(const StatusManager&) = default;

    /*! Assignment operator */
    StatusManager& operator=(const StatusManager&) = default;

    /*!
     * @brief Gets hardware status.
     * @return Module hardware status
     * */
    ModuleStatus::Status get_hw_status();

    /*!
     * @brief Gets software status.
     * @return Module software status
     * */
    ModuleStatus::Status get_sw_status();

    ~StatusManager();
};

}
}
}

#endif /* AGENT_STUBS_CHASSIS_STATUS_STATUS_MANAGER_HPP */

