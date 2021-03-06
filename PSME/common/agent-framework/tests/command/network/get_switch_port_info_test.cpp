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

#include "agent-framework/command/network/get_ethernet_switch_port_info.hpp"
#include "agent-framework/command/network/json/get_ethernet_switch_port_info.hpp"
#include "agent-framework/module-ref/constants/network.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::model;
using namespace agent_framework::command;
using namespace agent_framework::command::exception;

static constexpr enums::PortType TEST_TYPE
    { enums::PortType::Unknown };
static constexpr enums::LinkTechnology TEST_LINK_TECHNOLOGY
    { enums::LinkTechnology::Unknown };

class GetEthernetSwitchPortInfo : public network::GetEthernetSwitchPortInfo {
private:
    std::string m_port_uuid{};
public:
    GetEthernetSwitchPortInfo(const std::string& port_uuid) {
        m_port_uuid = port_uuid;
    }

    using network::GetEthernetSwitchPortInfo::execute;

    void execute(const Request& request, Response& response) {
        const auto& port_uuid = request.get_port();

        if (port_uuid != m_port_uuid) {
            throw exception::NotFound();
        }

        SwitchPort port{};
        port.set_port_identifier("Test Port Id");
        port.set_port_type(enums::PortType::Unknown);
        port.set_link_technology(enums::LinkTechnology::Unknown);
        port.set_link_speed_mbps(std::uint32_t(1000));
        port.set_max_speed_mbps(std::uint32_t(10000));
        port.set_port_width(std::uint32_t(1));
        port.set_frame_size(std::uint32_t(1));
        port.set_auto_sense(true);
        port.set_is_management_port(true);
        port.set_last_error_code(std::uint32_t(1));
        port.set_error_cleared(true);
        port.set_last_state_change_time("Test State Change Time");
        port.set_mac_address("Test Mac Address");
        port.set_operational_state(enums::OperationalState::Unknown);
        port.set_administrative_state(enums::AdministrativeState::Down);
        port.set_ipv4_address({"192.168.0.1", "255.255.255.0",
                enums::Ipv4AddressOrigin::Static,
                "192.168.0.254"});
        port.set_ipv6_address({"fe80::2e60:cff:fe5c:ca07", 10,
                enums::Ipv6AddressOrigin::Static,
                enums::Ipv6AddressState::Preferred});
        port.set_neighbor_info(attribute::NeighborInfo(
                "TestSwitchIdentifier", "TestPortIdentifier", "TestCableId"));
        port.set_vlan_enable(true);
        port.set_oem(attribute::Oem());
        response.set_port(port);
    }

    virtual ~GetEthernetSwitchPortInfo();
};

GetEthernetSwitchPortInfo::~GetEthernetSwitchPortInfo() { }

