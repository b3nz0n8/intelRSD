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
 * */

#ifndef APP_REST_MODEL_WATCHER_HPP
#define APP_REST_MODEL_WATCHER_HPP

#include "psme/rest-ref/model/handlers/generic_handler_deps.hpp"
#include "psme/rest-ref/model/handlers/root_handler.hpp"
#include "agent-framework/module-ref/requests/common/get_manager_collection.hpp"
#include <memory>
#include <thread>
#include <atomic>

namespace agent_framework {
namespace eventing {
    class EventData;
}
}

namespace psme {
namespace rest {
namespace model {

namespace handler {
class RootHandler;
}

class Watcher final {
public:
    explicit Watcher() {}
    ~Watcher();

    void start();
    void stop();
private:
    using Managers = agent_framework::model::attribute::Array<agent_framework::model::attribute::ManagerEntry>;
    using EventData = agent_framework::eventing::EventData;

    /*!
     * @brief infinite loop that reads agent events and triggers polling
     */
    void watch();
    /*!
     * @brief method called when event arrives from agent
     */
    void process_event(const EventData *const event);

    /*!
     * @brief Schedules next poling
     */
    void schedule_polling();

    /*!
     * @brief execute polling
     */
    void poll();

    std::thread m_thread{};
    volatile std::atomic<bool> m_running{false};
    std::chrono::time_point<std::chrono::system_clock> m_next_polling {};
    std::chrono::seconds m_interval {};

    psme::rest::model::handler::RootHandler m_root_handler {};
};

}
}
}

#endif
