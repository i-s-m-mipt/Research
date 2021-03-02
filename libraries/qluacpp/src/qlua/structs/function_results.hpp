#pragma once

#include <exception>
#include <stdexcept>
#include <string>

namespace qlua 
{
    namespace table 
    {
        LUA_TABLE_BEGIN(Money)
        LUA_TABLE_FIELD(money_open_limit, double)
        LUA_TABLE_FIELD(money_limit_locked_nonmarginal_value, double)
        LUA_TABLE_FIELD(money_limit_locked, double)
        LUA_TABLE_FIELD(money_open_balance, double)
        LUA_TABLE_FIELD(money_current_limit, double)
        LUA_TABLE_FIELD(money_current_balance, double)
        LUA_TABLE_FIELD(money_limit_available, double)
        LUA_TABLE_END

        LUA_TABLE_BEGIN(Depo)
        LUA_TABLE_FIELD(depo_limit_locked_buy_value, double)
        LUA_TABLE_FIELD(depo_current_balance, double)
        LUA_TABLE_FIELD(depo_limit_locked_buy, double)
        LUA_TABLE_FIELD(depo_limit_locked, double)
        LUA_TABLE_FIELD(depo_limit_available, double)
        LUA_TABLE_FIELD(depo_current_limit, double)
        LUA_TABLE_FIELD(depo_open_balance, double)
        LUA_TABLE_FIELD(depo_open_limit, double)
        LUA_TABLE_END
    }
}

LUA_TABLE_TYPE_ADAPTER(qlua::table::Depo)
LUA_TABLE_TYPE_ADAPTER(qlua::table::Money)

