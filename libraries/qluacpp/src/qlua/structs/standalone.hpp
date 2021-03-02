#pragma once

/*
  QLUA tables structures
  Список по "Таблицы, используемые в функциях «getItem», «getNumberOf» и «SearchItems»"; по таблицам, возвращаемым коллбеками
*/
#include <string>

#include <luacpp/luacpp>

#include "datetime.hpp"

// Regex:   *\(.*?\) +\(.*?\) +\(.*\)$LUA_TABLE_FIELD(\1, \2) // \3

// firms "Фирмы"
// Object names in qlua.chm: firm
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(firms)
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы 
    LUA_TABLE_FIELD(firm_name, std::string) // Название класса  
    LUA_TABLE_FIELD(status, unsigned int) // Статус  
    LUA_TABLE_FIELD(exchange, std::string) // Торговая площадка  
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::firms)

// classes "Классы"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(classes)
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы 
    LUA_TABLE_FIELD(name, std::string) // Название класса 
    LUA_TABLE_FIELD(code, std::string) // Код класса 
    LUA_TABLE_FIELD(npars, unsigned int) // Количество параметров в классе 
    LUA_TABLE_FIELD(nsecs, unsigned int) // Количество бумаг в классе
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::classes)

// securities "Инструменты" (ценные бумаги)
namespace qlua {
  namespace table {
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

// trade_accounts "Торговые счета"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(trade_accounts)
    LUA_TABLE_FIELD(class_codes, std::string) // Список кодов классов, разделенных символом «|» 
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы 
    LUA_TABLE_FIELD(trdaccid, std::string) // Код торгового счета 
    LUA_TABLE_FIELD(description, std::string) // Описание 
    LUA_TABLE_FIELD(fullcoveredsell, unsigned int) /* Запрет необеспеченных продаж. Возможные значения: 
                                                         «0» – Нет; 
                                                         «1» – Да */
    LUA_TABLE_FIELD(main_trdaccid, std::string) // Номер основного торгового счета 
    LUA_TABLE_FIELD(bankid_t0, std::string) // Расчетная организация по «Т0» 
    LUA_TABLE_FIELD(bankid_tplus, std::string) // Расчетная организация по «Т+» 
    LUA_TABLE_FIELD(trdacc_type, unsigned int) // Тип торгового счета 
    LUA_TABLE_FIELD(depunitid, std::string) // Раздел счета Депо 
    LUA_TABLE_FIELD(status, unsigned int) /* Статус торгового счета. Возможные значения: 
                                                «0» – операции разрешены; 
                                                «1» – операции запрещены
                                             */
    LUA_TABLE_FIELD(firmuse, unsigned int) /* Тип раздела. Возможные значения: 
                                                 «0» – раздел обеспечения; 
                                                 иначе – для торговых разделов */
    LUA_TABLE_FIELD(depaccid, std::string) // Номер счета депо в депозитарии 
    LUA_TABLE_FIELD(bank_acc_id, std::string) // Код дополнительной позиции по денежным средствам
    // Quik 8.1: Exists in qlua.chm help file, doesn't really exist in terminal
    //LUA_TABLE_FIELD(exec_market, std::string) // Идентификатор биржевой площадки 
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::trade_accounts)
    
// client_codes "Коды клиентов"
// (Not really a table, an array of strings)
namespace qlua {
  namespace table {
    using client_codes = std::string;
  }
}

// account_positions "Денежные позиции"
// Object names in qlua.chm: acc_pos
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(account_positions)
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы  
    LUA_TABLE_FIELD(currcode, std::string) // Код валюты  
    LUA_TABLE_FIELD(tag, std::string) // Тег расчетов  
    LUA_TABLE_FIELD(description, std::string) // Описание  
    LUA_TABLE_FIELD(openbal, double) // Входящий остаток  
    LUA_TABLE_FIELD(currentpos, double) // Текущий остаток  
    LUA_TABLE_FIELD(plannedpos, double) // Плановый остаток  
    LUA_TABLE_FIELD(limit1, double) // Внешнее ограничение по деньгам  
    LUA_TABLE_FIELD(limit2, double) // Внутреннее (собственное) ограничение по деньгам  
    LUA_TABLE_FIELD(orderbuy, double) // В заявках на продажу  
    LUA_TABLE_FIELD(ordersell, double) // В заявках на покупку  
    LUA_TABLE_FIELD(netto, double) // Нетто-позиция  
    LUA_TABLE_FIELD(plannedbal, double) // Плановая позиция  
    LUA_TABLE_FIELD(debit, double) // Дебет  
    LUA_TABLE_FIELD(credit, double) // Кредит  
    LUA_TABLE_FIELD(bank_acc_id, std::string) // Идентификатор счета  
    LUA_TABLE_FIELD(margincall, double) // Маржинальное требование на начало торгов  
    LUA_TABLE_FIELD(settlebal, double) // Плановая позиция после проведения расчетов  
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::account_positions)
    
