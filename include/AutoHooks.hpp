#pragma once

#include "beatsaber-hook/shared/utils/hooking.hpp"

namespace AutoHooks {
    inline std::vector<void (*)()> installFuncs;

    inline void AddInstallFunc(void (*installFunc)()) {
        installFuncs.push_back(installFunc);
    }

    inline void InstallHooks() {
        for (auto& func : installFuncs) func();
    }
}

#define HOOK_AUTO_INSTALL_ORIG(name_)                                                                       \
    struct Auto_Hook_##name_ {                                                                              \
        static void Auto_Hook_##name_##_Install() {                                                         \
            static constexpr auto logger = Paper::ConstLoggerContext(MOD_ID "_Install_" #name_);            \
            ::Hooking::InstallOrigHook<Hook_##name_>(logger);                                               \
        }                                                                                                   \
        Auto_Hook_##name_() { ::AutoHooks::AddInstallFunc(Auto_Hook_##name_##_Install); }     \
    };                                                                                                      \
    static Auto_Hook_##name_ Auto_Hook_Instance_##name_

#define HOOK_AUTO_INSTALL(name_)                                                                         \
    struct Auto_Hook_##name_ {                                                                           \
        static void Auto_Hook_##name_##_Install() {                                                      \
            static constexpr auto logger = Paper::ConstLoggerContext(MOD_ID "_Install_" #name_);         \
            INSTALL_HOOK(logger, name_);                                              \
        }                                                                                                \
        Auto_Hook_##name_() { ::AutoHooks::AddInstallFunc(Auto_Hook_##name_##_Install); }  \
    };                                                                                                   \
    static Auto_Hook_##name_ Auto_Hook_Instance_##name_

