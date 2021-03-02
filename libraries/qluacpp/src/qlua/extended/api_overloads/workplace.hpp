#pragma once

void getSecurityInfo(const std::string& class_code,
                     const std::string& sec_code,
                     std::function<void(const lua::Entity < lua::Type_Adapter<::qlua::table::securities>>&)> lambda) const {
  api::getSecurityInfo(class_code.c_str(), sec_code.c_str(), lambda);
}


void getSecurityInfo(const char* class_code,
                     const char* sec_code,
                     std::function<void(const lua::Entity < lua::Type_Adapter<::qlua::table::securities>>&)> lambda) const {
  api::getSecurityInfo(class_code, sec_code, lambda);
}

std::tuple<unsigned int, double> CalcBuySell(const std::string& class_code,
                                             const std::string& sec_code,
                                             const std::string& client_code,
                                             const std::string& account,
                                             const double price,
                                             const bool is_buy,
                                             const bool is_market) {
  return api::CalcBuySell(class_code.c_str(), sec_code.c_str(), client_code.c_str(), account.c_str(), price, is_buy, is_market);
}

std::tuple<unsigned int, double> CalcBuySell(const char* class_code,
                                             const char* sec_code,
                                             const char* client_code,
                                             const char* account,
                                             const double price,
                                             const bool is_buy,
                                             const bool is_market) {
  return api::CalcBuySell(class_code, sec_code, client_code, account, price, is_buy, is_market);
}

void getPortfolioInfo(const std::string& firm_id,
                      const std::string& client_code,
                      std::function<void(const lua::Entity < lua::Type_Adapter<::qlua::table::portfolio_info_getPortfolioInfo>>&)> lambda) const {
  api::getPortfolioInfo(firm_id.c_str(), client_code.c_str(), lambda);
}

void getPortfolioInfo(const char* firm_id,
                      const char* client_code,
                      std::function<void(const lua::Entity < lua::Type_Adapter<::qlua::table::portfolio_info_getPortfolioInfo>>&)> lambda) const {
  api::getPortfolioInfo(firm_id, client_code, lambda);
}

void getBuySellInfo(const std::string& firm_id,
                    const std::string& client_code,
                    const std::string& class_code,
                    const std::string& sec_code,
                    const double price,
                    std::function<void(const lua::Entity < lua::Type_Adapter<::qlua::table::buy_sell_info_getBuySellInfo>>&)> lambda) const {
  api::getBuySellInfo(firm_id.c_str(), client_code.c_str(), class_code.c_str(), sec_code.c_str(), price, lambda);
}

void getBuySellInfo(const char* firm_id,
                    const char* client_code,
                    const char* class_code,
                    const char* sec_code,
                    const double price,
                    std::function<void(const lua::Entity < lua::Type_Adapter<::qlua::table::buy_sell_info_getBuySellInfo>>&)> lambda) const {
  api::getBuySellInfo(firm_id, client_code, class_code, sec_code, price, lambda);
}