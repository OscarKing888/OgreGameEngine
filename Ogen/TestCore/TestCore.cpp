// TestCore.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
namespace TestNameSpace
{	
	class TestClass
	{
	};

	struct TestStruct
	{

	};
}

int _tmain(int /*argc*/, _TCHAR** /*argv*/)
{
	//setlocale(LC_ALL, "chs");

#if 0
	XMLDoc doc;
	assert(doc.loadFromFile("test.xml"));

	XMLConfigReader cfg("test.xml");
	printf("testBool %d\n", cfg.getBool("testConfig/testBool"));
	printf("testFloat %f\n", cfg.getFloat("testConfig/testFloat"));
	printf("testInt %d\n", cfg.getInt("testConfig/testInt"));
	
	{
		UTFString utf(cfg.getString("testConfig/testString"));
		printf("testString %s\n", utf.asMBCSStr().c_str());
		////::MessageBoxW(NULL, utf.asWStr_c_str(), L"", MB_OK);
		//::MessageBoxA(NULL, utf.asMBCSStr().c_str(), "", MB_OK);
		////::MessageBoxA(NULL, (char*)utf.c_str(), "", MB_OK);
	}

	{
		UTFString utf(String("这里是【MBCS】字符了★○●◎◇abc\n"), false);
		printf("testString %s\n", utf.asMBCSStr().c_str());
		////::MessageBoxW(NULL, utf.asWStr_c_str(), L"", MB_OK);
		//::MessageBoxA(NULL, utf.asMBCSStr().c_str(), "", MB_OK);
		////::MessageBoxA(NULL, (char*)utf.c_str(), "", MB_OK);
	}

#endif

	printf("%s\n", getClassName<TestNameSpace::TestClass>(false).c_str());
	printf("%s\n", getClassName<TestNameSpace::TestClass>(true).c_str());

	// struct is error
// 	printf("%s\n", getClassName<TestNameSpace::TestStruct>(false).c_str());
// 	printf("%s\n", getClassName<TestNameSpace::TestStruct>(true).c_str());

	DEBUG_TRACE(getClassName<TestNameSpace::TestClass>(false).c_str() << 5 + 8 << " :)");

	XGUID guid = XGUID::createGUID();
	String str = guid.toString();
	printf("%s\n", str.c_str());
	XGUID guid2;
	guid2.fromString(str);

	printf("%s\n == %d", guid2.toString().c_str(), (guid2 == guid));

	return 0;
}

