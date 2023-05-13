#define CALL_PREFIX_OPERATOR(prefix_operator) \
switch (type) \
{ \
case array_type: \
	prefix_operator##get<iterator_type_enum::array_type>(current_iterator); \
	return *this; \
case container_type: \
	prefix_operator##get<iterator_type_enum::container_type>(current_iterator); \
	return *this; \
default: \
	return *this; \
}

#define CALL_POSTFIX_OPERATOR(postfix_operator) \
Context::BaseContextIterator temp = *this; \
switch (type) \
{ \
case array_type: \
	postfix_operator##get<iterator_type_enum::array_type>(current_iterator); \
	return temp; \
case container_type: \
	postfix_operator##get<iterator_type_enum::container_type>(current_iterator); \
	return temp; \
default: \
	return temp; \
}

template<typename ArrayIteratorT, typename ContainerIteratorT>
Context::BaseContextIterator<ArrayIteratorT, ContainerIteratorT>::BaseContextIterator(const iterator_type& iterator) :
	current_iterator(iterator),
	type(static_cast<iterator_type_enum>(iterator.index()))
{

}

template<typename ArrayIteratorT, typename ContainerIteratorT>
Context::BaseContextIterator<ArrayIteratorT, ContainerIteratorT>& Context::BaseContextIterator<ArrayIteratorT, ContainerIteratorT>::operator++() noexcept
{
	CALL_PREFIX_OPERATOR(++);
}

template<typename ArrayIteratorT, typename ContainerIteratorT>
Context::BaseContextIterator<ArrayIteratorT, ContainerIteratorT> Context::BaseContextIterator<ArrayIteratorT, ContainerIteratorT>::operator++(int) noexcept
{
	CALL_POSTFIX_OPERATOR(++);
}

template<typename ArrayIteratorT, typename ContainerIteratorT>
Context::BaseContextIterator<ArrayIteratorT, ContainerIteratorT>& Context::BaseContextIterator<ArrayIteratorT, ContainerIteratorT>::operator--() noexcept
{
	CALL_PREFIX_OPERATOR(--);
}

template<typename ArrayIteratorT, typename ContainerIteratorT>
Context::BaseContextIterator<ArrayIteratorT, ContainerIteratorT> Context::BaseContextIterator<ArrayIteratorT, ContainerIteratorT>::operator--(int) noexcept
{
	CALL_POSTFIX_OPERATOR(--);
}

template<typename ArrayIteratorT, typename ContainerIteratorT>
bool Context::BaseContextIterator<ArrayIteratorT, ContainerIteratorT>::operator==(const Context::BaseContextIterator<ArrayIteratorT, ContainerIteratorT>& other) const noexcept
{
	return current_iterator == other.current_iterator;
}

#undef CALL_PREFIX_OPERATOR
#undef CALL_POSTFIX_OPERATOR