// portfolio_info_getPortfolioInfo (table returned from getPortfolioInfo)
namespace qlua {
    namespace table {
        LUA_TABLE_BEGIN(portfolio_info_getPortfolioInfo)
            LUA_TABLE_FIELD(is_leverage, std::string) /*Признак использования схемы кредитования с контролем текущей стоимости активов. Возможные значения:
                                                           «МЛ» – используется схема ведения позиции «по плечу», «плечо» рассчитано по значению Входящего лимита,
                                                           «МП» – используется схема ведения позиции «по плечу», «плечо» указано явным образом,
                                                           «МОП» – используется схема ведения позиции «лимит на открытую позицию»;
                                                           «МД» – используется схема ведения позиции «по дисконтам»;
                                                           «C» – используется схема ведения позиций «срочный рынок». Для клиентов срочного рынка без единой денежной позиции;
                                                           <пусто> – используется схема ведения позиции «по лимитам»
                                                           Тип клиента */
            LUA_TABLE_FIELD(in_assets, std::string) // Оценка собственных средств клиента до начала торгов Вход. активы 
            LUA_TABLE_FIELD(leverage, std::string) // Плечо. Если не задано явно, то отношение Входящего лимита к Входящим активам Плечо 
            LUA_TABLE_FIELD(open_limit, std::string) // Оценка максимальной величины заемных средств до начала торгов Вход. лимит 
            LUA_TABLE_FIELD(val_short, std::string) // Оценка стоимости коротких позиций. Значение всегда отрицательное Шорты 
            LUA_TABLE_FIELD(val_long, std::string) // Оценка стоимости длинных позиций Лонги 
            LUA_TABLE_FIELD(val_long_margin, std::string) // Оценка стоимости длинных позиций по маржинальным бумагам, принимаемым в обеспечение Лонги МО 
            LUA_TABLE_FIELD(val_long_asset, std::string) // Оценка стоимости длинных позиций по немаржинальным бумагам, принимаемым в обеспечение Лонги О 
            LUA_TABLE_FIELD(assets, std::string) // Оценка собственных средств клиента по текущим позициям и ценам Тек. активы 
            LUA_TABLE_FIELD(cur_leverage, std::string) // Текущее плечо Тек.плечо 
            LUA_TABLE_FIELD(margin, std::string) // Уровень маржи, в процентах Ур. маржи 
            LUA_TABLE_FIELD(lim_all, std::string) // Текущая оценка максимальной величины заемных средств Тек. лимит 
            LUA_TABLE_FIELD(av_lim_all, std::string) // Оценка величины заемных средств, доступных для дальнейшего открытия позиций ДостТекЛимит 
            LUA_TABLE_FIELD(locked_buy, std::string) // Оценка стоимости активов в заявках на покупку Блок. покупка 
            LUA_TABLE_FIELD(locked_buy_margin, std::string) // Оценка стоимости активов в заявках на покупку маржинальных бумаг, принимаемых в обеспечение Блок. пок. маржин. 
            LUA_TABLE_FIELD(locked_buy_asset, std::string) // Оценка стоимости активов в заявках на покупку немаржинальных бумаг, принимаемых в обеспечение Блок.пок. обесп. 
            LUA_TABLE_FIELD(locked_sell, std::string) // Оценка стоимости активов в заявках на продажу маржинальных бумаг Блок. продажа 
            LUA_TABLE_FIELD(locked_value_coef, std::string) // Оценка стоимости активов в заявках на покупку немаржинальных бумаг Блок. пок. немарж. 
            LUA_TABLE_FIELD(in_all_assets, std::string) // Оценка стоимости всех позиций клиента в ценах закрытия предыдущей торговой сессии, включая позиции по немаржинальным бумагам ВходСредства 
            LUA_TABLE_FIELD(all_assets, std::string) // Текущая оценка стоимости всех позиций клиента ТекСредства 
            LUA_TABLE_FIELD(profit_loss, std::string) // Абсолютная величина изменения стоимости всех позиций клиента Прибыль/убытки 
            LUA_TABLE_FIELD(rate_change, std::string) // Относительная величина изменения стоимости всех позиций клиента ПроцИзмен 
            LUA_TABLE_FIELD(lim_buy, std::string) // Оценка денежных средств, доступных для покупки маржинальных бумаг На покупку 
            LUA_TABLE_FIELD(lim_sell, std::string) // Оценка стоимости маржинальных бумаг, доступных для продажи На продажу 
            LUA_TABLE_FIELD(lim_non_margin, std::string) // Оценка денежных средств, доступных для покупки немаржинальных бумаг НаПокупНеМаржин 
            LUA_TABLE_FIELD(lim_buy_asset, std::string) // Оценка денежных средств, доступных для покупки бумаг, принимаемых в обеспечение НаПокупОбесп 
            LUA_TABLE_FIELD(val_short_net, std::string) // Оценка стоимости коротких позиций. При расчете не используется коэффициент дисконтирования ** Шорты (нетто) 
            LUA_TABLE_FIELD(val_long_net, std::string) // Оценка стоимости длинных позиций. При расчете не используется коэффициент дисконтирования ** Лонги (нетто) 
            LUA_TABLE_FIELD(total_money_bal, std::string) // Сумма остатков по денежным средствам по всем лимитам, без учета средств, заблокированных под исполнение обязательств, выраженная в выбранной валюте расчета Сумма ден. остатков 
            LUA_TABLE_FIELD(total_locked_money, std::string) // Cумма заблокированных средств со всех денежных лимитов клиента, пересчитанная в валюту расчетов через кросс-курсы на сервере Суммарно заблок. 
            LUA_TABLE_FIELD(haircuts, std::string) // Сумма дисконтов стоимости длинных (только по бумагам обеспечения) и коротких бумажных позиций, дисконтов корреляции между инструментами, а также дисконтов на задолженности по валютам, не покрытые бумажным обеспечением в этих же валютах Сумма дисконтов 
            LUA_TABLE_FIELD(assets_without_hc, std::string) // Суммарная величина денежных остатков, стоимости длинных позиций по бумагам обеспечения и стоимости коротких позиций, без учета дисконтирующих коэффициентов, без учета неттинга стоимости бумаг в рамках объединенной бумажной позиции и без учета корреляции между инструментами ТекАктБезДиск 
            LUA_TABLE_FIELD(status_coef, std::string) // Отношение суммы дисконтов к текущим активам без учета дисконтов Статус счета 
            LUA_TABLE_FIELD(varmargin, std::string) // Текущая вариационная маржа по позициям клиента, по всем инструментам Вариац. маржа 
            LUA_TABLE_FIELD(go_for_positions, std::string) // Размер денежных средств, уплаченных под все открытые позиции на срочном рынке ГО поз. 
            LUA_TABLE_FIELD(go_for_orders, std::string) // Оценка стоимости активов в заявках на срочном рынке ГО заяв. 
            LUA_TABLE_FIELD(rate_futures, std::string) // Отношение ликвидационной стоимости портфеля к ГО по срочному рынку Активы/ГО 
            LUA_TABLE_FIELD(is_qual_client, std::string) // Признак «квалифицированного» клиента, которому разрешено кредитование заемными средствами с плечом 1:3. Возможные значения: «ПовышУрРиска» – квалифицированный, <пусто> – нет ПовышУрРиска 
            LUA_TABLE_FIELD(is_futures, std::string) // Счет клиента на FORTS, в случае наличия объединенной позиции, иначе поле остается пустым Сроч. счет 
            LUA_TABLE_FIELD(curr_tag, std::string) // Актуальные текущие параметры расчета для данной строки в формате «<Валюта>-<Тег расчётов>». Пример: «SUR-EQTV» Парам
            LUA_TABLE_END
    }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::portfolio_info_getPortfolioInfo)

