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

Context::Context(initializer_list<double> values) :
	Context()
{
	for (double value : values)
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
	switch (other.data.index())
	{
	case Context::int_type:
		data = other.get_int();

		break;

	case Context::double_type:
		data = other.get_double();

		break;

	case Context::bool_type:
		data = other.get_bool();

		break;

	case Context::string_type:
		data = other.get_string();

		break;

	case Context::array_type:
		break;
	case Context::container_type:
		break;
	}

	spaces_per_depth = other.spaces_per_depth;

	return *this;
}

Context& Context::operator = (Context&& other) noexcept
{
	data = move(other.data);

	spaces_per_depth = other.spaces_per_depth;

	return *this;
}

void Context::add_element(const string& key, const Context& context)
{
	if (data.index() != type_enum::container_type)
	{
		data = unordered_map<string, unique_ptr<Context>>();
	}

	get<unordered_map<string, unique_ptr<Context>>>(data)[key] = make_unique<Context>(context);
}

void Context::add_element(const string& key, Context&& context)
{
	if (data.index() != type_enum::container_type)
	{
		data = unordered_map<string, unique_ptr<Context>>();
	}

	get<unordered_map<string, unique_ptr<Context>>>(data)[key] = make_unique<Context>(move(context));
}

void Context::add_element(const Context& context)
{
	if (data.index() != type_enum::array_type)
	{
		data = vector<unique_ptr<Context>>();
	}

	get<vector<unique_ptr<Context>>>(data).push_back(make_unique<Context>(context));
}

void Context::add_element(Context&& context)
{
	if (data.index() != type_enum::array_type)
	{
		data = vector<unique_ptr<Context>>();
	}

	get<vector<unique_ptr<Context>>>(data).push_back(make_unique<Context>(move(context)));
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

vector<Context> Context::get_array() const
{
	vector<Context> contexts;

	for (const auto& value : get<vector<unique_ptr<Context>>>(data))
	{
		contexts.push_back(*value);
	}

	return contexts;
}

const unordered_map<string, unique_ptr<Context>>& Context::get_container() const
{
	return get<unordered_map<string, unique_ptr<Context>>>(data);
}

const Context& Context::get_element(const string& key)
{
	return *get<unordered_map<string, unique_ptr<Context>>>(data)[key];
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
				tem += format("{}{}: {},\n", offset, key, value->get_str(depth + 1));
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
