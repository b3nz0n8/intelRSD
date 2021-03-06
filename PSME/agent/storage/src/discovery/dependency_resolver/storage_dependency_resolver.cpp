/*!
 * @section LICENSE
 *
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
 * @section DESCRIPTION
 *
 * @file storage_dependency_resolver.cpp
 *
 * @brief ...
 * */
#include "discovery/dependency_resolver/storage_dependency_resolver.hpp"
#include "discovery/dependency_resolver/logical_drive_dependency_resolver.hpp"
#include "discovery/dependency_resolver/iscsi_target_dependency_resolver.hpp"

using namespace agent::storage::discovery;

void StorageDependencyResolver::resolve() {
    if (m_module.get_submodules().empty()) {
        log_error(GET_LOGGER("storage"), "Submodules empty!");
        return;
    }

    for (auto dependency_resolver : m_dependency_resolvers) {
        dependency_resolver->initialize();
        dependency_resolver->resolve();
    }
}