// orders "Заявки"
// Object names in qlua.chm: order "Таблица с параметрами заявки"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(orders)
    LUA_TABLE_FIELD(order_num, std::string) // Номер заявки в торговой системе  
    LUA_TABLE_FIELD(flags, unsigned int) // Набор битовых флагов 
    LUA_TABLE_FIELD(brokerref, std::string) // Комментарий, обычно: <код клиента>/<номер поручения>  
    LUA_TABLE_FIELD(userid, std::string) // Идентификатор трейдера  
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы  
    LUA_TABLE_FIELD(account, std::string) // Торговый счет  
    LUA_TABLE_FIELD(price, double) // Цена  
    LUA_TABLE_FIELD(qty, unsigned int) // Количество в лотах  
    LUA_TABLE_FIELD(balance, unsigned int) // Остаток  
    LUA_TABLE_FIELD(value, double) // Объем в денежных средствах  
    LUA_TABLE_FIELD(accruedint, double) // Накопленный купонный доход  
    LUA_TABLE_FIELD(yield, double) // Доходность  
    LUA_TABLE_FIELD(trans_id, unsigned int) // Идентификатор транзакции  
    LUA_TABLE_FIELD(client_code, std::string) // Код клиента  
    LUA_TABLE_FIELD(price2, double) // Цена выкупа  
    LUA_TABLE_FIELD(settlecode, std::string) // Код расчетов  
    LUA_TABLE_FIELD(uid, unsigned int) // Идентификатор пользователя  
    LUA_TABLE_FIELD(canceled_uid, unsigned int) // Идентификатор пользователя, снявшего заявку  
    LUA_TABLE_FIELD(exchange_code, std::string) // Код биржи в торговой системе  
    LUA_TABLE_FIELD(activation_time, int) // Время активации  
    LUA_TABLE_FIELD(linkedorder, unsigned int) // Номер заявки в торговой системе  
    LUA_TABLE_FIELD(expiry, int) // Дата окончания срока действия заявки  
    LUA_TABLE_FIELD(sec_code, std::string) // Код бумаги заявки  
    LUA_TABLE_FIELD(class_code, std::string) // Код класса заявки
    QLUACPP_DATETIME_TABLE_FIELD(datetime) // Дата и время 
    QLUACPP_DATETIME_TABLE_FIELD(withdraw_datetime) // Дата и время снятия заявки  
    LUA_TABLE_FIELD(bank_acc_id, std::string) // Идентификатор расчетного счета/кода в клиринговой организации  
    LUA_TABLE_FIELD(value_entry_type, unsigned int) /* Способ указания объема заявки. Возможные значения:

«0» – по количеству, 
«1» – по объему */
    LUA_TABLE_FIELD(repoterm, double) // Срок РЕПО, в календарных днях  
    LUA_TABLE_FIELD(repovalue, double) // Сумма РЕПО на текущую дату. Отображается с точностью 2 знака  
    LUA_TABLE_FIELD(repo2value, double) // Объём сделки выкупа РЕПО. Отображается с точностью 2 знака  
    LUA_TABLE_FIELD(repo_value_balance, double) // Остаток суммы РЕПО за вычетом суммы привлеченных или предоставленных по сделке РЕПО денежных средств в неисполненной части заявки, по состоянию на текущую дату. Отображается с точностью 2 знака  
    LUA_TABLE_FIELD(start_discount, double) // Начальный дисконт, в %  
    LUA_TABLE_FIELD(reject_reason, std::string) // Причина отклонения заявки брокером  
    LUA_TABLE_FIELD(ext_order_flags, unsigned int) // Битовое поле для получения специфических параметров с западных площадок  
    LUA_TABLE_FIELD(min_qty, unsigned int) // Минимально допустимое количество, которое можно указать в заявке по данному инструменту. Если имеет значение «0», значит ограничение по количеству не задано  
    LUA_TABLE_FIELD(exec_type, unsigned int) // Тип исполнения заявки. Если имеет значение «0», значит значение не задано  
    LUA_TABLE_FIELD(side_qualifier, unsigned int) // Поле для получения параметров по западным площадкам. Если имеет значение «0», значит значение не задано  
    LUA_TABLE_FIELD(acnt_type, unsigned int) // Поле для получения параметров по западным площадкам. Если имеет значение «0», значит значение не задано  
    LUA_TABLE_FIELD(capacity, double) // Поле для получения параметров по западным площадкам. Если имеет значение «0», значит значение не задано  
    LUA_TABLE_FIELD(passive_only_order, unsigned int) // Поле для получения параметров по западным площадкам. Если имеет значение «0», значит значение не задано  
    LUA_TABLE_FIELD(visible, unsigned int) // Видимое количество. Параметр айсберг-заявок, для обычных заявок выводится значение: «0»

    LUA_TABLE_FIELD(awg_price, double) // Средняя цена приобретения. Актуально, когда заявка выполнилась частями 
    LUA_TABLE_FIELD(expiry_time, int) // Время окончания срока действия заявки в формате <ЧЧММСС DESIGNTIMESP=19552>. Для GTT-заявок, используется вместе со сроком истечения заявки (Expiry)  
    LUA_TABLE_FIELD(revision_number, unsigned int) // Номер ревизии заявки. Используется, если заявка была заменена с сохранением номера 
    LUA_TABLE_FIELD(price_currency, std::string) // Валюта цены заявки 
    LUA_TABLE_FIELD(ext_order_status, unsigned int) /* Расширенный статус заявки. Возможные значения: 
                                                          «0» (по умолчанию) – не определено; 
                                                          «1» – заявка активна; 
                                                          «2» – заявка частично исполнена; 
                                                          «3» – заявка исполнена; 
                                                          «4» – заявка отменена; 
                                                          «5» – заявка заменена; 
                                                          «6» – заявка в состоянии отмены; 
                                                          «7» – заявка отвергнута; 
                                                          «8» – приостановлено исполнение заявки; 
                                                          «9» – заявка в состоянии регистрации; 
                                                          «10» – заявка снята по времени действия; 
                                                          «11» – заявка в состоянии замены 
                                                       */
 
    LUA_TABLE_FIELD(accepted_uid, unsigned int) // UID пользователя-менеджера, подтвердившего заявку при работе в режиме с подтверждениями 
    LUA_TABLE_FIELD(filled_value, double) // Исполненный объем заявки в валюте цены для частично или полностью исполненных заявок 
    LUA_TABLE_FIELD(extref, std::string) // Внешняя ссылка, используется для обратной связи с внешними системами  
    LUA_TABLE_FIELD(settle_currency, std::string) // Валюта расчетов по заявке 
    LUA_TABLE_FIELD(on_behalf_of_uid, unsigned int) // UID пользователя, от имени которого выставлена заявка 
    LUA_TABLE_FIELD(client_qualifier, unsigned int) /* Квалификатор клиента, от имени которого выставлена заявка. Возможные значения: 
                                                          «0» – не определено; 
                                                          «1» – Natural Person; 
                                                          «3» – Legal Entity
                                                       */
 
    LUA_TABLE_FIELD(client_short_code, unsigned int) // Краткий идентификатор клиента, от имени которого выставлена заявка 
    LUA_TABLE_FIELD(investment_decision_maker_qualifier, unsigned int) /* Квалификатор принявшего решение о выставлении заявки. Возможные значения: 
                                                                             «0» – не определено; 
                                                                             «1» – Natural Person; 
                                                                             «2» – Algorithm
                                                                          */
 
    LUA_TABLE_FIELD(investment_decision_maker_short_code, unsigned int) // Краткий идентификатор принявшего решение о выставлении заявки 
    LUA_TABLE_FIELD(executing_trader_qualifier, unsigned int) /* Квалификатор трейдера, исполнившего заявку. Возможные значения: 
                                                                    «0» – не определено; 
                                                                    «1» – Natural Person; 
                                                                    «2» – Algorithm
                                                                 */
 
    LUA_TABLE_FIELD(executing_trader_short_code, unsigned int) // Краткий идентификатор трейдера, исполнившего заявку 

    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::orders)
    
// futures_client_holding "Позиции по клиентским счетам (фьючерсы)"
// Object names in qlua.chm: fut_pos "Таблица с описанием позиции по срочному рынку"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(futures_client_holding)
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы  
    LUA_TABLE_FIELD(trdaccid, std::string) // Торговый счет  
    LUA_TABLE_FIELD(sec_code, std::string) // Код фьючерсного контракта  
    LUA_TABLE_FIELD(type, double) /* Тип лимита. Возможные значения: 
                                        «0» – не определён; 
                                        «1» – основной счет; 
                                        «2» – клиентские и дополнительные счета; 
                                        «4» – все счета торг. членов */
    LUA_TABLE_FIELD(startbuy, double) // Входящие длинные позиции  
    LUA_TABLE_FIELD(startsell, double) // Входящие короткие позиции  
    LUA_TABLE_FIELD(startnet, double) // Входящие чистые позиции  
    LUA_TABLE_FIELD(todaybuy, double) // Текущие длинные позиции  
    LUA_TABLE_FIELD(todaysell, double) // Текущие короткие позиции  
    LUA_TABLE_FIELD(totalnet, double) // Текущие чистые позиции  
    LUA_TABLE_FIELD(openbuys, double) // Активные на покупку  
    LUA_TABLE_FIELD(opensells, double) // Активные на продажу  
    LUA_TABLE_FIELD(cbplused, double) // Оценка текущих чистых позиций  
    LUA_TABLE_FIELD(cbplplanned, double) // Плановые чистые позиции  
    LUA_TABLE_FIELD(varmargin, double) // Вариационная маржа  
    LUA_TABLE_FIELD(avrposnprice, double) // Эффективная цена позиций  
    LUA_TABLE_FIELD(positionvalue, double) // Стоимость позиций  
    LUA_TABLE_FIELD(real_varmargin, double) // Реально начисленная в ходе клиринга вариационная маржа. Отображается с точностью до 2 двух знаков. При этом, в поле «varmargin» транслируется вариационная маржа, рассчитанная с учетом установленных границ изменения цены  
    LUA_TABLE_FIELD(total_varmargin, double) // Суммарная вариационная маржа по итогам основного клиринга начисленная по всем позициям. Отображается с точностью до 2 двух знаков  
    LUA_TABLE_FIELD(session_status, unsigned int) /* Актуальный статус торговой сессии. Возможные значения: 
                                                        «0» – не определено; 
                                                        «1» – основная сессия; 
                                                        «2» – начался промклиринг; 
                                                        «3» – завершился промклиринг; 
                                                        «4» – начался основной клиринг; 
                                                        «5» – основной клиринг: новая сессия назначена; 
                                                        «6» – завершился основной клиринг; 
                                                        «7» – завершилась вечерняя сессия */
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::futures_client_holding)
    
// futures_client_limits "Лимиты по фьючерсам"
// Object name in qlua.chm: fut_limit "Таблица с текущими значениями лимита по срочному рынку"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(futures_client_limits)
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы  
    LUA_TABLE_FIELD(trdaccid, std::string) // Торговый счет  
    LUA_TABLE_FIELD(limit_type, unsigned int) /* Тип лимита. Возможные значения: 
                                                    «0» – «Денежные средства»; 
                                                    «1» – «Залоговые денежные средства»; 
                                                    «2» – «По совокупным средствам»; 
                                                    «3» – «Клиринговые денежные средства»; 
                                                    «4» – «Клиринговые залоговые денежные средства»; 
                                                    «5» – «Лимит открытых позиций на спот-рынке»; 
                                                    «6» – «Суммарные залоговые средства в иностранной валюте (в рублях)»; 
                                                    «7» – «Залоговые средства в иностранной валюте» */
 
    LUA_TABLE_FIELD(liquidity_coef, double) // Коэффициент ликвидности  
    LUA_TABLE_FIELD(cbp_prev_limit, double) // Предыдущий лимит открытых позиций на спот-рынке» 
    LUA_TABLE_FIELD(cbplimit, double) // Лимит открытых позиций  
    LUA_TABLE_FIELD(cbplused, double) // Текущие чистые позиции  
    LUA_TABLE_FIELD(cbplplanned, double) // Плановые чистые позиции  
    LUA_TABLE_FIELD(varmargin, double) // Вариационная маржа  
    LUA_TABLE_FIELD(accruedint, double) // Накопленный доход   
    LUA_TABLE_FIELD(cbplused_for_orders, double) // Текущие чистые позиции (под заявки)  
    LUA_TABLE_FIELD(cbplused_for_positions, double) // Текущие чистые позиции (под открытые позиции)  
    LUA_TABLE_FIELD(options_premium, double) // Премия по опционам  
    LUA_TABLE_FIELD(ts_comission, double) // Биржевые сборы  
    LUA_TABLE_FIELD(kgo, double) // Коэффициент клиентского гарантийного обеспечения  
    LUA_TABLE_FIELD(currcode, std::string) // Валюта, в которой транслируется ограничение  
    LUA_TABLE_FIELD(real_varmargin, double) // Реально начисленная в ходе клиринга вариационная маржа. Отображается с точностью до 2 двух знаков. При этом в поле «varmargin» транслируется вариационная маржа, рассчитанная с учетом установленных границ изменения цены  
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::futures_client_limits)

