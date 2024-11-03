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

## Testing

Binary for testing is created together, with the project execution file.

```bash
    ./unit_tests
```


## Structure