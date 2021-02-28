#pragma once

#define LUA_TABLE_BEGIN(NAME) \
class NAME \
{ \
private: \
\
    lua::State m_state; \
    int   m_index; \
\
public: \
\
    explicit NAME(lua::State state, int index) : \
        m_state(state), m_index(index) \
    {} \
\
    ~##NAME() noexcept = default; \
\
public: \
\
    void create() const \
    { \
        m_state.new_table(); \
    } \
\
    bool is_nil() const \
    { \
        return m_state.is_nil(m_index); \
    } \

#define LUA_MAKE_TABLE_FIELD_KEY(NAME) #NAME

#define LUA_TABLE_FIELD_KEY_VALUE(NAME, KEY_TYPE, VALUE_TYPE) \
class NAME##_Table_Field : public lua::types::Table_Field < KEY_TYPE, VALUE_TYPE > \
{ \
public: \
\
    using base_t = lua::types::Table_Field < KEY_TYPE, VALUE_TYPE > ; \
\
public: \
\
    static auto get(lua::State state, int index) \
    { \
        return base_t::get(state, index, LUA_MAKE_TABLE_FIELD_KEY(NAME)); \
    } \
\
static void set(lua::State state, int index, VALUE_TYPE value) \
{ \
    base_t::set(state, index, value, LUA_MAKE_TABLE_FIELD_KEY(NAME)); \
} \
\
    template < typename F > \
    static void apply(lua::State state, int index, F function) \
    { \
        base_t::apply(state, index, function, LUA_MAKE_TABLE_FIELD_KEY(NAME)); \
    } \
}; \
\
lua::Entity < NAME##_Table_Field > NAME{ m_state, m_index }; \

#define LUA_TABLE_FIELD(NAME, VALUE_TYPE) LUA_TABLE_FIELD_KEY_VALUE(NAME, const char *, VALUE_TYPE)

#define LUA_TABLE_END() };

#define LUA_TABLE_TYPE_ADAPTER(TABLE_TYPE) \
namespace lua \
{ \
    template <> \
    class Type_Adapter < TABLE_TYPE > \
    { \
    public: \
\
        using type = TABLE_TYPE; \
\
    public: \
\
        static auto type_matches(lua::State state, int index) \
        { \
            return (state.is_nil(index) || state.is_table(index)); \
        } \
\
        static auto get(lua::State state, int index) \
        { \
            if (state.is_nil(index)) \
            {  \
                state.new_table(); \
                state.replace(index); \
            } \
\
            return type(state, index); \
        } \
\
        static void set(lua::State state, int index, type value) \
        {} \
\
        template < typename F > \
        static void apply(lua::State state, int index, F function) \
        { \
            function(state, index); \
        } \
    }; \
\
} \