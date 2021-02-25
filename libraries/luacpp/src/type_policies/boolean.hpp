#pragma once

namespace lua {
  namespace detail {
    template <typename T>
    struct boolean {
      // Booleans are ints in Lua
      typedef int write_type;
      typedef typename std::remove_reference<T>::type read_type;
      
      inline static bool type_matches(lua::State s, int idx) {
        return s.is_boolean(idx);
      }
    
      inline static read_type get_unsafe(lua::State s, int idx) {
        return s.to_boolean(idx);
      }

      template <typename F>
      inline static void apply_unsafe(lua::State s, int idx, F f) {
        f(s, idx);
      }

      inline static void set(lua::State s, int idx, T value) {
        s.push_boolean(write_type(value));
        if (idx != 0) s.replace(idx - 1);
      }
    };
  }
  
  template <>
  struct type_policy<bool> : public detail::boolean<bool> {
  };

  template <>
  struct type_policy<const bool> : public detail::boolean<const bool> {
  };

  template <>
  struct type_policy<const bool&> : public detail::boolean<const bool&> {
  };
}
