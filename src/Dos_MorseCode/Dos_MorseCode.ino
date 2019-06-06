/*
<script src="https://gist.github.com/madc/4474559.js"></script>
  
Simple morse en- & decoding for Arduino (including LED Example)Build after the specs on http://en.wikipedia.org/wiki/Morse_code
 LICENSE
Copyright (c) 2015 Matthias Esterl


Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 morse.ino
*/


//Define the LED Pin
#define PIN_OUT        5
//Define unit length in ms
#define UNIT_LENGTH    250

//Build a struct with the morse code mapping
static const struct {const char letter, *code;} MorseMap[] =
{
  { 'A', ".-" },
  { 'B', "-..." },
  { 'C', "-.-." },
  { 'D', "-.." },
  { 'E', "." },
  { 'F', "..-." },
  { 'G', "--." },
  { 'H', "...." },
  { 'I', ".." },
  { 'J', ".---" },
  { 'K', ".-.-" },
  { 'L', ".-.." },
  { 'M', "--" },
  { 'N', "-." },
  { 'O', "---" },
  { 'P', ".--." },
  { 'Q', "--.-" },
  { 'R', ".-." },
  { 'S', "..." },
  { 'T', "-" },
  { 'U', "..-" },
  { 'V', "...-" },
  { 'W', ".--" },
  { 'X', "-..-" },
  { 'Y', "-.--" },
  { 'Z', "--.." },
  { ' ', "     " }, //Gap between word, seven units 
    
  { '1', ".----" },
  { '2', "..---" },
  { '3', "...--" },
  { '4', "....-" },
  { '5', "....." },
  { '6', "-...." },
  { '7', "--..." },
  { '8', "---.." },
  { '9', "----." },
  { '0', "-----" },
    
  { '.', "·–·–·–" },
  { ',', "--..--" },
  { '?', "..--.." },
  { '!', "-.-.--" },
  { ':', "---..." },
  { ';', "-.-.-." },
  { '(', "-.--." },
  { ')', "-.--.-" },
  { '"', ".-..-." },
  { '@', ".--.-." },
  { '&', ".-..." },
};

void setup()
{
  pinMode( PIN_OUT, OUTPUT );
  digitalWrite( PIN_OUT, LOW );
  pinMode( 4, OUTPUT );
  digitalWrite( 4, LOW );
  pinMode( 12, OUTPUT );
  digitalWrite( 12, LOW );
  pinMode( 13, OUTPUT );
  digitalWrite( 13, LOW );
}

void loop()
{
  String morseWord = encode( "SOS " );
    
  for(int i=0; i<=morseWord.length(); i++)
  {
    switch( morseWord[i] )
    {
      case '.': //dit
        digitalWrite( PIN_OUT, HIGH );
        delay( UNIT_LENGTH );
        digitalWrite( PIN_OUT, LOW );
        delay( UNIT_LENGTH );
          
        break;

      case '-': //dah
        digitalWrite( PIN_OUT, HIGH );
        delay( UNIT_LENGTH*3 );
        digitalWrite( PIN_OUT, LOW );
        delay( UNIT_LENGTH );
          
        break;

      case ' ': //gap
        delay( UNIT_LENGTH );
    }

  }
  digitalWrite( 12, LOW );
  delay( UNIT_LENGTH );
  digitalWrite( 12, HIGH );
  delay( UNIT_LENGTH * 3 );
    

}

String encode(const char *string)
{
  size_t i, j;
  String morseWord = "";
  
  for( i = 0; string[i]; ++i )
  {
    for( j = 0; j < sizeof MorseMap / sizeof *MorseMap; ++j )
    {
      if( toupper(string[i]) == MorseMap[j].letter )
      {
        morseWord += MorseMap[j].code;
        break;
      }
    }
    morseWord += " "; //Add tailing space to seperate the chars
  }

  return morseWord;  
}

String decode(String morse)
{
  String msg = "";
  
  int lastPos = 0;
  int pos = morse.indexOf(' ');
  while( lastPos <= morse.lastIndexOf(' ') )
  {    
    for( int i = 0; i < sizeof MorseMap / sizeof *MorseMap; ++i )
    {
      if( morse.substring(lastPos, pos) == MorseMap[i].code )
      {
        msg += MorseMap[i].letter;
      }
    }

    lastPos = pos+1;
    pos = morse.indexOf(' ', lastPos);
    
    // Handle white-spaces between words (7 spaces)
    while( morse[lastPos] == ' ' && morse[pos+1] == ' ' )
    {
      pos ++;
    }
  }

  return msg;
}

/*
 @kristianlm
kristianlm commented Oct 23, 2016 • 
I believe your K is wrong: it should be -.- (see wikipedia)

 @5tevan
5tevan commented Feb 16, 2017 • 
The encoding for the period character { '.', "·–·–·–" } uses different dots and dashes than the other encodings.

 @tonytozoo
tonytozoo commented Feb 16, 2017
What if I want to translate 2400 characters, I've try in many different ways but none of them it works.
It works until 1000, any Idea why and how to solve this?

 @ai8n
ai8n commented Jan 25, 2018
Newbie comment: Could you add more comments to this code? Ignorant minds want to know. :)

 @pharrisuk
pharrisuk commented Feb 17, 2018
@a18n: One thing you might want to understand, as a newbie, is that, he's building (declaring, and initialising) an array of structures at the top, not just a struct as the comment implies.

 @azcharlie2009
azcharlie2009 commented Mar 31, 2018
Does this work? I loaded it into my Teensy 3.6 and it does nothing.

 @azcharlie2009
azcharlie2009 commented Mar 31, 2018
Sorry, my mistake. Needed to change PIN_OUT to 13 on Teensy

 @ghost
ghost commented Apr 9, 2018
Does anyone have the schematics for it?

 @Erhannis
Erhannis commented Jun 6, 2018
Most everything here appears to be compliant with the spec described on Wikipedia. However, I believe to get "The letters of a word are separated by a space equal to three dots (one dash)", it should be morseWord += " "; //Add tailing space to seperate the chars, with two spaces.

 @Erhannis
Erhannis commented Jun 6, 2018
I fixed the mentioned bugs, including (in my tests) the out-of-memory bug I think @tonytozoo was having.
https://gist.github.com/Erhannis/17de678c9b6ac4d75ffa585156f513b8
I also added another space between chars, for spec compliance. (Should be three dots worth of space between chars, rather than two.)

 @mlh1407
mlh1407 commented Jun 17, 2018
will this code auto detect the users speed - im doing a project with kids and we have som trouble with the fact that kids morse with different speed -

 @riosrodriguez
   
 
 
Leave a comment
Attach files by dragging & dropping, selecting or pasting them.

 Styling with Markdown is supported
© 2019 GitHub, Inc.
Terms
Privacy
Security
Status
Help
Contact GitHub
Pricing
API
Training
Blog
About
*/
