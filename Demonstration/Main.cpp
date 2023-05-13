#include <iostream>
#include <format>

#include "Context.h"

using namespace std;

void preview_set_scalars();

void preview_container();

void preview_array();

void preview_nesting();

void preview_iterators();

int main(int argc, char** argv)
{
	preview_set_scalars();

	preview_container();

	preview_array();

	preview_nesting();

	preview_iterators();

#if NDEBUG
	char c = getchar();
#endif

	return 0;
}

void preview_set_scalars()
{
	cout << "Scalars" << endl;

	Context first;
	Context second;
	Context third;
	string string_value = "hello";
	int int_value = 8;
	double double_value = 131.31;

	first.set_scalar(string_value);

	second.set_scalar(int_value);

	third.set_scalar(double_value);

	cout << format("{} {} {}", first.get_string(), second.get_int(), third.get_double()) << endl << endl;
}

void preview_container()
{
	cout << "Container" << endl;

	Context context;

	context.add_element("name", Context("str"));
	context.add_element("age", Context(18));
	context.add_element("is_student", Context(true));
	context.add_element("grades", Context({ 85, 90, 95 }));

	cout << context.get_element("name").get_string() << endl;

	cout << context.get_str() << endl << endl;
}

void preview_array()
{
	cout << "Array" << endl;

	Context context;

	context.add_element(Context(1));
	context.add_element(Context(2));
	context.add_element(Context(3));

	for (const auto& i : context.get_array())
	{
		cout << i.get_int() << endl;
	}

	cout << endl;
}

void preview_nesting()
{
	cout << "Nesting" << endl;

	Context context;
	Context array;

	array.add_element
	(
		Context().add_element
		(
			"bbox",
			Context().add_element
			(
				"x",
				Context(5.5)
			)
		)
	);

	context.add_element("objects", array);

	cout << context["objects"][0]["bbox"]["x"].get_str() << endl << endl;
}

void preview_iterators()
{
	cout << "Iterators" << endl;

	Context array({ 1, 2, 3, 4, 5, 6, 7 });
	Context container =
	{
		{ "first", Context(1) },
		{ "second", Context({ 1, 2, 3, 4 }) },
		{
			"third",
			Context
			{
				{ "first", Context("first") },
				{ "second", Context({ 2.3, 4.4, 5.5 }) },
				{ "third", Context(true) }
			}
		}
	};

	cout << "Array: ";

	for (const Context& value : array)
	{
		cout << value.get_int() << ' ';
	}

	cout << endl;

	cout << format("find: {}", array.find(Context(6))->get_int()) << endl;

	cout << "Remove 2 from array: ";

	array.remove(array.find(Context(2)));

	for (const Context& value : array)
	{
		cout << value.get_int() << ' ';
	}

	cout << endl;

	cout << "Container: " << endl;

	cout << container << endl;
}
