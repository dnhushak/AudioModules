#include <iostream>

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
    private:
};
