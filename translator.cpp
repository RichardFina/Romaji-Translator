#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>

using namespace std;

string filename;
ofstream outfile;
//============================ translator.cpp =====================================================

/* INSTRUCTION:  copy your parser.cpp here
      cp ../ParserFiles/parser.cpp .
   Then, insert or append its contents into this file and edit.
   Complete all ** parts.
*/

//=================================================
// File translator.cpp written by Group Number: 19
//=================================================

// ----- Additions to the parser.cpp ---------------------

// ** Declare Lexicon (i.e. dictionary) that will hold the content of lexicon.txt
// Make sure it is easy and fast to look up the translation.
// Do not change the format or content of lexicon.txt 
//  Done by: Chihiro Nishijima
vector<vector<string>> dictionary;

// ** Additions to parser.cpp here:
string saved_E_word;
// #getEword# function
//  Obtain English word from dictionary
//  Done by: Ricardo Lizarraga

void getEword() {
    for (int i = 0; i < dictionary.size(); i++) {    //For loop to run through dictionary
        vector<string> entry = dictionary[i];        //Entry from dictionary
        if(saved_lexeme.compare(entry[0]) == 0)        //Compare saved_lexeme with Japanese entry
          {  
            saved_E_word = entry[1];                //If the same, place the English entry into saved_E_word
            return;                                 //Leave function
          }                             
    }
    saved_E_word = saved_lexeme;  
}                 
//  Done by: Ryan Michels
//  Generates the semantic lines, given an input string 

void gen(string str) 
{
    if(str == "TENSE")
    {
        outfile <<str<<": "<<tokenName[saved_token]<<endl; //Print out the tense
    }
    else
    {
        outfile <<str<<": "<<saved_E_word<<endl; //Print out the english word
    }
}

// ----- Changes to the parser.cpp content ---------------------

// ** Comment update: Be sure to put the corresponding grammar 
//    rule with semantic routine calls
//    above each non-terminal function 

// ** Each non-terminal function should be calling
//    getEword and/or gen now.

