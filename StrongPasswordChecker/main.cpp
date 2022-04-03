#include <cassert>
#include <cstring>
#include <iostream>
using namespace std;

bool hasRequiredLength(string password)
{
	/*
	* Input:	a string, the password we want to verify
	* Output:	returns true if the password has the required length
	*			or false otherwise		
	*/
	return 6 <= password.length() and password.length() <= 20;
}

int getPasswordLength(string password)
{
	/*
	* Input:	a string, the password we want to verifiy
	* Output:	returns an integer, the length of the password
	*/
	return password.length();
}

bool hasAtLeastOneUppercaseLetter(string password)
{
	/*
	* Input:	a string, the password we want to verify
	* Output:	returns true if the password has at least one uppercase letter
	*			or false otherwise
	*/
	string uppercaseLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (size_t character = 0; character < uppercaseLetters.length(); character++)
	{
		//we search for the uppercase letter
		if (password.find(uppercaseLetters.at(character)) != std::string::npos)
			return true;
	}
	return false;
}

bool hasAtLeastOneLowercaseLetter(string password)
{
	/*
	* Input:	a string, the password we want to verify
	* Output:	returns true if the password has at least one lowercase letter
	*			or false otherwise
	*/
	string lowercaseLetters = "abcdefghijklmnopqrstuvwxyz";
	for (size_t character = 0; character < lowercaseLetters.length(); character++)
	{
		//searching for the lowercase letter
		if (password.find(lowercaseLetters.at(character)) != std::string::npos)
			return true;
	}
	return false;
}

bool hasAtLeastOneDigit(string password)
{
	/*
	* Input:	a string, the password we want to verify
	* Output:	returns true if the password has at least one digit
	*			or false otherwise
	*/
	string digits = "0123456789";
	for (size_t character = 0; character < digits.length(); character++)
	{
		//searching for the digit
		if (password.find(digits.at(character)) != std::string::npos)
			return true;
	}
	return false;
}

bool hasThreeRepeatingCharacters(string password)
{
	/*
	* Input:	a string, the password we want to verify
	* Output:	returns true if the password has three repeating characters in a row
	*			or false otherwise
	*/
	for (size_t character = 0; character < password.length() - 2; character++)
	{
		//checks if the string has three repeating characters
		if (password.at(character) == password.at(character + 1) && password.at(character + 1) == password.at(character + 2))
			return true;
	}
	return false;
}

bool isStrongPassword(string password)
{
	/*
	* Input:	a string, the password we want to verify
	* Output:	returns true if the password meets all the conditions to be a strong password
	*			or false otherwise
	*/
	if (hasAtLeastOneDigit(password) == true && hasAtLeastOneLowercaseLetter(password) == true && hasAtLeastOneUppercaseLetter(password) == true
		&& hasRequiredLength(password) == true && hasThreeRepeatingCharacters(password) == false)
		return true;
	return false;
}

int minimumChangesRequired(string s)
{
	/*
	* Input:	a string, the password we want to verify
	* Output:	an integer, returns the number of minimum changes
	*			needed for the password to become a strong one or
	*			0 if the password already is strong
	*/
	int changes = 0;	//variable that will store the minimum number of changes required for the password to be a strong one
	int sameLength = s.length();
	int* sameCharacter = new int[sameLength * sizeof(int)];
	memset(sameCharacter, 0, sameLength);	//array that will store the number of appearances for each character
	for (size_t index = 0; index < s.length(); index++)
	{
		size_t j = index;
		while (index < sameLength && s.at(index) == s.at(j))
			index++;
		sameCharacter[j] = index - j;	//computing the number of appearances for each character
	}
	if (isStrongPassword(s))
		return 0;	//if the password is already a strong one we don't need to make any changes, returns 0
	//we need to calculate what we are missing in the password in order to be strong, uppercase letters, digits or lowercase letters
	int missing = !hasAtLeastOneDigit(s) + !hasAtLeastOneLowercaseLetter(s) + !hasAtLeastOneUppercaseLetter(s);
	if (sameLength < 6)
	{
		//if the password does not have the required length i.e has a length smaller that the left-side limit (6 characters)
		//the number of changes we have to make are insertions of the missing requirements of the password
		if (6 - (sameLength + missing) > 0)
			changes += missing + (6 - (sameLength + missing));
		else
			changes += 0;
	}
	else
	{
		//we need to calculate the number of characters over 20 if the password has more than 20 characters
		//for each character over 20 a deletion has to be made, which will be added to the number of changes
		int over = 0, leftOver = 0;
		if (sameLength - 20 > 0)
			over = s.length() - 20;
		else
			over = 0;
		//addition to the number of changes of the character deleted
		changes += over;
		//here we will check the sequences of repeating characters
		//we have to reduce the number of repeating characters with insertions as it is stronger than deletion
		//because deletion will not guarantee that we will in the end have the required length and if not
		//then we will have to make more unnecessary insertions thus contributing to a bigger number changes
		for (int repeating = 1; repeating < 3; repeating++)
		{
			for (int character = 0; character < sameLength && over > 0; character++)
			{
				//if the number of repeating characters is a multiple of 3 apply one change 
				//so the number of repeating characters become k - 1, otherwise it becomes k - 2
				//if it is a multiple of 3 + 1
				if (!(sameCharacter[character] < 3 || sameCharacter[character] % 3 != repeating - 1))
				{
					if (over < repeating)
						sameCharacter[character] -= over;
					else
						sameCharacter[character] -= repeating;
				}
				//we subtract the repeating characters that we have just replaced
				over -= repeating;
			}
		}
		//here we will apply the deletion after the replacement took place above
		for (int repeating = 0; repeating < sameLength; repeating++)
		{
			//if we still have repeating characters in a row and the password has still more than 20 characters
			//calculte the characters needed after the deletion and update the number of repeating characters in a row
			if (sameCharacter[repeating] >= 3 && over > 0)
			{
				//we calculate the characters needed after the deletion
				int needingCharacters = sameCharacter[repeating] - 2;
				//we update the number of repeating characters in a row
				sameCharacter[repeating] -= over;
				//we deleted the characters over
				over -= needingCharacters;
			}
			//if we still have a sequence of three repeating characters in a row
			//we calculate what we are left with after the deletion of the middle character as 
			//this is the optimal delete operation to do and turn the repeating characters into 
			//a sequnce of 2
			if (sameCharacter[repeating] >= 3)
				leftOver += sameCharacter[repeating] / 3;
		}
		//lastly we have to add what we miss if the number of 
		//requirements is greater than the number of leftover repeating characters
		//or the number of leftovers otherwise
		if (missing > leftOver)
			changes += missing;
		else
			changes += leftOver;
	}
	//lastly we have to deallocate the sameCharacter aray
	delete[] sameCharacter;
	//we return the minimum number of changes that we have to make in order
	//to have a strong password
	return changes;

}

int main()
{
	//test cases for the function
	assert(minimumChangesRequired("a") == 5);
	assert(minimumChangesRequired("aA1") == 3);
	assert(minimumChangesRequired("1337C0d3") == 0);
	assert(minimumChangesRequired("AAAAAA") == 2);
	assert(minimumChangesRequired("aaa5A6") == 1);
	assert(minimumChangesRequired("aba5A") == 1);
	return 0;
}