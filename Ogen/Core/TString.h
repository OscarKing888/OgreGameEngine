// *************************************************************************************
// 
// 	Copyright (c) 2009 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2009-6-14
// *************************************************************************************

#pragma once
#include "CorePrerequisites.h"
#include <functional>
#include <hash_map>

#ifndef OGRE_VERSION
typedef std::string String;
typedef std::vector<std::string> StringVector;
#endif

template<class T>
struct NoCaseLess: public std::less<T>
{
	bool operator()(const T& x, const T& y) const
	{
		int iRe = _stricmp( x.c_str(), y.c_str() );
		return( iRe < 0 );
	}
};

struct NoCaseEqual: public std::less<Ogre::String>
{
	bool operator()(const Ogre::String& x, const Ogre::String& y) const
	{
		int iRe = _stricmp( x.c_str(), y.c_str() );
		return( iRe == 0 );
	}
};

#ifndef OGRE_VERSION

/** Utility class for manipulating Strings.  */
class _CoreExport StringUtil
{
public:
	typedef std::ostringstream StrStreamType;

	/** Removes any whitespace characters, be it standard space or
	TABs and so on.
	@remarks
	The user may specify wether they want to trim only the
	beginning or the end of the String ( the default action is
	to trim both).
	*/
	static void trim( String& str, bool left = true, bool right = true );

	/** Returns a StringVector that contains all the substrings delimited
	by the characters in the passed <code>delims</code> argument.
	@param
	delims A list of delimiter characters to split by
	@param
	maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
	parameters is > 0, the splitting process will stop after this many splits, left to right.
	*/
	static StringVector split( const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0);

	/** Upper-cases all the characters in the string.
	*/
	static void toLowerCase( String& str );

	/** Lower-cases all the characters in the string.
	*/
	static void toUpperCase( String& str );


	/** Returns whether the string begins with the pattern passed in.
	@param pattern The pattern to compare with.
	@param lowerCase If true, the end of the string will be lower cased before
	comparison, pattern should also be in lower case.
	*/
	static bool startsWith(const String& str, const String& pattern, bool lowerCase = true);

	/** Returns whether the string ends with the pattern passed in.
	@param pattern The pattern to compare with.
	@param lowerCase If true, the end of the string will be lower cased before
	comparison, pattern should also be in lower case.
	*/
	static bool endsWith(const String& str, const String& pattern, bool lowerCase = true);

	/** Method for standardising paths - use forward slashes only, end with slash.
	*/
	static String standardisePath( const String &init);

	/** Method for splitting a fully qualified filename into the base name
	and path.
	@remarks
	Path is standardised as in standardisePath
	*/
	static void splitFilename(const String& qualifiedName,
		String& outBasename, String& outPath);

	/** Method for splitting a fully qualified filename into the base name,
	extension and path.
	@remarks
	Path is standardised as in standardisePath
	*/
	static void splitFullFilename(const String& qualifiedName, 
		String& outBasename, String& outExtention, 
		String& outPath);

	/** Method for splitting a filename into the base name
	and extension.
	*/
	static void splitBaseFilename(const String& fullName, 
		String& outBasename, String& outExtention);


	/** Simple pattern-matching routine allowing a wildcard pattern.
	@param str String to test
	@param pattern Pattern to match against; can include simple '*' wildcards
	@param caseSensitive Whether the match is case sensitive or not
	*/
	static bool match(const String& str, const String& pattern, bool caseSensitive = true);

	/// Constant blank string, useful for returning by ref where local does not exist
	static const String BLANK;
};
#endif

typedef stdext::hash_compare< _StringBase, std::less< _StringBase > > _StringHash;
_CoreExport extern String formatString(const char* fmtStr, ...);

class _CoreExport StringConverterBase
{
public:

	/** Converts a float to a String. */
	static String toString(float val, unsigned short precision = 6, 
		unsigned short width = 0, char fill = ' ', 
		std::ios::fmtflags flags = std::ios::fmtflags(0) );

	/** Converts an int to a String. */
	static String toString(int val, unsigned short width = 0, 
		char fill = ' ', 
		std::ios::fmtflags flags = std::ios::fmtflags(0) );

	/** Converts a size_t to a String. */
	static String toString(size_t val, 
		unsigned short width = 0, char fill = ' ', 
		std::ios::fmtflags flags = std::ios::fmtflags(0) );

	/** Converts an unsigned long to a String. */
	static String toString(unsigned long val, 
		unsigned short width = 0, char fill = ' ', 
		std::ios::fmtflags flags = std::ios::fmtflags(0) );

	/** Converts a long to a String. */
	static String toString(long val, 
		unsigned short width = 0, char fill = ' ', 
		std::ios::fmtflags flags = std::ios::fmtflags(0) );

	/** Converts a boolean to a String. 
	@param yesNo If set to true, result is 'yes' or 'no' instead of 'true' or 'false'
	*/
	static String toString(bool val, bool yesNo = false);

	/** Converts a StringVector to a string.
	@remarks
	Strings must not contain spaces since space is used as a delimiter in
	the output.
	*/
	static String toString(const StringVector& val);


	//------------------------------------------------------------------------
	// parse functions
	//------------------------------------------------------------------------

	/** Converts a String to a Real. 
	@returns
	0.0 if the value could not be parsed, otherwise the Real version of the String.
	*/
	static float parseFloat(const String& val);

	/** Converts a String to a whole number. 
	@returns
	0.0 if the value could not be parsed, otherwise the numeric version of the String.
	*/
	static int parseInt(const String& val);

	/** Converts a String to a whole number. 
	@returns
	0.0 if the value could not be parsed, otherwise the numeric version of the String.
	*/
	static unsigned int parseUnsignedInt(const String& val);

	/** Converts a String to a whole number. 
	@returns
	0.0 if the value could not be parsed, otherwise the numeric version of the String.
	*/
	static long parseLong(const String& val);

	/** Converts a String to a whole number. 
	@returns
	0.0 if the value could not be parsed, otherwise the numeric version of the String.
	*/
	static unsigned long parseUnsignedLong(const String& val);

	/** Converts a String to a boolean. 
	@remarks
	Returns true if case-insensitive match of the start of the string
	matches "true", "yes" or "1", false otherwise.
	*/
	static bool parseBool(const String& val);

	/** Pareses a StringVector from a string.
	@remarks
	Strings must not contain spaces since space is used as a delimiter in
	the output.
	*/
	static StringVector parseStringVector(const String& val);

	/** Checks the String is a valid number value. */
	static bool isNumber(const String& val);
};