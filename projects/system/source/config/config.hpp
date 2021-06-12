#ifndef SOLUTION_SYSTEM_CONFIG_HPP
#define SOLUTION_SYSTEM_CONFIG_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_IA64_)  && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
      defined(_M_IX86))
#  define _X86_
#  if (!defined(_CHPE_X86_ARM64_) && defined(_M_HYBRID))
#    define _CHPE_X86_ARM64_
#  endif // #  if (!defined(_CHPE_X86_ARM64_) && defined(_M_HYBRID))
#endif // #if (!defined(_68K_)   && \

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_IA64_)  && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
      defined(_M_AMD64))
#  define _AMD64_
#endif // #if (!defined(_68K_)   && \

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_IA64_)  && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
      defined(_M_ARM))
#  define _ARM_
#endif // #if (!defined(_68K_)   && \

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_IA64_)  && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
      defined(_M_ARM64))
#  define _ARM64_
#endif // #if (!defined(_68K_)   && \

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_IA64_)  && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
      defined(_M_M68K))
#  define _68K_
#endif // #if (!defined(_68K_)   && \

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_IA64_)  && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
      defined(_M_MPPC))
#  define _MPPC_
#endif // #if (!defined(_68K_)   && \

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_M_IX86) && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
     !defined(_IA64_)  && \
      defined(_M_IA64))
#  define _IA64_
#endif // #if (!defined(_68K_)   && \

#if !defined(_MAC) && (defined(_68K_) || defined(_MPPC_)) 
#  define _MAC
#endif // #if !defined(_MAC) && (defined(_68K_) || defined(_MPPC_))

#include <string>

namespace solution
{
    namespace system
    {
        struct Config
        {
            bool required_charts            = false;
            bool required_self_similarities = false;
            bool required_pair_similarities = false;
            bool required_pair_correlations = false;

            int self_similarity_DTW_delta = 1'000'000;

            std::string cumulative_distances_asset;
            std::string cumulative_distances_scale_1;
            std::string cumulative_distances_scale_2;

            bool required_deviations = false;

            bool required_tagged_charts = false;

            bool required_environment = false;

            double min_price_change   = 0.020;
            double max_price_rollback = 0.333;

            double level_max_deviation = 0.005;

            double level_max_lifetime = 1825.0;
            double level_max_strength = 25.0;

            std::string level_resolution = "D";

            std::size_t level_frame = 5U;

            double critical_deviation = 0.75;

            bool required_level_reduction = false;

            bool required_quik = false;

            bool required_supports_resistances = false;

            double classification_max_deviation = 0.005;

            bool run_julia_test = false;

            std::string prediction_timeframe = "D";
            std::size_t prediction_timesteps = 20U;

            double transaction_base_value = 0.0;

            std::time_t days_for_dividends = 5LL;

            double deviation_threshold = 1.0;

            bool run_model_sensibility_test = true;

            std::time_t model_stabilization_time = 3600LL;
        };

    } // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_CONFIG_HPP