#include "unity.h"
#include <string.h>
#include "Dictionary.h"
#include "Compression.h"
#include "mock_Stream.h"
#include "Utility.h"

void setUp(void)
{
}

void tearDown(void)
{
}


/* Variable mode LZ78_Compressor */
/* Variable mode LZ78_Compressor */

void test_LZ78_Compressor_given_input_A_should_output_1A()
{
    Dictionary *dict = initDictionary(10);
    InStream in ; 
    OutStream out ;
    
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,1,1);
    streamWriteBits_Expect(&out,'A',8);
    
    streamReadBits_ExpectAndReturn(&in,8,EOF);
    checkEndOfFile_ExpectAndReturn(&in,1);
    
    LZ78_Compressor(dict,&in,&out,Variable);

    TEST_ASSERT_EQUAL_STRING("A",dict->Entry[0].data);
    destroyDictionary(dict,10);
}

void test_LZ78_Compressor_given_input_AAAAAAAAA_should_output_1A_2A_3A_4()
{
    Dictionary *dict = initDictionary(10);
    InStream in ; 
    OutStream out ;
    
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,1,1);
    streamWriteBits_Expect(&out,'A',8);
    
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,2,2);
    streamWriteBits_Expect(&out,'A',8);
    
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,3,2);
    streamWriteBits_Expect(&out,'A',8);
    
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,EOF);
    checkEndOfFile_ExpectAndReturn(&in,1);
    streamWriteBits_Expect(&out,4,3);
    
    LZ78_Compressor(dict,&in,&out,Variable);
    

    TEST_ASSERT_EQUAL_STRING("A",dict->Entry[0].data);
    TEST_ASSERT_EQUAL_STRING("AA",dict->Entry[1].data);
    TEST_ASSERT_EQUAL_STRING("AAA",dict->Entry[2].data);
    TEST_ASSERT_EQUAL(3,dict->currentIndex);
    
    destroyDictionary(dict,10);
}

void test_LZ78_Compressor_given_input_space_A_space_B_should_output_1space_1A_2B()
{
    Dictionary *dict = initDictionary(10);
    InStream in ; 
    OutStream out ;

    streamReadBits_ExpectAndReturn(&in,8,32);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,1,1);
    streamWriteBits_Expect(&out,32,8);
    
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,1,2);
    streamWriteBits_Expect(&out,'A',8);

    streamReadBits_ExpectAndReturn(&in,8,32);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,'B');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,2,2);
    streamWriteBits_Expect(&out,'B',8);
    
    streamReadBits_ExpectAndReturn(&in,8,EOF);
    checkEndOfFile_ExpectAndReturn(&in,1);
    
    LZ78_Compressor(dict,&in,&out,Variable);

    TEST_ASSERT_EQUAL(32,dict->Entry[0].data[0]);
    TEST_ASSERT_EQUAL_STRING("A",dict->Entry[1].data);
    TEST_ASSERT_EQUAL_STRING(" B",dict->Entry[2].data);
    
    destroyDictionary(dict,10);
}

void test_LZ78_Compressor_dictionaryIndex_4095_input_ABCABC_should_refresh_dictionary_and_add()
{
    Dictionary *dict = initDictionary(4096);
    dict->currentIndex = 4095;
    InStream in ; 
    OutStream out ;

     /*Dictionary Entry 4095*/
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,1,13);
    streamWriteBits_Expect(&out,'A',8);
    
    /*Dictionary Entry 0*/
    streamReadBits_ExpectAndReturn(&in,8,'B');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,1,1);
    streamWriteBits_Expect(&out,'B',8);
    
    /*Dictionary Entry 1*/
    streamReadBits_ExpectAndReturn(&in,8,'C');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,1,2);
    streamWriteBits_Expect(&out,'C',8);

    /*Dictionary Entry 2*/
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,1,2);
    streamWriteBits_Expect(&out,'A',8);
    
    /*Dictionary Entry 3*/
    streamReadBits_ExpectAndReturn(&in,8,'B');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,'C');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,2,3);
    streamWriteBits_Expect(&out,'C',8);
    
    streamReadBits_ExpectAndReturn(&in,8,EOF);
    checkEndOfFile_ExpectAndReturn(&in,1);
    
    LZ78_Compressor(dict,&in,&out,Variable);
 
    TEST_ASSERT_EQUAL_STRING("B",dict->Entry[0].data);
    TEST_ASSERT_EQUAL_STRING("C",dict->Entry[1].data);
    TEST_ASSERT_EQUAL_STRING("A",dict->Entry[2].data);
    TEST_ASSERT_EQUAL_STRING("BC",dict->Entry[3].data);
    TEST_ASSERT_EQUAL(4,dict->currentIndex);
    
    destroyDictionary(dict,4096);
}



