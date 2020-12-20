//============================= parser.cpp ============================================

/* INSTRUCTION:  Complete all ** parts.
   You may use any method to connect this file to scanner.cpp
   that you had written.  
  e.g. You can copy scanner.cpp here by:
          cp ../ScannerFiles/scanner.cpp .  
       and then append the two files into one: 
          cat scanner.cpp parser.cpp > myparser.cpp
*/

//=================================================
// File parser.cpp written by Group Number: 19
//=================================================

// ----- Four Utility Functions and Globals -----------------------------------

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
//    to display syntax error messages as specified by me.
// ** Need the updated match and next_token with 2 global vars
// saved_token and saved_lexeme

tokentype saved_token;
string saved_lexeme;

ofstream errorfile;

bool token_available;
bool t_flag; // trasing messege flag: true = turn on & false = turn off

// Purpose: Finds the value of the next token, which can then be passed to the "match" function. 
// Done by: Ryan Michels
tokentype next_token() // pass back token type into global variable
{
  if(!token_available) //fetch next token if there isnt already one available to analyze
    {
      scanner(saved_token, saved_lexeme);
      token_available = true;
    }
  return saved_token;
}

// Type of error: Match fails
// Done by: Ricardo Lizarraga, Chihiro Nishijima(syntax error recovery)
bool syntaxerror1(tokentype token_type, string saved_lexeme){
    cout << "SYNTAX ERROR: expected " << tokenName[token_type] << " but found " << saved_lexeme << endl;
    string input;
    cout << "Skip or replace the token? (s/r): ";
    cin >> input;
    
    if(input == "s") // if the user choice is "skip"
    {
       errorfile << "SYNTAX ERROR: expected " << tokenName[token_type] 
                 << " but found " << saved_lexeme << endl; // error message for errors.txt
       token_available = false; // false to go to the next token in scanner.
       if(next_token() != token_type)  // if the token is incorrect then syntaxerror1 again.
         syntaxerror1(token_type, saved_lexeme);
       else
      {
        token_available = false; // false to go to the next token in scanner
        if(t_flag == true)  // if tracing feature is turned on by the user
           cout << "Matched " << tokenName[saved_token] << endl;
        return true;  // get out from this function.
      } // end of else
    } // end of if
    else 
    if(input == "r") // if the user choice is "replace"
    {
       errorfile << "SYNTAX ERROR: expected " << tokenName[token_type] 
                 << " but found " << saved_lexeme << endl;  // error message for errors.txt
       saved_token = token_type; 
       token_available = false;
       if(t_flag == true)
         cout << "Matched " << tokenName[token_type] << endl;
       return true;
    } // end of else if
}

// Type of error: Switch default in a parser function
// Done by: Ricardo Lizarraga
void syntaxerror2(string saved_lexeme, string parser_function) {
    cout << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in " << parser_function << endl;
    exit(1);
}


// Purpose: Checks the next token to see if it follows the RDP rules. RDP = "Recursive Descent Parsing"
// Done by: Ryan Michels
bool match(tokentype expected) 
{
  if(next_token() != expected) // if returned token value doesnt == expected, error. Else a match
    {
      syntaxerror1(expected, saved_lexeme);
    }
  else
    {
      token_available = false; //ready up the next_token
      if(t_flag == true)
         cout << "Matched " << tokenName[saved_token] << endl;
      return true; // there was a match!!!
    }
}