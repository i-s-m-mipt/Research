#ifndef SOLUTION_SYSTEM_MARKET_INDICATORS_AMA_HPP
#define SOLUTION_SYSTEM_MARKET_INDICATORS_AMA_HPP

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
				class AMA
				{
				public:

					explicit AMA(std::size_t timesteps, std::size_t f, std::size_t s) : 
						m_timesteps(timesteps), m_f(f), m_s(s)
					{
						initialize();
					}

				private:

					void initialize() const;

				public:

					void operator()(candles_container_t & candles) const;

				private:

					std::size_t m_timesteps;

					std::size_t m_f;
					std::size_t m_s;
				};

			} // namespace indicators

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_INDICATORS_AMA_HPP