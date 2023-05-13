#include "Context.h"

#include <format>

using namespace std;

using ContextIterator = Context::ContextIterator;
using ConstContextIterator = Context::ConstContextIterator;

template<typename T>
bool Context::recursive_remove(const T& remove_value, vector<Context>& data)
{
	for (T& value : data)
	{
		if (value.remove(remove_value, true))
		{
			return true;
		}
	}

	return false;
}

template<typename T>
bool Context::recursive_remove(const T& remove_value, unordered_map<string, Context>& data)
{
	for (auto& [key, value] : data)
	{
		if (value.remove(remove_value, true))
		{
			return true;
		}
	}

	return false;
}

vector<Context>& Context::get_array()
{
	return get<vector<Context>>(data);
}

unordered_map<string, Context>& Context::get_container()
{
	return get<unordered_map<string, Context>>(data);
}

ContextIterator::ContextIterator(const iterator_type& iterator) :
	BaseContextIterator(iterator)
{

}

Context& ContextIterator::operator*() noexcept
{
	switch (type)
	{
	case Context::ContextIterator::array_type:
		return *get<iterator_type_enum::array_type>(current_iterator);

	case Context::ContextIterator::container_type:
		return (*get<iterator_type_enum::container_type>(current_iterator)).second;
	}
}

Context* ContextIterator::operator->() noexcept
{
	switch (type)
	{
	case Context::ContextIterator::array_type:
		return get<iterator_type_enum::array_type>(current_iterator).operator->();

	case Context::ContextIterator::container_type:
		return &(*get<iterator_type_enum::container_type>(current_iterator)).second;
	}
}

ConstContextIterator::ConstContextIterator(const iterator_type& iterator) :
	BaseContextIterator(iterator)
{

}

const Context& ConstContextIterator::operator*() noexcept
{
	switch (type)
	{
	case Context::ConstContextIterator::array_type:
		return *get<iterator_type_enum::array_type>(current_iterator);

	case Context::ConstContextIterator::container_type:
		return (*get<iterator_type_enum::container_type>(current_iterator)).second;
	}
}

const Context* ConstContextIterator::operator->() noexcept
{
	switch (type)
	{
	case Context::ConstContextIterator::array_type:
		return get<iterator_type_enum::array_type>(current_iterator).operator->();

	case Context::ConstContextIterator::container_type:
		return &(*get<iterator_type_enum::container_type>(current_iterator)).second;
	}
}

Context::InvalidContextIterator::InvalidContextIterator() :
	runtime_error("Can't create iterator for this context")
{

}

Context::Context() :
	spaces_per_depth(4)
{

}

Context::Context(int value) :
	Context()
{
	this->set_scalar(value);
}

Context::Context(double value) :
	Context()
{
	this->set_scalar(value);
}

Context::Context(bool value) :
	Context()
{
	this->set_scalar(value);
}

Context::Context(const string& value) :
	Context()
{
	this->set_scalar(value);
}

Context::Context(string&& value) :
	Context()
{
	this->set_scalar(move(value));
}

Context::Context(const char* value) :
	Context(string(value))
{

}

Context::Context(initializer_list<double> values) :
	Context()
{
	for (double value : values)
	{
		this->add_element(Context(value));
	}
}

Context::Context(initializer_list<int> values) :
	Context()
{
	for (int value : values)
	{
		this->add_element(Context(value));
	}
}

Context::Context(const Context& other)
{
	(*this) = other;
}

Context::Context(Context&& other) noexcept
{
	(*this) = move(other);
}

Context& Context::operator=(const Context& other)
{
	data = other.data;

	spaces_per_depth = other.spaces_per_depth;

	return *this;
}

Context& Context::operator=(Context&& other) noexcept
{
	data = move(other.data);

	spaces_per_depth = other.spaces_per_depth;

	return *this;
}

Context& Context::add_element(const string& key, const Context& context)
{
	if (data.index() != type_enum::container_type)
	{
		data = unordered_map<string, Context>();
	}

	get<unordered_map<string, Context>>(data)[key] = context;

	return *this;
}

Context& Context::add_element(const string& key, Context&& context)
{
	if (data.index() != type_enum::container_type)
	{
		data = unordered_map<string, Context>();
	}

	get<unordered_map<string, Context>>(data)[key] = move(context);

	return *this;
}

Context& Context::add_element(const Context& context)
{
	if (data.index() != type_enum::array_type)
	{
		data = vector<Context>();
	}

	get<vector<Context>>(data).push_back(context);

	return *this;
}

