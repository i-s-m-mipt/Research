#pragma once

#include "../structs/function_results.hpp"

// getSecurityInfo - функция для получения информации по инструменту
QLUACPP_DETAIL_API_FUNCTION_RES1_APPLY6(::qlua::table::securities,
                            getSecurityInfo,
                            const char*, class_code,
                            const char*, sec_code
                            )
// getTradeDate - функция для получения даты торговой сессии
QLUACPP_DETAIL_API_FUNCTION_RES1_APPLY2(::qlua::table::date_,
                            getTradeDate
                            )

// sendTransaction - функция для работы с заявками
void sendTransaction(const std::map<std::string, std::string>& transaction // Поля таблицы в соответствии с описанием .tri файла по quik.chm
                     ) {
  l_.get_global("sendTransaction");
  l_.new_table();
  for (const auto& p : transaction) {
    l_.push_string(p.first.c_str());
    l_.push_string(p.second.c_str());
    l_.set_table(-3);
  }
  l_.pcall(1, 1, 0);
  if (l_.is_string(-1)) {
    const auto& msg = l_.at<const char*>(-1).get();
    if (msg[0] == 0) {
      l_.pop();
    } else {
      const std::string msg_str = msg;
      l_.pop();
      throw std::runtime_error("sendTransaction error: " + msg_str);
    }
  } else {
    l_.pop();
    throw std::runtime_error("sendTransaction returned unexpected type");
  }
}

// CalcBuySell - функция для расчета максимально возможного количества лотов в заявке
QLUACPP_DETAIL_API_FUNCTION_TUPLE2_17(unsigned int, double, // qty,  comission,
                            CalcBuySell,
                            const char*, class_code,
                            const char*, sec_code,
                            const char*, client_code,
                            const char*, account,
                            const double, price,
                            const bool, is_buy,
                            const bool, is_market
                            ) 

// getPortfolioInfo - функция для получения значений параметров таблицы «Клиентский портфель»
QLUACPP_DETAIL_API_FUNCTION_RES1_APPLY6(::qlua::table::portfolio_info_getPortfolioInfo,
                            getPortfolioInfo,
                            const char*, firm_id,
                            const char*, client_code
                            )

// getBuySellInfo - функция для получения параметров таблицы «Купить/Продать»
QLUACPP_DETAIL_API_FUNCTION_RES1_APPLY12(::qlua::table::buy_sell_info_getBuySellInfo,
                            getBuySellInfo,
                            const char*, firm_id,
                            const char*, client_code,
                            const char*, class_code,
                            const char*, sec_code,
                            const double, price
                            )