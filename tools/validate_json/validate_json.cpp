/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <fstream>
#include <iostream>
#include "argh.h"


#include "json.hpp"
using json = nlohmann::json;

#include <valijson/adapters/nlohmann_json_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

using valijson::Schema;
using valijson::SchemaParser;
using valijson::Validator;
using valijson::adapters::NlohmannJsonAdapter;

int main(int, char** argv)
{
    argh::parser cmdl(argv);
    if (!cmdl(2))
    {
        std::cerr <<"[Error] Program requires 2 arguments: json path and validator path\n";
        return EXIT_FAILURE;
    }
    std::string jsonPath;
    cmdl(1) >> jsonPath;

    std::string validatorPath;
    cmdl(2) >> validatorPath;

    std::ifstream jsonFile(jsonPath);
    if(!jsonFile.is_open())
    {
        std::cerr <<"[Error] Could not open file: "<<jsonPath<<'\n';
        return EXIT_FAILURE;
    }
    json jsonContent;
    try
    {
        jsonFile >> jsonContent;
    }
    catch (json::parse_error& error)
    {
        std::cerr <<"[Error] Could not parse json file correctly\nLog: "<<error.what();
        return EXIT_FAILURE;
    }

    std::ifstream validatorFile(validatorPath);
    if(!validatorFile.is_open())
    {
        std::cerr <<"[Error] Could not open file: "<<validatorPath<<'\n';
        return EXIT_FAILURE;
    }
    json validatorContent;
    try
    {
        validatorFile >> validatorContent;
    }
    catch (json::parse_error& error)
    {
        std::cerr <<"[Error] Could not parse validator file correctly\nLog: "<<error.what();
        return EXIT_FAILURE;
    }

    // Parse JSON schema content using valijson
    Schema mySchema;
    SchemaParser parser;
    NlohmannJsonAdapter mySchemaAdapter(validatorContent);
    parser.populateSchema(mySchemaAdapter, mySchema);

    Validator validator;
    NlohmannJsonAdapter myTargetAdapter(jsonContent);
    if (!validator.validate(mySchema, myTargetAdapter, nullptr))
    {
        std::cerr <<"[Error] File: "<<jsonPath<<" is not valid with validator: "<<validatorPath<<'\n';

        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}