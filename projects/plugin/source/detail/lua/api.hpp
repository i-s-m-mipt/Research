#ifndef SOLUTION_PLUGIN_DETAIL_LUA_API_HPP
#define SOLUTION_PLUGIN_DETAIL_LUA_API_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <exception>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "state.hpp"
#include "tables.hpp"

namespace solution
{
    namespace plugin
    {
        namespace detail
        {
            namespace lua
            {
                class api_exception : public std::exception
                {
                public:

                    explicit api_exception(const std::string & message) noexcept :
                        std::exception(message.c_str())
                    {}

                    explicit api_exception(const char * const message) noexcept :
                        std::exception(message)
                    {}

                    ~api_exception() noexcept = default;
                };

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

                    auto is_connected() const
                    {
                        return m_state.call < unsigned int > ("isConnected");
                    }

                    auto send_message(const std::string & message) const
                    {
                        return m_state.call < unsigned int > ("message", message);
                    }

                    auto get_portfolio(const std::string & client_id, const std::string & client_code) const
                    {
                        return m_state.call < tables::Portfolio > ("getPortfolioInfo", client_id, client_code);
                    }

                    auto get_security_info(const std::string & class_code, const std::string & asset_code) const
                    {
                        return m_state.call < tables::Asset > ("getSecurityInfo", class_code, asset_code);
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

        } // namespace detail

    } // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_DETAIL_LUA_API_HPP