Context& Context::add_element(Context&& context)
{
	if (data.index() != type_enum::array_type)
	{
		data = vector<Context>();
	}

	get<vector<Context>>(data).push_back(move(context));

	return *this;
}

bool Context::is_valid() const noexcept
{
	return data.index();
}

bool Context::is_int() const
{
	return data.index() == type_enum::int_type;
}

bool Context::is_double() const
{
	return data.index() == type_enum::double_type;
}

bool Context::is_bool() const
{
	return data.index() == type_enum::bool_type;
}

bool Context::is_string() const
{
	return data.index() == type_enum::string_type;
}

bool Context::is_array() const
{
	return data.index() == type_enum::array_type;
}

bool Context::is_container() const
{
	return data.index() == type_enum::container_type;
}

bool Context::insert(size_t index, const Context& context)
{
	if (!this->is_array())
	{
		return false;
	}

	vector<Context>& array = this->get_array();

	if (array.size() <= index)
	{
		return false;
	}

	array.insert(array.begin() + index, context);

	return true;
}

bool Context::insert(size_t index, Context&& context)
{
	if (!this->is_array())
	{
		return false;
	}

	vector<Context>& array = this->get_array();

	if (array.size() <= index)
	{
		return false;
	}

	array.insert(array.begin() + index, move(context));

	return true;
}

bool Context::pop_back()
{
	if (!this->is_array())
	{
		return false;
	}

	vector<Context>& array = this->get_array();

	if (array.size())
	{
		array.pop_back();

		return true;
	}

	return false;
}

bool Context::remove(const Context& context, bool recursive)
{
	if (this->is_array())
	{
		if (erase(this->get_array(), context))
		{
			return true;
		}

		return recursive && Context::recursive_remove(context, this->get_array());
	}
	else if (this->is_container())
	{
		unordered_map<string, Context>& container = this->get_container();

		for (auto it = container.begin(); it != container.end(); ++it)
		{
			if (it->second == context)
			{
				container.erase(it);

				return true;
			}
		}

		return recursive && Context::recursive_remove(context, container);
	}

	return false;
}

bool Context::remove(size_t index)
{
	if (!this->is_array())
	{
		return false;
	}

	vector<Context>& array = this->get_array();

	if (array.size() <= index)
	{
		return false;
	}

	array.erase(array.begin() + index);

	return true;
}

bool Context::remove(const string& key, bool recursive)
{
	if (!this->is_container())
	{
		return false;
	}

	unordered_map<string, Context>& container = this->get_container();

	if (container.erase(key))
	{
		return true;
	}

	return recursive && Context::recursive_remove(key, container);
}

ContextIterator Context::remove(const ContextIterator& it)
{
	switch (it.type)
	{
	case ContextIterator::iterator_type_enum::array_type:
		return ContextIterator(this->get_array().erase(get<ContextIterator::iterator_array_type>(it.current_iterator)));

	case ContextIterator::iterator_type_enum::container_type:
		return ContextIterator(this->get_container().erase(get<ContextIterator::iterator_container_type>(it.current_iterator)));
	}
}

ConstContextIterator Context::remove(const ConstContextIterator& it)
{
	switch (it.type)
	{
	case ConstContextIterator::iterator_type_enum::array_type:
		return ConstContextIterator(this->get_array().erase(get<ConstContextIterator::iterator_array_type>(it.current_iterator)));

	case ConstContextIterator::iterator_type_enum::container_type:
		return ConstContextIterator(this->get_container().erase(get<ConstContextIterator::iterator_container_type>(it.current_iterator)));
	}
}

ContextIterator Context::find(const string& key)
{
	if (!this->is_container())
	{
		throw InvalidContextIterator();
	}

	unordered_map<string, Context>& container = this->get_container();

	if (auto it = container.find(key); it != container.end())
	{
		return ContextIterator(it);
	}

	return ContextIterator(container.end());
}

ContextIterator Context::find(const Context& context)
{
	if (this->is_array())
	{
		vector<Context>& array = this->get_array();

		for (size_t i = 0; i < array.size(); i++)
		{
			if (array[i] == context)
			{
				return ContextIterator(array.begin() + i);
			}
		}

		return ContextIterator(array.end());
	}
	else if (this->is_container())
	{
		unordered_map<string, Context>& container = this->get_container();

		for (const auto& [key, value] : container)
		{
			if (value == context)
			{
				return ContextIterator(container.find(key));
			}
		}

		return ContextIterator(container.end());
	}

	throw InvalidContextIterator();
}

