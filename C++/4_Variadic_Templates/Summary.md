* By using parameter packs, templates can be defined for an arbitrary number of template parameters of arbitrary type.
* To process the parameters, you need recursion and/or a matching nonvariadic function.
* Operator sizeof... yields the number of arguments provided for a parameter pack.
* A typical application of variadic templates is forwarding an arbitrary number of arguments of arbitrary type.
By using fold expressions, you can apply operators to all arguments of a parameter pack.