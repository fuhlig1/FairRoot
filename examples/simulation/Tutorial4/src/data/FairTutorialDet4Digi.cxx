#include "FairTutorialDet4Digi.h"

#include <sstream>  // for operator<<, basic_ostream, char_traits

using std::string;
using std::stringstream;

// --- String output
string FairTutorialDet4Digi::ToString() const
{
  stringstream ss;
  ss << "FairTutorialDet4Digi: address " << fAddress << " | row " << fRow 
     << " | column " << fColumn << " | charge " << fCharge << " | time " << fTime;
  return ss.str();
}

ClassImp(FairTutorialDet4Digi)
