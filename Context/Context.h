#pragma once

#include <variant>
#include <vector>
#include <string>
#include <unordered_map>

class Context
{
private:
	using type = std::variant<
		nullptr_t,
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

public:
	Context();

	explicit Context(int value);

	explicit Context(double value);

	explicit Context(bool value);

	explicit Context(const std::string& value);

	explicit Context(std::string&& value);

	explicit Context(const char* value);

	explicit Context(std::initializer_list<double> values);

	Context(const Context& other);

	Context(Context&& other) noexcept;

	Context& operator = (const Context& other);

	Context& operator = (Context&& other) noexcept;

	void add_element(const std::string& key, const Context& context);

	void add_element(const std::string& key, Context&& context);

	void add_element(const Context& context);

	void add_element(Context&& context);

	bool is_valid() const;

	bool is_int() const;

	bool is_double() const;

	bool is_bool() const;

	bool is_string() const;

	bool is_array() const;

	bool is_container() const;

	void set_scalar(int value);
	
	void set_scalar(double value);
	
	void set_scalar(bool value);
	
	void set_scalar(const std::string& value);

	void set_scalar(std::string&& value);

	int get_int() const;

	double get_double() const;

	bool get_bool() const;

	const std::string& get_string() const;

	const std::vector<Context>& get_array() const;

	const std::unordered_map<std::string, Context>& get_container() const;

	const Context& get_element(const std::string& key);
	
	std::string get_str(int depth = 1) const;

	explicit operator bool () const;

	~Context() = default;
};