// Grammar: <tense>::=VERBPAST | VERBPASTNEG | VERB | VERBNEG
// Done by: Chihiro Nishijima
void tense()
{
   if(t_flag == true)
      cout << "Processing <tense>" << endl;   // print out the message mentioning the beginning of tense parser function.
   switch(next_token())           // check next_token()
   {
      case VERBPAST:              // if next_token() is VERBPAST
         match(VERBPAST);         // match VERBPAST
         break;                   // then break to get out of switch
      case VERBPASTNEG:           // if next_token() is VERBPASTNEG
         match(VERBPASTNEG);      // match VERBPASTNEG
         break;                   // then break to get out of switch
      case VERB:                  // if next_token() is VERB
         match(VERB);             // match VERB
         break;                   // then break to get out of switch
      case VERBNEG:               // if next_token() is VERBNEG
         match(VERBNEG);          // match VERBNEG
         break;                   // then break to get out of switch
      default:                    // otherwise,
         syntaxerror2(saved_lexeme, "tense"); // go to syntaxerror2 to mention the error
   }
}
// Grammar: <be>::=IS | WAS
// Done by: Richard Fina
void be()
{
    if(t_flag == true)
       cout << "Processing <be>" << endl;
    switch(next_token())
    {
        case IS:
            match(IS);
            break;
        case WAS:
            match(WAS);
            break;
        default:
            syntaxerror2(saved_lexeme,"be");
    }
}
// Grammar: <verb>::=WORD2
// Done by: Richard Fina
void verb()
{
    if(t_flag == true)
       cout << "Processing <verb>" << endl;
    switch(next_token())
    {
        case WORD2:
            match(WORD2);
            break;
        default:
            syntaxerror2(saved_lexeme, "verb");
    }
}
// Grammar: <noun>::=WORD1 | PRONOUN
// Done by: Richard Fina
void noun()
{
    if(t_flag == true)
       cout << "Processing <noun>" << endl;
    switch(next_token())
    {
        case WORD1:
            match(WORD1);
            break;
        case PRONOUN:
            match(PRONOUN);
            break;
        default:
            syntaxerror2(saved_lexeme, "noun");
    }
}
// Grammar: <afterObject>::=<verb> #getEword# #gen(�ACTION�)# <tense> #gen(�TENSE�)# PERIOD | 
//                          <noun> #getEword# DESTINATION #gen(�TO�)# <verb> #getEword# #gen(�ACTION�)# <tense> #gen(�TENSE�)# PERIOD
// Done by: Ryan Michels
void afterObject()
{
    if(t_flag == true)
        cout << "Processing <afterObject>" << endl;
    switch(next_token())
    {
        case WORD2:               // WORD2 == verb
          verb();
          getEword();            //Grab english word (if available)
          gen("ACTION");        //Generate "ACTION"
          tense();
          gen("TENSE");            //Generate "TENSE"
          match(PERIOD);
          outfile << endl;
          break;
       case WORD1:               // WORD1/PRONOUN == noun
       case PRONOUN:
          noun();
          getEword();            //Grab english word
          match(DESTINATION);
          gen("TO");            //Generate "ACTION"
          verb();
          getEword();            //Grab english word
          gen("ACTION");
          tense();
          gen("TENSE");            //Generate "TENSE"
          match(PERIOD);
          outfile << endl;
          break;
       default: 
          syntaxerror2(saved_lexeme, "afterObject"); 
    }
}
// Grammar: <afterNoun>::=<be> #gen(�DESCRIPTION�)# #gen(�TENSE�)# PERIOD | 
//                        DESTINATION #gen(�TO�)# <verb> #getEword# #gen(�ACTION�)# <tense> #gen(�TENSE�)# PERIOD | 
//                        OBJECT #gen(�OBJECT�)# <afterObject>
// Done by: Richard Fina
void afterNoun()
{
    if(t_flag == true)
        cout << ("Processing <afterNoun>\n") << endl;

    switch(next_token())
    {
        case IS: 
        case WAS:
            be();
            gen("DESCRIPTION");
            gen("TENSE");
            match(PERIOD);
            outfile << endl;
            break;
        case DESTINATION:
            match(DESTINATION);
            gen("TO");
            verb();
            getEword();
            gen("ACTION");
            tense();
            gen("TENSE");
            match(PERIOD);
            outfile << endl;
            break;
        case OBJECT:
            match(OBJECT);
            gen("OBJECT");
            afterObject();
            break;
        default:
            syntaxerror2(saved_lexeme, "<afterNoun>");
    }
}
// Grammar: <afterSubject>::=<verb> #getEword# #gen(�ACTION�)# <tense> #gen(�TENSE�)# PERIOD | 
//                           <noun> #getEword#  <afterNoun>
// Done by: Ricardo Lizarraga
void afterSubject()
{
    if(t_flag == true)
        cout << "Processing <afterSubject>" << endl;  // print out the message mentioning the beginning of afterSubject parser function.
    switch(next_token())         // check next_token()
    {
       case WORD2:               // if next_token() is WORD2 meaning <verb> 
          verb();                // go to verb() function
          getEword();             //go to getEword() function
          gen("ACTION");        //Generate semantic routine "ACTION"
          tense();               // go to tense() function
          gen("TENSE");            //Generate semantic routine "TENSE"
          match(PERIOD);         // match PERIOD
          outfile << endl;
          break;                 // then break to get out of switch
       case WORD1:               // if next_token() is either WORD1 or PRONOUN
       case PRONOUN:             // which means <noun>
          noun();                // got to noun() function
          getEword();            //go to getEword() function
          afterNoun();         // go to afterNoun()
          break;                 // then break to get out of switch
       default:                  // otherwise,
          syntaxerror2(saved_lexeme, "afterSubject"); // go to syntaxerror2 to mention the error
    } // end of switch
}
// Grammar: <s>::=[CONNECTOR #getEword# #gen(�CONNECTOR�)#] <noun> #getEword# SUBJECT #gen(�ACTOR�)# <afterSubject>
// Done by: Chihiro Nishijima
void s()
{
   if(t_flag == true)
      cout << "Processing <s>" << endl;    // print out the message mentioning the beginning of s parser function.
   switch(next_token())         // check next_token()
   { 
      case CONNECTOR:          // if next_token() is CONNECTOR
         match(CONNECTOR);     // match CONNECTOR
         getEword();           // go to getEword() function
         gen("CONNECTOR");     // sematic routine: "CONNECTOR" line type
         noun();               // go to noun() function
         getEword();           // go to getEword() function
         match(SUBJECT);       // match SUBJECT
         gen("ACTOR");         // sematic routine: "ACTOR" line type
         afterSubject();       // go to afterSubject() function
         break;                // then break to get out of switch
      case WORD1:              // if next_token() is WORD1/PRONOUN meaning <noun>
      case PRONOUN:            
         noun();               // go to noun() function
         getEword();           // go to getEword() function
         match(SUBJECT);       // match SUBJECT
         gen("ACTOR");         // sematic routine: "ACTOR" line type
         afterSubject();       // go to afterSubject() function
         break;                // then break to get out of switch
      default:                 // otherwise,
         syntaxerror2(saved_lexeme, "s"); // go to syntaxerror2 to mention the error
   } // end of switch
}

