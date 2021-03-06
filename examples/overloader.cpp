
#include <iomanip>
#include <iostream>
#include <string>

#include <rttg/rttg.h>

using TupleT = std::tuple<int, double, std::string>;

int main()
{
    const TupleT tuple{ 5, 3.0, "str"};
    for (std::size_t idx{}; idx < std::tuple_size_v<TupleT>; ++idx)
    {
        rttg::visit(rttg::Overloader {
            [](auto arg) { std::cout << arg << ' '; },
            [](double arg) { std::cout << std::fixed << arg << ' '; },
            [](const std::string & arg) { std::cout << std::quoted(arg) << ' '; } },
            rttg::get(tuple, idx));
    }
}
