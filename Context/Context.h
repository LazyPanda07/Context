#pragma once

#include <variant>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>

#if defined(WIN32) && defined(CONTEXT_EXPORT)
#define CONTEXT_API __declspec(dllexport)
#else
#define CONTEXT_API
#endif // CONTEXT_API

class CONTEXT_API Context
{
private:
	using type = std::variant<
		std::nullptr_t,
		int, double, bool, std::string,
		std::vector<Context>,
		std::unordered_map<std::string, Context>
	>;

	enum type_enum
	{
		int_type = 1,
		double_type,
		bool_type,
		string_type,
		array_type,
		container_type
	};

private:
	type data;
	int spaces_per_depth;

private:
	template<typename T>
	static bool recursive_remove(const T& remove_value, std::vector<Context>& data);

	template<typename T>
	static bool recursive_remove(const T& remove_value, std::unordered_map<std::string, Context>& data);

public:
	template<typename ArrayIteratorT, typename ContainerIteratorT>
	class CONTEXT_API BaseContextIterator
	{
	public:
		using iterator_array_type = ArrayIteratorT;
		using iterator_container_type = ContainerIteratorT;
		using iterator_type = std::variant<iterator_array_type, iterator_container_type>;

		enum iterator_type_enum
		{
			array_type,
			container_type
		};

	protected:
		iterator_type current_iterator;
		iterator_type_enum type;

	public:
		BaseContextIterator(const iterator_type& iterator);

		BaseContextIterator(const BaseContextIterator& other) = default;

		BaseContextIterator(BaseContextIterator&& other) noexcept = default;

		BaseContextIterator& operator=(const BaseContextIterator& other) = default;

		BaseContextIterator& operator=(BaseContextIterator&& other) noexcept = default;

		BaseContextIterator& operator++() noexcept;

		BaseContextIterator operator++(int) noexcept;

		BaseContextIterator& operator--() noexcept;

		BaseContextIterator operator--(int) noexcept;

		bool operator==(const BaseContextIterator& other) const noexcept;

		virtual ~BaseContextIterator() = default;
	};

	class CONTEXT_API ContextIterator : public BaseContextIterator<std::vector<Context>::iterator, std::unordered_map<std::string, Context>::iterator>
	{
	public:
		ContextIterator(const iterator_type& iterator);

		Context& operator*() noexcept;

		Context* operator->() noexcept;

		~ContextIterator() = default;

		friend class Context;
	};

	class CONTEXT_API ConstContextIterator : public BaseContextIterator<std::vector<Context>::const_iterator, std::unordered_map<std::string, Context>::const_iterator>
	{
	public:
		ConstContextIterator(const iterator_type& iterator);

		const Context& operator*() noexcept;

		const Context* operator->() noexcept;

		~ConstContextIterator() = default;

		friend class Context;
	};

	class CONTEXT_API InvalidContextIterator : public std::runtime_error
	{
	public:
		InvalidContextIterator();

		~InvalidContextIterator() = default;
	};

public:
	Context();

	explicit Context(int value);

	explicit Context(double value);

	explicit Context(bool value);

	explicit Context(const std::string& value);

	explicit Context(std::string&& value);

	explicit Context(const char* value);

	explicit Context(std::initializer_list<double> values);

	explicit Context(std::initializer_list<int> values);

	Context(const Context& other);

	Context(Context&& other) noexcept;

	Context& operator=(const Context& other);

	Context& operator=(Context&& other) noexcept;

	Context& add_element(const std::string& key, const Context& context);

	Context& add_element(const std::string& key, Context&& context);

	Context& add_element(const Context& context);

	Context& add_element(Context&& context);

	bool is_valid() const noexcept;

	bool is_int() const;

	bool is_double() const;

	bool is_bool() const;

	bool is_string() const;

	bool is_array() const;

	bool is_container() const;

	bool insert(size_t index, const Context& context);

	bool insert(size_t index, Context&& context);

	bool pop_back();

	bool remove(const Context& context, bool recursive = false);

	bool remove(size_t index);

	/*
	* @param recursive If key not in this container then try to find key in other containers inside this
	*/
	bool remove(const std::string& key, bool recursive = false);

	ContextIterator remove(const ContextIterator& it);

	ConstContextIterator remove(const ConstContextIterator& it);

	ContextIterator find(const std::string& key);

	ContextIterator find(const Context& context);

	ConstContextIterator find(const std::string& key) const;

	ConstContextIterator find(const Context& context) const;

	ConstContextIterator begin() const;

	ContextIterator begin();

	ConstContextIterator end() const;

	ContextIterator end();

	void set_scalar(int value);
	
	void set_scalar(double value);
	
	void set_scalar(bool value);
	
	void set_scalar(const std::string& value);

	void set_scalar(std::string&& value);

	int get_int() const;

	double get_double() const;

	bool get_bool() const;

	const std::string& get_string() const;

	std::vector<Context>& get_array();

	const std::vector<Context>& get_array() const;

	std::unordered_map<std::string, Context>& get_container();

	const std::unordered_map<std::string, Context>& get_container() const;

	const Context& get_element(const std::string& key);
	
	std::string get_str(int depth = 1) const;

	Context& operator[](size_t index);

	Context& operator[](const std::string& key);

	bool operator==(const Context& other) const noexcept;

	explicit operator bool() const noexcept;

	~Context() = default;
};

#include "Context.inl"
