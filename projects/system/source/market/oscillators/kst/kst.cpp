#include "kst.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void KST::initialize() const
				{
					RUN_LOGGER(logger);

					try
					{
						if (m_timesteps_roc_1 == 0U)
						{
							throw std::domain_error("required: (timesteps_roc_1 > 0)");
						}

						if (m_timesteps_roc_2 == 0U)
						{
							throw std::domain_error("required: (timesteps_roc_2 > 0)");
						}

						if (m_timesteps_roc_3 == 0U)
						{
							throw std::domain_error("required: (timesteps_roc_3 > 0)");
						}

						if (m_timesteps_roc_4 == 0U)
						{
							throw std::domain_error("required: (timesteps_roc_4 > 0)");
						}

						if (m_timesteps_ema_1 == 0U)
						{
							throw std::domain_error("required: (timesteps_ema_1 > 0)");
						}

						if (m_timesteps_ema_2 == 0U)
						{
							throw std::domain_error("required: (timesteps_ema_2 > 0)");
						}

						if (m_timesteps_ema_3 == 0U)
						{
							throw std::domain_error("required: (timesteps_ema_3 > 0)");
						}

						if (m_timesteps_ema_4 == 0U)
						{
							throw std::domain_error("required: (timesteps_ema_4 > 0)");
						}

						if (m_timesteps_roc_4 < m_timesteps_roc_1 ||
							m_timesteps_roc_4 < m_timesteps_roc_2 ||
							m_timesteps_roc_4 < m_timesteps_roc_3)
						{
							throw std::domain_error("required: (timesteps_roc_4 = max)");
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < oscillator_exception > (logger, exception);
					}
				}

				void KST::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto k_ema_1 = 2.0 / (m_timesteps_ema_1 + 1.0);
						const auto k_ema_2 = 2.0 / (m_timesteps_ema_2 + 1.0);
						const auto k_ema_3 = 2.0 / (m_timesteps_ema_3 + 1.0);
						const auto k_ema_4 = 2.0 / (m_timesteps_ema_4 + 1.0);

						const auto k_signal = 2.0 / (m_timesteps_signal + 1.0);

						auto ema_1 = 0.0;
						auto ema_2 = 0.0;
						auto ema_3 = 0.0;
						auto ema_4 = 0.0;

						auto ema_signal = 0.0;

						for (auto i = m_timesteps_roc_4; i < std::size(candles); ++i)
						{
							auto roc_1 = 100.0 * (candles[i].price_close / 
								candles[i - m_timesteps_roc_1].price_close - 1.0);
							auto roc_2 = 100.0 * (candles[i].price_close /
								candles[i - m_timesteps_roc_2].price_close - 1.0);
							auto roc_3 = 100.0 * (candles[i].price_close /
								candles[i - m_timesteps_roc_3].price_close - 1.0);
							auto roc_4 = 100.0 * (candles[i].price_close /
								candles[i - m_timesteps_roc_4].price_close - 1.0);

							if (i == m_timesteps_roc_4)
							{
								ema_1 = roc_1;
								ema_2 = roc_2;
								ema_3 = roc_3;
								ema_4 = roc_4;
							}
							else
							{
								ema_1 = k_ema_1 * roc_1 + (1.0 - k_ema_1) * ema_1;
								ema_2 = k_ema_2 * roc_2 + (1.0 - k_ema_2) * ema_2;
								ema_3 = k_ema_3 * roc_3 + (1.0 - k_ema_3) * ema_3;
								ema_4 = k_ema_4 * roc_4 + (1.0 - k_ema_4) * ema_4;
							}

							auto basic_kst = (1.0 * ema_1 + 2.0 * ema_2 + 3.0 * ema_3 + 4.0 * ema_4) / 10.0;

							if (i == m_timesteps_roc_4)
							{
								ema_signal = basic_kst;
							}
							else
							{
								ema_signal = k_signal * basic_kst + (1.0 - k_signal) * ema_signal;
							}

							candles[i].oscillators.push_back(ema_signal);
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < oscillator_exception > (logger, exception);
					}
				}

			} // namespace oscillators

		} // namespace market

	} // namespace system

} // namespace solution