// portfolio_info_getPortfolioInfoEx (table returned from getPortfolioInfoEx)
namespace qlua {
    namespace table {
        LUA_TABLE_BEGIN(portfolio_info_getPortfolioInfoEx)
            LUA_TABLE_FIELD(is_leverage, std::string) /*Признак использования схемы кредитования с контролем текущей стоимости активов. Возможные значения:
                                                           «МЛ» – используется схема ведения позиции «по плечу», «плечо» рассчитано по значению Входящего лимита,
                                                           «МП» – используется схема ведения позиции «по плечу», «плечо» указано явным образом,
                                                           «МОП» – используется схема ведения позиции «лимит на открытую позицию»;
                                                           «МД» – используется схема ведения позиции «по дисконтам»;
                                                           «C» – используется схема ведения позиций «срочный рынок». Для клиентов срочного рынка без единой денежной позиции;
                                                           <пусто> – используется схема ведения позиции «по лимитам»
                                                           Тип клиента */
            LUA_TABLE_FIELD(in_assets, std::string) // Оценка собственных средств клиента до начала торгов Вход. активы 
            LUA_TABLE_FIELD(leverage, std::string) // Плечо. Если не задано явно, то отношение Входящего лимита к Входящим активам Плечо 
            LUA_TABLE_FIELD(open_limit, std::string) // Оценка максимальной величины заемных средств до начала торгов Вход. лимит 
            LUA_TABLE_FIELD(val_short, std::string) // Оценка стоимости коротких позиций. Значение всегда отрицательное Шорты 
            LUA_TABLE_FIELD(val_long, std::string) // Оценка стоимости длинных позиций Лонги 
            LUA_TABLE_FIELD(val_long_margin, std::string) // Оценка стоимости длинных позиций по маржинальным бумагам, принимаемым в обеспечение Лонги МО 
            LUA_TABLE_FIELD(val_long_asset, std::string) // Оценка стоимости длинных позиций по немаржинальным бумагам, принимаемым в обеспечение Лонги О 
            LUA_TABLE_FIELD(assets, std::string) // Оценка собственных средств клиента по текущим позициям и ценам Тек. активы 
            LUA_TABLE_FIELD(cur_leverage, std::string) // Текущее плечо Тек.плечо 
            LUA_TABLE_FIELD(margin, std::string) // Уровень маржи, в процентах Ур. маржи 
            LUA_TABLE_FIELD(lim_all, std::string) // Текущая оценка максимальной величины заемных средств Тек. лимит 
            LUA_TABLE_FIELD(av_lim_all, std::string) // Оценка величины заемных средств, доступных для дальнейшего открытия позиций ДостТекЛимит 
            LUA_TABLE_FIELD(locked_buy, std::string) // Оценка стоимости активов в заявках на покупку Блок. покупка 
            LUA_TABLE_FIELD(locked_buy_margin, std::string) // Оценка стоимости активов в заявках на покупку маржинальных бумаг, принимаемых в обеспечение Блок. пок. маржин. 
            LUA_TABLE_FIELD(locked_buy_asset, std::string) // Оценка стоимости активов в заявках на покупку немаржинальных бумаг, принимаемых в обеспечение Блок.пок. обесп. 
            LUA_TABLE_FIELD(locked_sell, std::string) // Оценка стоимости активов в заявках на продажу маржинальных бумаг Блок. продажа 
            LUA_TABLE_FIELD(locked_value_coef, std::string) // Оценка стоимости активов в заявках на покупку немаржинальных бумаг Блок. пок. немарж. 
            LUA_TABLE_FIELD(in_all_assets, std::string) // Оценка стоимости всех позиций клиента в ценах закрытия предыдущей торговой сессии, включая позиции по немаржинальным бумагам ВходСредства 
            LUA_TABLE_FIELD(all_assets, std::string) // Текущая оценка стоимости всех позиций клиента ТекСредства 
            LUA_TABLE_FIELD(profit_loss, std::string) // Абсолютная величина изменения стоимости всех позиций клиента Прибыль/убытки 
            LUA_TABLE_FIELD(rate_change, std::string) // Относительная величина изменения стоимости всех позиций клиента ПроцИзмен 
            LUA_TABLE_FIELD(lim_buy, std::string) // Оценка денежных средств, доступных для покупки маржинальных бумаг На покупку 
            LUA_TABLE_FIELD(lim_sell, std::string) // Оценка стоимости маржинальных бумаг, доступных для продажи На продажу 
            LUA_TABLE_FIELD(lim_non_margin, std::string) // Оценка денежных средств, доступных для покупки немаржинальных бумаг НаПокупНеМаржин 
            LUA_TABLE_FIELD(lim_buy_asset, std::string) // Оценка денежных средств, доступных для покупки бумаг, принимаемых в обеспечение НаПокупОбесп 
            LUA_TABLE_FIELD(val_short_net, std::string) // Оценка стоимости коротких позиций. При расчете не используется коэффициент дисконтирования ** Шорты (нетто) 
            LUA_TABLE_FIELD(val_long_net, std::string) // Оценка стоимости длинных позиций. При расчете не используется коэффициент дисконтирования ** Лонги (нетто) 
            LUA_TABLE_FIELD(total_money_bal, std::string) // Сумма остатков по денежным средствам по всем лимитам, без учета средств, заблокированных под исполнение обязательств, выраженная в выбранной валюте расчета Сумма ден. остатков 
            LUA_TABLE_FIELD(total_locked_money, std::string) // Cумма заблокированных средств со всех денежных лимитов клиента, пересчитанная в валюту расчетов через кросс-курсы на сервере Суммарно заблок. 
            LUA_TABLE_FIELD(haircuts, std::string) // Сумма дисконтов стоимости длинных (только по бумагам обеспечения) и коротких бумажных позиций, дисконтов корреляции между инструментами, а также дисконтов на задолженности по валютам, не покрытые бумажным обеспечением в этих же валютах Сумма дисконтов 
            LUA_TABLE_FIELD(assets_without_hc, std::string) // Суммарная величина денежных остатков, стоимости длинных позиций по бумагам обеспечения и стоимости коротких позиций, без учета дисконтирующих коэффициентов, без учета неттинга стоимости бумаг в рамках объединенной бумажной позиции и без учета корреляции между инструментами ТекАктБезДиск 
            LUA_TABLE_FIELD(status_coef, std::string) // Отношение суммы дисконтов к текущим активам без учета дисконтов Статус счета 
            LUA_TABLE_FIELD(varmargin, std::string) // Текущая вариационная маржа по позициям клиента, по всем инструментам Вариац. маржа 
            LUA_TABLE_FIELD(go_for_positions, std::string) // Размер денежных средств, уплаченных под все открытые позиции на срочном рынке ГО поз. 
            LUA_TABLE_FIELD(go_for_orders, std::string) // Оценка стоимости активов в заявках на срочном рынке ГО заяв. 
            LUA_TABLE_FIELD(rate_futures, std::string) // Отношение ликвидационной стоимости портфеля к ГО по срочному рынку Активы/ГО 
            LUA_TABLE_FIELD(is_qual_client, std::string) // Признак «квалифицированного» клиента, которому разрешено кредитование заемными средствами с плечом 1:3. Возможные значения: «ПовышУрРиска» – квалифицированный, <пусто> – нет ПовышУрРиска 
            LUA_TABLE_FIELD(is_futures, std::string) // Счет клиента на FORTS, в случае наличия объединенной позиции, иначе поле остается пустым Сроч. счет 
            LUA_TABLE_FIELD(curr_tag, std::string) // Актуальные текущие параметры расчета для данной строки в формате «<Валюта>-<Тег расчётов>». Пример: «SUR-EQTV» Парам

