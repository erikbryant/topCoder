#include <iostream>
#include <vector>
#include <string>
#include <list>

using namespace std;

class Segment
{
public:
  int major;
  int minor1;
  int minor2;
};

class PenLift
{
public:
  int numTimes( vector<string> segments, int n );
  PenLift( void );

private:
  typedef pair<int,int> point_t;

  void clearEdges( void );
  bool overlap( const Segment &s1, const Segment &s2 ) const;
  void add( list<Segment> &segList, Segment segment );
  int addVertex( int x, int y );
  void addEdge( int x1, int y1, int x2, int y2 );
  void collapse( void );
  void printMatrix( void ) const;

  vector< point_t > vertices;
  static const unsigned int maxVertices = 90;
  bool edges[maxVertices][maxVertices];
};

PenLift::PenLift( void ) :
  vertices()
{
  clearEdges();
}

void PenLift::clearEdges( void )
{
  unsigned int row = 0;
  unsigned int col = 0;

  for ( row=0; row<maxVertices; row++ )
    {
      for ( col=0; col<maxVertices; col++ )
	{
	  edges[row][col] = false;
	}
    }
}

int PenLift::addVertex( int x, int y )
{
  // Do we already know about this vertex?
  unsigned int i = 0;
  
  for ( i=0; i<vertices.size(); i++ )
    {
      if ( vertices[i].first == x && vertices[i].second == y )
	{
	  return i;
	}
    }

  // No? Add it to the end.
  point_t p(x,y);
  vertices.push_back( p );

  return vertices.size() - 1;
}

void PenLift::addEdge( int x1, int y1, int x2, int y2 )
{
  int v1 = addVertex( x1, y1 );
  int v2 = addVertex( x2, y2 );

  if ( edges[min(v1,v2)][max(v1,v2)] == true )
    {
      cout << "WARNING: There was already an edge here: " << v1 << " / " << v2 << endl;
    }

  edges[min(v1,v2)][max(v1,v2)] = true;
}

void PenLift::collapse( int n )
{
  bool changeMade = true;

  while ( changeMade )
    {
      changeMade = false;

      // Remove arcs to self
      // Remove vertices in the middle of segments
      // Remove singleton segments

      if ( n % 2 == 0 )
	{
	  // n is Even

	  // Remove lonely vertices
	}
      else
	{
	  // n is Odd

	  // Remove duplicate arcs
	}
    }
}

void PenLift::printMatrix( void ) const
{
  unsigned int row = 0;
  unsigned int col = 0;
  unsigned int maxVertex = vertices.size() - 1;

  cout << "     ";
  for ( col=0; col<=maxVertex; col++ )
    {
      int digit = (col / 10);
      if ( digit == 0 )
	{
	  cout << " ";
	}
      else
	{
	  cout << digit;
	}
    }
  cout << endl;
  cout << "     ";
  for ( col=0; col<=maxVertex; col++ )
    {
      int digit = col % 10;
      cout << digit;
    }
  cout << endl;

  for ( row=0; row<=maxVertex; row++ )
    {
      if ( row <  10 ) { cout << " "; }
      if ( row < 100 ) { cout << " "; }
      cout << row << ": ";
      for ( col=0; col<=maxVertex; col++ )
	{
	  cout << ( edges[row][col] ? "T" : "-" );
	}
      cout << endl;
    }
}

bool PenLift::overlap( const Segment &s1, const Segment &s2 ) const
{
  return s1.major == s2.major && s1.minor2 >= s2.minor1 && s1.minor1 <= s2.minor2;
}

void PenLift::add( list<Segment> &segList, Segment segment )
{
  list<Segment>::iterator it;
  bool found = false;

  for ( it=segList.begin(); it!=segList.end(); it++ )
    {
      if ( overlap( segment, *it ) )
	{
	  it->minor1 = min( segment.minor1, it->minor1 );
	  it->minor2 = max( segment.minor2, it->minor2 );
	  found = true;
	  break;
	}
    }

  if ( !found )
    {
      segList.push_back( segment );
      return;
    }

  while ( found )
    {
      found = false;
      // A segment has changed in length. Examine all pairs of segments.
      // If they now overlap, collapse them.
      for ( it=segList.begin(); it!=segList.end(); it++ )
	{
	  list<Segment>::iterator it2 = it;
	  it2++;
	  for ( ; it2!=segList.end(); it2++ )
	    {
	      if ( overlap( *it, *it2 ) )
		{
		  it->minor1 = min( it2->minor1, it->minor1 );
		  it->minor2 = max( it2->minor2, it->minor2 );
		  segList.erase( it2 );
		  found = true;
		  break;
		}
	    }
	  if ( found ) { break; }
	}
    }
}

