#pragma once

#include <unordered_map>

#include "ComputedStyle.h"

namespace gpugraph
{

    class ComputedStyleSet
    {
    public:
        static ComputedStyleSet const Initial;

        using SetType = std::unordered_map<std::uint8_t, std::shared_ptr<ComputedStyle const>>;
        ComputedStyleSet() = default;
        ComputedStyleSet(SetType);

        void clear();

        void unset(StyleIdentifier);
        void insert(StyleIdentifier, std::shared_ptr<ComputedStyle const>);

        template<typename T>
        std::shared_ptr<T const> get() const;

        template<typename T>
        bool contains() const;

        std::shared_ptr<ComputedStyle const> operator[](StyleIdentifier) const;

    private:
        SetType _set;
    };

    inline void ComputedStyleSet::clear()
    {
        _set.clear();
    }

    inline void ComputedStyleSet::unset(StyleIdentifier identifier)
    {
        _set.erase(static_cast<std::uint8_t>(identifier));
    }

    inline ComputedStyleSet::ComputedStyleSet(SetType set)
        : _set(std::move(set))
    {
    }

    inline std::shared_ptr<ComputedStyle const> ComputedStyleSet::operator[](StyleIdentifier index) const
    {
        auto it = _set.find(static_cast<std::uint8_t>(index));
        return it == _set.end() ? nullptr : it->second;
    }

    inline void ComputedStyleSet::insert(StyleIdentifier identifier, std::shared_ptr<ComputedStyle const> style)
    {
        _set[static_cast<std::uint8_t> (identifier)] = std::move(style);
    }

    template<typename T>
    std::shared_ptr<T const> ComputedStyleSet::get() const
    {
        return std::static_pointer_cast<T const>(ComputedStyleSet::operator[](T::Identifier));
    }

    template<typename T>
    bool ComputedStyleSet::contains() const
    {
        return _set.count(static_cast<std::uint8_t>(T::Identifier));
    }

}