            LUA_TABLE_FIELD(init_margin, std::string) // Значение начальной маржи. Заполняется для клиентов типа «МД» Нач.маржа 
            LUA_TABLE_FIELD(min_margin, std::string) // Значение минимальной маржи. Заполняется для клиентов типа «МД» Мин.маржа 
            LUA_TABLE_FIELD(corrected_margin, std::string) // Значение скорректированной маржи. Заполняется для клиентов типа «МД» Скор.маржа 
            LUA_TABLE_FIELD(client_type, std::string) // Тип клиента Тип клиента 
            LUA_TABLE_FIELD(portfolio_value, std::string) // Стоимость портфеля. Для клиентов типа «МД» возвращается значение для строк с максимальным видом лимита limit_kind Стоимость портфеля 
            LUA_TABLE_FIELD(start_limit_open_pos, std::string) // Лимит открытых позиций на начало дня ЛимОткрПозНачДня 
            LUA_TABLE_FIELD(total_limit_open_pos, std::string) // Лимит открытых позиций ЛимОткрПоз 
            LUA_TABLE_FIELD(limit_open_pos, std::string) // Планируемые чистые позиции ПланЧистПоз 
            LUA_TABLE_FIELD(used_lim_open_pos, std::string) // Текущие чистые позиции ТекЧистПоз 
            LUA_TABLE_FIELD(acc_var_margin, std::string) // Накопленная вариационная маржа НакопВарМаржа 
            LUA_TABLE_FIELD(cl_var_margin, std::string) // Вариационная маржа по итогам промклиринга ВарМаржаПромклир. 
            LUA_TABLE_FIELD(opt_liquid_cost, std::string) // Ликвидационная стоимость опционов ЛиквСтоимОпционов 
            LUA_TABLE_FIELD(fut_asset, std::string) // Сумма оценки средств клиента на срочном рынке СумАктивовНаСрчРынке 
            LUA_TABLE_FIELD(fut_total_asset, std::string) // Сумма оценки собственных средств клиента на фондовом и срочном рынках ПолнСтоимостьПортфеля 
            LUA_TABLE_FIELD(fut_debt, std::string) // Текущая задолженность на срочном рынке ТекЗадолжНаСрчРынке 
            LUA_TABLE_FIELD(fut_rate_asset, std::string) // Достаточность средств Дост. Средств 
            LUA_TABLE_FIELD(fut_rate_asset_open, std::string) // Достаточность средств (под открытые позиции) Дост. Средств (ОткрПоз) 
            LUA_TABLE_FIELD(fut_rate_go, std::string) // Коэффициент ликвидности ГО КоэффЛикв ГО 
            LUA_TABLE_FIELD(planed_rate_go, std::string) // Ожидаемый коэффициент ликвидности ГО Ожид. КоэффЛикв ГО 
            LUA_TABLE_FIELD(cash_leverage, std::string) // Cash Leverage Cash Leverage 
            LUA_TABLE_FIELD(fut_position_type, std::string) /* Тип позиции на срочном рынке. Возможные значения
                                                                  «0» – нет позиции;
                                                                  «1» – фьючерсы;
                                                                  «2» – опционы;
                                                                  «3» – фьючерсы и опционы
                                                                  ТипПозНаСрчРынке */
            LUA_TABLE_FIELD(fut_accured_int, std::string) // Накопленный доход с учётом премии по опционам и биржевым сборам НакопДоход
            LUA_TABLE_FIELD(rcv1, std::string) // Норматив покрытия риска 1. Рассчитывается как разница параметров «Стоимость портфеля» и «Начальная маржа». Для клиентов типа «МД» и «МД+» НПР1 
            LUA_TABLE_FIELD(rcv2, std::string) // Норматив покрытия риска 2. Рассчитывается как разница параметров «Стоимость портфеля» и «Минимальная маржа». Для клиентов типа «МД» и «МД+» НПР2 
            LUA_TABLE_END
    }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::portfolio_info_getPortfolioInfoEx)

