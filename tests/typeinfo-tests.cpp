#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>

#include <accel/typeinfo>

template<typename T>
void _add_type_info(std::vector<const accel::type_info*>& type_infos)
{
    type_infos.push_back(&accel::get_qualified_type_info<T>());
    type_infos.push_back(&accel::get_qualified_type_info<const T>());
    type_infos.push_back(&accel::get_qualified_type_info<volatile T>());
    type_infos.push_back(&accel::get_qualified_type_info<const volatile T>());
    type_infos.push_back(&accel::get_qualified_type_info<T*>());
    type_infos.push_back(&accel::get_qualified_type_info<const T*>());
    type_infos.push_back(&accel::get_qualified_type_info<volatile T*>());
    type_infos.push_back(&accel::get_qualified_type_info<const volatile T*>());
    type_infos.push_back(&accel::get_qualified_type_info<T&>());
    type_infos.push_back(&accel::get_qualified_type_info<const T&>());
    type_infos.push_back(&accel::get_qualified_type_info<volatile T&>());
    type_infos.push_back(&accel::get_qualified_type_info<const volatile T&>());
    type_infos.push_back(&accel::get_qualified_type_info<T&&>());
    type_infos.push_back(&accel::get_qualified_type_info<const T&&>());
    type_infos.push_back(&accel::get_qualified_type_info<volatile T&&>());
    type_infos.push_back(&accel::get_qualified_type_info<const volatile T&&>());
    type_infos.push_back(&accel::get_qualified_type_info<T[]>());
    type_infos.push_back(&accel::get_qualified_type_info<const T[]>());
    type_infos.push_back(&accel::get_qualified_type_info<volatile T[]>());
    type_infos.push_back(&accel::get_qualified_type_info<const volatile T[]>());
}

template<typename T, typename... Ts>
struct add_type_info
{
    static void add(std::vector<const accel::type_info*>& type_infos)
    {
        _add_type_info<T>(type_infos);
        add_type_info<Ts...>::add(type_infos);
    }
};

template<typename T>
struct add_type_info<T>
{
    static void add(std::vector<const accel::type_info*>& type_infos)
    {
        _add_type_info<T>(type_infos);
    }
};

template<typename TupleT>
struct add_tuple;

template<typename... Ts>
struct add_tuple<std::tuple<Ts...>>
{
    static void add(std::vector<const accel::type_info*>& type_infos)
    {
        add_type_info<Ts...>::add(type_infos);
    }
};

template <typename T>
bool is_unique(std::vector<T>& vec)
{
    std::sort(vec.begin(), vec.end());
    return std::unique(vec.begin(), vec.end()) == vec.end();
}

int main()
{
    std::tuple<
        char,
        unsigned char,
        short,
        unsigned short,
        int,
        unsigned int,
        long,
        unsigned long,
        long long,
        unsigned long long,
        float,
        double,
        long double,
        bool> types;

    std::vector<const accel::type_info*> type_infos;
    add_tuple<decltype(types)>::add(type_infos);

    std::vector<std::string> type_names;
    type_names.reserve(type_infos.size());
    for (const auto& type_info : type_infos)
    {
        type_names.push_back(type_info->get_name());
    }
    ACC_ASSERT(is_unique(type_names));

    std::vector<accel::type_id> type_indices;
    type_indices.reserve(type_infos.size());
    for (const auto& type_info : type_infos)
    {
        type_indices.push_back(type_info->get_index());
    }
    ACC_ASSERT(is_unique(type_indices));

    std::vector<accel::type_id> type_hashes;
    type_hashes.reserve(type_infos.size());
    for (const auto& type_info : type_infos)
    {
        type_hashes.push_back(type_info->get_hash());
    }
    ACC_ASSERT(is_unique(type_hashes));

    std::cout << "All tests passed!" << std::endl;

    return 0;
}