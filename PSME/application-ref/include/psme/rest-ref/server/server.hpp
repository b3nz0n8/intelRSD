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

#ifndef PSME_REST_SERVER_SERVER_HPP
#define PSME_REST_SERVER_SERVER_HPP

#include "request.hpp"
#include "response.hpp"

namespace psme {
namespace rest {
namespace server {

class Server {
public:
    virtual ~Server();

    virtual void handle(Response& res, Request& req) = 0;
};

}
}
}

#endif /* PSME_REST_SERVER_SERVER_HPP */