// buy_sell_info_getBuySellInfo (table returned from getBuySellInfo)
namespace qlua {
    namespace table {
        LUA_TABLE_BEGIN(buy_sell_info_getBuySellInfo)
            LUA_TABLE_FIELD(is_margin_sec, std::string) /* Признак маржинальности инструмента. Возможные значения:
                                                         «0» – не маржинальная;
                                                         «1» – маржинальная.*/
            LUA_TABLE_FIELD(is_asset_sec, std::string) /* Принадлежность инструмента к списку бумаг, принимаемых в обеспечение. Возможные значения:
                                                        «0» – не принимается в обеспечение;
                                                        «1» – принимается в обеспечение.
                                                     */
            LUA_TABLE_FIELD(balance, std::string) // Текущая позиция по инструменту, в лотах  
            LUA_TABLE_FIELD(can_buy, std::string) // Оценка количества лотов, доступных на покупку по указанной цене * 
            LUA_TABLE_FIELD(can_sell, std::string) // Оценка количества лотов, доступных на продажу по указанной цене * 
            LUA_TABLE_FIELD(position_valuation, std::string) // Денежная оценка позиции по инструменту по ценам спроса/предложения 
            LUA_TABLE_FIELD(value, std::string) // Оценка стоимости позиции по цене последней сделки  
            LUA_TABLE_FIELD(open_value, std::string) // Оценка стоимости позиции клиента, рассчитанная по цене закрытия предыдущей торговой сессии  
            LUA_TABLE_FIELD(lim_long, std::string) // Предельный размер позиции по данному инструменту, принимаемый в обеспечение длинных позиций  
            LUA_TABLE_FIELD(long_coef, std::string) // Коэффициент дисконтирования, применяемый для длинных позиций по данному инструменту  
            LUA_TABLE_FIELD(lim_short, std::string) // Предельный размер короткой позиции по данному инструменту  
            LUA_TABLE_FIELD(short_coef, std::string) // Коэффициент дисконтирования, применяемый для коротких позиций по данному инструменту  
            LUA_TABLE_FIELD(value_coef, std::string) // Оценка стоимости позиции по цене последней сделки, с учетом дисконтирующих коэффициентов  
            LUA_TABLE_FIELD(open_value_coef, std::string) // Оценка стоимости позиции клиента, рассчитанная по цене закрытия предыдущей торговой сессии с учетом дисконтирующих коэффициентов  
            LUA_TABLE_FIELD(share, std::string) // Процентное отношение стоимости позиции по данному инструменту к стоимости всех активов клиента, рассчитанное по текущим ценам  
            LUA_TABLE_FIELD(short_wa_price, std::string) // Средневзвешенная стоимость коротких позиций по инструментам  
            LUA_TABLE_FIELD(long_wa_price, std::string) // Средневзвешенная стоимость длинных позиций по инструментам  
            LUA_TABLE_FIELD(profit_loss, std::string) // Разница между средневзвешенной ценой приобретения бумаг и их рыночной оценки  
            LUA_TABLE_FIELD(spread_hc, std::string) // Коэффициент корреляции между инструментами  
            LUA_TABLE_FIELD(can_buy_own, std::string) // Максимально возможное количество бумаг в заявке на покупку этого инструмента на этом классе на собственные средства клиента, исходя из цены лучшего предложения  
            LUA_TABLE_FIELD(can_sell_own, std::string) // Максимально возможное количество бумаг в заявке на продажу этого инструмента на этом классе из собственных активов клиента, исходя из цены лучшего спроса
            LUA_TABLE_END;
    }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::buy_sell_info_getBuySellInfo)

