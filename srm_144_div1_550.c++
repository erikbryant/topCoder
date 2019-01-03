#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

class Lottery
{
public:
  vector<string> sortByOdds( const vector<string> rules );

private:
  unsigned long long int odds( int choices, int blanks, bool sorted, bool unique );
  unsigned long long int power( unsigned long long int base, unsigned long long int exponent );
  unsigned long long int factorial( int n, int stop = 1 );
  unsigned long long int permutations( int n, int r );
  unsigned long long int combinations( int n, int r );
  pair<unsigned long long int, string> parseRecord( const string &ticket );
};

unsigned long long int Lottery::power( unsigned long long int base, unsigned long long int exponent )
{
  unsigned long long int result = 1;

  while ( exponent != 0 )
  {
    result *= base;
    exponent--;
  }
  
  return result;
}

//
// Compute the factorial of a given input.
// Optionally, stop counting down before a
// given point. E.g.,
//
//   factorial(5)   = 5 * 4 * 3 * 2
//   factorial(5,3) = 5 * 4
//
unsigned long long int Lottery::factorial( int n, int stop )
{
  unsigned long long int result = 1;

  for ( ; n>stop; n-- )
    {
      result *= n;
    }

  return result;
}

//
// nPr = n! / (n-r)!
//     = n * (n-1) * (n-2) * .. * (n-(r-1))
//
unsigned long long int Lottery::permutations( int n, int r )
{
  return factorial( n, n-r );
}

//
// nCr = n! / r!(n-r)!
//     = r >= (n-r) : factorial( n, r )   / (n-r)!
//       r <  (n-r) : factorial( n, n-r ) / r!
//
// Note: This can also be written as permutation(n, r) / r!
//
unsigned long long int Lottery::combinations( int n, int r )
{
  unsigned long long int result = 0;

  if ( r >= (n-r) )
    {
      result = factorial( n, r ) / factorial( n-r );
    }
  else
    {
      result = factorial( n, n-r ) / factorial( r );
    }

  return result;
}

unsigned long long int Lottery::odds( int choices, int blanks, bool sorted, bool unique )
{
  unsigned long long int total = 0;

  if ( sorted )
    {
      if ( unique )
	{
	  total = this->combinations( choices, blanks );
	}
      else
	{
	  // This equation copied from: http://codeherb.com/Lottery/
	  total = factorial( choices + blanks - 1, choices - 1 ) / factorial( blanks );
	}
    }
  else
    {
      if ( unique )
	{
	  total = this->permutations( choices, blanks );
	}
      else
	{
	  total = this->power( choices, blanks );
	}
    }

  return total;
}

pair<unsigned long long int, string> Lottery::parseRecord( const string &record )
{
  string name;
  string ticket = record;
  int choices = 0;
  int blanks = 0;
  bool sorted = false;
  bool unique = false;
  unsigned long long int combinations = 0;
  string::size_type pos = ticket.find( ':', 0 );

  name = ticket.substr( 0, pos );
  pos += 2;
  ticket = ticket.substr( pos, ticket.length() );
  choices = atoi( ticket.c_str() );
  pos = ticket.find( ' ', 0 ) + 1;
  ticket = ticket.substr( pos, ticket.length() );
  blanks = atoi( ticket.c_str() );
  pos = ticket.find( ' ', 0 ) + 1;
  while ( ticket[pos] == ' ' )
    {
      pos++;
    }
  sorted = ticket[pos] == 'T';
  unique = ticket[pos+2] == 'T';
  combinations = odds( choices, blanks, sorted, unique );

  cout << name << ": " << choices << " " << blanks << " " << (sorted ? 'T' : 'F') << " " << (unique ? 'T' : 'F') << "  -> " << combinations << endl;

  pair<unsigned long long int, string> entry( combinations, name );

  return entry;
}

vector<string> Lottery::sortByOdds( const vector<string> rules )
{
  vector<string> result;
  list< pair<unsigned long long int, string> > games;
  vector<string>::const_iterator it;

  for ( it=rules.begin(); it!=rules.end(); it++ )
    {
      pair<unsigned long long int, string> entry = parseRecord( *it );
      games.push_back( entry );
    }

  games.sort();

  list< pair<unsigned long long int, string> >::iterator lit;
  for ( lit=games.begin(); lit!=games.end(); lit++ )
    {
      result.push_back( (*lit).second );
    }

  return result;
}

int main( int argc, char *argv[] )
{
  Lottery lottery;
  vector<string> rules;
  vector<string> result;
  vector<string>::const_iterator it;

  cout << "Example 1: " << endl;
  rules.clear();
  rules.push_back( "PICK ANY TWO: 10 2 F F" );
  rules.push_back( "PICK TWO IN ORDER: 10 2 T F" );
  rules.push_back( "PICK TWO DIFFERENT: 10 2 F T" );
  rules.push_back( "PICK TWO LIMITED: 10 2 T T" );
  result = lottery.sortByOdds( rules );
  for ( it=result.begin(); it!=result.end(); it++ )
    {
      cout << *it << ", ";
    }
  cout << endl;

  cout << endl << "Example 2: " << endl;
  rules.clear();
  rules.push_back( "INDIGO: 93 8 T F" );
  rules.push_back( "ORANGE: 29 8 F T" );
  rules.push_back( "VIOLET: 76 6 F F" );
  rules.push_back( "BLUE: 100 8 T T" );
  rules.push_back( "RED: 99 8 T T" );
  rules.push_back( "GREEN: 78 6 F T" );
  rules.push_back( "YELLOW: 75 6 F F" );
  result = lottery.sortByOdds( rules );
  for ( it=result.begin(); it!=result.end(); it++ )
    {
      cout << *it << ", ";
    }
  cout << endl;

  cout << endl << "Example 3: " << endl;
  rules.clear();
  result = lottery.sortByOdds( rules );
  for ( it=result.begin(); it!=result.end(); it++ )
    {
      cout << *it << ", ";
    }
  cout << endl;

  cout << endl << "Example 4: " << endl;
  rules.clear();
  rules.push_back( "6/49: 49 6 T T" );
  rules.push_back( "6/49: 49 6 T F" );
  rules.push_back( "6/49: 49 6 F T" );
  rules.push_back( "6/49: 49 6 F F" );
  result = lottery.sortByOdds( rules );
  for ( it=result.begin(); it!=result.end(); it++ )
    {
      cout << *it << ", ";
    }
  cout << endl;
}
