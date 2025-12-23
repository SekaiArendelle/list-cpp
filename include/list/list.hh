#pragma once

#include <cstddef>
#include <memory>
#include <type_traits>
#include <concepts>

namespace list {

namespace details {

template<typename T>
concept pointer = ::std::is_pointer_v<T>;

template<typename Allocator>
concept allocator = requires(Allocator allocator) {
    { allocator.allocate(::std::size_t{}) } -> pointer;
    { allocator.deallocate(nullptr, ::std::size_t{}) } -> ::std::same_as<void>;
};

struct ListPtrGroup {
    ListPtrGroup* next{this};
    ListPtrGroup* prev{this};

protected:
    constexpr ListPtrGroup const* get_self_ptr_group() const noexcept {
        return this;
    }

    constexpr ListPtrGroup* get_self_ptr_group() noexcept {
        return this;
    }
};

} // namespace details

template<typename T, details::allocator Allocator = ::std::allocator<T>>
class List : private details::ListPtrGroup {
public:
    using value_type = T;
    using size_type = ::std::size_t;
    using difference_type = ::std::ptrdiff_t;

private:
    struct Node : public details::ListPtrGroup {
        value_type value;
    };

public:
    using allocator_type = ::std::allocator_traits<Allocator>::template rebind_alloc<Node>;

private:
#if __has_cpp_attribute(no_unique_address)
    [[no_unique_address]]
#elif __has_cpp_attribute(msvc::no_unique_address)
    [[msvc::no_unique_address]]
#endif
    allocator_type allocator{};

public:
    constexpr List() noexcept = default;

    template<::std::same_as<value_type>... Args>
        requires (sizeof...(Args) > 0)
    constexpr List(Args&&... args) {
        // TODO
    }

    constexpr List(List<T, Allocator> const& other) {
        // TODO
    }

    constexpr List(List<T, Allocator>&& other) {
        // TODO
    }

    constexpr ~List() noexcept {
        // TODO
    }

    constexpr auto operator=(this List& self, List const& other) -> List& {
    } // TODO

    constexpr auto operator=(this List& self, List&& other) -> List& {
    } // TODO

    constexpr auto is_empty(this List const& self) noexcept -> bool {
        // self.next == self.prev == this
        return self.next == self.get_self_ptr_group();
    }

    constexpr auto has_single(this List const& self) noexcept -> bool {
        // self.next == self.prev != this
        return self.next != self.get_self_ptr_group() && self.next == self.prev;
    }

    constexpr auto has_multiple(this List const& self) noexcept -> bool {
        return self.next != self.prev;
    }

    constexpr void push_front(this auto&& self, T const& value) noexcept(
        noexcept(self.allocator.allocate(sizeof(Node))) &&
        noexcept(::std::construct_at(self.allocator.allocate(sizeof(Node)), value))) {
        Node* ptr = static_cast<Node*>(self.allocator.allocate(sizeof(Node)));
        ::std::construct_at(::std::addressof(ptr->value), value);
        ptr->next = self.next;
        ptr->prev = static_cast<details::ListPtrGroup*>(::std::addressof(self));
        self.next = ptr;
        ptr->next->prev = ptr;
    }

    constexpr void push_front(this auto&& self,
                              T&& value) noexcept(noexcept(self.allocator.allocate(sizeof(Node))) &&
                                                  noexcept(::std::construct_at(self.allocator.allocate(sizeof(Node)),
                                                                               ::std::move(value)))) {
        Node* ptr = static_cast<Node*>(self.allocator.allocate(sizeof(Node)));
        ::std::construct_at(::std::addressof(ptr->value), ::std::move(value));
        ptr->next = self.next;
        ptr->prev = static_cast<details::ListPtrGroup*>(::std::addressof(self));
        self.next = ptr;
        ptr->next->prev = ptr;
    }
};

} // namespace list
