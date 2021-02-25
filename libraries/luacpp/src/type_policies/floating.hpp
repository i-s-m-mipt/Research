#pragma once

namespace lua {
  namespace detail {
    template <typename T>
    struct floating {
      typedef T write_type;
      typedef typename std::remove_reference<T>::type read_type;
      
      inline static bool type_matches(lua::State s, int idx) {
        return s.is_number(idx);
      }
    
      inline static read_type get_unsafe(lua::State s, int idx) {
        return read_type(s.to_number(idx));
      }

      template <typename F>
      inline static void apply_unsafe(lua::State s, int idx, F f) {
        f(s, idx);
      }

      inline static void set(lua::State s, int idx, T value) {
        s.push_number(write_type(value));
        if (idx != 0) s.replace(idx - 1);
      }
    };
  
  }
  
  template <>
  struct type_policy<double> : public detail::floating<double> {
  };

  template <>
  struct type_policy<const double> : public detail::floating<const double> {
  };
  
  template <>
  struct type_policy<const double&> : public detail::floating<const double&> {
  };
  
  template <>
  struct type_policy<float> : public detail::floating<float> {
  };

  template <>
  struct type_policy<const float> : public detail::floating<const float> {
  };
  
  template <>
  struct type_policy<const float&> : public detail::floating<const float&> {
  };
  
}

