#ifndef META_PROGRAMING_UTILS_LIBRARY_H
#define META_PROGRAMING_UTILS_LIBRARY_H
#include <functional>
#include <concepts>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <iomanip>
#include <cstddef>
#include <utility>
#include <sstream>
#include <cstdlib>
#include <span>
#include <iostream>

// Concept to check if a type T is common_with all other types Us...
template<typename T, typename... Us>
concept common_with_n_others = ( ... && (!std::same_as<T, Us> ? std::common_with<T, Us> : true));
// Concept to check if all types Ts... are common_with each other
template<typename... Args>
concept n_common_with = ( ... && common_with_n_others<Args, Args...> );

// Concept that ensures a function F can be invoked with arguments of types T1 and T2 and the result is convertible to bool
template <typename F, typename T1, typename T2>
concept BoolReturningFunction = requires(F f, T1 a, T2 b) {
    { f(a, b) } -> std::convertible_to<bool>;
};

template <typename Op, typename... Args>
concept InvocableWithAllAdjacentPairs = sizeof...(Args) < 2 || (
    []<std::size_t... Is>(std::index_sequence<Is...>) -> bool {
        return (... && std::regular_invocable<
            Op,
            std::tuple_element_t<Is, std::tuple<Args...>>,
            std::tuple_element_t<Is + 1, std::tuple<Args...>>
        >);
    }(std::make_index_sequence<sizeof...(Args) - 1>{})
);

// processPair function: Returns a string result of two vars comped with an operation.
template <typename Op, typename T1, typename T2>
void processPair(std::stringstream ss, Op op, const T1& a1, const T2& a2) {
    auto result = op(a1, a2);
    ss << "Comparing: LHS: " << toMoreHumanReadableish(a1)
              << " and RHS: " << toMoreHumanReadableish(a2)
              << " Produces: " << result << "\n";
}

template <typename T>
concept OStreamInsertable = requires(std::ostream& os, const T& value) {
    os << value;
};

template <typename T>
concept StringConstructible = std::is_constructible_v<std::string, std::remove_cvref_t<T>>;

template <typename T>
concept HasToString = requires(const T& t) {
    { std::to_string(t) } -> std::convertible_to<std::string>;
};

template <typename T>
concept StringConvertible = requires (T t) {
    { static_cast<std::string>(t) } -> std::same_as<std::string>;
};

template <typename T>
concept stdStringy = StringConstructible<T> || HasToString<T> || StringConvertible<T>;

template <typename T>
concept TriviallyCopyable = std::is_trivially_copyable_v<std::remove_cvref_t<T>>;

// Function to convert input to human-readable string
template <typename T>
std::string toMoreHumanReadableish(const T& value) {
    using U = std::remove_cvref_t<T>;
    if constexpr (OStreamInsertable<U>) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    } else if constexpr (stdStringy<U>) {
        if constexpr (StringConstructible<U>) {
            return std::string(value);
        } else if constexpr (HasToString<U>) {
            return std::to_string(value);
        } else if constexpr (StringConvertible<U>) {
            return static_cast<std::string>(value);
        } else {
            static_assert([] { return false; }(), "Type not actually stdString, should never see this, bug here");
        }
    } else if constexpr (TriviallyCopyable<U>) {
        std::span<const U> objSpan{&value, 1};
        std::span<const std::byte> byteSpan = std::as_bytes(objSpan);

        std::ostringstream oss;
        oss << std::hex << std::setfill('0');

        for (const auto& byte : byteSpan) {
            oss << std::setw(2) << static_cast<int>(byte);
        }
        return oss.str();
    } else {
        static_assert([] { return false; }(), "Type cannot be converted to human-readable string");
    }
}

template <typename... Args>
void printHumanReadableStringsDelimiter(std::stringstream& ss, std::string del, const Args&... args) {
    ((ss << toMoreHumanReadableish(args) << "del"), ...);
}

template <typename... Args>
auto convertToHumanReadableStringsTuple(const Args&... args) {
    return std::make_tuple(toMoreHumanReadableish(args)...);
}

// compareAndPrint function
template <typename Op, typename... Args>
requires InvocableWithAllAdjacentPairs<Op, Args...>
void compareAndPrint(std::stringstream& ss, Op op, const Args&... args) {
    auto argTuple = std::tie(args...);
    constexpr std::size_t N = sizeof...(Args);

    if constexpr (N >= 2) {
        // Use indices to iterate over the pairs
        [&]<std::size_t... I>(std::index_sequence<I...>) {
            (processPair(ss, op, std::get<I>(argTuple), std::get<I + 1>(argTuple)), ...);
        }(std::make_index_sequence<N - 1>{});
    }
}

// Function template that checks if two values are valid for a boolean operation,
// returning a debug string or a memory hex view using std::span
template <typename T1, typename T2, typename F>
requires BoolReturningFunction<F, T1, T2>
void debugCheck(std::stringstream& ss, const T1& a, const T2& b, F f) {
    compareAndPrint(ss, f, a, b);
}

template<typename T1>
void compileTimeEnforceMemorySafty(){
 }

/*template<typename... T, size_t N>
 requires std::SequenceContainer<T...>
std::vector<T> crossProuct(const std::vector<std::vector<T>>& args) {
  std::vector<T> result;
  std::size_t dims = v.size();
  std::vector<T> result(N);
   for(std::size_t i = 0; i < v.size(); ++i) {
     result[i] = v1[i] + v2[i];
} */

#endif //META_PROGRAMING_UTILS_LIBRARY_H