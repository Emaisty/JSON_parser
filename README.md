# JSON Evaluator

Terminal-based application to parse a JSON file and evaluate
expressions over a JSON data structure. Supports binary operations,
functions, and operations over JSON.

![demo mode](./data/Usage.gif)

## How to use

Application, in the form of an executable binary file, accepts 2 two
arguments: path to the JSON file, with a JSON structure inside of
it and a string -- expression over JSON.

```bash
./j_eval data/test1.json "a"
>  [1,2,3]
./j_eval data/test1.json "b.d[0].y"
> {"a" : "b"}
./j_eval data/test2.json "(a + b - c) / (d[0] + d[1])"
> 1
./j_eval data/test3.json "[0]"
> 2
./j_eval data/test4.json "a.a[max(0,2,0)] + b[min(c)].a"
> 10
```

where:

```
test1.json << "{"a" : [1,2,3], "b" : {"d" : [{"y" : {"a" : "b"}}, 2]} }"
test2.json << "{"a" : 5, "b" : 10, "c" : 5, "d" : [6,4]}"
test3.json << "[2,3,4]"
test4.json << "{"a" : {"a" : [0,1,5]}, "b" : [0,{"a" : 5},10], "c" : [2,1,10]}"
```

### Compiling

```bash
cd JSON_parser
mdkir build
cd build
cmake ..
make
```

### Testing

Binary for testing is created together, with the project execution file.

```bash
./unit_tests
```


## Structure

JSON parser is divided into 3 main parts: Lexer, JSON and Expression parser/evaluator.

#### Lexer

The basic unit used in both JSON and Expression parsers. It reads characters from 
the file/stream and forms tokens from it.

#### JSON

Using the basic JSON grammar, it parses tokens and trying to form a JSON structure. 
Json consist of 4 elements:

- **Integer**: represents basic number types. Stores a number.
- **String**: represents an array of characters.
- **Map**: structure, consists of elements of type: *string -> JsonElement*
- **Array**: structure, linearly stores *JsonElements* and which can be accessed by an address of the cell.

#### Expression

AST of expression consist of several nodes:

- **Integer**: represents a number in the expression.
- **Variable**: if the top element of JSON is a map, access to the top-level items is taken
  through the simple use of the item's name, without specifying an object.
- **Function Call**: in a form of *name(args...)*.
- **MemberCall**: a.b -- access of the item from an object.
- **ElementCall**: a[b] -- access of the element from an array.


## Extra features

**FunctionCall** class created in the most extendable way. To add a new function, it is enough
to add a name of it and a lambda function of its body to the init list in the JSON.h file inside of context.