// buy_sell_info_getBuySellInfoEx (table returned from getBuySellInfoEx)
namespace qlua {
    namespace table {
        LUA_TABLE_BEGIN(buy_sell_info_getBuySellInfoEx)
            LUA_TABLE_FIELD(is_margin_sec, std::string) /* Признак маржинальности инструмента. Возможные значения:
                                                         «0» – не маржинальная;
                                                         «1» – маржинальная.*/
            LUA_TABLE_FIELD(is_asset_sec, std::string) /* Принадлежность инструмента к списку бумаг, принимаемых в обеспечение. Возможные значения:
                                                        «0» – не принимается в обеспечение;
                                                        «1» – принимается в обеспечение.
                                                     */
            LUA_TABLE_FIELD(balance, std::string) // Текущая позиция по инструменту, в лотах  
            LUA_TABLE_FIELD(can_buy, std::string) // Оценка количества лотов, доступных на покупку по указанной цене * 
            LUA_TABLE_FIELD(can_sell, std::string) // Оценка количества лотов, доступных на продажу по указанной цене * 
            LUA_TABLE_FIELD(position_valuation, std::string) // Денежная оценка позиции по инструменту по ценам спроса/предложения 
            LUA_TABLE_FIELD(value, std::string) // Оценка стоимости позиции по цене последней сделки  
            LUA_TABLE_FIELD(open_value, std::string) // Оценка стоимости позиции клиента, рассчитанная по цене закрытия предыдущей торговой сессии  
            LUA_TABLE_FIELD(lim_long, std::string) // Предельный размер позиции по данному инструменту, принимаемый в обеспечение длинных позиций  
            LUA_TABLE_FIELD(long_coef, std::string) // Коэффициент дисконтирования, применяемый для длинных позиций по данному инструменту  
            LUA_TABLE_FIELD(lim_short, std::string) // Предельный размер короткой позиции по данному инструменту  
            LUA_TABLE_FIELD(short_coef, std::string) // Коэффициент дисконтирования, применяемый для коротких позиций по данному инструменту  
            LUA_TABLE_FIELD(value_coef, std::string) // Оценка стоимости позиции по цене последней сделки, с учетом дисконтирующих коэффициентов  
            LUA_TABLE_FIELD(open_value_coef, std::string) // Оценка стоимости позиции клиента, рассчитанная по цене закрытия предыдущей торговой сессии с учетом дисконтирующих коэффициентов  
            LUA_TABLE_FIELD(share, std::string) // Процентное отношение стоимости позиции по данному инструменту к стоимости всех активов клиента, рассчитанное по текущим ценам  
            LUA_TABLE_FIELD(short_wa_price, std::string) // Средневзвешенная стоимость коротких позиций по инструментам  
            LUA_TABLE_FIELD(long_wa_price, std::string) // Средневзвешенная стоимость длинных позиций по инструментам  
            LUA_TABLE_FIELD(profit_loss, std::string) // Разница между средневзвешенной ценой приобретения бумаг и их рыночной оценки  
            LUA_TABLE_FIELD(spread_hc, std::string) // Коэффициент корреляции между инструментами  
            LUA_TABLE_FIELD(can_buy_own, std::string) // Максимально возможное количество бумаг в заявке на покупку этого инструмента на этом классе на собственные средства клиента, исходя из цены лучшего предложения  
            LUA_TABLE_FIELD(can_sell_own, std::string) // Максимально возможное количество бумаг в заявке на продажу этого инструмента на этом классе из собственных активов клиента, исходя из цены лучшего спроса