int PenLift::numTimes( vector<string> segments, int n )
{
  // This is a new run. Erase any old state.
  clearEdges();
  vertices.clear();

  vector<string>::iterator it;
  list<Segment> hList;
  list<Segment> vList;

  // We only care whether n is odd or even. Even if n is 9000 we
  // don't have to trace 9000 times; we just have to trace twice.
  n = n % 2 + 1;

  for ( it=segments.begin(); it!=segments.end(); it++ )
    {
      Segment s;

      // Parse the input string into two, separate points
      string points = *it;
      const char * ptr = points.c_str();
      int x1 = atoi( ptr );
      while ( *ptr++ != ' ' );
      int y1 = atoi( ptr );
      while ( *ptr++ != ' ' );
      int x2 = atoi( ptr );
      while ( *ptr++ != ' ' );
      int y2 = atoi( ptr );

      if ( y1 == y2 )
	{
	  // Horizontal
	  s.major = y1;
	  s.minor1 = x1;
	  s.minor2 = x2;
	  add( hList, s );
	}
      else
	{
	  // Vertical
	  s.major = x1;
	  s.minor1 = y1;
	  s.minor2 = y2;
	  add( vList, s );
	}
    }

  // Store the edges in the adjacency matrix
  list<Segment>::iterator hit;
  for ( hit=hList.begin(); hit!=hList.end(); hit++ )
    {
      int y1 = hit->major;
      int y2 = hit->major;
      int x1 = hit->minor1;
      int x2 = hit->minor2;
      addEdge( x1, y1, x2, y2 );
    }
  list<Segment>::iterator vit;
  for ( vit=vList.begin(); vit!=vList.end(); vit++ )
    {
      int x1 = vit->major;
      int x2 = vit->major;
      int y1 = vit->minor1;
      int y2 = vit->minor2;
      addEdge( x1, y1, x2, y2 );
    }

  // Collapse the adjacency matrix
  printMatrix();
  collapse();
  printMatrix();

  return n;
}