// all_trades "Обезличенные сделки"
// Object names in qlua.chm: alltrade
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(all_trades)

    LUA_TABLE_FIELD(trade_num, std::string) // Номер сделки в торговой системе 
    LUA_TABLE_FIELD(flags, unsigned int) // Набор битовых флагов  
    LUA_TABLE_FIELD(price, double) // Цена  
    LUA_TABLE_FIELD(qty, unsigned int) // Количество бумаг в последней сделке в лотах  
    LUA_TABLE_FIELD(value, double) // Объем в денежных средствах  
    LUA_TABLE_FIELD(accruedint, double) // Накопленный купонный доход  
    LUA_TABLE_FIELD(yield, double) // Доходность  
    LUA_TABLE_FIELD(settlecode, std::string) // Код расчетов  
    LUA_TABLE_FIELD(reporate, double) // Ставка РЕПО (%)  
    LUA_TABLE_FIELD(repovalue, double) // Сумма РЕПО  
    LUA_TABLE_FIELD(repo2value, double) // Объем выкупа РЕПО  
    LUA_TABLE_FIELD(repoterm, double) // Срок РЕПО в днях  
    LUA_TABLE_FIELD(sec_code, std::string) // Код бумаги заявки  
    LUA_TABLE_FIELD(class_code, std::string) // Код класса
    QLUACPP_DATETIME_TABLE_FIELD(datetime) // Дата и время 
    LUA_TABLE_FIELD(period, unsigned int) /* Период торговой сессии. Возможные значения:
    
    «0» – Открытие; 
    «1» – Нормальный; 
    «2» – Закрытие 
                                             */
    LUA_TABLE_FIELD(open_interest, double) // Открытый интерес 
    LUA_TABLE_FIELD(exchange_code, std::string) // Код биржи в торговой системе 
    LUA_TABLE_FIELD(exec_market, std::string) // Площадка исполнения 
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::all_trades)


// futures_limit_delete "Таблица с параметрами удаляемого лимита по срочному рынку" (callback)
// Object names in qlua.chm: lim_del
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(futures_limit_delete)
    LUA_TABLE_FIELD(trdaccid, std::string) // Код торгового счета  
    LUA_TABLE_FIELD(limit_type, unsigned int) /* Тип лимита. Возможные значения: 
                                                    «0» – «Денежные средства», 
                                                    «1» – «Залоговые денежные средства», 
                                                    «2» – «По совокупным средствам»; 
                                                    «3» – «Клиринговые денежные средства»; 
                                                    «4» – «Клиринговые залоговые денежные средства»; 
                                                    «5» – «Лимит открытых позиций на спот-рынке»; 
                                                    «6» – «Суммарные залоговые средства в иностранной валюте (в рублях)»; 
                                                    «7» – «Залоговые средства в иностранной валюте» */
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::futures_limit_delete)
    
// money_limits "Позиции по деньгам"
//   Old:
//     "Лимиты по денежным средствам"
// Object names in qlua.chm: mlimit
//     "Таблица «Позиции по деньгам»"
//   Old:
//     "Таблица с текущими значениями денежного лимита"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(money_limits)
    LUA_TABLE_FIELD(currcode, std::string) // Код валюты  
    LUA_TABLE_FIELD(tag, std::string) // Код позиции  
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы  
    LUA_TABLE_FIELD(client_code, std::string) // Код клиента  
    LUA_TABLE_FIELD(openbal, double) // Входящий остаток  
    LUA_TABLE_FIELD(openlimit, double) // Входящий лимит  
    LUA_TABLE_FIELD(currentbal, double) // Текущий остаток   
    LUA_TABLE_FIELD(currentlimit, double) // Текущий лимит  
    LUA_TABLE_FIELD(locked, double) // Заблокировано. Сумма средств, заблокированных под исполнение заявок клиента  
    LUA_TABLE_FIELD(locked_value_coef, double) // Стоимость активов в заявках на покупку немаржинальных инструментов 
    LUA_TABLE_FIELD(locked_margin_value, double) // Стоимость активов в заявках на покупку маржинальных инструментов 
    LUA_TABLE_FIELD(leverage, double) // Плечо 
    LUA_TABLE_FIELD(limit_kind, int) /* Срок расчётов. Возможные значения: 
                                           положительные целые числа, начиная с «0», соответствующие срокам расчётов из таблицы «Позиции по деньгам»: «0» – T0, «1» – T1, «2» – T2 и т.д.; 
                                           отрицательные целые числа – технологические лимиты (используются для внутренней работы системы QUIK) */
 
    LUA_TABLE_FIELD(wa_position_price, double) // Средневзвешенная цена приобретения позиции 
    LUA_TABLE_FIELD(orders_collateral, double) // Гарантийное обеспечение заявок  
    LUA_TABLE_FIELD(positions_collateral, double) // Гарантийное обеспечение позиций  
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::money_limits)

// money_limit_delete "Таблица с параметрами удаляемого денежного лимита" (callback)
// Object names in qlua.chm: mlimit_del
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(money_limit_delete)
    LUA_TABLE_FIELD(currcode, std::string) // Код валюты  
    LUA_TABLE_FIELD(tag, std::string) // Тег расчетов  
    LUA_TABLE_FIELD(client_code, std::string) // Код клиента  
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы  
    LUA_TABLE_FIELD(limit_kind, int) /* Вид лимита. Возможные значения: 
                                           положительные целые числа, начиная с «0», соответствующие видам лимитов из таблицы «Лимиты по денежным средствам»: «0» – T0, «1» – T1, «2» – T2 и т.д.; 
                                           отрицательные целые числа – технологические лимиты (используются для внутренней работы системы QUIK)  */
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::money_limit_delete)

// depo_limits "Лимиты по бумагам"
// Object names in qlua.chm: dlimit
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(depo_limits)
    LUA_TABLE_FIELD(sec_code, std::string) // Код бумаги  
    LUA_TABLE_FIELD(trdaccid, std::string) // Счет депо  
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы  
    LUA_TABLE_FIELD(client_code, std::string) // Код клиента  
    LUA_TABLE_FIELD(openbal, double) // Входящий остаток по бумагам  
    LUA_TABLE_FIELD(openlimit, double) // Входящий лимит по бумагам  
    LUA_TABLE_FIELD(currentbal, double) // Текущий остаток по бумагам  
    LUA_TABLE_FIELD(currentlimit, double) // Текущий лимит по бумагам  
    LUA_TABLE_FIELD(locked_sell, double) // Заблокировано на продажу количества лотов  
    LUA_TABLE_FIELD(locked_buy, double) // Заблокировано на покупку количества лотов 
    LUA_TABLE_FIELD(locked_buy_value, double) // Стоимость ценных бумаг, заблокированных под покупку 
    LUA_TABLE_FIELD(locked_sell_value, double) // Стоимость ценных бумаг, заблокированных под продажу  
    LUA_TABLE_FIELD(wa_position_price, double) // Цена приобретения  
    LUA_TABLE_FIELD(limit_kind, int) /* Вид лимита. Возможные значения: 
                                           числа, начиная с «0», соответствующие видам лимитов из таблицы «Лимиты по бумагам»: «0» – T0, «1» – T1, «2» – T2 и т.д.; 
                                           числа – технологические лимиты (используются для внутренней работы системы QUIK)*/
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::depo_limits)

// depo_limit_delete "Таблица с параметрами удаляемого лимита по бумагам" (callback)
// Object names in qlua.chm: dlimit_del
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(depo_limit_delete)
    LUA_TABLE_FIELD(sec_code, std::string) // Код инструмента  
    LUA_TABLE_FIELD(trdaccid, std::string) // Код торгового счета  
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы  
    LUA_TABLE_FIELD(client_code, std::string) // Код клиента  
    LUA_TABLE_FIELD(limit_kind, int) /* Вид лимита. Возможные значения: 
                                           положительные целые числа, начиная с «0», соответствующие видам лимитов из таблицы «Лимиты по бумагам»: «0» – T0, «1» – T1, «2» – T2 и т.д.; 
                                           отрицательные целые числа – технологические лимиты (используются для внутренней работы системы QUIK)  */
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::depo_limit_delete)

