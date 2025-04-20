#ifndef AUL_ALLOCATOR_AWARE_BASE_HPP
#define AUL_ALLOCATOR_AWARE_BASE_HPP

#include <memory>

namespace aul {

    template<class A>
    struct Stateful_allocator_container_base {
    protected:

        //=================================================
        // Type aliases
        //=================================================

        using alloc_traits = typename std::allocator_traits<A>;

        using size_type = typename alloc_traits::size_type;

        using pointer = typename alloc_traits::pointer;

        //=================================================
        // -ctors
        //=================================================

        explicit Stateful_allocator_container_base(const A& a):
        allocator(a) {}

        explicit Stateful_allocator_container_base(A&& a):
            allocator(std::forward<A&&>(a)) {}

        Stateful_allocator_container_base() = default;
        ~Stateful_allocator_container_base() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Stateful_allocator_container_base& operator=(const Stateful_allocator_container_base& rhs) {
            if constexpr (std::allocator_traits<A>::propagate_on_container_copy_assignment::value) {
                allocator = rhs.allocator;
            }

            return *this;
        }

        Stateful_allocator_container_base& operator=(Stateful_allocator_container_base&& rhs) {
            if constexpr (std::allocator_traits<A>::propagate_on_container_move_assignment::value) {
                allocator = std::move(rhs.allocator);
            }

            return *this;
        }

        //=================================================
        // Assignment operators
        //=================================================

        bool operator==(Stateful_allocator_container_base& rhs) {
            return (allocator == rhs.allocator);
        }

        bool operator!=(Stateful_allocator_container_base& rhs) {
            return (allocator != rhs.allocator);
        }

        //=================================================
        // Allocation methods
        //=================================================

        pointer allocate(size_type n) {
            return alloc_traits::allocate(allocator, n);
        }

        void deallocate(pointer p, size_type n) {
            return alloc_traits::deallocate(allocator, p, n);
        }

        //=================================================
        // Construction methods
        //=================================================

        template<class T, class...Args>
        void construct(T* p, Args&&...args) {
            alloc_traits::construct(allocator, p, std::forward<Args>(args)...);
        }

        template<class T>
        void destroy(T* p) {
            alloc_traits::destroy(allocator, p);
        }

        //=================================================
        // Accessors
        //=================================================

        A get_allocator() const {
            return allocator;
        }

        //=================================================
        // Misc. methods
        //=================================================

        void swap(Stateful_allocator_container_base& rhs) {
            if constexpr (std::allocator_traits<A>::propagate_on_container_move_assignment::value) {
                std::swap(allocator, rhs.allocator);
            }
        }

        //=================================================
        // Instance members
        //=================================================

        A allocator;

    };





    template<class A>
    struct Empty_allocator_container_base {
    protected:

        //=================================================
        // Type aliases
        //=================================================

        using alloc_traits = typename std::allocator_traits<A>;

        using size_type = typename alloc_traits::size_type;

        using pointer = typename alloc_traits::pointer;

        //=================================================
        // -ctors
        //=================================================

        explicit Empty_allocator_container_base(const A& a) {};

        explicit Empty_allocator_container_base(A&& a) {};

        Empty_allocator_container_base() = default;
        ~Empty_allocator_container_base() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Empty_allocator_container_base& operator=(const Empty_allocator_container_base& rhs) = default;
        Empty_allocator_container_base& operator=(Empty_allocator_container_base&& rhs) noexcept = default;

        //=================================================
        // Assignment operators
        //=================================================

        bool operator==(Empty_allocator_container_base& rhs) {
            return true;
        }

        bool operator!=(Empty_allocator_container_base& rhs) {
            return false;
        }

        //=================================================
        // Allocation methods
        //=================================================

        pointer allocate(size_type n) {
            A allocator{};
            return alloc_traits::allocate(allocator, n);
        }

        void deallocate(pointer p, size_type n) {
            A allocator{};
            return alloc_traits::deallocate(allocator, p, n);
        }

        //=================================================
        // Construction methods
        //=================================================

        template<class T, class...Args>
        void construct(T* p, Args&&...args) {
            A allocator{};
            alloc_traits::construct(allocator, p, std::forward<Args>(args)...);
        }

        template<class T>
        void destroy(T* p) {
            A allocator{};
            alloc_traits::destroy(allocator, p);
        }

        //=================================================
        // Accessors
        //=================================================

        A get_allocator() const {
            return A{};
        }

        //=================================================
        // Misc. methods
        //=================================================

        void swap(Empty_allocator_container_base& rhs) {}

    };





    template<class A>
    struct Allocator_aware_base : public std::conditional_t<
        std::allocator_traits<A>::is_always_equal::value,
        Empty_allocator_container_base<A>,
        Stateful_allocator_container_base<A>
    > {

        using base = std::conditional_t<
            std::allocator_traits<A>::is_always_equal::value,
            Empty_allocator_container_base<A>,
            Stateful_allocator_container_base<A>
        >;

        using base::base;

        using base::allocate;
        using base::deallocate;

        using base::construct;
        using base::destroy;

    };

}

#endif //AUL_ALLOCATOR_AWARE_BASE_HPP
