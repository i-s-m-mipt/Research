#pragma once

#include <exception>
#include <stdexcept>
#include <string>

#include "tables.hpp"

namespace lua
{
    class Source
    {
    public:

        using interval_t = unsigned int;

        using index_t = unsigned int;

    public:

        struct Date_Time
        {
        public:

            using date_t = unsigned int;
            using time_t = unsigned int;

        public:

            date_t year   = 0U;
            date_t month  = 0U;
            date_t day    = 0U;

            time_t hour   = 0U;
            time_t minute = 0U;
            time_t second = 0U;
        };

    public:

        explicit Source(State state, const std::string & class_code, 
            const std::string & asset_code, interval_t interval) : 
                m_state(state), m_reference(LUA_NOREF)
        {
            initialize(class_code, asset_code, interval);
        }

        ~Source() noexcept = default;

    private:

        void initialize(const std::string & class_code,
            const std::string & asset_code, interval_t interval)
        {
            m_state.get_global(table_name);

            if (m_state.is_nil(-1))
            {
                m_state.pop();

                m_state.new_table();

                m_state.set_global(table_name);
                m_state.get_global(table_name);
            }

            m_state.call < 2 > ("CreateDataSource", class_code, asset_code, interval);

            m_state.push_value(-2);

            m_reference = luaL_ref(m_state.state(), -4); // !

            m_state.pop(2);
        }

    public:

        double price_open(index_t index) const
        {
            return call_candle_function("O", index);
        }

        double price_high(index_t index) const
        {
            return call_candle_function("H", index);
        }

        double price_low(index_t index) const
        {
            return call_candle_function("L", index);
        }

        double price_close(index_t index) const
        {
            return call_candle_function("C", index);
        }

        double volume(index_t index) const
        {
            return call_candle_function("V", index);
        }

    private:

        double call_candle_function(const std::string & name, index_t index) const
        {
            m_state.get_global(table_name);
            m_state.push_number(m_reference);
            m_state.raw_get(-2);
            m_state.push_string(name);
            m_state.raw_get(-2);
            m_state.push_value(-2);
            m_state.push_number(index);

            m_state.pcall(2, 1, 0);

            auto result = m_state.top < double > ().get();

            m_state.pop(3);

            return result;
        }

    public:

        auto time(index_t index) const 
        {
            m_state.get_global(table_name);
            m_state.push_number(m_reference);
            m_state.raw_get(-2);
            m_state.push_string("T");
            m_state.raw_get(-2);
            m_state.push_value(-2);
            m_state.push_number(index);
            m_state.pcall(2, 1, 0);
            auto t = m_state.top < table::Chart_Time > ().get();
            Date_Time result = { 
                t.year.get(), 
                t.month.get(), 
                t.day.get(), 
                t.hour.get(), 
                t.min.get(), 
                t.sec.get() };
            m_state.pop(3);
            return result;
        }

    public:

        auto size() const
        {
            return source_function_call < std::size_t > ("Size");
        }

        auto close() const
        {
            return source_function_call < bool > ("Close");
        }

    private:

        template < typename T >
        auto source_function_call(const std::string & name) const 
        {
            m_state.get_global(table_name);
            m_state.push_number(m_reference);
            m_state.raw_get(-2);
            m_state.push_string(name);
            m_state.raw_get(-2);
            m_state.push_value(-2);
            m_state.pcall(1, 1, 0);
            auto result = m_state.top < T > ().get();
            m_state.pop(3);
            return result;
        }

    private:

        static inline const std::string table_name = "source_references";

    private:

        State m_state;
        int m_reference;
    };

} // namespace lua