// trades "Сделки"
// Object names in qlua.chm: trade "Таблица с параметрами сделки"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(trades)
    LUA_TABLE_FIELD(trade_num, std::string) // Номер сделки в торговой системе 
    LUA_TABLE_FIELD(order_num, std::string) // Номер заявки в торговой системе  
    LUA_TABLE_FIELD(brokerref, std::string) // Комментарий, обычно: <код клиента>/<номер поручения>  
    LUA_TABLE_FIELD(userid, std::string) // Идентификатор трейдера  
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор дилера  
    LUA_TABLE_FIELD(canceled_uid, unsigned int) // Идентификатор пользователя, отказавшегося от сделки  
    LUA_TABLE_FIELD(account, std::string) // Торговый счет  
    LUA_TABLE_FIELD(price, double) // Цена  
    LUA_TABLE_FIELD(qty, unsigned int) // Количество бумаг в последней сделке в лотах  
    LUA_TABLE_FIELD(value, double) // Объем в денежных средствах  
    LUA_TABLE_FIELD(accruedint, double) // Накопленный купонный доход  
    LUA_TABLE_FIELD(yield, double) // Доходность  
    LUA_TABLE_FIELD(settlecode, std::string) // Код расчетов  
    LUA_TABLE_FIELD(cpfirmid, std::string) // Код фирмы партнера  
    LUA_TABLE_FIELD(flags, unsigned int) // Набор битовых флагов  
    LUA_TABLE_FIELD(price2, double) // Цена выкупа  
    LUA_TABLE_FIELD(reporate, double) // Ставка РЕПО (%)  
    LUA_TABLE_FIELD(client_code, std::string) // Код клиента  
    LUA_TABLE_FIELD(accrued2, double) // Доход (%) на дату выкупа  
    LUA_TABLE_FIELD(repoterm, double) // Срок РЕПО, в календарных днях  
    LUA_TABLE_FIELD(repovalue, double) // Сумма РЕПО  
    LUA_TABLE_FIELD(repo2value, double) // Объем выкупа РЕПО  
    LUA_TABLE_FIELD(start_discount, double) // Начальный дисконт (%)  
    LUA_TABLE_FIELD(lower_discount, double) // Нижний дисконт (%)  
    LUA_TABLE_FIELD(upper_discount, double) // Верхний дисконт (%)  
    LUA_TABLE_FIELD(block_securities, unsigned int) // Блокировка обеспечения («Да»/«Нет»)  
    LUA_TABLE_FIELD(clearing_comission, double) // Клиринговая комиссия (ММВБ)  
    LUA_TABLE_FIELD(exchange_comission, double) // Комиссия Фондовой биржи (ММВБ)  
    LUA_TABLE_FIELD(tech_center_comission, double) // Комиссия Технического центра (ММВБ)  
    LUA_TABLE_FIELD(settle_date, int) // Дата расчетов  
    LUA_TABLE_FIELD(settle_currency, std::string) // Валюта расчетов  
    LUA_TABLE_FIELD(trade_currency, std::string) // Валюта  
    LUA_TABLE_FIELD(exchange_code, std::string) // Код биржи в торговой системе  
    LUA_TABLE_FIELD(station_id, std::string) // Идентификатор рабочей станции  
    LUA_TABLE_FIELD(sec_code, std::string) // Код бумаги заявки  
    LUA_TABLE_FIELD(class_code, std::string) // Код класса
    QLUACPP_DATETIME_TABLE_FIELD(datetime) // Дата и время  
    LUA_TABLE_FIELD(bank_acc_id, std::string) // Идентификатор расчетного счета/кода в клиринговой организации  
    LUA_TABLE_FIELD(broker_comission, double) // Комиссия брокера. Отображается с точностью до 2 двух знаков. Поле зарезервировано для будущего использования.  
    LUA_TABLE_FIELD(linked_trade, unsigned int) // Номер витринной сделки в Торговой Системе для сделок РЕПО с ЦК и SWAP  
    LUA_TABLE_FIELD(period, unsigned int) /* Период торговой сессии. Возможные значения: 
                                                «0» – Открытие; 
                                                «1» – Нормальный; 
                                                «2» – Закрытие */

    LUA_TABLE_FIELD(trans_id, unsigned int) // Идентификатор транзакции 
    LUA_TABLE_FIELD(kind, unsigned int) /* Тип сделки. Возможные значения:
                                              «1» – Обычная; 
                                              «2» – Адресная; 
                                              «3» – Первичное размещение; 
                                              «4» – Перевод денег/бумаг; 
                                              «5» – Адресная сделка первой части РЕПО; 
                                              «6» – Расчетная по операции своп; 
                                              «7» – Расчетная по внебиржевой операции своп; 
                                              «8» – Расчетная сделка бивалютной корзины; 
                                              «9» – Расчетная внебиржевая сделка бивалютной корзины; 
                                              «10» – Сделка по операции РЕПО с ЦК; 
                                              «11» – Первая часть сделки по операции РЕПО с ЦК; 
                                              «12» – Вторая часть сделки по операции РЕПО с ЦК; 
                                              «13» – Адресная сделка по операции РЕПО с ЦК; 
                                              «14» – Первая часть адресной сделки по операции РЕПО с ЦК; 
                                              «15» – Вторая часть адресной сделки по операции РЕПО с ЦК; 
                                              «16» – Техническая сделка по возврату активов РЕПО с ЦК; 
                                              «17» – Сделка по спреду между фьючерсами разных сроков на один актив; 
                                              «18» – Техническая сделка первой части от спреда между фьючерсами; 
                                              «19» – Техническая сделка второй части от спреда между фьючерсами; 
                                              «20» – Адресная сделка первой части РЕПО с корзиной; 
                                              «21» – Адресная сделка второй части РЕПО с корзиной; 
                                              «22» – Перенос позиций срочного рынка */
    LUA_TABLE_FIELD(clearing_bank_accid, std::string) // Идентификатор счета в НКЦ (расчетный код)
    QLUACPP_DATETIME_TABLE_FIELD(canceled_datetime) // Дата и время снятия сделки 
    LUA_TABLE_FIELD(clearing_firmid, std::string) // Идентификатор фирмы - участника клиринга 
    LUA_TABLE_FIELD(system_ref, std::string) // Дополнительная информация по сделке, передаваемая торговой системой 
    LUA_TABLE_FIELD(uid, unsigned int) // Идентификатор пользователя на сервере QUIK

    LUA_TABLE_FIELD(lseccode, std::string) // Приоритетное обеспечение 
    LUA_TABLE_FIELD(order_revision_number, unsigned int) // Номер ревизии заявки, по которой была совершена сделка 
    LUA_TABLE_FIELD(order_qty, unsigned int) // Количество в заявке на момент совершения сделки, в лотах 
    LUA_TABLE_FIELD(order_price, double) // Цена в заявке на момент совершения сделки 
    LUA_TABLE_FIELD(order_exchange_code, std::string) // Биржевой номер заявки 
    LUA_TABLE_FIELD(exec_market, std::string) // Площадка исполнения 
    LUA_TABLE_FIELD(liquidity_indicator, unsigned int) /* Индикатор ликвидности. Возможные значения: 
                                                             «0» – не определено; 
                                                             «1» – по заявке мейкера; 
                                                             «2» – по заявке тейкера; 
                                                             «3» – вывод ликвидности; 
                                                             «4» – по заявке в период аукциона 
                                                          */
 
    LUA_TABLE_FIELD(extref, std::string) // Внешняя ссылка, используется для обратной связи с внешними системами 
    LUA_TABLE_FIELD(ext_trade_flags, unsigned int) // Расширенные флаги, полученные от шлюза напрямую, без вмешательства сервера QUIK. Поле не заполняется 
    LUA_TABLE_FIELD(on_behalf_of_uid, unsigned int) // UID пользователя, от имени которого совершена сделка 
    LUA_TABLE_FIELD(client_qualifier, unsigned int) /* Квалификатор клиента, от имени которого совершена сделка. Возможные значения: 
                                                          «0» – не определено; 
                                                          «1» – Natural Person; 
                                                          «3» – Legal Entity 
                                                       */
 
    LUA_TABLE_FIELD(client_short_code, unsigned int) // Краткий идентификатор клиента, от имени которого совершена сделка 
    LUA_TABLE_FIELD(investment_decision_maker_qualifier, unsigned int) /* Квалификатор принявшего решение о совершении сделки. Возможные значения: 
                                                                             «0» – не определено; 
                                                                             «1» – Natural Person; 
                                                                             «3» – Algorithm 
                                                                          */
 
    LUA_TABLE_FIELD(investment_decision_maker_short_code, unsigned int) // Краткий идентификатор принявшего решение о совершении сделки 
    LUA_TABLE_FIELD(executing_trader_qualifier, unsigned int) /* Квалификатор трейдера, исполнившего заявку, по которой совершена сделка.Возможные значения: 
                                                                    «0» – не определено; 
                                                                    «1» – Natural Person; 
                                                                    «3» – Algorithm 
                                                                 */
 
    LUA_TABLE_FIELD(executing_trader_short_code, unsigned int) // Краткий идентификатор трейдера, исполнившего заявку, по которой совершена сделка 
    LUA_TABLE_FIELD(waiver_flag, unsigned int) /* Признак того, что транзакция совершена по правилам пре-трейда. Возможные значения битовых флагов: 
                                                     бит 0 (0x1) – RFPT; 
                                                     бит 1 (0x2) – NLIQ; 
                                                     бит 2 (0x4) – OILQ; 
                                                     бит 3 (0x8) – PRC; 
                                                     бит 4 (0x10)– SIZE; 
                                                     бит 5 (0x20) – ILQD 
                                                  */
 
    LUA_TABLE_FIELD(mleg_base_sid, unsigned int) // Идентификатор базового инструмента на сервере для multileg-инструментов  
    LUA_TABLE_FIELD(side_qualifier, unsigned int) /* Квалификатор операции. Возможные значения: 
                                                        «0» – не определено; 
                                                        «1» – Buy; 
                                                        «2» – Sell; 
                                                        «3» – Sell short; 
                                                        «4» – Sell short exempt; 
                                                        «5» – Sell undiclosed 
                                                     */
 
    LUA_TABLE_FIELD(otc_post_trade_indicator, unsigned int) /* OTC post-trade индикатор. Возможные значения битовых флагов: 
                                                                  бит 0 (0x1) – Benchmark; 
                                                                  бит 1 (0x2) – Agency cross; 
                                                                  бит 2 (0x4) – Large in scale; 
                                                                  бит 3 (0x8) – Illiquid instrument; 
                                                                  бит 4 (0x10) – Above specified size; 
                                                                  бит 5 (0x20) – Cancellations; 
                                                                  бит 6 (0x40) – Amendments; 
                                                                  бит 7 (0x80) – Special dividend; 
                                                                  бит 8 (0x100) – Price improvement; 
                                                                  бит 9 (0x200) – Duplicative; 
                                                                  бит 10 (0x400) – Not contributing to the price discovery process; 
                                                                  бит 11 (0x800) – Package; 
                                                                  бит 12 (0x1000) – Exchange for Physical 
                                                               */
 
    LUA_TABLE_FIELD(capacity, unsigned int) /* Роль в исполнении заявки. Возможные значения: 
                                                  «0» – не определено; 
                                                  «1» – Agent; 
                                                  «2» – Principal; 
                                                  «3» – Riskless principal; 
                                                  «4» – CFG give up; 
                                                  «5» – Cross as agent; 
                                                  «6» – Matched principal; 
                                                  «7» – Proprietary; 
                                                  «8» – Individual; 
                                                  «9» – Agent for other member; 
                                                  «10» – Mixed; 
                                                  «11» – Market maker
                                               */
 
    LUA_TABLE_FIELD(cross_rate, double) // Кросс-курс валюты цены сделки к валюте расчетов по сделке 

    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::trades)

