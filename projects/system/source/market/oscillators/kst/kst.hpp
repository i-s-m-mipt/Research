#ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_KST_HPP
#define SOLUTION_SYSTEM_MARKET_OSCILLATORS_KST_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include "../detail.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				class KST
				{
				public:

					explicit KST(
						std::size_t timesteps_roc_1, 
						std::size_t timesteps_roc_2,
						std::size_t timesteps_roc_3,
						std::size_t timesteps_roc_4,
						std::size_t timesteps_ema_1,
						std::size_t timesteps_ema_2,
						std::size_t timesteps_ema_3,
						std::size_t timesteps_ema_4,
						std::size_t timesteps_signal) :
							m_timesteps_roc_1(timesteps_roc_1),
							m_timesteps_roc_2(timesteps_roc_2),
							m_timesteps_roc_3(timesteps_roc_3),
							m_timesteps_roc_4(timesteps_roc_4),
							m_timesteps_ema_1(timesteps_ema_1),
							m_timesteps_ema_2(timesteps_ema_2),
							m_timesteps_ema_3(timesteps_ema_3),
							m_timesteps_ema_4(timesteps_ema_4),
							m_timesteps_signal(timesteps_signal)
					{
						initialize();
					}

				private:

					void initialize() const;

				public:

					void operator()(candles_container_t & candles) const;

				private:

					std::size_t m_timesteps_roc_1;
					std::size_t m_timesteps_roc_2;
					std::size_t m_timesteps_roc_3;
					std::size_t m_timesteps_roc_4;

					std::size_t m_timesteps_ema_1;
					std::size_t m_timesteps_ema_2;
					std::size_t m_timesteps_ema_3;
					std::size_t m_timesteps_ema_4;

					std::size_t m_timesteps_signal;
				};

			} // namespace oscillators

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_KST_HPP