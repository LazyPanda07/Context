#include "Context.h"

#include <format>

using namespace std;

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

Context& Context::operator = (const Context& other)
{
	data = other.data;

	spaces_per_depth = other.spaces_per_depth;

	return *this;
}

Context& Context::operator = (Context&& other) noexcept
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

bool Context::is_valid() const
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

bool Context::remove(const Context& context)
{
	if (this->is_array())
	{
		return erase(this->get_array(), context);
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

	if (recursive)
	{
		for (auto& [key, value] : container)
		{
			if (value.remove(key, true))
			{
				return true;
			}
		}
	}

	return false;
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

vector<Context>& Context::get_array()
{
	return get<vector<Context>>(data);
}

const vector<Context>& Context::get_array() const
{
	return get<vector<Context>>(data);
}

unordered_map<string, Context>& Context::get_container()
{
	return get<unordered_map<string, Context>>(data);
}

const unordered_map<string, Context>& Context::get_container() const
{
	return get<unordered_map<string, Context>>(data);
}

const Context& Context::get_element(const string& key)
{
	return get<unordered_map<string, Context>>(data)[key];
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

Context& Context::operator [] (size_t index)
{
	return get<vector<Context>>(data).at(index);
}

Context& Context::operator [] (const string& key)
{
	return get<unordered_map<string, Context>>(data).at(key);
}

bool Context::operator == (const Context& other) const noexcept
{
	return this->is_valid() && other.is_valid() && data == other.data;
}

Context::operator bool() const
{
	return this->is_valid();
}