// stop_orders "Стоп-заявки"
// Object name in qlua.chm: stop_order "Таблица с параметрами стоп-заявки"
//
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(stop_orders)

    LUA_TABLE_FIELD(order_num, std::string) // Регистрационный номер стоп-заявки на сервере QUIK
    LUA_TABLE_FIELD(ordertime, unsigned int) // Время выставления  
    LUA_TABLE_FIELD(flags, unsigned int) // Набор битовых флагов  
    LUA_TABLE_FIELD(brokerref, std::string) // Комментарий, обычно: <код клиента>/<номер поручения> 
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор дилера  
    LUA_TABLE_FIELD(account, std::string) // Торговый счет  
    LUA_TABLE_FIELD(condition, unsigned int) /* Направленность стоп-цены. Возможные значения: 
                                                   «4» – «<=», 
                                                   «5» – «>=» 
                                                */
 
    LUA_TABLE_FIELD(condition_price, double) // Стоп-цена  
    LUA_TABLE_FIELD(price, double) // Цена  
    LUA_TABLE_FIELD(qty, unsigned int) // Количество в лотах  
    LUA_TABLE_FIELD(linkedorder, unsigned int) // Номер заявки в торговой системе, зарегистрированной по наступлению условия стоп-цены  
    LUA_TABLE_FIELD(expiry, int) // Дата окончания срока действия заявки 
    LUA_TABLE_FIELD(trans_id, unsigned int) // Идентификатор транзакции  
    LUA_TABLE_FIELD(client_code, std::string) // Код клиента  
    LUA_TABLE_FIELD(co_order_num, std::string) // Связанная заявка  
    LUA_TABLE_FIELD(co_order_price, double) // Цена связанной заявки  
    LUA_TABLE_FIELD(stop_order_type, unsigned int) /* Вид стоп заявки. Возможные значения: 
                                                         «1» – стоп-лимит; 
                                                         «2» – условие по другому инструменту; 
                                                         «3» – со связанной заявкой; 
                                                         «6» – тейк-профит; 
                                                         «7» – стоп-лимит по исполнению активной заявки; 
                                                         «8» – тейк-профит по исполнению активной заявки; 
                                                         «9» - тейк-профит и стоп-лимит 
                                                      */
 
    LUA_TABLE_FIELD(orderdate, int) // Дата выставления  
    LUA_TABLE_FIELD(alltrade_num, std::string) // Сделка условия  
    LUA_TABLE_FIELD(stopflags, unsigned int) // Набор битовых флагов  
    LUA_TABLE_FIELD(offset, double) // Отступ от min/max  
    LUA_TABLE_FIELD(spread, double) // Защитный спред  
    LUA_TABLE_FIELD(balance, unsigned int) // Активное количество  
    LUA_TABLE_FIELD(uid, unsigned int) // Идентификатор пользователя  
    LUA_TABLE_FIELD(filled_qty, unsigned int) // Исполненное количество  
    LUA_TABLE_FIELD(withdraw_time, int) // Время снятия заявки  
    LUA_TABLE_FIELD(condition_price2, double) // Стоп-лимит цена (для заявок типа «Тэйк-профит и стоп-лимит»)  
    LUA_TABLE_FIELD(active_from_time, int) // Время начала периода действия заявки типа «Тэйк-профит и стоп-лимит»  
    LUA_TABLE_FIELD(active_to_time, int) // Время окончания периода действия заявки типа «Тэйк-профит и стоп-лимит»  
    LUA_TABLE_FIELD(sec_code, std::string) // Код инструмента заявки  
    LUA_TABLE_FIELD(class_code, std::string) // Код класса заявки  
    LUA_TABLE_FIELD(condition_sec_code, std::string) // Код инструмента стоп-цены  
    LUA_TABLE_FIELD(condition_class_code, std::string) // Код класса стоп-цены  
    LUA_TABLE_FIELD(canceled_uid, unsigned int) // Идентификатор пользователя, снявшего стоп-заявку 
    QLUACPP_DATETIME_TABLE_FIELD(order_date_time) // Время выставления стоп-заявки
    QLUACPP_DATETIME_TABLE_FIELD(withdraw_datetime) // Время снятия стоп-заявки
    QLUACPP_DATETIME_TABLE_FIELD(activation_date_time) // Дата и время активации стоп-заявки 
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::stop_orders)

