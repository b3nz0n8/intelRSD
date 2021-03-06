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
 * */

#include "agent-framework/command/compute/get_storage_controller_info.hpp"
#include "agent-framework/command/compute/json/get_storage_controller_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;
using namespace agent_framework::model;
using namespace agent_framework::model::literals;

static constexpr enums::DriveInterface TEST_INTERFACE{enums::DriveInterface::PCIe};
static constexpr char TEST_SERIAL[] = "TestSerialNumber";
static constexpr char TEST_MANUFACTURER[] = "TestManufacturer";
static constexpr char TEST_MODEL[] = "TestModelNumber";
static constexpr char TEST_PART[] = "TestPartNumber";

class GetStorageControllerInfo : public compute::GetStorageControllerInfo {
private:
    std::string m_controller{};

public:
    GetStorageControllerInfo(
        const std::string& controller) {
        m_controller = controller;
        }

    using compute::GetStorageControllerInfo::execute;

    void execute(const Request& request, Response& response) {
        const auto& controller_uuid = request.get_controller();

        if (controller_uuid != m_controller) {
            throw exception::NotFound();
        }

        agent_framework::model::StorageController storage_controller{};
        storage_controller.set_interface(TEST_INTERFACE);
        storage_controller.set_status(agent_framework::model::attribute::Status(enums::State::InTest, enums::Health::Warning));
        storage_controller.set_fru_info(agent_framework::model::attribute::FruInfo(
          TEST_SERIAL, TEST_MANUFACTURER, TEST_MODEL, TEST_PART));
        storage_controller.set_oem(agent_framework::model::attribute::Oem());
        response.set_controller(std::move(storage_controller));
    }

    virtual ~GetStorageControllerInfo();
};

GetStorageControllerInfo::~GetStorageControllerInfo() { }

class GetStorageControllerInfoTest : public ::testing::Test {
public:
    virtual ~GetStorageControllerInfoTest();
};

GetStorageControllerInfoTest::~GetStorageControllerInfoTest() { }

TEST_F(GetStorageControllerInfoTest, PositiveExecute) {
    compute::json::GetStorageControllerInfo command_json;
    GetStorageControllerInfo* command = new GetStorageControllerInfo("TestControllerId");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[literals::StorageController::CONTROLLER] = "TestControllerId";

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[literals::StorageController::INTERFACE].isString());
    ASSERT_TRUE(result[Status::STATUS].isObject());
    ASSERT_TRUE(result[Status::STATUS][Status::STATE].isString());
    ASSERT_TRUE(result[Status::STATUS][Status::HEALTH].isString());
    ASSERT_TRUE(result[FruInfo::FRU_INFO].isObject());
    ASSERT_TRUE(result[FruInfo::FRU_INFO][FruInfo::SERIAL].isString());
    ASSERT_TRUE(result[FruInfo::FRU_INFO][FruInfo::MANUFACTURER].isString());
    ASSERT_TRUE(result[FruInfo::FRU_INFO][FruInfo::MODEL].isString());
    ASSERT_TRUE(result[FruInfo::FRU_INFO][FruInfo::PART].isString());
    ASSERT_EQ(result[literals::StorageController::INTERFACE].asString(), TEST_INTERFACE.to_string());
    ASSERT_EQ(result[Status::STATUS][Status::STATE], "InTest");
    ASSERT_EQ(result[Status::STATUS][Status::HEALTH], "Warning");
    ASSERT_EQ(result[FruInfo::FRU_INFO][FruInfo::SERIAL].asString(), TEST_SERIAL);
    ASSERT_EQ(result[FruInfo::FRU_INFO][FruInfo::MANUFACTURER].asString(), TEST_MANUFACTURER);
    ASSERT_EQ(result[FruInfo::FRU_INFO][FruInfo::MODEL].asString(), TEST_MODEL);
    ASSERT_EQ(result[FruInfo::FRU_INFO][FruInfo::PART].asString(), TEST_PART);
}

TEST_F(GetStorageControllerInfoTest, NegativeModuleNotFound) {
    compute::json::GetStorageControllerInfo command_json;
    GetStorageControllerInfo* command = new GetStorageControllerInfo("TestControllerId");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[literals::StorageController::CONTROLLER] = "OtherTestControllerId";

    EXPECT_THROW(command_json.method(params, result), exception::NotFound);
}