namespace AutoHooks::HookMacros{
#define MAKE_AUTO_HOOK(name_, addr_, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    constexpr static void* addr() { return (void*) addr_; } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make an address-specified hook.
#define MAKE_AUTO_HOOK_NO_CATCH(name_, addr_, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    constexpr static void* addr() { return (void*) addr_; } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that resolves the 'infoGet' expression an installs the hook to that MethodInfo*, that has a catch handler.
#define MAKE_AUTO_HOOK_FIND_VERBOSE(name_, infoGet, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return infoGet; } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that resolves the 'infoGet' expression an installs the hook to that MethodInfo*.
#define MAKE_AUTO_HOOK_FIND_VERBOSE_NO_CATCH(name_, infoGet, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return infoGet; } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided Il2CppClass* with the provided method name, that has a catch handler
#define MAKE_AUTO_HOOK_FIND(name_, klass, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(klass, mName); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided Il2CppClass* with the provided method name.
#define MAKE_AUTO_HOOK_FIND_NO_CATCH(name_, klass, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(klass, mName); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name, that has a catch handler.
#define MAKE_AUTO_HOOK_FIND_CLASS(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(namespaze, klassName, mName); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
#define MAKE_AUTO_HOOK_FIND_CLASS_NO_CATCH(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(namespaze, klassName, mName); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided Il2CppClass* with the provided method name.
// Ignores matching the first parameter, assuming it is an instance method.
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_FIND_INSTANCE(name_, klass, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<typename ::il2cpp_utils::InstanceMethodConverter<funcType>::fType>::find(klass, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided Il2CppClass* with the provided method name.
// Ignores matching the first parameter, assuming it is an instance method.
#define MAKE_AUTO_HOOK_FIND_INSTANCE_NO_CATCH(name_, klass, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<typename ::il2cpp_utils::InstanceMethodConverter<funcType>::fType>::find(klass, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
// Ignores matching the first parameter, assuming it is an instance method.
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_FIND_CLASS_INSTANCE(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<typename ::il2cpp_utils::InstanceMethodConverter<funcType>::fType>::find(namespaze, klassName, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
// Ignores matching the first parameter, assuming it is an instance method.
#define MAKE_AUTO_HOOK_FIND_CLASS_INSTANCE_NO_CATCH(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<typename ::il2cpp_utils::InstanceMethodConverter<funcType>::fType>::find(namespaze, klassName, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
// THIS FUNCTION IS THE UNSAFE VARIANT, SUBTRACTS ONE FOR INSTANCE METHODS!
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_FIND_CLASS_UNSAFE_INSTANCE(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find_unsafe(namespaze, klassName, mName, true); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
// THIS FUNCTION IS THE UNSAFE VARIANT, SUBTRACTS ONE FOR INSTANCE METHODS!
#define MAKE_AUTO_HOOK_FIND_CLASS_UNSAFE_INSTANCE_NO_CATCH(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find_unsafe(namespaze, klassName, mName, true); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
// THIS FUNCTION IS THE UNSAFE VARIANT!
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_FIND_CLASS_UNSAFE_STATIC(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find_unsafe(namespaze, klassName, mName); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
// THIS FUNCTION IS THE UNSAFE VARIANT!
#define MAKE_AUTO_HOOK_FIND_CLASS_UNSAFE_STATIC_NO_CATCH(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find_unsafe(namespaze, klassName, mName); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that would be installed in a particular address, but ensures the signature matches the provided method pointer.
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_CHECKED_ADDR(name_, mPtr, addr_, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    constexpr static void* addr() { return addr_; } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that would be installed in a particular address, but ensures the signature matches the provided method pointer.
#define MAKE_AUTO_HOOK_CHECKED_ADDR_NO_CATCH(name_, mPtr, addr_, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    constexpr static void* addr() { return addr_; } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that ensures the signature matches the provided method pointer and finds a matching method from a class and method name.
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_CHECKED_FIND(name_, mPtr, klass, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(klass, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that ensures the signature matches the provided method pointer and finds a matching method from a class and method name.
#define MAKE_AUTO_HOOK_CHECKED_FIND_NO_CATCH(name_, mPtr, klass, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(klass, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that ensures the signature matches the provided method pointer and finds a matching method from namespace, name, and method name.
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_CHECKED_FIND_CLASS(name_, mPtr, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(namespaze, klassName, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that ensures the signature matches the provided method pointer and finds a matching method from namespace, name, and method name.
#define MAKE_AUTO_HOOK_CHECKED_FIND_CLASS_NO_CATCH(name_, mPtr, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(namespaze, klassName, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that ensures the signature matches the provided method pointer and finds a matching method from a class and method name.
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_CHECKED_INSTANCE_FIND(name_, mPtr, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    using classType = ::Hooking::InternalClassGetter<decltype(mPtr)>::instanceType; \
    static_assert(!std::is_same_v<classType, void>, "MAKE_HOOK_INSTANCE_FIND was not provided an instance method pointer!"); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(classof(classType), mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that ensures the signature matches the provided method pointer and finds a matching method from a class and method name.
#define MAKE_AUTO_HOOK_CHECKED_INSTANCE_FIND_NO_CATCH(name_, mPtr, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    using classType = ::Hooking::InternalClassGetter<decltype(mPtr)>::instanceType; \
    static_assert(!std::is_same_v<classType, void>, "MAKE_HOOK_INSTANCE_FIND was not provided an instance method pointer!"); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(classof(classType), mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_AUTO_HOOK_MATCH(name_, mPtr, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(MATCH_HOOKABLE_ASSERT(mPtr)); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::il2cpp_type_check::MetadataGetter<mPtr>::methodInfo(); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that uses the provided method pointer in a match an ensures the signature matches.
// This should be your go-to hook macro when hooking anything that has a codegen equivalent.
#define MAKE_AUTO_HOOK_MATCH_NO_CATCH(name_, mPtr, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(MATCH_HOOKABLE_ASSERT(mPtr)); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::il2cpp_type_check::MetadataGetter<mPtr>::methodInfo(); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)
#pragma endregion
}
/**
Auto hooks that automatically install themselves when the game is loaded.
 */
namespace AutoHooks::OrigHookMacros {
#define MAKE_AUTO_HOOK_ORIG(name_, addr_, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    constexpr static void* addr() { return (void*) addr_; } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make an address-specified hook.
#define MAKE_AUTO_HOOK_NO_CATCH_ORIG(name_, addr_, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    constexpr static void* addr() { return (void*) addr_; } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that resolves the 'infoGet' expression an installs the hook to that MethodInfo*, that has a catch handler.
#define MAKE_AUTO_HOOK_FIND_VERBOSE_ORIG(name_, infoGet, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return infoGet; } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that resolves the 'infoGet' expression an installs the hook to that MethodInfo*.
#define MAKE_AUTO_HOOK_FIND_VERBOSE_NO_CATCH_ORIG(name_, infoGet, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return infoGet; } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided Il2CppClass* with the provided method name, that has a catch handler
#define MAKE_AUTO_HOOK_FIND_ORIG(name_, klass, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(klass, mName); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided Il2CppClass* with the provided method name.
#define MAKE_AUTO_HOOK_FIND_NO_CATCH_ORIG(name_, klass, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(klass, mName); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name, that has a catch handler.
#define MAKE_AUTO_HOOK_FIND_CLASS_ORIG(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(namespaze, klassName, mName); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
#define MAKE_AUTO_HOOK_FIND_CLASS_NO_CATCH_ORIG(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(namespaze, klassName, mName); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided Il2CppClass* with the provided method name.
// Ignores matching the first parameter, assuming it is an instance method.
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_FIND_INSTANCE_ORIG(name_, klass, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<typename ::il2cpp_utils::InstanceMethodConverter<funcType>::fType>::find(klass, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided Il2CppClass* with the provided method name.
// Ignores matching the first parameter, assuming it is an instance method.
#define MAKE_AUTO_HOOK_FIND_INSTANCE_NO_CATCH_ORIG(name_, klass, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<typename ::il2cpp_utils::InstanceMethodConverter<funcType>::fType>::find(klass, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
// Ignores matching the first parameter, assuming it is an instance method.
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_FIND_CLASS_INSTANCE_ORIG(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<typename ::il2cpp_utils::InstanceMethodConverter<funcType>::fType>::find(namespaze, klassName, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
// Ignores matching the first parameter, assuming it is an instance method.
#define MAKE_AUTO_HOOK_FIND_CLASS_INSTANCE_NO_CATCH_ORIG(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<typename ::il2cpp_utils::InstanceMethodConverter<funcType>::fType>::find(namespaze, klassName, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
// THIS FUNCTION IS THE UNSAFE VARIANT, SUBTRACTS ONE FOR INSTANCE METHODS!
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_FIND_CLASS_UNSAFE_INSTANCE_ORIG(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find_unsafe(namespaze, klassName, mName, true); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
// THIS FUNCTION IS THE UNSAFE VARIANT, SUBTRACTS ONE FOR INSTANCE METHODS!
#define MAKE_AUTO_HOOK_FIND_CLASS_UNSAFE_INSTANCE_NO_CATCH_ORIG(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find_unsafe(namespaze, klassName, mName, true); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
// THIS FUNCTION IS THE UNSAFE VARIANT!
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_FIND_CLASS_UNSAFE_STATIC_ORIG(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find_unsafe(namespaze, klassName, mName); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that finds a method that matches the signature provided and exists on the provided namespace and type name, with the provided method name.
// THIS FUNCTION IS THE UNSAFE VARIANT!
#define MAKE_AUTO_HOOK_FIND_CLASS_UNSAFE_STATIC_NO_CATCH_ORIG(name_, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find_unsafe(namespaze, klassName, mName); } \
    using funcType = retval (*)(__VA_ARGS__); \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that would be installed in a particular address, but ensures the signature matches the provided method pointer.
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_CHECKED_ADDR_ORIG(name_, mPtr, addr_, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    constexpr static void* addr() { return addr_; } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that would be installed in a particular address, but ensures the signature matches the provided method pointer.
#define MAKE_AUTO_HOOK_CHECKED_ADDR_NO_CATCH_ORIG(name_, mPtr, addr_, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    constexpr static void* addr() { return addr_; } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that ensures the signature matches the provided method pointer and finds a matching method from a class and method name.
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_CHECKED_FIND_ORIG(name_, mPtr, klass, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(klass, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that ensures the signature matches the provided method pointer and finds a matching method from a class and method name.
#define MAKE_AUTO_HOOK_CHECKED_FIND_NO_CATCH_ORIG(name_, mPtr, klass, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(klass, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that ensures the signature matches the provided method pointer and finds a matching method from namespace, name, and method name.
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_CHECKED_FIND_CLASS_ORIG(name_, mPtr, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(namespaze, klassName, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that ensures the signature matches the provided method pointer and finds a matching method from namespace, name, and method name.
#define MAKE_AUTO_HOOK_CHECKED_FIND_CLASS_NO_CATCH_ORIG(name_, mPtr, namespaze, klassName, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(namespaze, klassName, mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that ensures the signature matches the provided method pointer and finds a matching method from a class and method name.
// Also includes a catch handler.
#define MAKE_AUTO_HOOK_CHECKED_INSTANCE_FIND_ORIG(name_, mPtr, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    using classType = ::Hooking::InternalClassGetter<decltype(mPtr)>::instanceType; \
    static_assert(!std::is_same_v<classType, void>, "MAKE_HOOK_INSTANCE_FIND was not provided an instance method pointer!"); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(classof(classType), mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that ensures the signature matches the provided method pointer and finds a matching method from a class and method name.
#define MAKE_AUTO_HOOK_CHECKED_INSTANCE_FIND_NO_CATCH_ORIG(name_, mPtr, mName, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    using classType = ::Hooking::InternalClassGetter<decltype(mPtr)>::instanceType; \
    static_assert(!std::is_same_v<classType, void>, "MAKE_HOOK_INSTANCE_FIND was not provided an instance method pointer!"); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::MethodTypeCheck<funcType>::find(classof(classType), mName); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define MAKE_AUTO_HOOK_MATCH_ORIG(name_, mPtr, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(MATCH_HOOKABLE_ASSERT(mPtr)); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::il2cpp_type_check::MetadataGetter<mPtr>::methodInfo(); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return &::Hooking::HookCatchWrapper<&hook_##name_, funcType>::wrapper; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)

// Make a hook that uses the provided method pointer in a match an ensures the signature matches.
// This should be your go-to hook macro when hooking anything that has a codegen equivalent.
#define MAKE_AUTO_HOOK_MATCH_NO_CATCH_ORIG(name_, mPtr, retval, ...) \
struct Hook_##name_ { \
    using funcType = retval (*)(__VA_ARGS__); \
    static_assert(MATCH_HOOKABLE_ASSERT(mPtr)); \
    static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
    constexpr static const char* name() { return #name_; } \
    static const MethodInfo* getInfo() { return ::il2cpp_utils::il2cpp_type_check::MetadataGetter<mPtr>::methodInfo(); } \
    static funcType* trampoline() { return &name_; } \
    static inline retval (*name_)(__VA_ARGS__) = nullptr; \
    static funcType hook() { return hook_##name_; } \
    static retval hook_##name_(__VA_ARGS__); \
}; \
HOOK_AUTO_INSTALL_ORIG(name_); \
retval Hook_##name_::hook_##name_(__VA_ARGS__)
}

using namespace AutoHooks::HookMacros;
using namespace AutoHooks::OrigHookMacros
