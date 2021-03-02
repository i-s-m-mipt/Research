#pragma once

#include <string>

#include <luacpp/luacpp>

#include "datetime.hpp"

namespace qlua
{
    namespace table
    {
        LUA_TABLE_BEGIN(securities)
            LUA_TABLE_FIELD(code, std::string) // Код инструмента  
            LUA_TABLE_FIELD(name, std::string) // Наименование инструмента  
            LUA_TABLE_FIELD(short_name, std::string) // Короткое наименование инструмента  
            LUA_TABLE_FIELD(class_code, std::string) // Код класса инструментов  
            LUA_TABLE_FIELD(class_name, std::string) // Наименование класса инструментов  
            LUA_TABLE_FIELD(face_value, double) // Номинал  
            LUA_TABLE_FIELD(face_unit, std::string) // Валюта номинала  
            LUA_TABLE_FIELD(scale, unsigned int) // Точность (количество значащих цифр после запятой)  
            LUA_TABLE_FIELD(mat_date, unsigned int) // Дата погашения  
            LUA_TABLE_FIELD(lot_size, double) // Размер лота  
            LUA_TABLE_FIELD(isin_code, std::string) // ISIN  
            LUA_TABLE_FIELD(min_price_step, double) // Минимальный шаг цены
            LUA_TABLE_FIELD(bsid, std::string) // Bloomberg ID  
            LUA_TABLE_FIELD(cusip_code, std::string) // CUSIP  
            LUA_TABLE_FIELD(stock_code, std::string) // StockCode  
            LUA_TABLE_FIELD(couponvalue, double) // Размер купона  
            LUA_TABLE_FIELD(sell_leg_classcode, std::string) // Код класса инструмента ноги на продажу  
            LUA_TABLE_FIELD(sell_leg_seccode, std::string) // Код инструмента ноги на продажу  
            LUA_TABLE_FIELD(first_currcode, std::string) // Код котируемой валюты в паре  
            LUA_TABLE_FIELD(second_currcode, std::string) // Код базовой валюты в паре  
            LUA_TABLE_FIELD(buy_leg_classcode, std::string) // Код класса инструмента ноги на покупку  
            LUA_TABLE_FIELD(buy_leg_seccode, std::string) // Код инструмента ноги на покупку  
            LUA_TABLE_FIELD(base_active_classcode, std::string) // Код класса базового актива  
            LUA_TABLE_FIELD(base_active_seccode, std::string) // Базовый актив  
            LUA_TABLE_FIELD(buy_mat_date, int) // Дата расчетов сделки на покупку  
            LUA_TABLE_FIELD(sell_mat_date, int) // Дата расчетов сделки на продажу  
            LUA_TABLE_FIELD(option_strike, double) // Страйк опциона  
            LUA_TABLE_FIELD(qty_multiplier, unsigned int) // Кратность при вводе количества  
            LUA_TABLE_FIELD(step_price_currency, std::string) // Валюта шага цены  
            LUA_TABLE_FIELD(sedol_code, std::string) // SEDOL  
            LUA_TABLE_FIELD(cfi_code, std::string) // CFI  
            LUA_TABLE_FIELD(ric_code, std::string) // RIC  
            LUA_TABLE_FIELD(buybackdate, int) // Дата оферты  
            LUA_TABLE_FIELD(buybackprice, int) // Цена оферты  
            LUA_TABLE_FIELD(list_level, int) // Уровень листинга  
            LUA_TABLE_FIELD(qty_scale, double) // Точность количества  
            LUA_TABLE_FIELD(yieldatprevwaprice, double) // Доходность по предыдущей оценке  
            LUA_TABLE_FIELD(regnumber, std::string) // Регистрационный номер  
            LUA_TABLE_FIELD(trade_currency, std::string) // Валюта торгов  
            LUA_TABLE_FIELD(second_curr_qty_scale, double) // Точность количества котируемой валюты  
            LUA_TABLE_FIELD(first_curr_qty_scale, double) // Точность количества базовой валюты  
            LUA_TABLE_FIELD(accruedint, double) // Накопленный купонный доход 
            LUA_TABLE_FIELD(stock_name, std::string) // Код деривативного контракта в формате QUIK  
            LUA_TABLE_FIELD(nextcoupon, int) // Дата выплаты купона  
            LUA_TABLE_FIELD(couponperiod, int) // Длительность купона  
            LUA_TABLE_END
    }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::securities)

namespace qlua 
{
    namespace table 
    {
        LUA_TABLE_BEGIN(candle)
            LUA_TABLE_FIELD(open, double) // Цена открытия  
            LUA_TABLE_FIELD(close, double) // Цена закрытия  
            LUA_TABLE_FIELD(high, double) // Максимальная цена сделки  
            LUA_TABLE_FIELD(low, double) // Минимальная цена сделки  
            LUA_TABLE_FIELD(volume, double) // Объем последней сделки
            QLUACPP_DATETIME_TABLE_FIELD(datetime) // Формат даты и времени  
            LUA_TABLE_FIELD(doesExist, unsigned int) /* Признак расчета индикатора при наличии свечки. Возможные значения:
                                                           «0» – индикатор не рассчитан,
                                                           «1» – индикатор рассчитан */
            LUA_TABLE_END
    }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::candle)

#define QLUACPP_DETAIL_TABLE_NAME(NAME)                 \
  template <>                                           \
  struct name_for_type<::qlua::table::NAME> {           \
    static const char* value() { return ""#NAME; }      \
  };                                                    \

namespace qlua 
{
    namespace table 
    {
        namespace detail 
        {
            template <typename Table>
            struct name_for_type;


            QLUACPP_DETAIL_TABLE_NAME(securities);
        }
    }
}

#undef QLUACPP_DETAIL_TABLE_NAME