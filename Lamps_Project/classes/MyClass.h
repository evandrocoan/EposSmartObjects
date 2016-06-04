






/**
 * Preprocessor directive designed to cause the current source file to be included only once in a
 * single compilation. Thus, serves the same purpose as #include guards, but with several
 * advantages, including: less code, avoidance of name clashes, and sometimes improvement in
 * compilation speed. In main file this is enabled by default.
 */
#pragma once


class MyClass
{
public:

    int hi;
    
    MyClass();
    
    int get_hi();

};



MyClass::MyClass()
{
    this->hi = 5;
}

int MyClass::get_hi()
{
    return this->hi;
}




