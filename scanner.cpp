#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>

using namespace std;

//=====================================================
// File scanner.cpp written by: Group Number: 19 
//=====================================================

// --------- Two DFAs ---------------------------------

// WORD DFA 
// Done by: Chihiro Nishijima
// RE:  ( vowel | vowel n | consonant vowel | consonant vowel n | consonant-pair vowel | consonant-pair vowel n)^+ 
bool word (string s)
{
  int state = 0;    // declare integer called state as 0 for state number; DFA diagram: 0='q0', 1='q0q1', 2='q0qy', 3='qc', 4='qt', 5='qsa', 6='qy', 7='qs' 
  int charpos = 0;  // declare integer called charpos as 0 for each position in the string s.
  vector<char> vowel = {'a', 'i', 'I', 'u', 'e', 'E', 'o'};
  vector<char> bmknhprg = {'b', 'm', 'k', 'n', 'h', 'p', 'r', 'g'};
  vector<char> dwzyj = {'d', 'w', 'z', 'y', 'j'};
  
  //replace the following todo the word dfa  **
  while (s[charpos] != '\0')  // check every character in the position of string s. 
    {
      if ((state >= 0 && state <= 7) && find(vowel.begin(), vowel.end(), s[charpos]) != vowel.end()) // if current state: 0, 1, 2, 3, 4, 5, 6, or 7; the character at charpos is any character in vowel vector(:'a', 'i', 'I', 'u', 'e', 'E', 'o')
      state = 1; // then go to state 1.
      else
      if (state == 1 && s[charpos] == 'n')  // current state: 1; if the character at charpos is 'n'
      state = 2;  // then go to state 2.
      else
      if ((state >= 0 && state <= 2) && s[charpos] == 'c') // if current state: 0, 1, or 2; the character at charpos is 'c'
      state = 3;  // then go to state 3.
      else
      if ((state >= 0 && state <= 2) && s[charpos] == 't') // if current state: 0, 1, or 2;  the character at charpos is 't'
      state = 4;  // then go to state 4.
      else                                        // either of these cases below...
      if ((state == 3 && s[charpos] == 'h') ||    // if current state: 3;  the character at charpos is 'h' 
          (state == 7 && s[charpos] == 'h') ||    // if current state: 7;  the character at charpos is 'h' 
          (state == 4 && s[charpos] == 's') ||    // if current state: 4;  the character at charpos is 's' 
          (state == 6 && s[charpos] == 'y') ||    // if current state: 6;  the character at charpos is 'y' 
          ((state >= 0 && state <= 2) && find(dwzyj.begin(), dwzyj.end(), s[charpos]) != dwzyj.end()))   // if current state: 0, 1, or 2;  the character at charpos is any character in dwzyj vector(:'d', 'w', 'z', 'y', 'j')
      state = 5;  // then go to state 5.
      else
      if ((state >= 0 && state <= 2) && find(bmknhprg.begin(), bmknhprg.end(), s[charpos]) != bmknhprg.end())   // if current state: 0, 1, or 2;  the character at charpos is any character in bmknhprg vector(:'b', 'm', 'k', 'n', 'h', 'p', 'r', 'g')
      state = 6;  // then go to state 6.
      else
      if ((state >= 0 && state <= 2) && s[charpos] == 's') // if current state: 0, 1, or 2; the character at charpos is 's'
      state = 7;  // then go to state 7.
      else
	    return(false); // otherwise, return false which means this string is not suitable for this token. 
      charpos++;  // add 1 into charpos to move to the next character.
    }//end of while

  // where did I end up????
  if (state == 0 || state == 1 || state == 2) return(true);  // end in a final state: if state ends up either 0, 1 or 2, then return true. 
   else return(false);  // otherwise return false.
}

// PERIOD DFA 
// Done by: Chihiro Nishijima
// RE: .
bool period (string s)
{  
   int state = 0;     // declare integer called state as 0 for state number.
   if(state == 0 && s == ".") // if current state: 0; the string is "."
   {
      state = 1;  // then go to state 1.
      return (true); // since state ends up 1, return true meaning the string is suitable for this token.
   } else return(false); // otherwise, return false.
}

