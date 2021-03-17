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

#endif // #ifndef SOLUTION_SYSTEM_CONFIG_HPP