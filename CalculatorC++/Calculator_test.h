#ifndef __INSTRUCTOR__TEST_
#define __INSTRUCTOR__TEST_

#include <cxxtest/TestSuite.h>
#include "Calculator.h"
//Livia Rose and Jolie Rabideau
/**
 *  Test class for Calculator.
 *
 *  @author  Stephen Edwards
 *  @version 2006.06.14
 *  Modified by Stefan Brandle
 *
 *  Comments: The class declaration line right below must be all on the same line, 
 *    else you get a compiler error. Astounding, but true.
 */

class Instructor__test : public CxxTest::TestSuite {
public:
    /**
     * Check that a new calculator starts off with a zero value.
     */
    void testConstructor() {
        Calculator calc;
        TS_ASSERT_EQUALS( calc.getValue(), 0 );
    }

    /**
     * Check that setValue/getValue work correctly.
     */
    void testSetValue() {
        Calculator calc;
        calc.setValue( 11 );
        TS_ASSERT_EQUALS( calc.getValue(), 11 );
    }

    /**
     * Check that clear works correctly.
     */
    void testClear() {
        // Put testing code here.
        Calculator calc;
        calc.clear();
        TS_ASSERT_EQUALS( calc.getValue(), 0 );
    }

    /**
     * Check that add works correctly.
     */
    void testAdd() {
        Calculator calc;
        calc.setValue(5);
        calc.add(10);
        TS_ASSERT_EQUALS( calc.getValue(), 15 );
    }

    /**
     * Check that subtract works correctly.
     */

    // Write the entire testing function here.
    void testSubtract() {
        Calculator calc;
        calc.setValue(5);
        calc.subtract(2);
        TS_ASSERT_EQUALS( calc.getValue(), 3 );
    }

    /**
     * Check that multiplyBy works correctly.
     */

    // Write the entire testing function here.
    void testMultiplyBy() {
        Calculator calc;
        calc.setValue(5);
        calc.multiplyBy(2);
        TS_ASSERT_EQUALS( calc.getValue(), 10 );
    }


    /**
     * Check that divideBy works correctly.
     */

    // Write the entire testing function here.
    void testDivideBy() {
        Calculator calc;
        calc.setValue(10);
        calc.divideBy(2);
        TS_ASSERT_EQUALS( calc.getValue(), 5 );
    }


};

#endif /*__INSTRUCTOR__TEST_*/