// ------ Three  Tables -------------------------------------

// TABLES Done by: Chihiro Nishijima

// ** Update the tokentype to be WORD1, WORD2, PERIOD, ERROR, EOFM, etc.
enum tokentype {ERROR, WORD1, WORD2, PERIOD, VERB, VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS, 
                OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR, EOFM};

// ** For the display names of tokens - must be in the same order as the tokentype.
string tokenName[30] = {"ERROR", "WORD1", "WORD2", "PERIOD", "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", "IS", "WAS", 
                "OBJECT", "SUBJECT", "DESTINATION", "PRONOUN", "CONNECTOR", "EOFM"}; 

// ** Need the reservedwords table to be set up here. 
// ** Do not require any file input for this. Hard code the table.
// ** a.out should work without any additional files.
// structure w_words with string wd and enum's tokentype t to check reserved words in scanner() function.
struct r_words 
{
   string wd;
   tokentype t;
};
r_words reservedWordsTable[] = {{"masu", VERB}, 
                                       {"masen", VERBNEG},
                                       {"mashita", VERBPAST},
                                       {"masendeshita", VERBPASTNEG},
                                       {"desu", IS},
                                       {"deshita", WAS},
                                       {"o", OBJECT},
                                       {"wa", SUBJECT},
                                       {"ni", DESTINATION},
                                       {"watashi", PRONOUN},
                                       {"anata", PRONOUN},
                                       {"kare", PRONOUN},
                                       {"kanojo", PRONOUN},
                                       {"sore", PRONOUN},
                                       {"mata", CONNECTOR},
                                       {"soshite", CONNECTOR},
                                       {"shikashi", CONNECTOR},
                                       {"dakara", CONNECTOR},
                                       {"eofm", EOFM}};

// ------------ Scanner and Driver ----------------------- 

ifstream fin;  // global stream for reading from the input file

// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
// ** Done by: Chihiro Nishijima
int scanner(tokentype& tt, string& w)
{ 
  fin >> w;   // ** Grab the next word from the file via fin
  cout << "Scanner called using word: " << w << endl;      // print out the word checked in scanner.
  
  if(w == "eofm") // if the word is eofm, 
  { tt = EOFM; return 0;}    // return right now.
  
  if(period(w))       // if period(w) is true meaning w is a valid token of PERIOD,
  { tt = PERIOD; }    // set tokentype tt as PERIOD. 
  else 
  if(word(w))         // else if word(w) is true meaning w is a valid token of WORD,
  {
     int endpos = w.length() - 1;  // declare integer charpos as the string w's size - 1 to get the last character of w.
     bool flag = false;    // declare flag as false to check whether or not the word is reserved. false = NOT reserved; true = reserved.
     int size = sizeof(reservedWordsTable)/sizeof(reservedWordsTable[0]);  // declare the size of reservedWordsTable.
     for(int i = 0; i < size; i++)  // check the every words in the table. 
     {
         if (reservedWordsTable[i].wd == w)  // if the word is found in the reservedWordsTable
         {
            tt = reservedWordsTable[i].t;
            flag = true;     // set the flag is true which means the word is reserved.
            break;
         }  // end of if
     }  // end of for
     if (flag)   // if flag is true meaning the word is reserved,
     {  tt = tt;  }    // the tokentype is the type set as reservedWordsTable[i].t.
     else
     if (w[endpos] == 'I' || w[endpos] == 'E')  // if the last character of string w is either 'I' or 'E',
     {  tt = WORD2; }                           // set tokentype as WORD2.
     else
     {  tt = WORD1; }                          // otherwise, set tokentype as WORD1.
  } // end of if
  else
  {
     cout << endl << "Lexical Error: " << w << " is not a valid token" << endl;  // print out the Lexical error message with the word.
     tt = ERROR;                                                            // set the tokentype as ERROR.
  } // end of else
  return 0;   // retun the tokentype and the string (pass by reference)

}//the end of scanner