// Grammar: <story>::=<s>{ <s> }
// Done by: Ricardo Lizarraga, Chihiro Nishijima
void story()
{
   if(t_flag == true)
      cout << "Processing <story>" << endl << endl; // print out the message mentioning the beginning of s parser function.
   s(); // go to s() function for <s>
   while(true)  // repeat loop 
   {
      if(saved_lexeme == "eofm") break;  // if the saved_token is end of the word, break the loop.
      switch(next_token())   // check next_token()
      {
         case CONNECTOR:     // if next_token() is CONNECTOR
         case WORD1:         // if next_token() is WORD1/PRONOUN meaning <noun>
         case PRONOUN:
            s();             // go to s() function
            break;           // then break to get out of switch
         case EOFM: break;   // if next_token() is EOFM, break the switch
         default:            // otherwise,
            syntaxerror2(saved_lexeme, "story"); // go to syntaxerror2 to mention the error
      } // end of switch
   } // end of while 
   
   if(t_flag == true)
      cout << endl << "Successfully parsed <story>." << endl;  // print out the story was parsed without any error. 
}
// ---------------- Driver ---------------------------

// The final test driver to start the translator
// Done by:  Chihiro Nishijima
int main()
{
  //** opens the lexicon.txt file and reads it into Lexicon
  ifstream infile;
  infile.open("lexicon.txt");
  string jap, eng;
  while(infile >> jap >> eng)
  {
     vector<string> row;
     row.push_back(jap);
     row.push_back(eng);
     dictionary.push_back(row);
  }
  //** closes lexicon.txt 
  infile.close();
  
  // Turn On/Off tracing messages
  while(true)
  {  
     string trace_input;
     cout << "Turn On the tracing messages (y/n): ";
     cin >> trace_input;
     if (trace_input == "y") // turn on the tracing message
     {
        t_flag = true;
        break;
     }
     else 
     if(trace_input == "n") // turn off the tracing massage
     {
        t_flag = false;
        break;
     }
  }
  
  //** opens the output file translated.txt
  outfile.open("translated.txt", ofstream::out | ofstream::trunc);
  errorfile.open("errors.txt", ofstream::out | ofstream::trunc);
  
  cout << "Enter the input file name: ";
  cin >> filename;
  fin.open(filename.c_str());

  //** calls the <story> to start parsing
  story();
  //** closes the input file 
  fin.close();
  //** closes traslated.txt
  outfile.close();
  errorfile.close();
 
}// end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions