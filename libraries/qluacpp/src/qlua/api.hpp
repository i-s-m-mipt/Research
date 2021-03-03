#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include <luacpp/luacpp>

#include "date_time.hpp"
#include "source.hpp"
#include "tables.hpp"

namespace lua 
{
    class API 
    {
    public:

        explicit API(State state) : m_state(state)
        {}

        ~API() noexcept = default;

    public:

        const auto state() const noexcept
        {
            return m_state;
        }

    public:

        template < typename T >
        auto constant(const std::string & name) const 
        {
            m_state.get_global(name.c_str());

            auto result = m_state.top < T > ().get();

            m_state.pop();

            return result;
        }

    public:

        auto is_connected() const
        {
            return std::get < 0 > (m_state.call < std::tuple < unsigned int > > ("isConnected"));
        }

        auto send_message(const char * text) const
        {
            return std::get < 0 > (m_state.call < std::tuple < unsigned int > > ("message", text));
        }

        auto get_security_info(const char * class_code, const char * asset_code) const
        {
            return std::get < 0 > (m_state.call < std::tuple < unsigned int > > (
                "getSecurityInfo", class_code, asset_code));
        }

        auto send_transaction(const std::unordered_map < std::string, std::string > & transaction) 
        {
            m_state.get_global("sendTransaction");

            m_state.new_table();

            for (const auto & [field, value] : transaction) 
            {
                m_state.push_string(field.c_str());
                m_state.push_string(value.c_str());

                m_state.set_table(-3);
            }

            m_state.call(1, 1);

            return m_state.top < const char * > ().get();
        }

        auto get_portfolio(const char * client_id, const char * client_code) const
        {
            return std::get < 0 > (m_state.call < std::tuple < qlua::table::portfolio_info_getPortfolioInfo > > (
                "getPortfolioInfo", client_id, client_code));
        }

        auto create_source(const char * class_code, const char * asset_code,
            unsigned int interval) const 
        {
            return qlua::data_source(m_state, class_code, asset_code, interval);
        }

    private:

        State m_state;
    };

} // namespace lua