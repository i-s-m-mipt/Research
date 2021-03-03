#pragma once

#include <exception>
#include <stdexcept>
#include <string>

#include <luacpp/luacpp>

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