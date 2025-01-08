* Function templates define a family of functions for different template arguments
* When you pass arguments to function parameters depending on template parameters, function templates deduce the template parameters to be instantiated for the corresponding parameter types
    * You can explicitly qualify the leading template parameters
    * You can define default arguments for template parameters. These may refer to previous template parameters and be followed by parameters not having default arguments
* You can overload function templates
    * When overloading function templates with other function templates, you should ensure that only one of them matches for any call
    * When you overload function templates, limit your changes to specifying template parameters explicitly
    * Ensure the compiler sees all overloaded versions of function templates before you call them