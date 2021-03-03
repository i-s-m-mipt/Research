#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include <luacpp/luacpp>

#include "source.hpp"
#include "tables.hpp"

namespace lua 
{
    class API 
    {
    public:

        using interval_t = Source::interval_t;

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

        auto constant(const std::string & name) const 
        {
            m_state.get_global(name);

            auto result = m_state.top < interval_t > ().get();

            m_state.pop();

            return result;
        }

    public:

        auto is_connected() const
        {
            return m_state.call < unsigned int > ("isConnected");
        }

        auto send_message(const std::string & message) const
        {
            return m_state.call < unsigned int > ("message", message);
        }

        auto create_source(const std::string & class_code, const std::string & asset_code,
            interval_t interval) const
        {
            return lua::Source(m_state, class_code, asset_code, interval);
        }

        auto get_portfolio(const std::string & client_id, const std::string & client_code) const
        {
            return m_state.call < lua::table::Portfolio > ("getPortfolioInfo", client_id, client_code);
        }

        auto get_security_info(const std::string & class_code, const std::string & asset_code) const
        {
            return m_state.call < unsigned int > ("getSecurityInfo", class_code, asset_code);
        }

        auto send_transaction(const std::unordered_map < std::string, std::string > & transaction) const
        {
            m_state.get_global("sendTransaction");

            m_state.new_table();

            for (const auto & [field, value] : transaction) 
            {
                m_state.push_string(field);
                m_state.push_string(value);

                m_state.set_table(-3);
            }

            m_state.call(1, 1);

            return m_state.top < std::string > ().get();
        }

    private:

        State m_state;
    };

} // namespace lua