            LUA_TABLE_FIELD(limit_kind, int) // Вид лимита. Возможные значения: положительные целые числа, начиная с «0», соответствующие видам лимитов из таблицы «Лимиты по бумагам»: «0» – T0, «1» – T1, «2» – T2 и т.д.  
            LUA_TABLE_FIELD(d_long, std::string) // Эффективный начальный дисконт для длинной позиции. Заполняется для клиентов типа «МД» 
            LUA_TABLE_FIELD(d_min_long, std::string) // Эффективный минимальный дисконт для длинной позиции. Заполняется для клиентов типа «МД» 
            LUA_TABLE_FIELD(d_short, std::string) // Эффективный начальный дисконт для короткой позиции. Заполняется для клиентов типа «МД» 
            LUA_TABLE_FIELD(d_min_short, std::string) // Эффективный минимальный дисконт для короткой позиции. Заполняется для клиентов типа «МД» 
            LUA_TABLE_FIELD(client_type, std::string) // Тип клиента 
            LUA_TABLE_FIELD(is_long_allowed, std::string) /* Признак того, является ли бумага разрешенной для покупки на заемные средства. Возможные значения:
                                                           «1» – разрешена, «0» – не разрешена.
                                                           Заполняется для клиентов типа «МД» */
            LUA_TABLE_FIELD(is_short_allowed, std::string) /* Признак того, является ли бумага разрешенной для продажи на заемные средства. Возможные значения:
                                                            «1» – разрешена, «0» – не разрешена.
                                                            Заполняется для клиентов типа «МД» */