// neg_deals "Заявки на внебиржевые сделки"
// Object names in qlua.chm: "Таблица с параметрами заявки на внебиржевые сделки"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(neg_deals)

    LUA_TABLE_FIELD(neg_deal_num, unsigned int) // Номер  
    LUA_TABLE_FIELD(neg_deal_time, int) // Время выставления заявки  
    LUA_TABLE_FIELD(flags, unsigned int) // Набор битовых флагов  
    LUA_TABLE_FIELD(brokerref, std::string) // Комментарий, обычно: <код клиента>/<номер поручения>  
    LUA_TABLE_FIELD(userid, std::string) // Трейдер  
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор дилера  
    LUA_TABLE_FIELD(cpuserid, std::string) // Трейдер партнера  
    LUA_TABLE_FIELD(cpfirmid, std::string) // Код фирмы партнера  
    LUA_TABLE_FIELD(account, std::string) // Счет  
    LUA_TABLE_FIELD(price, double) // Цена  
    LUA_TABLE_FIELD(qty, unsigned int) // Количество  
    LUA_TABLE_FIELD(matchref, std::string) // Ссылка  
    LUA_TABLE_FIELD(settlecode, std::string) // Код расчетов  
    LUA_TABLE_FIELD(yield, double) // Доходность  
    LUA_TABLE_FIELD(accruedint, double) // Купонный процент  
    LUA_TABLE_FIELD(value, double) // Объем  
    LUA_TABLE_FIELD(price2, double) // Цена выкупа  
    LUA_TABLE_FIELD(reporate, double) // Ставка РЕПО (%)  
    LUA_TABLE_FIELD(refundrate, double) // Ставка возмещения (%)  
    LUA_TABLE_FIELD(trans_id, unsigned int) // ID транзакции  
    LUA_TABLE_FIELD(client_code, std::string) // Код клиента  
    LUA_TABLE_FIELD(repoentry, unsigned int) /* Тип ввода заявки РЕПО. Возможные значения: 
                                                   «0» – «Не определен»; 
                                                   «1» – «Цена1+Ставка»; 
                                                   «2» – «Ставка+ Цена2»; 
                                                   «3» – «Цена1+Цена2»; 
                                                   «4» – «Сумма РЕПО + Количество»; 
                                                   «5» – «Сумма РЕПО + Дисконт»; 
                                                   «6» – «Количество + Дисконт»; 
                                                   «7» – «Сумма РЕПО»; 
                                                   «8» – «Количество» 
                                                */

    LUA_TABLE_FIELD(repovalue, double) // Сумма РЕПО  
    LUA_TABLE_FIELD(repo2value, double) // Объем выкупа РЕПО  
    LUA_TABLE_FIELD(repoterm, double) // Срок РЕПО  
    LUA_TABLE_FIELD(start_discount, double) // Начальный дисконт (%) 
    LUA_TABLE_FIELD(lower_discount, double) // Нижний дисконт (%)  
    LUA_TABLE_FIELD(upper_discount, double) // Верхний дисконт (%)  
    LUA_TABLE_FIELD(block_securities, unsigned int) // Блокировка обеспечения («Да»/«Нет»)  
    LUA_TABLE_FIELD(uid, unsigned int) // Идентификатор пользователя  
    LUA_TABLE_FIELD(withdraw_time, int) // Время снятия заявки  
    LUA_TABLE_FIELD(neg_deal_date, int) // Дата выставления заявки  
    LUA_TABLE_FIELD(balance, double) // Остаток  
    LUA_TABLE_FIELD(origin_repovalue, double) // Сумма РЕПО первоначальная  
    LUA_TABLE_FIELD(origin_qty, unsigned int) // Количество первоначальное  
    LUA_TABLE_FIELD(origin_discount, double) // Процент дисконта первоначальный  
    LUA_TABLE_FIELD(neg_deal_activation_date, int) // Дата активации заявки  
    LUA_TABLE_FIELD(neg_deal_activation_time, int) // Время активации заявки  
    LUA_TABLE_FIELD(quoteno, unsigned int) // Встречная безадресная заявка  
    LUA_TABLE_FIELD(settle_currency, std::string) // Валюта расчетов  
    LUA_TABLE_FIELD(sec_code, std::string) // Код инструмента  
    LUA_TABLE_FIELD(class_code, std::string) // Код класса  
    LUA_TABLE_FIELD(bank_acc_id, std::string) // Идентификатор расчетного счета/кода в клиринговой организации  
    LUA_TABLE_FIELD(withdraw_date, int) // Дата снятия адресной заявки в формате «ГГГГММДД»  
    LUA_TABLE_FIELD(linkedorder, unsigned int) // Номер предыдущей заявки. Отображается с точностью «0»  
    QLUACPP_DATETIME_TABLE_FIELD(activation_date_time) // Дата и время активации заявки 
    QLUACPP_DATETIME_TABLE_FIELD(withdraw_date_time) // Дата и время снятия заявки 
    QLUACPP_DATETIME_TABLE_FIELD(date_time) // Дата и время заявки
    LUA_TABLE_FIELD(lseccode, std::string) // Приоритетное обеспечение 
    LUA_TABLE_FIELD(canceled_uid, unsigned int) // UID снявшего заявку 
    LUA_TABLE_FIELD(system_ref, std::string) // Системная ссылка 
    LUA_TABLE_FIELD(price_currency, std::string) // Валюта, в которой указана цена заявки 
    LUA_TABLE_FIELD(order_exchange_code, std::string) // Биржевой номер заявки 
    LUA_TABLE_FIELD(extref, std::string) // Внешняя ссылка, используется для обратной связи с внешними системами 
    LUA_TABLE_FIELD(period, unsigned int) // Период торговой сессии, в которую была подана заявка 
    LUA_TABLE_FIELD(client_qualifier, unsigned int) /* Квалификатор клиента, от имени которого выставлена заявка. Возможные значения: 
                                                          «0» – не определено; 
                                                          «1» – Natural Person; 
                                                          «3» – Legal Entity 
                                                       */
 
    LUA_TABLE_FIELD(client_short_code, unsigned int) // Краткий идентификатор клиента, от имени которого выставлена заявка 
    LUA_TABLE_FIELD(investment_decision_maker_qualifier, unsigned int) /* Квалификатор принявшего решение о выставлении заявки. Возможные значения: 
                                                                             «0» – не определено; 
                                                                             «1» – Natural Person; 
                                                                             «3» – Algorithm 
                                                                          */
    LUA_TABLE_FIELD(investment_decision_maker_short_code, unsigned int) // Краткий идентификатор принявшего решение о выставлении заявки 
    LUA_TABLE_FIELD(executing_trader_qualifier, unsigned int) /* Квалификатор трейдера, исполнившего заявку. Возможные значения: 
                                                                    «0» – не определено; 
                                                                    «1» – Natural Person; 
                                                                    «3» – Algorithm 
                                                                 */
    LUA_TABLE_FIELD(executing_trader_short_code, unsigned int) // Краткий идентификатор трейдера, исполнившего заявку 
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::neg_deals)

