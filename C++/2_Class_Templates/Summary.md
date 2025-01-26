* A class template is a class that is implemented with one or more type parameters left open.
* To use a class template, you pass the open types as template arguments. The class template is then instantiated (and compiled) for these types.
* For class templates only those member functions that are called are instantiated.
* You can specialise class templates for certain types.
* You can partially specialise class templates for certain types.
* Since C++17, class template arguments can automatically be deduced from constructors.
* You can define aggregate class templates.
* Call parameters of a template type decay if declared to be called by value.
* Templates can only be declared and defined in global/namespace scope or inside class declarations.