/* CR = carriage return = 13 , 0x0D
 * LF = line feed  = 10 0x0A
 */
void test_LZ78_Compressor_given_input_CR_LF_LF_A_should_output_1CR_1LF_3A()
{
    Dictionary *dict = initDictionary(10);
    InStream in ; 
    OutStream out ;

    streamReadBits_ExpectAndReturn(&in,8,13);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,1,1);
    streamWriteBits_Expect(&out,'\r',8);
    
    streamReadBits_ExpectAndReturn(&in,8,10);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,1,2);
    streamWriteBits_Expect(&out,'\n',8);

    streamReadBits_ExpectAndReturn(&in,8,10);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,'A');
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,3,2);
    streamWriteBits_Expect(&out,'A',8);
    
    streamReadBits_ExpectAndReturn(&in,8,EOF);
    checkEndOfFile_ExpectAndReturn(&in,1);
    
    LZ78_Compressor(dict,&in,&out,Variable);
 
    TEST_ASSERT_EQUAL(0x0D,*dict->Entry[0].data);
    TEST_ASSERT_EQUAL(0x0A,*dict->Entry[1].data);
    TEST_ASSERT_EQUAL(0X0A,*dict->Entry[2].data);
    TEST_ASSERT_EQUAL('A',dict->Entry[2].data[1]);
    
    destroyDictionary(dict,10);
}

void test_LZ78_Compressor_given_9x_NULL_should_return_1NULL_2NULL_3NULL_4()
{
    Dictionary *dict = initDictionary(10);
    InStream in ;
    OutStream out ;
    char dataString1[1] = {} , dataString2[2] = {} , dataString3[3] = {} ;
    
    /*Dictionary Entry 0 will Contains 0 EntrySize : 1*/
    streamReadBits_ExpectAndReturn(&in,8,0);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,1,1);
    streamWriteBits_Expect(&out,0,8);

    /*Dictionary Entry 1 will Contains 00 EntrySize : 2*/
    streamReadBits_ExpectAndReturn(&in,8,0);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,0);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,2,2);
    streamWriteBits_Expect(&out,0,8);

    /*Dictionary Entry 2 will Contains 000 EntrySize : 3*/
    streamReadBits_ExpectAndReturn(&in,8,0);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,0);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,0);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamWriteBits_Expect(&out,3,2);
    streamWriteBits_Expect(&out,0,8);

    streamReadBits_ExpectAndReturn(&in,8,0);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,0);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,0);
    checkEndOfFile_ExpectAndReturn(&in,0);
    streamReadBits_ExpectAndReturn(&in,8,EOF);
    checkEndOfFile_ExpectAndReturn(&in,1);
    streamWriteBits_Expect(&out,4,3);

    LZ78_Compressor(dict,&in,&out,Variable);

    
    TEST_ASSERT_EQUAL(1,dict->Entry[0].entrySize);
    TEST_ASSERT_EQUAL(0,memcmp(dataString1,(dict->Entry[0].data),1));
    
    TEST_ASSERT_EQUAL(2,dict->Entry[1].entrySize);
    TEST_ASSERT_EQUAL(0,memcmp(dataString2,(dict->Entry[1].data),2));
    
    TEST_ASSERT_EQUAL(3,dict->Entry[2].entrySize);
    TEST_ASSERT_EQUAL(0,memcmp(dataString3,(dict->Entry[0].data),3));
    
    destroyDictionary(dict,10);    
}