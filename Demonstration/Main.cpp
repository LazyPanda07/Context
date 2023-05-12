#include <iostream>

#include "Context.h"

using namespace std;

int main(int argc, char** argv)
{
	Context ctx1_a;
	Context ctx1_b;
	Context ctx1_c;

	string ctx_str = "hello";
	int ctx_int = 8;
	double ctx_double = 131.31;

	ctx1_a.set_scalar(ctx_str);
	ctx1_b.set_scalar(ctx_int);
	ctx1_c.set_scalar(ctx_double);

	cout << ctx1_a.get_str() << endl << endl;

	Context ctx2;

	ctx2.add_element("name", Context("str"));
	ctx2.add_element("age", Context(18));
	ctx2.add_element("is_student", Context(true));
	ctx2.add_element("grades", Context({ 85, 90, 95 }));

	cout << ctx2.get_element("name").get_string() << endl << endl;

	Context ctx3;

	ctx3.add_element(Context(1));
	ctx3.add_element(Context(2));
	ctx3.add_element(Context(3));

	for (const auto& i : ctx3.get_array())
	{
		cout << i.get_int() << endl;
	}

	cout << endl;

	Context ctx;
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

	ctx.add_element("objects", array);

	cout << ctx["objects"][0]["bbox"]["x"].get_str() << endl;

#if NDEBUG
	char c = getchar();
#endif

	return 0;
}
