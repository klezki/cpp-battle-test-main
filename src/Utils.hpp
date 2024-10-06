#include <cassert>
#include <functional>
#include <type_traits>
#include <optional>


template<class T>
std::optional<std::pair<T, T>> getFirstElement(const T radius, const T x, const T y, const std::vector<std::vector<T>>& map, const std::function<bool(const T&)>& pred)
{
    // can be narrowing, but it should be fine
    T i_max = T( map.size() - 1 );
    T j_max = T( map[0].size() - 1 );

    static_assert(std::is_integral_v<T>);
    assert(x >= 0 && x <= i_max);
    assert(y >= 0 && y <= j_max);
    assert(radius > 0);

    // when T is unsigned, we have to be careful to avoid wrap around on substraction (e.g. 0u - 1u)

    // top left to top right
    if (x >= radius)
    {
        auto i = x - radius;

        auto start = y > radius ? y - radius : 0;
        auto end = y + radius <= j_max ? y + radius : j_max + 1;

        for (auto j = start; j < end; ++j)
        {
            const auto& elem = map[i][j];
            if (pred(elem))
                return std::optional<std::pair<T, T>>{ std::in_place, i, j };
        }
    }

    // top right to bottom right
    if (y + radius <= j_max)
    {
        auto j = y + radius;

        auto start = x > radius ? x - radius : 0;
        auto end = x + radius <= i_max ? x + radius : i_max + 1;

        for (auto i = start; i < end; ++i)
        {
            const auto& elem = map[i][j];
            if (pred(elem))
                return std::optional<std::pair<T, T>>{ std::in_place, i, j };;
        }
    }

    // bottom right to bottom left
    if (x + radius <= i_max)
    {
        auto i = x + radius;

        auto end = y >= radius ? y - radius + 1 : 0;
        auto start = y + radius < j_max ? y + radius : j_max;

        auto j = start;
        do
        {
            const auto& elem = map[i][j];
            if (pred(elem))
                return std::optional<std::pair<T, T>>{ std::in_place, i, j };;
        } while (j-- > end);
    }


    // bottom left to top left
    if (y >= radius)
    {
        auto j = y - radius;

        auto end = x >= radius ? x - radius + 1 : 0;
        auto start = x + radius < i_max ? x + radius : i_max;

        auto i = start;
        do
        {
            const auto& elem = map[i][j];
            if (pred(elem))
                return std::optional<std::pair<T, T>>{ std::in_place, i, j };;
        } while (i-- > end);
    }
    
    return {};
}