int main( int argc, char *argv[] )
{
  PenLift p;
  vector<string> segments;

  // Example 0
  segments.clear();
  segments.push_back( "-10 0 10 0" );
  segments.push_back( "0 -10 0 10" );
  cout << "Pen lift count = " << p.numTimes( segments, 1 ) << " (1)" << endl << endl;

  // Example 1
  segments.clear();
  segments.push_back( "-10 0 0 0" );
  segments.push_back( "0 0 10 0" );
  segments.push_back( "0 -10 0 0" );
  segments.push_back( "0 0 0 10" );
  cout << "Pen lift count = " << p.numTimes( segments, 1 ) << " (1)" << endl << endl;

  // Example 2
  segments.clear();
  segments.push_back( "-10 0 0 0" );
  segments.push_back( "0 0 10 0" );
  segments.push_back( "0 -10 0 0" );
  segments.push_back( "0 0 0 10" );
  cout << "Pen lift count = " << p.numTimes( segments, 4 ) << " (0)" << endl << endl;

  // Example 3
  segments.clear();
  segments.push_back( "0 0 1 0" );
  segments.push_back( "2 0 4 0" );
  segments.push_back( "5 0 8 0" );
  segments.push_back( "9 0 13 0" );
  segments.push_back( "0 1 1 1" );
  segments.push_back( "2 1 4 1" );
  segments.push_back( "5 1 8 1" );
  segments.push_back( "9 1 13 1" );
  segments.push_back( "0 0 0 1" );
  segments.push_back( "1 0 1 1" );
  segments.push_back( "2 0 2 1" );
  segments.push_back( "3 0 3 1" );
  segments.push_back( "4 0 4 1" );
  segments.push_back( "5 0 5 1" );
  segments.push_back( "6 0 6 1" );
  segments.push_back( "7 0 7 1" );
  segments.push_back( "8 0 8 1" );
  segments.push_back( "9 0 9 1" );
  segments.push_back( "10 0 10 1" );
  segments.push_back( "11 0 11 1" );
  segments.push_back( "12 0 12 1" );
  segments.push_back( "13 0 13 1" );
  cout << "Pen lift count = " << p.numTimes( segments, 1 ) << " (6)" << endl << endl;

  // Example 4
  segments.clear();
  segments.push_back( "-2 6 -2 1" );
  segments.push_back( "2 6 2 1" );
  segments.push_back( "6 -2 1 -2" );
  segments.push_back( "6 2 1 2" );
  segments.push_back( "-2 5 -2 -1" );
  segments.push_back( "2 5 2 -1" );
  segments.push_back( "5 -2 -1 -2" );
  segments.push_back( "5 2 -1 2" );
  segments.push_back( "-2 1 -2 -5" );
  segments.push_back( "2 1 2 -5" );
  segments.push_back( "1 -2 -5 -2" );
  segments.push_back( "1 2 -5 2" );
  segments.push_back( "-2 -1 -2 -6" );
  segments.push_back( "2 -1 2 -6" );
  segments.push_back( "-1 -2 -6 -2" );
  segments.push_back( "-1 2 -6 2" );
  cout << "Pen lift count = " << p.numTimes( segments, 5 ) << " (3)" << endl << endl;

  // Example 5
  segments.clear();
  segments.push_back( "-252927 -1000000 -252927 549481" );
  segments.push_back( "628981 580961 -971965 580961" );
  segments.push_back( "159038 -171934 159038 -420875" );
  segments.push_back( "159038 923907 159038 418077" );
  segments.push_back( "1000000 1000000 -909294 1000000" );
  segments.push_back( "1000000 -420875 1000000 66849" );
  segments.push_back( "1000000 -171934 628981 -171934" );
  segments.push_back( "411096 66849 411096 -420875" );
  segments.push_back( "-1000000 -420875 -396104 -420875" );
  segments.push_back( "1000000 1000000 159038 1000000" );
  segments.push_back( "411096 66849 411096 521448" );
  segments.push_back( "-971965 580961 -909294 580961" );
  segments.push_back( "159038 66849 159038 -1000000" );
  segments.push_back( "-971965 1000000 725240 1000000" );
  segments.push_back( "-396104 -420875 -396104 -171934" );
  segments.push_back( "-909294 521448 628981 521448" );
  segments.push_back( "-909294 1000000 -909294 -1000000" );
  segments.push_back( "628981 1000000 -909294 1000000" );
  segments.push_back( "628981 418077 -396104 418077" );
  segments.push_back( "-971965 -420875 159038 -420875" );
  segments.push_back( "1000000 -1000000 -396104 -1000000" );
  segments.push_back( "-971965 66849 159038 66849" );
  segments.push_back( "-909294 418077 1000000 418077" );
  segments.push_back( "-909294 418077 411096 418077" );
  segments.push_back( "725240 521448 725240 418077" );
  segments.push_back( "-252927 -1000000 -1000000 -1000000" );
  segments.push_back( "411096 549481 -1000000 549481" );
  segments.push_back( "628981 -171934 628981 923907" );
  segments.push_back( "-1000000 66849 -1000000 521448" );
  segments.push_back( "-396104 66849 -396104 1000000" );
  segments.push_back( "628981 -1000000 628981 521448" );
  segments.push_back( "-971965 521448 -396104 521448" );
  segments.push_back( "-1000000 418077 1000000 418077" );
  segments.push_back( "-1000000 521448 -252927 521448" );
  segments.push_back( "725240 -420875 725240 -1000000" );
  segments.push_back( "-1000000 549481 -1000000 -420875" );
  segments.push_back( "159038 521448 -396104 521448" );
  segments.push_back( "-1000000 521448 -252927 521448" );
  segments.push_back( "628981 580961 628981 549481" );
  segments.push_back( "628981 -1000000 628981 521448" );
  segments.push_back( "1000000 66849 1000000 -171934" );
  segments.push_back( "-396104 66849 159038 66849" );
  segments.push_back( "1000000 66849 -396104 66849" );
  segments.push_back( "628981 1000000 628981 521448" );
  segments.push_back( "-252927 923907 -252927 580961" );
  segments.push_back( "1000000 549481 -971965 549481" );
  segments.push_back( "-909294 66849 628981 66849" );
  segments.push_back( "-252927 418077 628981 418077" );
  segments.push_back( "159038 -171934 -909294 -171934" );
  segments.push_back( "-252927 549481 159038 549481" );
  cout << "Pen lift count = " << p.numTimes( segments, 824759 ) << " (19)" << endl << endl;
}
