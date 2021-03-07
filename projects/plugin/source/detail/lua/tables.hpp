#pragma once

#include <exception>
#include <stdexcept>
#include <string>

#include "state.hpp"
#include "entity.hpp"
#include "vector.hpp"
#include "types/boolean.hpp"
#include "types/integer.hpp"
#include "types/real.hpp"
#include "types/string.hpp"
#include "types/pointer.hpp"

#define LUA_TABLE_BEGIN(NAME) \
class NAME \
{ \
private: \
\
    lua::State m_state; \
    int   m_index; \
\
public: \
\
    explicit NAME(lua::State state, int index) : \
        m_state(state), m_index(index) \
    {} \
\
    ~##NAME() noexcept = default; \
\
public: \
\
    void create() const \
    { \
        m_state.new_table(); \
    } \
\
    bool is_nil() const \
    { \
        return m_state.is_nil(m_index); \
    } \

#define LUA_MAKE_TABLE_FIELD_KEY(NAME) #NAME

#define LUA_TABLE_FIELD_KEY_VALUE(NAME, KEY_TYPE, VALUE_TYPE) \
class NAME##_Table_Field : public lua::types::Table_Field < KEY_TYPE, VALUE_TYPE > \
{ \
public: \
\
    using base_t = lua::types::Table_Field < KEY_TYPE, VALUE_TYPE > ; \
\
public: \
\
    static auto get(lua::State state, int index) \
    { \
        return base_t::get(state, index, LUA_MAKE_TABLE_FIELD_KEY(NAME)); \
    } \
\
static void set(lua::State state, int index, VALUE_TYPE value) \
{ \
    base_t::set(state, index, value, LUA_MAKE_TABLE_FIELD_KEY(NAME)); \
} \
\
    template < typename F > \
    static void apply(lua::State state, int index, F function) \
    { \
        base_t::apply(state, index, function, LUA_MAKE_TABLE_FIELD_KEY(NAME)); \
    } \
}; \
\
lua::Entity < NAME##_Table_Field > NAME{ m_state, m_index }; \

#define LUA_TABLE_FIELD(NAME, VALUE_TYPE) LUA_TABLE_FIELD_KEY_VALUE(NAME, const char *, VALUE_TYPE)

#define LUA_TABLE_END };

#define LUA_TABLE_TYPE_ADAPTER(TABLE_TYPE) \
namespace lua \
{ \
    template <> \
    class Type_Adapter < TABLE_TYPE > \
    { \
    public: \
\
        using type = TABLE_TYPE; \
\
    public: \
\
        static auto type_matches(lua::State state, int index) \
        { \
            return (state.is_nil(index) || state.is_table(index)); \
        } \
\
        static auto get(lua::State state, int index) \
        { \
            if (state.is_nil(index)) \
            {  \
                state.new_table(); \
                state.replace(index); \
            } \
\
            return type(state, index); \
        } \
\
        static void set(lua::State state, int index, type value) \
        {} \
\
        template < typename F > \
        static void apply(lua::State state, int index, F function) \
        { \
            function(state, index); \
        } \
    }; \
\
} \

namespace lua
{
    namespace table
    {
        LUA_TABLE_BEGIN(Asset)

        LUA_TABLE_FIELD(code, std::string) 
        LUA_TABLE_FIELD(name, std::string)
        LUA_TABLE_FIELD(short_name, std::string)
        LUA_TABLE_FIELD(class_code, std::string)
        LUA_TABLE_FIELD(class_name, std::string)
        LUA_TABLE_FIELD(face_value, double) 
        LUA_TABLE_FIELD(face_unit, std::string)
        LUA_TABLE_FIELD(scale, unsigned int)  
        LUA_TABLE_FIELD(mat_date, unsigned int)
        LUA_TABLE_FIELD(lot_size, double)  
        LUA_TABLE_FIELD(isin_code, std::string)
        LUA_TABLE_FIELD(min_price_step, double)
        LUA_TABLE_FIELD(bsid, std::string)
        LUA_TABLE_FIELD(cusip_code, std::string)
        LUA_TABLE_FIELD(stock_code, std::string)
        LUA_TABLE_FIELD(couponvalue, double)
        LUA_TABLE_FIELD(sell_leg_classcode, std::string)
        LUA_TABLE_FIELD(sell_leg_seccode, std::string)
        LUA_TABLE_FIELD(first_currcode, std::string)
        LUA_TABLE_FIELD(second_currcode, std::string)
        LUA_TABLE_FIELD(buy_leg_classcode, std::string) 
        LUA_TABLE_FIELD(buy_leg_seccode, std::string)
        LUA_TABLE_FIELD(base_active_classcode, std::string)
        LUA_TABLE_FIELD(base_active_seccode, std::string)
        LUA_TABLE_FIELD(buy_mat_date, int)
        LUA_TABLE_FIELD(sell_mat_date, int)
        LUA_TABLE_FIELD(option_strike, double)
        LUA_TABLE_FIELD(qty_multiplier, unsigned int)
        LUA_TABLE_FIELD(step_price_currency, std::string)
        LUA_TABLE_FIELD(sedol_code, std::string)
        LUA_TABLE_FIELD(cfi_code, std::string)
        LUA_TABLE_FIELD(ric_code, std::string)
        LUA_TABLE_FIELD(buybackdate, int)
        LUA_TABLE_FIELD(buybackprice, int)
        LUA_TABLE_FIELD(list_level, int)
        LUA_TABLE_FIELD(qty_scale, double)
        LUA_TABLE_FIELD(yieldatprevwaprice, double) 
        LUA_TABLE_FIELD(regnumber, std::string)
        LUA_TABLE_FIELD(trade_currency, std::string)
        LUA_TABLE_FIELD(second_curr_qty_scale, double)
        LUA_TABLE_FIELD(first_curr_qty_scale, double)
        LUA_TABLE_FIELD(accruedint, double)
        LUA_TABLE_FIELD(stock_name, std::string)
        LUA_TABLE_FIELD(nextcoupon, int)
        LUA_TABLE_FIELD(couponperiod, int)

