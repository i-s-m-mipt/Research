#include "ics.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void ICS::initialize() const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						if (m_timesteps_s == 0U)
						{
							throw std::domain_error("required: (timesteps > 0)");
						}

						if (m_timesteps_m == 0U)
						{
							throw std::domain_error("required: (timesteps > 0)");
						}

						if (m_timesteps_l == 0U)
						{
							throw std::domain_error("required: (timesteps > 0)");
						}
					}
					catch (const std::exception& exception)
					{
						shared::catch_handler < indicator_exception >(logger, exception);
					}
				}

				void ICS::operator()(candles_container_t& candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						std::vector < double > tenkan;

						tenkan.reserve(std::size(candles) - m_timesteps_s);

						for (auto i = m_timesteps_s; i < std::size(candles); ++i)
						{
							auto max = std::max_element(std::next(std::begin(candles), i - m_timesteps_s),
								std::next(std::begin(candles), i),
								[](const auto& lhs, const auto& rhs) { return (lhs.price_high < rhs.price_high); });

							auto min = std::min_element(std::next(std::begin(candles), i - m_timesteps_s),
								std::next(std::begin(candles), i),
								[](const auto& lhs, const auto& rhs) { return (lhs.price_low < rhs.price_low); });

							tenkan.push_back( ((*max).price_high + (*min).price_low) / 2.0) ;

							candles[i - 1U].indicators.push_back(((*max).price_high + (*min).price_low) / 2.0 );
						}

						std::vector < double > kijun;

						kijun.reserve(std::size(candles) - m_timesteps_m);

						for (auto i = m_timesteps_m; i < std::size(candles); ++i)
						{
							auto max = std::max_element(std::next(std::begin(candles), i - m_timesteps_m),
								std::next(std::begin(candles), i),
								[](const auto& lhs, const auto& rhs) { return (lhs.price_high < rhs.price_high); });

							auto min = std::min_element(std::next(std::begin(candles), i - m_timesteps_m),
								std::next(std::begin(candles), i),
								[](const auto& lhs, const auto& rhs) { return (lhs.price_low < rhs.price_low); });

							kijun.push_back(((*max).price_high + (*min).price_low) / 2.0);


						}

						std::vector < double > senkou_a;

						senkou_a.reserve(std::size(kijun) - m_timesteps_m);

						for (auto i = m_timesteps_m; i < std::size(kijun); ++i)
						{
							senkou_a.push_back((tenkan[i - m_timesteps_m] + kijun[i - m_timesteps_m]) / 2.0);
						}

						std::vector < double > senkou_b;

						senkou_b.reserve(std::size(candles) - m_timesteps_m - m_timesteps_l);

						for (auto i = m_timesteps_m + m_timesteps_l; i < std::size(candles); ++i)
						{
							auto max = std::max_element(std::next(std::begin(candles), i - m_timesteps_m - m_timesteps_l),
								std::next(std::begin(candles), i),
								[](const auto& lhs, const auto& rhs) { return (lhs.price_high < rhs.price_high); });

							auto min = std::min_element(std::next(std::begin(candles), i - m_timesteps_m - m_timesteps_l),
								std::next(std::begin(candles), i),
								[](const auto& lhs, const auto& rhs) { return (lhs.price_low < rhs.price_low); });

							senkou_b.push_back(((*max).price_high + (*min).price_low) / 2.0);
						}

						/*std::vector < double > chikou;

						chikou.reserve(std::size(candles) - m_timesteps_s);

						for (auto i = 0; i < std::size(candles) - m_timesteps_s; ++i)
						{
							chikou.push_back(candles[i + m_timesteps_s].price_close);
						}
						*/

						for (auto i = m_timesteps_l + m_timesteps_m; i < std::size(candles); i++)
						{
							candles[i].indicators.push_back(tenkan[i - m_timesteps_m - m_timesteps_l + m_timesteps_s]);
							candles[i].indicators.push_back(kijun[i - m_timesteps_l]);
							candles[i].indicators.push_back(senkou_a[i - m_timesteps_l]);
							candles[i].indicators.push_back(senkou_b[i - m_timesteps_l - m_timesteps_m]);
						}
			
					}
					catch (const std::exception& exception)
					{
						shared::catch_handler < indicator_exception >(logger, exception);
					}
				}

			} // namespace indicators

		} // namespace market

	} // namespace system

} // namespace solution
