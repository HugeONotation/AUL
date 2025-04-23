#ifndef AUL_SBO_BASE_HPP
#define AUL_SBO_BASE_HPP

#include <cstdint>
#include <cstddef>

namespace aul {

    template<std::size_t Size, std::size_t Alignment = alignof(std::max_align_t)>
    class SBO_base {
    public:

        //=================================================
        // Static constants
        //=================================================

        static constexpr std::size_t size = Size;
        static constexpr std::size_t alignment = Alignment;

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        std::byte* small_buffer() {
            return buffer;
        }

        [[nodiscard]]
        const std::byte* small_buffer() const {
            return buffer;
        }

    private:

        //=================================================
        // Instance fields
        //=================================================

        alignas(Alignment) std::byte buffer[Size];

    };

    template<std::size_t Alignment>
    class SBO_base<0, Alignment> {
    public:

        //=================================================
        // Static constants
        //=================================================

        static constexpr std::size_t size = 0;
        static constexpr std::size_t alignment = Alignment;

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        std::byte* small_buffer() {
            return nullptr;
        }

        [[nodiscard]]
        const std::byte* small_buffer() const {
            return nullptr;
        }

    };

}

#endif //AUL_SBO_BASE_HPP