// neg_trades "Сделки для исполнения"
// Object names in qlua.chm "Таблица с параметрами сделки для исполнения"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(neg_trades)
    LUA_TABLE_FIELD(trade_num, std::string) // Номер сделки  
    LUA_TABLE_FIELD(trade_date, int) // Дата торгов  
    LUA_TABLE_FIELD(settle_date, int) // Дата расчетов  
    LUA_TABLE_FIELD(flags, unsigned int) // Набор битовых флагов  
    LUA_TABLE_FIELD(brokerref, std::string) // Комментарий, обычно: <код клиента>/<номер поручения>  
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор дилера  
    LUA_TABLE_FIELD(account, std::string) // Счет депо  
    LUA_TABLE_FIELD(cpfirmid, std::string) // Код фирмы партнера  
    LUA_TABLE_FIELD(cpaccount, std::string) // Счет депо партнера  
    LUA_TABLE_FIELD(price, double) // Цена  
    LUA_TABLE_FIELD(qty, unsigned int) // Количество  
    LUA_TABLE_FIELD(value, double) // Объем  
    LUA_TABLE_FIELD(settlecode, std::string) // Код расчетов  
    LUA_TABLE_FIELD(report_num, unsigned int) // Отчет  
    LUA_TABLE_FIELD(cpreport_num, unsigned int) // Отчет партнера  
    LUA_TABLE_FIELD(accruedint, double) // Купонный процент  
    LUA_TABLE_FIELD(repotradeno, double) // Номер сделки 1-ой части РЕПО  
    LUA_TABLE_FIELD(price1, double) // Цена 1-ой части РЕПО  
    LUA_TABLE_FIELD(reporate, double) // Ставка РЕПО (%)  
    LUA_TABLE_FIELD(price2, double) // Цена выкупа  
    LUA_TABLE_FIELD(client_code, std::string) // Код клиента  
    LUA_TABLE_FIELD(ts_comission, double) // Комиссия торговой системы  
    LUA_TABLE_FIELD(balance, double) // Остаток  
    LUA_TABLE_FIELD(settle_time, int) // Время исполнения  
    LUA_TABLE_FIELD(amount, double) // Сумма обязательства  
    LUA_TABLE_FIELD(repovalue, double) // Сумма РЕПО  
    LUA_TABLE_FIELD(repoterm, double) // Срок РЕПО  
    LUA_TABLE_FIELD(repo2value, double) // Объем выкупа РЕПО  
    LUA_TABLE_FIELD(return_value, double) // Сумма возврата РЕПО  
    LUA_TABLE_FIELD(discount, double) // Дисконт (%)  
    LUA_TABLE_FIELD(lower_discount, double) // Нижний дисконт (%)  
    LUA_TABLE_FIELD(upper_discount, double) // Верхний дисконт (%)  
    LUA_TABLE_FIELD(block_securities, unsigned int) // Блокировать обеспечение («Да»/«Нет»)  
    LUA_TABLE_FIELD(urgency_flag, unsigned int) // Исполнить («Да»/«Нет»)  
    LUA_TABLE_FIELD(type, unsigned int) /* Тип. Возможные значения: 
                                              «0» – «Внесистемная сделка», 
                                              «1» – «Первая часть сделки РЕПО», 
                                              «2» – «Вторая часть сделки РЕПО», 
                                              «3» – «Компенсационный взнос», 
                                              «4» – «Дефолтер: отложенные обязательства и требования», 
                                              «5» – «Пострадавший: отложенные обязательства и требования». 
                                           */
 
    LUA_TABLE_FIELD(operation_type, unsigned int) /* Направленность. Возможные значения: 
                                                        «1» – «Зачислить», 
                                                        «2» – «Списать». 
                                                     */
 
    LUA_TABLE_FIELD(expected_discount, double) // Дисконт после взноса (%)  
    LUA_TABLE_FIELD(expected_quantity, unsigned int) // Количество после взноса  
    LUA_TABLE_FIELD(expected_repovalue, double) // Сумма РЕПО после взноса  
    LUA_TABLE_FIELD(expected_repo2value, double) // Стоимость выкупа после взноса  
    LUA_TABLE_FIELD(expected_return_value, double) // Сумма возврата после взноса  
    LUA_TABLE_FIELD(order_num, std::string) // Номер заявки  
    LUA_TABLE_FIELD(report_trade_date, int) // Дата заключения  
    LUA_TABLE_FIELD(settled, unsigned int) /* Состояние расчетов по сделке. Возможные значения: 
                                                 «1» – «Processed», 
                                                 «2» – «Not processed», 
                                                 «3» – «Is processing». 
                                              */
 
    LUA_TABLE_FIELD(clearing_type, unsigned int) /* Тип клиринга. Возможные значения: 
                                                       «1» – «Not set», 
                                                       «2» – «Simple», 
                                                       «3» – «Multilateral». 
                                                    */
 
    LUA_TABLE_FIELD(report_comission, double) // Комиссия за отчет  
    LUA_TABLE_FIELD(coupon_payment, double) // Купонная выплата  
    LUA_TABLE_FIELD(principal_payment, double) // Выплата по основному долгу  
    LUA_TABLE_FIELD(principal_payment_date, int) // Дата выплаты по основному долгу  
    LUA_TABLE_FIELD(nextdaysettle, int) // Дата следующего дня расчетов  
    LUA_TABLE_FIELD(settle_currency, std::string) // Валюта расчетов  
    LUA_TABLE_FIELD(sec_code, std::string) // Код инструмента  
    LUA_TABLE_FIELD(class_code, std::string) // Код класса  
    LUA_TABLE_FIELD(compval, double) // Сумма отступного в валюте сделки 
    LUA_TABLE_FIELD(parenttradeno, unsigned int) // Идентификационный номер витринной сделки 
    LUA_TABLE_FIELD(bankid, std::string) // Расчетная организация 
    LUA_TABLE_FIELD(bankaccid, std::string) // Код позиции 
    LUA_TABLE_FIELD(precisebalance, unsigned int) // Количество инструментов к исполнению (в лотах) 
    LUA_TABLE_FIELD(confirmtime, int) // Время подтверждения в формате «ЧЧММСС» 
    LUA_TABLE_FIELD(ex_flags, unsigned int) /* Расширенные флаги сделки для исполнения. Возможные значения: 
                                                  «1» – «Подтверждена контрагентом»; 
                                                  «2» – «Подтверждена» 
                                               */
    LUA_TABLE_FIELD(confirmreport, unsigned int) // Номер поручения 
    LUA_TABLE_FIELD(extref, std::string) // Внешняя ссылка, используется для обратной связи с внешними системами 
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::neg_trades)

// neg_deal_reports "Отчеты по сделкам для исполнения"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(neg_deal_reports)
    LUA_TABLE_FIELD(report_num, unsigned int) // Отчет  
    LUA_TABLE_FIELD(report_date, int) // Дата отчета  
    LUA_TABLE_FIELD(flags, unsigned int) // Набор битовых флагов  
    LUA_TABLE_FIELD(userid, std::string) // Идентификатор пользователя  
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы  
    LUA_TABLE_FIELD(account, std::string) // Счет депо  
    LUA_TABLE_FIELD(cpfirmid, std::string) // Код фирмы партнера  
    LUA_TABLE_FIELD(cpaccount, std::string) // Код торгового счета партнера  
    LUA_TABLE_FIELD(qty, unsigned int) // Количество бумаг, в лотах  
    LUA_TABLE_FIELD(value, double) // Объем сделки, выраженный в рублях  
    LUA_TABLE_FIELD(withdraw_time, int) // Время снятия заявки  
    LUA_TABLE_FIELD(report_type, unsigned int) // Тип отчета  
    LUA_TABLE_FIELD(report_kind, unsigned int) // Вид отчета  
    LUA_TABLE_FIELD(commission, double) // Объем комиссии по сделке, выраженный в руб  
    LUA_TABLE_FIELD(sec_code, std::string) // Код бумаги 
    LUA_TABLE_FIELD(class_code, std::string) // Код класса 
    LUA_TABLE_FIELD(report_time, int) // Время отчета 
    QLUACPP_DATETIME_TABLE_FIELD(report_date_time) // Дата и время отчета
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::neg_deal_reports)

// firm_holding "Текущие позиции по бумагам"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(firm_holding)
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы  
    LUA_TABLE_FIELD(sec_code, std::string) // Код бумаги  
    LUA_TABLE_FIELD(openbal, double) // Входящий остаток  
    LUA_TABLE_FIELD(currentpos, double) // Текущий остаток  
    LUA_TABLE_FIELD(plannedposbuy, double) // Объем активных заявок на покупку, в ценных бумагах  
    LUA_TABLE_FIELD(plannedpossell, double) // Объем активных заявок на продажу, в ценных бумагах  
    LUA_TABLE_FIELD(usqtyb, double) // Куплено  
    LUA_TABLE_FIELD(usqtys, double) // Продано
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::firm_holding)

// account_balance "Текущиее позиции по клиентским счетам"
// Объект фигурирует в qlua.chm как acc_bal
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(account_balance)
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы  
    LUA_TABLE_FIELD(sec_code, std::string) // Код бумаги  
    LUA_TABLE_FIELD(trdaccid, std::string) // Торговый счет  
    LUA_TABLE_FIELD(depaccid, std::string) // Счет депо  
    LUA_TABLE_FIELD(openbal, double) // Входящий остаток  
    LUA_TABLE_FIELD(currentpos, double) // Текущий остаток  
    LUA_TABLE_FIELD(plannedpossell, double) // Плановая продажа  
    LUA_TABLE_FIELD(plannedposbuy, double) // Плановая покупка  
    LUA_TABLE_FIELD(planbal, double) // Контрольный остаток простого клиринга, равен входящему остатку минус плановая позиция на продажу, включенная в простой клиринг  
    LUA_TABLE_FIELD(usqtyb, unsigned int) // Куплено  
    LUA_TABLE_FIELD(usqtys, unsigned int) // Продано  
    LUA_TABLE_FIELD(planned, double) // Плановый остаток, равен текущему остатку минус плановая позиция на продажу  
    LUA_TABLE_FIELD(settlebal, double) // Плановая позиция после проведения расчетов  
    LUA_TABLE_FIELD(bank_acc_id, std::string) // Идентификатор расчетного счета/кода в клиринговой организации  
    LUA_TABLE_FIELD(firmuse, unsigned int) /* Признак счета обеспечения. Возможные значения: 
                                                 «0» – для обычных счетов, 
                                                 «1» – для счета обеспечения. */
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::account_balance)
  
