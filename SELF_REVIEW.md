# Self-review

## Global improvements 

### LL to LR parser (_architecture_)

In general, LR parsers are more powerful. Usually, automatic parsers use the LALR subset to parse expressions, which does not require a pre-processing of the language. Also, even though asymptotically they are equal, the LR parser, in general, works faster. On such a small project with very easy grammar, it is not the issue, but theoretically, it is better to use bottom-up parsers rather than top-down ones.

### Evaluation method (_architecture_)

Usually, the evaluation of the AST is executed with the help of the visit function, which is overridden to accept each type of node. Use of the visitor pattern, allow us to not modify the AST node classes and separate logic. Also, it is easier to handle different node types with double dispatch. I choose a form of a methods for it simplicity. In the end, it generates the same result, as a visitor pattern. And on a small project, difference is such small, that it is not too much important, which type of evaluation to choose.

[example1](https://github.com/Emaisty/JSON_parser/blob/51d5e3c1747f0bef18a474690ce40d3708af40ee/JSON_parser/Expr/AST_Expr.h#L39);
[example2](https://github.com/Emaisty/JSON_parser/blob/51d5e3c1747f0bef18a474690ce40d3708af40ee/JSON_parser/Expr/AST_Expr.h#L53);
[example3](https://github.com/Emaisty/JSON_parser/blob/51d5e3c1747f0bef18a474690ce40d3708af40ee/JSON_parser/Expr/AST_Expr.h#L67);
[example4](https://github.com/Emaisty/JSON_parser/blob/51d5e3c1747f0bef18a474690ce40d3708af40ee/JSON_parser/Expr/AST_Expr.h#L82).


### Rewrite dynamic_cast to an inner type system (_architecture & performance_)

Dynamic cast is quite an expensive operation. It is used to verify the type of the cell to which the variable points. A much easier way to fix it can be the introduction of the inner type system. We will create a unique pointer for each type and store them in a context. Each time when we need to verify the type, we call the method on a pointer, which is overloaded for each JSON element and returns its type (different JSON elements with the same type will return the same pointer). In that case, our comparison will consist of a call and comparison of addresses, which is lighter operation, than cast.

[example1](https://github.com/Emaisty/JSON_parser/blob/51d5e3c1747f0bef18a474690ce40d3708af40ee/JSON_parser/Expr/AST_Expr.cpp#L15);
[example2](https://github.com/Emaisty/JSON_parser/blob/51d5e3c1747f0bef18a474690ce40d3708af40ee/JSON_parser/Expr/AST_Expr.cpp#L63);
[example3](https://github.com/Emaisty/JSON_parser/blob/51d5e3c1747f0bef18a474690ce40d3708af40ee/JSON_parser/Expr/AST_Expr.cpp#L106);
[example4](https://github.com/Emaisty/JSON_parser/blob/51d5e3c1747f0bef18a474690ce40d3708af40ee/JSON_parser/JSON/JSON.h#L191).


### Return new created objects in evaluation, rather than pointer(_Not sure if this is improvement. Debatable_)

It is possible in the evaluation method to change the returning value from a pointer to an object. This will solve the issue with the math operations when the object which we want to return does not exist in an original JSON and we need to [store it somewhere](https://github.com/Emaisty/JSON_parser/blob/51d5e3c1747f0bef18a474690ce40d3708af40ee/JSON_parser/JSON/JSON.h#L124)(sum of two integers, just integer in expression, etc.). Such approach will allow us to remove creating of a new object for a new integer ([example1](https://github.com/Emaisty/JSON_parser/blob/51d5e3c1747f0bef18a474690ce40d3708af40ee/JSON_parser/Expr/AST_Expr.cpp#L28);[example2](https://github.com/Emaisty/JSON_parser/blob/51d5e3c1747f0bef18a474690ce40d3708af40ee/JSON_parser/Expr/AST_Expr.cpp#L111)). However, we would still create such objects and propagate them up. It is true, that they would be propagated only for one node up and then is going to be deleted since it is not going to be needed anywhere else. However, in that case, we would create a copy of already existing maps and vectors also, which will take a lot of time. In my opinion, such a drawback is significant and it is better to sacrifice the total memory, rather than such a drop in speed.  

## Minor ones

### Paralleling (_performance_)

It is possible to run functions to [parse a JSON file](https://github.com/Emaisty/JSON_parser/blob/51d5e3c1747f0bef18a474690ce40d3708af40ee/JSON_parser/JSON/parser.cpp#L3) and [parser for an expression](https://github.com/Emaisty/JSON_parser/blob/51d5e3c1747f0bef18a474690ce40d3708af40ee/JSON_parser/Expr/AST_Expr.cpp#L132) simultaneously. They are not dependent on each other and it would be very easy to implement, such as running these 2 functions with pthread library. It would only require a barrier before the evaluation function, which will wait for parsers to finish their work.

