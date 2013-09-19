#include <iostream>
#include <string>
#include <sstream>
#include <vector>

class Assert
{ 
    public:
    
    void static isTrue( bool actual, const char* message )
    {
        if ( actual )
        {
            std::cout << "Passed: Actual " << actual << ". " << message;
        }
        else 
        {
            std::cout << "ERROR : Expected true. Actual value = " 
                << actual << ". " << message;
        }
    }

    void static isFalse( bool actual, const char* message )
    {
        if ( actual == false )
        {
            std::cout << "Passed: Actual " << actual << ". " << message;
        }
        else 
        {
            std::cout << "ERROR : Expected false. Actual value = " 
                << actual << ". " << message;
        }
    }

    void static isNULL( void* actual, const char* message )
    {
        if ( actual == NULL )
        {
            std::cout << "Passed: Actual " << actual << ". " << message;
        }
        else 
        {
            std::cout << "ERROR - Expected NULL. Actual value = " 
                << actual << ". " << message;
        }
    }

    void static isNotNULL( void* actual, const char* message )
    {
        if ( actual != NULL )
        {
            std::cout << "Passed: Actual " << actual << ". " << message;
        }
        else 
        {
            std::cout << "ERROR - Expected not NULL. Actual value = " 
                << actual << ". " << message;
        }
    }
    
    void static areEqual( void* expected, void* actual, const char* message )
    {
        if ( expected == actual )
        {
            std::cout << "Passed: Value = " << actual << ". " << message;
        }
        else 
        {
            std::cout << "ERROR - Expected " << expected << ", but was " 
                << actual << ". " << message;
        }
    }
    
    void static areEqual( float expected, float actual, const char* message )
    {
        if ( expected == actual )
        {
            std::cout << "Passed: Value = " << actual << ". " << message;
        }
        else 
        {
            std::cout << "ERROR - Expected " << expected << " but was " 
                << actual << ". " << message;
        }
    }
    
    void static areEqual( std::vector<float> expected, std::vector<float> actual, const char* message )
    {
        int passed = 1;
        int expectedSize = expected.size();
        int actualSize = actual.size();
        
        if( expectedSize == actualSize )
        {
            for( int i = 0; i < expectedSize; i++ )
            {
                if( expected[i] != actual[i] )
                {
                    passed = 0;
                    break;
                }
            }
        }
        else
        {
            passed = 0;
        }
        
        if( passed )
        {
            std::cout << "Passed: Arrays contained same values. " 
                << vectorToString(expected) << " " << message;
        }
        else
        {   
            std::cout << "ERROR - Expected " 
                << vectorToString(expected) << " but was " 
                << vectorToString(actual) << ". " << message;
        }
    }
    
    void static areNotEqual( int a, float b, const char* message )
    {
        if ( a != b )
        {
            std::cout << "Passed: " << a << " != " << b << ". " << message;
        }
        else 
        {
            std::cout << "ERROR - Values are not supposed to be equal =" 
                << a << ". " << message;
        }
    }
    
    static std::string vectorToString( std::vector<float> vector )
    {
        std::stringstream builder;
        builder << "[";
        
        if( vector.size() >= 1 ) 
        {
            unsigned int i = 0;
            for( i = 0; i < vector.size() - 1; i++)
            {
                builder << vector[i] << ", ";
            }
            builder << vector[vector.size()-1];
        }
        builder << "]";
        
        return builder.str();
    }
};