            LUA_TABLE_END;
    }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::buy_sell_info_getBuySellInfoEx)

// candle_time (Returned by Функции O, H, L, C, V, T)
namespace qlua {
    namespace table {
        LUA_TABLE_BEGIN(candle_time)
            LUA_TABLE_FIELD(year, unsigned int)
            LUA_TABLE_FIELD(month, unsigned int)
            LUA_TABLE_FIELD(day, unsigned int)
            LUA_TABLE_FIELD(week_day, unsigned int)
            LUA_TABLE_FIELD(hour, unsigned int)
            LUA_TABLE_FIELD(min, unsigned int)
            LUA_TABLE_FIELD(sec, unsigned int)
            LUA_TABLE_FIELD(ms, unsigned int)
            LUA_TABLE_FIELD(count, unsigned int) // количество тиковых интервалов в секунду. Может принимать значения от «1» до «10000» включительно.
            LUA_TABLE_END
    }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::candle_time)

// class_info (Returned by getClassInfo)
namespace qlua {
    namespace table {
        LUA_TABLE_BEGIN(class_info_getClassInfo)
            LUA_TABLE_FIELD(firmid, const char *) // Код фирмы 
            LUA_TABLE_FIELD(name, const char *) // Наименование класса 
            LUA_TABLE_FIELD(code, const char *) // Код класса 
            LUA_TABLE_FIELD(npars, unsigned int) // Количество параметров в классе 
            LUA_TABLE_FIELD(nsecs, unsigned int) // Количество бумаг в классе
            LUA_TABLE_END
    }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::class_info_getClassInfo)

// chart_time (Returned by ds:T)
namespace qlua {
    namespace table {
        LUA_TABLE_BEGIN(chart_time)
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
    }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::chart_time)
