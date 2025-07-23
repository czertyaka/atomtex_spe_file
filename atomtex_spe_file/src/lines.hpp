#pragma once

#include <iterator>
#include <cstddef>
#include <string_view>
#include <concepts>
#include <stdexcept>
#include <optional>
#include <algorithm>
#include <format>

namespace atomtex_spe_file
{

template<typename T>
concept Character = std::same_as<T, char> || std::same_as<T, signed char> ||
                    std::same_as<T, unsigned char> ||
                    std::same_as<T, wchar_t> || std::same_as<T, char8_t> ||
                    std::same_as<T, char16_t> || std::same_as<T, char32_t>;

template<class T>
concept StringReverseIterator = requires {
    requires Character<std::iter_value_t<T>>;
    requires std::bidirectional_iterator<T>;
    typename std::remove_cvref_t<T>::iterator_type;
    requires std::same_as<std::remove_cvref_t<T>,
        std::reverse_iterator<typename std::remove_cvref_t<T>::iterator_type>>;
};

template<class T>
concept StringForwardIterator = requires {
    requires Character<std::iter_value_t<T>>;
    requires std::bidirectional_iterator<T>;
    requires !StringReverseIterator<T>;
};

template<class Iterator>
    requires StringForwardIterator<Iterator> || StringReverseIterator<Iterator>
class Lines
{
public:
    using Char = typename Iterator::value_type;
    using CIterator = std::const_iterator<Iterator>;
    using Line = std::basic_string_view<Char>;

    Lines(CIterator begin, CIterator end, const Char separator = '\n');
    Line operator[](const std::size_t number) const;

private:
    const CIterator begin_;
    const CIterator end_;
    const Char separator_;

    struct Cache
    {
        std::size_t number;
        CIterator it;
    } mutable cache_;
};

template<class Iterator>
    requires StringForwardIterator<Iterator> || StringReverseIterator<Iterator>
Lines<Iterator>::Lines(CIterator begin, CIterator end, const Char separator)
    : begin_(begin), end_(end), separator_(separator), cache_({0, begin})
{
    if (begin_ == end_)
    {
        throw std::invalid_argument("Empty range");
    }
}

template<class Iterator>
    requires StringForwardIterator<Iterator> || StringReverseIterator<Iterator>
Lines<Iterator>::Line Lines<Iterator>::operator[](
    const std::size_t number) const
{
    CIterator it{number > cache_.number ? begin_ : cache_.it};

    for (std::size_t i{0}; i < number; ++i)
    {
        it = std::ranges::find(it, end_, separator_);
        if (it++ == end_)
        {
            const auto err{std::format("{} is out of range", number)};
            throw std::out_of_range(err);
        }
    }

    CIterator next = std::find(it, end_, separator_);

    cache_ = {number, it};

    if constexpr (StringReverseIterator<Iterator>)
    {
        return Line(next.base(), it.base());
    }
    else
    {
        return Line(it.base(), next.base());
    }
}

} // namespace atomtex_spe_file
