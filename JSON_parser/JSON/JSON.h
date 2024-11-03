#ifndef JSON_PARSER_JSON_H
#define JSON_PARSER_JSON_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>

namespace JSON {

    struct Context;

    class JsonElement {
    public:
        JsonElement() = default;

        virtual ~JsonElement() = default;

        virtual void print(std::ostream &) const = 0;

    protected:

    };

    class IntElem : public JsonElement {
    public:
        IntElem() = default;

        IntElem(long long new_value) : value(new_value) {}

        long long getValue() const { return value; }

        void print(std::ostream &) const override;

    private:
        long long value = 0;

    };

    class StringElem : public JsonElement {
    public:
        StringElem() = default;

        StringElem(std::string &&new_name) : name(std::move(new_name)) {}

        void print(std::ostream &) const override;

    private:
        std::string name;

    };

    class FuncElem : public JsonElement {
    public:
        FuncElem() = default;

        FuncElem(std::string &&new_name,
                 std::function<std::unique_ptr<JsonElement> *(std::vector<std::unique_ptr<JsonElement> *>,
                                                              Context &)> &&fun)
                : name_of_func(std::move(new_name)), fBody(std::move(fun)) {}

        std::function<std::unique_ptr<JsonElement> *(std::vector<std::unique_ptr<JsonElement> *>, Context &)> *
        getBody();

        void print(std::ostream &) const override;

    private:
        std::string name_of_func;

        std::function<std::unique_ptr<JsonElement> *(std::vector<std::unique_ptr<JsonElement> *>, Context &)> fBody;

    };

    class MapElem : public JsonElement {
    public:
        MapElem() = default;

        void addNewEntry(const std::string &, std::unique_ptr<JsonElement> &&);

        std::unique_ptr<JsonElement> *getItem(const std::string &);

        long long countElements() const;

        const std::map<std::string, std::unique_ptr<JsonElement>> *getObject() const;

        void print(std::ostream &) const override;

    private:
        std::map<std::string, std::unique_ptr<JsonElement>> json_map;

    };

    class ArrayElem : public JsonElement {
    public:
        ArrayElem() = default;

        void addNewEntry(std::unique_ptr<JsonElement> &&);

        std::unique_ptr<JsonElement> *getElement(long long);

        long long countElements() const;

        const std::vector<std::unique_ptr<JsonElement>> *getArray() const;

        void print(std::ostream &) const override;

    private:
        std::vector<std::unique_ptr<JsonElement>> json_vector;

    };

    struct Context {
        std::unique_ptr<JsonElement> *top_element;

        bool look_for_func = false;

        std::list<std::unique_ptr<JSON::JsonElement>> numberElements;

        std::map<std::string, std::unique_ptr<JSON::JsonElement>> functions;

        Context();

        std::unique_ptr<JSON::JsonElement> *addNewNumberElem(std::unique_ptr<JSON::JsonElement> &&);

        std::list<std::pair<std::string, std::function<std::unique_ptr<JSON::JsonElement> *(
                std::vector<std::unique_ptr<JSON::JsonElement> *>, Context &)>>> init_funcs = {
                {"size", [](std::vector<std::unique_ptr<JSON::JsonElement> *> vec,
                            Context &ctx) {
                    if (vec.size() == 0)
                        throw std::invalid_argument("ERROR. Function need some arguments.");
                    if (vec.size() > 1)
                        throw std::invalid_argument("ERROR. Function size can take only 1 argument.");
                    auto item = vec[0];
                    if (!dynamic_cast<JSON::MapElem *>(item->get()) && !dynamic_cast<JSON::ArrayElem *>(item->get()))
                        throw std::invalid_argument(
                                "ERROR. Function size can take as an argument only object or array.");
                    if (dynamic_cast<JSON::MapElem *>(item->get()))
                        return ctx.addNewNumberElem(std::make_unique<JSON::IntElem>(
                                dynamic_cast<JSON::MapElem *>(item->get())->countElements()));
                    return ctx.addNewNumberElem(std::make_unique<JSON::IntElem>(
                            dynamic_cast<JSON::ArrayElem *>(item->get())->countElements()));
                }},
                {"max",  [](std::vector<std::unique_ptr<JSON::JsonElement> *> vec,
                            Context &ctx) {
                    if (vec.size() == 0)
                        throw std::invalid_argument("ERROR. Function need some arguments.");
                    // Must be an array of integers
                    if (vec.size() == 1) {
                        if (!dynamic_cast<JSON::ArrayElem *>(vec[0]->get()))
                            throw std::invalid_argument("ERROR. Max can take only an array as a single argument.");
                        auto array = dynamic_cast<JSON::ArrayElem *>(vec[0]->get())->getArray();
                        long long m_value = LLONG_MIN;
                        for (auto &i: *array) {
                            if (!dynamic_cast<const JSON::IntElem *>(i.get()))
                                throw std::invalid_argument(
                                        "ERROR. In max function, array must contain only integers.");
                            m_value = std::max(m_value, dynamic_cast<const JSON::IntElem *>(i.get())->getValue());
                        }
                        return ctx.addNewNumberElem(std::make_unique<JSON::IntElem>(m_value));
                    }
                    // All arguments must be integers
                    long long m_value = LLONG_MIN;
                    for (auto &i: vec) {
                        if (!dynamic_cast<const JSON::IntElem *>(i->get()))
                            throw std::invalid_argument(
                                    "ERROR. In max function, array must contain only integers.");
                        m_value = std::max(m_value, dynamic_cast<const JSON::IntElem *>(i->get())->getValue());
                    }
                    return ctx.addNewNumberElem(std::make_unique<JSON::IntElem>(m_value));
                }},
                {"min",  [](std::vector<std::unique_ptr<JSON::JsonElement> *> vec,
                            Context &ctx) {
                    if (vec.size() == 0)
                        throw std::invalid_argument("ERROR. Function need some arguments.");
                    // Must be an array of integers
                    if (vec.size() == 1) {
                        if (!dynamic_cast<JSON::ArrayElem *>(vec[0]->get()))
                            throw std::invalid_argument("ERROR. Max can take only an array as a single argument.");
                        auto array = dynamic_cast<JSON::ArrayElem *>(vec[0]->get())->getArray();
                        long long m_value = LLONG_MAX;
                        for (auto &i: *array) {
                            if (!dynamic_cast<const JSON::IntElem *>(i.get()))
                                throw std::invalid_argument(
                                        "ERROR. In max function, array must contain only integers.");
                            m_value = std::min(m_value, dynamic_cast<const JSON::IntElem *>(i.get())->getValue());
                        }
                        return ctx.addNewNumberElem(std::make_unique<JSON::IntElem>(m_value));
                    }
                    // All arguments must be integers
                    long long m_value = LLONG_MAX;
                    for (auto &i: vec) {
                        if (!dynamic_cast<const JSON::IntElem *>(i->get()))
                            throw std::invalid_argument(
                                    "ERROR. In max function, array must contain only integers.");
                        m_value = std::min(m_value, dynamic_cast<const JSON::IntElem *>(i->get())->getValue());
                    }
                    return ctx.addNewNumberElem(std::make_unique<JSON::IntElem>(m_value));
                }}
        };
    };

}

#endif //JSON_PARSER_JSON_H
