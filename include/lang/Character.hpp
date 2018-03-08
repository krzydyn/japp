#ifndef __LANG_CHARACTER_HPP
#define __LANG_CHARACTER_HPP

#include <lang/Comparable.hpp>
#include <lang/Exception.hpp>

namespace lang {

class CharacterData : Object {
public:
	static CharacterData& of(int ch);
	virtual int getType(int ch) = 0;
	virtual int digit(int ch, int radix) = 0;
};

class Character : extends Object, implements Comparable<Character> {
public:
	static const int MIN_RADIX = 2;
	static const int MAX_RADIX = 36;
	static const jchar MIN_VALUE = u'\u0000';
	static const jchar MAX_VALUE = u'\uFFFF';
	
	static const byte UNASSIGNED = 0;
	static const byte UPPERCASE_LETTER = 1;
	static const byte LOWERCASE_LETTER = 2;
	static const byte TITLECASE_LETTER = 3;
	static const byte MODIFIER_LETTER = 4;
	static const byte OTHER_LETTER = 5;
	static const byte NON_SPACING_MARK = 6;
	static const byte ENCLOSING_MARK = 7;
	static const byte COMBINING_SPACING_MARK = 8;
	static const byte DECIMAL_DIGIT_NUMBER = 9;
	static const byte LETTER_NUMBER = 10;
	static const byte OTHER_NUMBER = 11;
	static const byte SPACE_SEPARATOR = 12;
	static const byte LINE_SEPARATOR = 13;
	static const byte PARAGRAPH_SEPARATOR = 14;
	static const byte CONTROL = 15;
	static const byte FORMAT = 16;
	static const byte PRIVATE_USE = 18;
	static const byte SURROGATE = 19;

	static const byte FINAL_QUOTE_PUNCTUATION = 30;

	static const int ERROR = 0xFFFFFFFF;

	static boolean isDigit(char ch) {
		return isDigit((int)ch);
	}
	static boolean isDigit(int codePoint) {
		return getType(codePoint) == Character::DECIMAL_DIGIT_NUMBER;
	}
	static int digit(char ch, int radix) {
		return digit((int)ch, radix);
	}
	static int digit(int codePoint, int radix) {
		return CharacterData::of(codePoint).digit(codePoint, radix);
	}
	
	static int getType(int codePoint) {
		return CharacterData::of(codePoint).getType(codePoint);
	}
};

}

#endif
