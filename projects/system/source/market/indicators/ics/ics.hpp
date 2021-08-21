#ifndef SOLUTION_SYSTEM_MARKET_INDICATORS_ICS_HPP
#define SOLUTION_SYSTEM_MARKET_INDICATORS_ICS_HPP

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
			namespace indicators
			{
				class ICS
				{
				public:

					explicit ICS(std::size_t timesteps_s, std::size_t timesteps_m, std::size_t timesteps_l) :
						m_timesteps_s(timesteps_s), m_timesteps_m(timesteps_m), m_timesteps_l(timesteps_l)
					{
						initialize();
					}

				private:

					void initialize() const;

				public:

					void operator()(candles_container_t & candles) const;

				private:

					std::size_t m_timesteps_s;
					std::size_t m_timesteps_m;
					std::size_t m_timesteps_l;
				};

			} // namespace indicators

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_INDICATORS_ICS_HPP