ConstContextIterator Context::find(const string& key) const
{
	if (!this->is_container())
	{
		throw InvalidContextIterator();
	}

	const unordered_map<string, Context>& container = this->get_container();

	if (auto it = container.find(key); it != container.end())
	{
		return ConstContextIterator(it);
	}

	return ConstContextIterator(container.end());
}

ConstContextIterator Context::find(const Context& context) const
{
	if (this->is_array())
	{
		const vector<Context>& array = this->get_array();

		for (size_t i = 0; i < array.size(); i++)
		{
			if (array[i] == context)
			{
				return ConstContextIterator(array.begin() + i);
			}
		}

		return ConstContextIterator(array.end());
	}
	else if (this->is_container())
	{
		const unordered_map<string, Context>& container = this->get_container();

		for (const auto& [key, value] : container)
		{
			if (value == context)
			{
				return ConstContextIterator(container.find(key));
			}
		}

		return ConstContextIterator(container.end());
	}

	throw InvalidContextIterator();
}

ConstContextIterator Context::begin() const
{
	if (this->is_array())
	{
		return ConstContextIterator(this->get_array().cbegin());
	}
	else if (this->is_container())
	{
		return ConstContextIterator(this->get_container().cbegin());
	}

	throw InvalidContextIterator();
}

ContextIterator Context::begin()
{
	if (this->is_array())
	{
		return ContextIterator(this->get_array().begin());
	}
	else if (this->is_container())
	{
		return ContextIterator(this->get_container().begin());
	}

	throw InvalidContextIterator();
}

ConstContextIterator Context::end() const
{
	if (this->is_array())
	{
		return ConstContextIterator(this->get_array().cend());
	}
	else if (this->is_container())
	{
		return ConstContextIterator(this->get_container().cend());
	}

	throw InvalidContextIterator();
}

ContextIterator Context::end()
{
	if (this->is_array())
	{
		return ContextIterator(this->get_array().end());
	}
	else if (this->is_container())
	{
		return ContextIterator(this->get_container().end());
	}

	throw InvalidContextIterator();
}

void Context::set_scalar(int value)
{
	data = value;
}

void Context::set_scalar(double value)
{
	data = value;
}

void Context::set_scalar(bool value)
{
	data = value;
}

void Context::set_scalar(const string& value)
{
	data = value;
}

void Context::set_scalar(string&& value)
{
	data = move(value);
}

int Context::get_int() const
{
	return get<int>(data);
}

double Context::get_double() const
{
	return get<double>(data);
}

bool Context::get_bool() const
{
	return get<bool>(data);
}

const string& Context::get_string() const
{
	return get<string>(data);
}

const vector<Context>& Context::get_array() const
{
	return get<vector<Context>>(data);
}

const unordered_map<string, Context>& Context::get_container() const
{
	return get<unordered_map<string, Context>>(data);
}

const Context& Context::get_element(const string& key)
{
	return get<unordered_map<string, Context>>(data).at(key);
}

string Context::get_str(int depth) const
{
	string result;

	switch (data.index())
	{
	case Context::int_type:
		result += to_string(this->get_int());

		break;
	case Context::double_type:
		result += to_string(this->get_double());

		break;
	case Context::bool_type:
		result += this->get_bool() ? "true" : "false";

		break;

	case Context::string_type:
		result += this->get_string();

		break;

	case Context::array_type:
		result += format("[{}]", [this]() -> string
			{
				string tem;
				vector<Context> contexts = this->get_array();

				for (size_t i = 0; i < contexts.size(); i++)
				{
					if (i + 1 == contexts.size())
					{
						tem += contexts[i].get_str();
					}
					else
					{
						tem += format("{}, ", contexts[i].get_str());
					}
				}

				return tem;
			}());

		break;

	case Context::container_type:
		result += [this, depth]() -> string
		{
			string offset(depth * spaces_per_depth, ' ');
			string_view previousDepthOffset(offset.data(), (depth - 1) * spaces_per_depth);
			string tem = "{\n";

			for (const auto& [key, value] : this->get_container())
			{
				tem += format("{}{}: {},\n", offset, key, value.get_str(depth + 1));
			}

			tem.pop_back(); // \n
			tem.pop_back(); // ,

			tem.append("\n").append(previousDepthOffset).append("}");

			return tem;
		}();

		break;
	}

	return result;
}

Context& Context::operator[](size_t index)
{
	return get<vector<Context>>(data).at(index);
}

Context& Context::operator[](const string& key)
{
	return get<unordered_map<string, Context>>(data).at(key);
}

bool Context::operator==(const Context& other) const noexcept
{
	return this->is_valid() && other.is_valid() && data == other.data;
}

Context::operator bool() const noexcept
{
	return this->is_valid();
}