// ccp_holdings "Обязательства и требования по активам"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(ccp_holdings)
    LUA_TABLE_FIELD(firmid, std::string) // Идентификатор фирмы  
    LUA_TABLE_FIELD(depo_account, std::string) // Номер счета депо в Депозитарии (НДЦ)  
    LUA_TABLE_FIELD(account, std::string) // Торговый счет  
    LUA_TABLE_FIELD(bank_acc_id, std::string) // Идентификатор расчетного счета/кода в клиринговой организации  
    LUA_TABLE_FIELD(settle_date, int) // Дата расчетов  
    LUA_TABLE_FIELD(qty, unsigned int) // Количество ценных бумаг в сделках  
    LUA_TABLE_FIELD(qty_buy, unsigned int) // Количество ценных бумаг в заявках на покупку  
    LUA_TABLE_FIELD(qty_sell, unsigned int) // Количество ценных бумаг в заявках на продажу  
    LUA_TABLE_FIELD(netto, double) // Нетто-позиция  
    LUA_TABLE_FIELD(debit, double) // Дебет  
    LUA_TABLE_FIELD(credit, double) // Кредит  
    LUA_TABLE_FIELD(sec_code, std::string) // Код бумаги заявки  
    LUA_TABLE_FIELD(class_code, std::string) // Код класса заявки  
    LUA_TABLE_FIELD(planned_covered, double) // Плановая позиция Т+  
    LUA_TABLE_FIELD(firm_use, double) /*  Тип раздела. Возможные значения: 
                                             «0» – торговый раздел; 
                                             «1» – раздел обеспечения */
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::ccp_holdings)

// rm_holdings "Валюта: обящательства и требования по активам"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(rm_holdings)
    LUA_TABLE_FIELD(sec_code, std::string) // Код бумаги  
    LUA_TABLE_FIELD(class_code, std::string) // Код класса  
    LUA_TABLE_FIELD(firmId, std::string) // Идентификатор фирмы  
    LUA_TABLE_FIELD(account, std::string) // Торговый счет  
    LUA_TABLE_FIELD(bank_acc_id, std::string) // Идентификатор расчетного счета в НКЦ  
    LUA_TABLE_FIELD(date, int) // Дата расчётов  
    LUA_TABLE_FIELD(debit, double) // Размер денежных обязательств  
    LUA_TABLE_FIELD(credit, double) // Размер денежных требований  
    LUA_TABLE_FIELD(value_buy, double) // Сумма денежных средств в заявках на покупку  
    LUA_TABLE_FIELD(value_sell, double) // Сумма денежных средств в заявках на продажу  
    LUA_TABLE_FIELD(margin_call, double) // Сумма возврата компенсационного перевода  
    LUA_TABLE_FIELD(planned_covered, double) // Плановая позиция Т+  
    LUA_TABLE_FIELD(debit_balance, double) // Размер денежных обязательств на начало дня, с точностью до 2 знака после десятичного разделителя  
    LUA_TABLE_FIELD(credit_balance, double) // Размер денежных требований на начало дня, с точностью до 2 знака после десятичного разделителя
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::rm_holdings)

// trans_reply "Таблица с описанием транзакций" (callback)
// Object names in qlua.chm: trans_reply
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(trans_reply)
    LUA_TABLE_FIELD(trans_id, unsigned int) // Пользовательский идентификатор транзакции 
    LUA_TABLE_FIELD(status, unsigned int) /* Статус транзакции. Возможные значения: 
                                                «0» – транзакция отправлена серверу; 
                                                «1» – транзакция получена на сервер QUIK от клиента; 
                                                «2» – ошибка при передаче транзакции в торговую систему. Так как отсутствует подключение шлюза Московской Биржи, повторно транзакция не отправляется; 
                                                «3» – транзакция выполнена; 
                                                «4» – транзакция не выполнена торговой системой. Более подробное описание ошибки отражается в поле «Сообщение»; 
                                                «5» – транзакция не прошла проверку сервера QUIK по каким-либо критериям. Например, проверку на наличие прав у пользователя на отправку транзакции данного типа; 
                                                «6» – транзакция не прошла проверку лимитов сервера QUIK; 
                                                «10» – транзакция не поддерживается торговой системой; 
                                                «11» – транзакция не прошла проверку правильности электронной цифровой подписи; 
                                                «12» – не удалось дождаться ответа на транзакцию, т.к. истек таймаут ожидания. Может возникнуть при подаче транзакций из QPILE; 
                                                «13» – транзакция отвергнута, так как ее выполнение могло привести к кросс-сделке (т.е. сделке с тем же самым клиентским счетом); 
                                                «14» – транзакция не прошла контроль дополнительных ограничений; 
                                                «15» – транзакция принята после нарушения дополнительных ограничений; 
                                                «16» – транзакция отменена пользователем в ходе проверки дополнительных ограничений */
 
    LUA_TABLE_FIELD(result_msg, std::string) // Сообщение
    QLUACPP_DATETIME_TABLE_FIELD(date_time) // Дата и время 
    LUA_TABLE_FIELD(uid, unsigned int) // Идентификатор 
    LUA_TABLE_FIELD(flags, unsigned int) // Флаги транзакции (временно не используется) 
    LUA_TABLE_FIELD(server_trans_id, unsigned int) // Идентификатор транзакции на сервере 
    LUA_TABLE_FIELD(order_num, std::optional<std::string>) // Номер заявки *
    LUA_TABLE_FIELD(price, std::optional<double>) // Цена *
    LUA_TABLE_FIELD(quantity, std::optional<unsigned int>) // Количество *
    LUA_TABLE_FIELD(balance, std::optional<unsigned int>) // Остаток *
    LUA_TABLE_FIELD(firm_id, std::optional<std::string>) // Идентификатор фирмы *
    LUA_TABLE_FIELD(account, std::optional<std::string>) // Торговый счет *
    LUA_TABLE_FIELD(client_code, std::optional<std::string>) // Код клиента * 
    LUA_TABLE_FIELD(brokerref, std::optional<std::string>) // Поручение *
    LUA_TABLE_FIELD(class_code, std::optional<std::string>) // Код класса *
    LUA_TABLE_FIELD(sec_code, std::optional<std::string>) // Код бумаги *
    LUA_TABLE_FIELD(exchange_code, std::optional<std::string>) /* Биржевой номер заявки *
                                                                   * - параметр может иметь значение nil
                                                                   */
    LUA_TABLE_FIELD(error_code, int) // Числовой код ошибки. Значение равно «0», если транзакция выполнена успешно 
    LUA_TABLE_FIELD(error_source, unsigned int) /* Источник сообщения. Возможные значения: 
                                                      «1» – Торговая система; 
                                                      «2» – Сервер QUIK; 
                                                      «3» – Библиотека расчёта лимитов; 
                                                      «4» – Шлюз торговой системы */
    
    LUA_TABLE_FIELD(first_ordernum, double) // Номер первой заявки, которая выставлялась при автоматической замене кода клиента. Используется, если на сервере QUIK настроена замена кода клиента для кросс-сделки
    QLUACPP_DATETIME_TABLE_FIELD(gate_reply_time) // Дата и время получения шлюзом ответа на транзакцию
    LUA_TABLE_END
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::trans_reply)

namespace qlua {
  namespace table {
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
  
namespace qlua {
  namespace table {
    namespace detail {
      template <typename Table>
      struct name_for_type;


      QLUACPP_DETAIL_TABLE_NAME(firms);
      QLUACPP_DETAIL_TABLE_NAME(classes);
      QLUACPP_DETAIL_TABLE_NAME(securities);
      QLUACPP_DETAIL_TABLE_NAME(trade_accounts);
      QLUACPP_DETAIL_TABLE_NAME(client_codes);
      QLUACPP_DETAIL_TABLE_NAME(account_positions);
      QLUACPP_DETAIL_TABLE_NAME(orders);
      QLUACPP_DETAIL_TABLE_NAME(futures_client_holding);
      QLUACPP_DETAIL_TABLE_NAME(futures_client_limits);
      QLUACPP_DETAIL_TABLE_NAME(all_trades);
      QLUACPP_DETAIL_TABLE_NAME(money_limits);
      QLUACPP_DETAIL_TABLE_NAME(depo_limits);
      QLUACPP_DETAIL_TABLE_NAME(trades);
      QLUACPP_DETAIL_TABLE_NAME(stop_orders);
      QLUACPP_DETAIL_TABLE_NAME(neg_deals);
      QLUACPP_DETAIL_TABLE_NAME(neg_trades);
      QLUACPP_DETAIL_TABLE_NAME(neg_deal_reports);
      QLUACPP_DETAIL_TABLE_NAME(firm_holding);
      QLUACPP_DETAIL_TABLE_NAME(account_balance);
      QLUACPP_DETAIL_TABLE_NAME(ccp_holdings);
      QLUACPP_DETAIL_TABLE_NAME(rm_holdings);
    }
  }
}

#undef QLUACPP_DETAIL_TABLE_NAME
