#pragma once

namespace lua {
  namespace detail {
    template <typename T>
    struct void_pointer {
      // Void_Pointers are ints in Lua
      typedef T write_type;
      typedef typename std::remove_reference<T>::type read_type;
      
      inline static bool type_matches(lua::State s, int idx) {
        return s.is_light_user_data(idx) || s.is_function(idx) || s.is_cfunction(idx) || s.is_thread(idx);
      }
    
      inline static read_type get_unsafe(lua::State s, int idx) {
        return (void*)s.to_pointer(idx);
      }

      template <typename F>
      inline static void apply_unsafe(lua::State s, int idx, F f) {
        f(s, idx);
      }

      inline static void set(lua::State s, int idx, T value) {
        // Writes void always as light user data
        s.push_light_user_data(write_type(value));
        if (idx != 0) s.replace(idx);
      }
    };
  }
  
  template <>
  struct type_policy<void*> : public detail::void_pointer<void*> {
  };

  template <>
  struct type_policy<const void*> : public detail::void_pointer<const void*> {
  };
}
