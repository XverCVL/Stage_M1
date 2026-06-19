#pragma once

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <utility>
#include <vector>

namespace solverco::utils::containers {

class DynamicBitset {
   public:
    static constexpr size_t NPOS = static_cast<size_t>(-1);
    static constexpr size_t BITS_PER_WORD = 64U;
    static constexpr size_t WORD_MASK = 63U;
    static constexpr size_t WORD_SHIFT = 6U;

    struct Reference {
        uint64_t& word;
        uint64_t mask;

        Reference(uint64_t& w, uint64_t m) : word(w), mask(m) {}

        ~Reference() = default;

        Reference(const Reference&) = default;

        Reference(Reference&&) = default;

        explicit operator bool() const noexcept
        {
            return (word & mask) != 0;
        }

        Reference& operator=(bool b) noexcept
        {
            if (b) {
                word |= mask;
            } else {
                word &= ~mask;
            }
            return *this;
        }

        Reference& operator=(const Reference& other) noexcept
        {
            return *this = static_cast<bool>(other);
        }

        Reference& operator=(Reference&&) = delete;

        void flip() noexcept
        {
            word ^= mask;
        }
    };

    explicit DynamicBitset(size_t n = 0) : num_bits_(n), words_((n + WORD_MASK) >> WORD_SHIFT, 0ULL)
    {}

    void resize(size_t n)
    {
        num_bits_ = n;
        words_.resize((n + WORD_MASK) >> WORD_SHIFT, 0ULL);
        clean_padding();
    }

    [[nodiscard]] size_t size() const noexcept
    {
        return num_bits_;
    }
    [[nodiscard]] bool empty() const noexcept
    {
        return num_bits_ == 0;
    }

    [[nodiscard]] bool test(size_t i) const
    {
        if (i >= num_bits_) {
            throw std::out_of_range("DynamicBitset index out of range");
        }
        return (words_[i >> WORD_SHIFT] & (1ULL << (i & WORD_MASK))) != 0;
    }

    [[nodiscard]] bool operator[](size_t i) const noexcept
    {
        return (words_[i >> WORD_SHIFT] & (1ULL << (i & WORD_MASK))) != 0;
    }

    Reference operator[](size_t i) noexcept
    {
        return {words_[i >> WORD_SHIFT], 1ULL << (i & WORD_MASK)};
    }

    void set(size_t i)
    {
        words_[i >> WORD_SHIFT] |= (1ULL << (i & WORD_MASK));
    }

    void reset(size_t i)
    {
        words_[i >> WORD_SHIFT] &= ~(1ULL << (i & WORD_MASK));
    }

    void flip(size_t i)
    {
        words_[i >> WORD_SHIFT] ^= (1ULL << (i & WORD_MASK));
    }

    void set_all()
    {
        std::fill(words_.begin(), words_.end(), ~0ULL);
        clean_padding();
    }

    void reset_all() noexcept
    {
        std::fill(words_.begin(), words_.end(), 0ULL);
    }

    void flip_all() noexcept
    {
        for (auto& w : words_) {
            w = ~w;
        }
        clean_padding();
    }

    [[nodiscard]] size_t count() const noexcept
    {
        size_t c = 0;
        for (const uint64_t& w : words_) {
            c += static_cast<size_t>(__builtin_popcountll(w));
        }
        return c;
    }

    [[nodiscard]] bool any() const noexcept
    {
        for (const uint64_t& w : words_) {  // NOLINT
            if (w != 0) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] bool none() const noexcept
    {
        return !any();
    }

    [[nodiscard]] bool all() const noexcept
    {
        if (empty()) {
            return true;
        }
        for (size_t i = 0; i < words_.size() - 1; ++i) {
            if (words_[i] != ~0ULL) {
                return false;
            }
        }
        uint64_t last_mask =
            (num_bits_ & WORD_MASK) == 0 ? ~0ULL : (1ULL << (num_bits_ & WORD_MASK)) - 1ULL;
        return words_.back() == last_mask;
    }

    DynamicBitset& operator&=(const DynamicBitset& other)
    {
        size_t min_words = std::min(words_.size(), other.words_.size());
        for (size_t i = 0; i < min_words; ++i) {
            words_[i] &= other.words_[i];
        }
        if (words_.size() > min_words) {
            std::fill(
                words_.begin() + static_cast<std::vector<uint64_t>::difference_type>(min_words),
                words_.end(), 0ULL);
        }
        return *this;
    }

    void and_not(const DynamicBitset& other) noexcept
    {
        size_t min_words = std::min(words_.size(), other.words_.size());
        for (size_t i = 0; i < min_words; ++i) {
            words_[i] &= ~other.words_[i];
        }
    }

    DynamicBitset& operator|=(const DynamicBitset& other)
    {
        if (other.num_bits_ > num_bits_) {
            resize(other.num_bits_);
        }
        for (size_t i = 0; i < other.words_.size(); ++i) {
            words_[i] |= other.words_[i];
        }
        return *this;
    }

    DynamicBitset& operator^=(const DynamicBitset& other)
    {
        if (other.num_bits_ > num_bits_) {
            resize(other.num_bits_);
        }
        for (size_t i = 0; i < other.words_.size(); ++i) {
            words_[i] ^= other.words_[i];
        }
        clean_padding();
        return *this;
    }

    bool operator==(const DynamicBitset& other) const noexcept
    {
        if (num_bits_ != other.num_bits_) {
            return false;
        }
        return words_ == other.words_;
    }

    bool operator!=(const DynamicBitset& other) const noexcept
    {
        return !(*this == other);
    }

    template <typename F>
    void for_each_bit(F&& func) const
    {
        for (size_t i = 0; i < words_.size(); ++i) {
            uint64_t w = words_[i];
            while (w != 0) {
                int bit = __builtin_ctzll(w);
                func((i << WORD_SHIFT) | static_cast<size_t>(bit));
                w &= (w - 1);
            }
        }
    }

    [[nodiscard]] size_t find_first() const noexcept
    {
        for (size_t i = 0; i < words_.size(); ++i) {
            if (words_[i] != 0) {
                return (i << WORD_SHIFT) + static_cast<size_t>(__builtin_ctzll(words_[i]));
            }
        }
        return NPOS;
    }

    [[nodiscard]] size_t find_next(size_t prev) const noexcept
    {
        size_t bit_idx = prev + 1;
        size_t word_idx = bit_idx >> WORD_SHIFT;
        if (word_idx >= words_.size()) {
            return NPOS;
        }
        uint64_t w = words_[word_idx] & (~0ULL << (bit_idx & WORD_MASK));
        if (w != 0) {
            return (word_idx << WORD_SHIFT) + static_cast<size_t>(__builtin_ctzll(w));
        }
        for (size_t i = word_idx + 1; i < words_.size(); ++i) {
            if (words_[i] != 0) {
                return (i << WORD_SHIFT) + static_cast<size_t>(__builtin_ctzll(words_[i]));
            }
        }
        return NPOS;
    }

   private:
    size_t num_bits_;
    std::vector<uint64_t> words_;

    void clean_padding() noexcept
    {
        if (words_.empty()) {
            return;
        }
        size_t extra_bits = num_bits_ & WORD_MASK;
        if (extra_bits != 0) {
            words_.back() &= (1ULL << extra_bits) - 1ULL;
        }
    }
};

}  // namespace solverco::utils::containers