TEST(GetEthernetSwitchPortInfoTest, PositiveExecute) {
    network::json::GetEthernetSwitchPortInfo command_json;
    GetEthernetSwitchPortInfo* command = new GetEthernetSwitchPortInfo("Test UUID");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[literals::SwitchPort::PORT] = "Test UUID";

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[literals::SwitchPort::PORT_IDENTIFIER].isString());
    ASSERT_TRUE(result[literals::SwitchPort::LINK_TECHNOLOGY].isString());
    ASSERT_TRUE(result[literals::SwitchPort::PORT_TYPE].isString());
    ASSERT_TRUE(result[literals::SwitchPort::LINK_SPEED_MBPS].isUInt());
    ASSERT_TRUE(result[literals::SwitchPort::MAX_SPEED_MBPS].isUInt());
    ASSERT_TRUE(result[literals::SwitchPort::OPERATIONAL_STATE].isString());
    ASSERT_TRUE(result[literals::SwitchPort::ADMINISTRATIVE_STATE].isString());
    ASSERT_TRUE(result[literals::SwitchPort::PORT_WIDTH].isUInt());
    ASSERT_TRUE(result[literals::SwitchPort::FRAME_SIZE].isUInt());
    ASSERT_TRUE(result[literals::SwitchPort::AUTO_SENSE].isBool());
    ASSERT_TRUE(result[literals::SwitchPort::IS_MANAGEMENT_PORT].isBool());
    ASSERT_TRUE(result[literals::SwitchPort::LAST_ERROR_CODE].isUInt());
    ASSERT_TRUE(result[literals::SwitchPort::ERROR_CLEARED].isBool());
    ASSERT_TRUE(result[literals::SwitchPort::LAST_STATE_CHANGE_TIME].isString());
    ASSERT_TRUE(result[literals::SwitchPort::MAC_ADDRESS].isString());
    ASSERT_TRUE(result[literals::SwitchPort::IPV4_ADDRESS].isObject());
    ASSERT_TRUE(result[literals::SwitchPort::IPV6_ADDRESS].isObject());
    ASSERT_TRUE(result[literals::SwitchPort::NEIGHBOR_INFO].isObject());
    ASSERT_TRUE(result[literals::SwitchPort::VLAN_ENABLE].isBool());
    ASSERT_TRUE(result[literals::SwitchPort::OEM].isObject());

    ASSERT_EQ(result[literals::SwitchPort::PORT_IDENTIFIER].asString(), "Test Port Id");
    ASSERT_EQ(result[literals::SwitchPort::LINK_TECHNOLOGY].asString(),
                    TEST_LINK_TECHNOLOGY.to_string());
    ASSERT_EQ(result[literals::SwitchPort::PORT_TYPE].asString(), TEST_TYPE.to_string());
    ASSERT_EQ(result[literals::SwitchPort::LINK_SPEED_MBPS].asUInt(), 1000);
    ASSERT_EQ(result[literals::SwitchPort::MAX_SPEED_MBPS].asUInt(), 10000);
    ASSERT_EQ(result[literals::SwitchPort::OPERATIONAL_STATE].asString(), "Unknown");
    ASSERT_EQ(result[literals::SwitchPort::ADMINISTRATIVE_STATE].asString(), "Down");
    ASSERT_EQ(result[literals::SwitchPort::PORT_WIDTH].asUInt(), 1);
    ASSERT_EQ(result[literals::SwitchPort::FRAME_SIZE].asUInt(), 1);
    ASSERT_EQ(result[literals::SwitchPort::AUTO_SENSE].asBool(), true);
    ASSERT_EQ(result[literals::SwitchPort::IS_MANAGEMENT_PORT].asBool(), true);
    ASSERT_EQ(result[literals::SwitchPort::LAST_ERROR_CODE].asUInt(), 1);
    ASSERT_EQ(result[literals::SwitchPort::ERROR_CLEARED].asBool(), true);
    ASSERT_EQ(result[literals::SwitchPort::LAST_STATE_CHANGE_TIME].asString(),
                     "Test State Change Time");
    ASSERT_EQ(result[literals::SwitchPort::MAC_ADDRESS].asString(), "Test Mac Address");
    ASSERT_EQ(result[literals::SwitchPort::NEIGHBOR_INFO]
                    [literals::NeighborInfo::SWITCH_IDENTIFIER].asString(),
                    "TestSwitchIdentifier");
    ASSERT_EQ(result[literals::SwitchPort::NEIGHBOR_INFO]
                    [literals::NeighborInfo::PORT_IDENTIFIER].asString(),
                    "TestPortIdentifier");
    ASSERT_EQ(result[literals::SwitchPort::VLAN_ENABLE].asBool(), true);
}

TEST(GetEthernetSwitchPortInfoTest, NegativeComponentNotFound) {
    network::json::GetEthernetSwitchPortInfo command_json;
    GetEthernetSwitchPortInfo* command = new GetEthernetSwitchPortInfo("Test UUID");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[literals::SwitchPort::PORT] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51c";

    EXPECT_ANY_THROW(command_json.method(params, result));
}

TEST(GetEthernetSwitchPortInfoTest, NegativePortIdentifierNotFound) {
    network::json::GetEthernetSwitchPortInfo command_json;
    GetEthernetSwitchPortInfo* command = new GetEthernetSwitchPortInfo("Test UUID");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[literals::SwitchPort::PORT] = "8d2c1ac0-2f82-11e5-8333-00aabbccddee";

    EXPECT_ANY_THROW(command_json.method(params, result));
}