        LUA_TABLE_END

        LUA_TABLE_BEGIN(Portfolio)

        LUA_TABLE_FIELD(is_leverage, std::string)
        LUA_TABLE_FIELD(in_assets, std::string)
        LUA_TABLE_FIELD(leverage, std::string)
        LUA_TABLE_FIELD(open_limit, std::string)
        LUA_TABLE_FIELD(val_short, std::string)
        LUA_TABLE_FIELD(val_long, std::string)
        LUA_TABLE_FIELD(val_long_margin, std::string)
        LUA_TABLE_FIELD(val_long_asset, std::string)
        LUA_TABLE_FIELD(assets, std::string)
        LUA_TABLE_FIELD(cur_leverage, std::string)
        LUA_TABLE_FIELD(margin, std::string)
        LUA_TABLE_FIELD(lim_all, std::string)
        LUA_TABLE_FIELD(av_lim_all, std::string)
        LUA_TABLE_FIELD(locked_buy, std::string)
        LUA_TABLE_FIELD(locked_buy_margin, std::string)
        LUA_TABLE_FIELD(locked_buy_asset, std::string)
        LUA_TABLE_FIELD(locked_sell, std::string)
        LUA_TABLE_FIELD(locked_value_coef, std::string)
        LUA_TABLE_FIELD(in_all_assets, std::string)
        LUA_TABLE_FIELD(all_assets, std::string)
        LUA_TABLE_FIELD(profit_loss, std::string)
        LUA_TABLE_FIELD(rate_change, std::string)
        LUA_TABLE_FIELD(lim_buy, std::string)
        LUA_TABLE_FIELD(lim_sell, std::string)
        LUA_TABLE_FIELD(lim_non_margin, std::string)
        LUA_TABLE_FIELD(lim_buy_asset, std::string)
        LUA_TABLE_FIELD(val_short_net, std::string)
        LUA_TABLE_FIELD(val_long_net, std::string)
        LUA_TABLE_FIELD(total_money_bal, std::string)
        LUA_TABLE_FIELD(total_locked_money, std::string)
        LUA_TABLE_FIELD(haircuts, std::string)
        LUA_TABLE_FIELD(assets_without_hc, std::string)
        LUA_TABLE_FIELD(status_coef, std::string)
        LUA_TABLE_FIELD(varmargin, std::string)
        LUA_TABLE_FIELD(go_for_positions, std::string)
        LUA_TABLE_FIELD(go_for_orders, std::string)
        LUA_TABLE_FIELD(rate_futures, std::string)
        LUA_TABLE_FIELD(is_qual_client, std::string)
        LUA_TABLE_FIELD(is_futures, std::string)
        LUA_TABLE_FIELD(curr_tag, std::string)
        
        LUA_TABLE_END

        LUA_TABLE_BEGIN(Candle_Time)

        LUA_TABLE_FIELD(year, unsigned int)
        LUA_TABLE_FIELD(month, unsigned int)
        LUA_TABLE_FIELD(day, unsigned int)
        LUA_TABLE_FIELD(week_day, unsigned int)
        LUA_TABLE_FIELD(hour, unsigned int)
        LUA_TABLE_FIELD(min, unsigned int)
        LUA_TABLE_FIELD(sec, unsigned int)
        LUA_TABLE_FIELD(ms, unsigned int)
        LUA_TABLE_FIELD(count, unsigned int)

        LUA_TABLE_END

        LUA_TABLE_BEGIN(Chart_Time)

        LUA_TABLE_FIELD(year, unsigned int)
        LUA_TABLE_FIELD(month, unsigned int)
        LUA_TABLE_FIELD(day, unsigned int)
        LUA_TABLE_FIELD(week_day, unsigned int)
        LUA_TABLE_FIELD(hour, unsigned int)
        LUA_TABLE_FIELD(min, unsigned int)
        LUA_TABLE_FIELD(sec, unsigned int)
        LUA_TABLE_FIELD(ms, unsigned int)
        LUA_TABLE_FIELD(count, unsigned int)

        LUA_TABLE_END

    } // namespace table

} // namespace lua

LUA_TABLE_TYPE_ADAPTER(lua::table::Asset)
LUA_TABLE_TYPE_ADAPTER(lua::table::Portfolio)
LUA_TABLE_TYPE_ADAPTER(lua::table::Candle_Time)
LUA_TABLE_TYPE_ADAPTER(lua::table::Chart_Time)