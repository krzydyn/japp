#include <lang/Character.hpp>
#include <lang/System.hpp>

#include <ctype.h>

namespace lang {

namespace {
class CharacterDataLatin1 : extends CharacterData {
    /* The character properties are currently encoded into 32 bits in the following manner:
        1 bit   mirrored property
        4 bits  directionality property
        9 bits  signed offset used for converting case
        1 bit   if 1, adding the signed offset converts the character to lowercase
        1 bit   if 1, subtracting the signed offset converts the character to uppercase
        1 bit   if 1, this character has a titlecase equivalent (possibly itself)
        3 bits  0  may not be part of an identifier
                1  ignorable control; may continue a Unicode identifier or Java identifier
                2  may continue a Java identifier but not a Unicode identifier (unused)
                3  may continue a Unicode identifier or Java identifier
                4  is a Java whitespace character
                5  may start or continue a Java identifier;
                   may continue but not start a Unicode identifier (underscores)
                6  may start or continue a Java identifier but not a Unicode identifier ($)
                7  may start or continue a Unicode identifier or Java identifier
                Thus:
                   5, 6, 7 may start a Java identifier
                   1, 2, 3, 5, 6, 7 may continue a Java identifier
                   7 may start a Unicode identifier
                   1, 3, 5, 7 may continue a Unicode identifier
                   1 is ignorable within an identifier
                   4 is Java whitespace
        2 bits  0  this character has no numeric property
                1  adding the digit offset to the character code and then
                   masking with 0x1F will produce the desired numeric value
                2  this character has a "strange" numeric value
                3  a Java supradecimal digit: adding the digit offset to the
                   character code, then masking with 0x1F, then adding 10
                   will produce the desired numeric value
        5 bits  digit offset
        5 bits  character type

        The encoding of character properties is subject to change at any time.
     */
private:
	int A[256];
	CharacterDataLatin1() {
		for (int i=0; i < 256; ++i) {
			A[i] = 0;
			if (isdigit(i)) A[i] = ('0' << 5)*0x3E0 + Character::DECIMAL_DIGIT_NUMBER;
		}
	}
public:
	static CharacterDataLatin1 instance;

	int getProperties(int ch) {
		return A[ch&0xff];
	}
	int getType(int ch) {
		int props = getProperties(ch);
		return (props & 0x1F);
	}
	int digit(int ch, int radix) {
		int value = -1;
		if (radix >= Character::MIN_RADIX && radix <= Character::MAX_RADIX) {
			int val = getProperties(ch);
			int kind = val & 0x1F;
			if (kind == Character::DECIMAL_DIGIT_NUMBER) {
				//value = (ch + ((val & 0x3E0) >> 5)) & 0x1F;
				value = ch - '0';
			}
			else if ((val & 0xC00) == 0x00000C00) { // Java supradecimal digit
				value = ((ch + ((val & 0x3E0) >> 5)) & 0x1F) + 10;
			}
		}
		return (value < radix) ? value : -1;
	}
};
CharacterDataLatin1 CharacterDataLatin1::instance;
}

CharacterData& CharacterData::of(int ch) {
	return CharacterDataLatin1::instance;
/*
	if (ch >>> 8 == 0) {	 // fast-path
		return CharacterDataLatin1.instance;
	} else {
		switch(ch >>> 16) {  //plane 00-16
		case(0):
			return CharacterData00.instance;
		case(1):
			return CharacterData01.instance;
		case(2):
			return CharacterData02.instance;
		case(14):
			return CharacterData0E.instance;
		case(15):   // Private Use
		case(16):   // Private Use
			return CharacterDataPrivateUse.instance;
		default:
			return CharacterDataUndefined.instance;
		}
	}
*/
}

}
