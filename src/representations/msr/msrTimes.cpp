/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <sstream>

#include "utilities.h"
#include "messagesHandling.h"

#include "msrTimes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrTimeItem msrTimeItem::create (
  int inputLineNumber)
{
  msrTimeItem* o =
    new msrTimeItem (
      inputLineNumber);
  assert (o!=0);

  return o;
}

msrTimeItem::msrTimeItem (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fTimeBeatValue = -1;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
      "Creating time item" <<
      ", line = " << inputLineNumber <<
      endl;
  }
#endif
}

msrTimeItem::~msrTimeItem ()
{}

bool msrTimeItem::isEqualTo (S_msrTimeItem otherTimeItem) const
{
  if (! otherTimeItem) {
    return false;
  }

  if (
    ! (
        fTimeBeatValue == otherTimeItem->fTimeBeatValue
            &&
        fTimeBeatsNumbersVector.size ()
          ==
        otherTimeItem->fTimeBeatsNumbersVector.size ()
      )
    ) {
    return false;
  }

  for (unsigned int i = 0; i < fTimeBeatsNumbersVector.size (); i++) {
    if (
      ! (
        fTimeBeatsNumbersVector [i]
          ==
        otherTimeItem->fTimeBeatsNumbersVector [i]
        )
      ) {
      return false;
    }
  } // for

  return true;
}

void msrTimeItem::appendBeatsNumber (int beatsNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
      "Appending beat number '" <<
      beatsNumber <<
      "' to time item '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  fTimeBeatsNumbersVector.insert (
    fTimeBeatsNumbersVector.end (),
    beatsNumber);
}

void msrTimeItem::setTimeBeatValue (int timeBeatValue)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
      "Setting beat value to '" <<
      timeBeatValue <<
      "' in time item '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  fTimeBeatValue = timeBeatValue;
}

int msrTimeItem::getTimeBeatsNumber () const
{
  int result = 0;

  for (unsigned int i = 0; i < fTimeBeatsNumbersVector.size (); i++) {
    result +=
      fTimeBeatsNumbersVector [i];
    } // for

  return result;
}

void msrTimeItem::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTimeItem::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTimeItem>*
    p =
      dynamic_cast<visitor<S_msrTimeItem>*> (v)) {
        S_msrTimeItem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTimeItem::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTimeItem::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTimeItem::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTimeItem>*
    p =
      dynamic_cast<visitor<S_msrTimeItem>*> (v)) {
        S_msrTimeItem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTimeItem::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTimeItem::browseData (basevisitor* v)
{}

string msrTimeItem::asString () const
{
  stringstream s;

  s <<
    "TimeItem ";

  int vectorSize =
    fTimeBeatsNumbersVector.size ();

  switch (vectorSize) {
    case 0:
    /* JMI
      msrInternalError (
        gGlobalOahOahGroup->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "time item beats numbers vector is empty");
        */
      s <<
        "beats numbers: none";
      break;

    case 1:
      s <<
        fTimeBeatsNumbersVector [0] << "/" << fTimeBeatValue;
      break;

    default:
      s <<
        "beats numbers: ";

      for (int i = 0; i < vectorSize; i++) {
        s <<
          fTimeBeatsNumbersVector [i];

        if (i != vectorSize - 1) {
          s <<
            " ";
        }
      } // for

      s <<
        ", beat value: " << fTimeBeatValue;
  } // switch

  s <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrTimeItem::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrTimeItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrTime msrTime::create (
  int               inputLineNumber,
  msrTimeSymbolKind timeSymbolKind)
{
  msrTime* o =
    new msrTime (
      inputLineNumber, timeSymbolKind);
  assert (o!=0);
  return o;
}

msrTime::msrTime (
  int               inputLineNumber,
  msrTimeSymbolKind timeSymbolKind)
    : msrMeasureElement (inputLineNumber)
{
  fTimeSymbolKind = timeSymbolKind;

  fTimeIsCompound = false;
}

bool msrTime::isEqualTo (S_msrTime otherTime) const
{
  if (! otherTime) {
    return false;
  }

  if (
    ! (
        fTimeSymbolKind == otherTime->fTimeSymbolKind
          &&
        fTimeIsCompound == otherTime->fTimeIsCompound
          &&
        fTimeItemsVector.size () == otherTime->fTimeItemsVector.size ()
      )
    ) {
    return false;
  }

  for (unsigned int i = 0; i < fTimeItemsVector.size (); i++) {
    if (
      ! (
        fTimeItemsVector [i]->isEqualTo (
          otherTime->fTimeItemsVector [i])
        )
      ) {
      return false;
    }
  } // for

  return true;
}

S_msrTime msrTime::createFourQuartersTime (
  int inputLineNumber)
{
  // create the time
  S_msrTime
    time =
      msrTime::create (
        inputLineNumber,
        kTimeSymbolNone);

  // create a four quarters time item
  S_msrTimeItem
    timeItem =
      msrTimeItem::create (
        inputLineNumber);

  timeItem->
    appendBeatsNumber (4);
  timeItem->
    setTimeBeatValue (4);

  // append the time item to the time
  time->
    appendTimeItem (timeItem);

  // return the time
  return time;
}

msrTime::~msrTime ()
{}

void msrTime::appendTimeItem (
  S_msrTimeItem timeItem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
      "Append item '" <<
      timeItem->asString () <<
      "' to time '" <<
      "JMI" <<
      "'" <<
      endl;
    }
#endif

  if (! fTimeItemsVector.size ()) {
    // this is the first item inserted
    if (
      timeItem->
        getTimeBeatsNumbersVector (). size ()
        >
      1) {
      // this time is compound
      fTimeIsCompound = true;
    }
  }

  else {
    // there are several time items, this time is compound
    fTimeIsCompound = true;
  }

  // append the time item to the vector
  fTimeItemsVector.insert (
    fTimeItemsVector.end (), timeItem);
}

rational msrTime::wholeNotesPerMeasure () const
{
  rational result (0, 1); // addition neutral element

  int vectorSize = fTimeItemsVector.size ();

  if (vectorSize) {
    /* JMI
    // start with first item
    result =
      rational (
        fTimeItemsVector [0]->getTimeBeatsNumber (),
        fTimeItemsVector [0]->getTimeBeatValue ());
*/

/* JMI
    gLogStream <<
      endl << endl <<
      "result1 = " <<
      result.getNumerator () <<
      "/" <<
      result.getDenominator () <<
      endl << endl;
*/

    // iterate over the others
    for (int i = 0; i < vectorSize; i++) {
      result +=
        rational (
          fTimeItemsVector [i]->getTimeBeatsNumber (),
          fTimeItemsVector [i]->getTimeBeatValue ());

/* JMI
      gLogStream <<
        endl << endl <<
        "result2 = " <<
        result.getNumerator () <<
        "/" <<
        result.getDenominator () <<
        endl << endl;
        */

    } // for
  }

  else {
    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      "time items vector is empty");
  }

  // rationalize result
  result.rationalise ();

  // return it
  return result;
}

void msrTime::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTime::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTime::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTime::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTime::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTime::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTime::browseData (basevisitor* v)
{}

string msrTime::asString () const
{
  stringstream s;

  s <<
    "[Time " <<
    ", timeSymbolKind: " <<
    timeSymbolKindAsString (fTimeSymbolKind) <<
    ", timeIsCompound: " <<
    booleanAsString (fTimeIsCompound) <<
    ", fTimeItemsVector.size(): " <<
    singularOrPlural (
      fTimeItemsVector.size (), "item", "items") <<
    singularOrPlural (
      fTimeItemsVector.size (), "item", "items") <<
    ", line " << fInputLineNumber;

  if (fTimeItemsVector.size ()) {
    s <<
      ", ";

    vector<S_msrTimeItem>::const_iterator
      iBegin = fTimeItemsVector.begin (),
      iEnd   = fTimeItemsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i)->asString ();
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }
  else {
    if (fTimeSymbolKind != kTimeSymbolSenzaMisura) {
      msrInternalError (
        gGlobalOahOahGroup->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "time  items vector is empty");
    }
  }

  s << "]";

  return s.str ();
}

string msrTime::asShortString () const
{
  /* JMI
  stringstream s;

  s <<
    "Time, " <<
    ", timeSymbolKind: " <<
    timeSymbolKindAsString (fTimeSymbolKind) <<
    ", compound " <<
    booleanAsString (
      fTimeIsCompound) <<
    ", " <<
    singularOrPlural (
      fTimeItemsVector.size (), "item", "items") <<
    ", line " << fInputLineNumber;

  return s.str ();
  */

  return asString ();
}

void msrTime::print (ostream& os) const
{
  os <<
    "Time" <<
    ", line"  << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 23;

  os << left <<
    setw (fieldWidth) <<
    "timeSymbolKind" << " : " <<
    timeSymbolKindAsString (fTimeSymbolKind) <<
    endl <<
    setw (fieldWidth) <<
    "timeIsCompound" << " : " <<
    booleanAsString (fTimeIsCompound) <<
    endl <<
    setw (fieldWidth) <<
    "timeItemsVector.size()" << " : " <<
    singularOrPlural (
      fTimeItemsVector.size (), "item", "items") <<
    endl <<
    setw (fieldWidth) <<
    "fTimeItemsVector" << " : ";

  if (fTimeItemsVector.size ()) {
    os << endl;

    gIndenter++;

    vector<S_msrTimeItem>::const_iterator
      iBegin = fTimeItemsVector.begin (),
      iEnd   = fTimeItemsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for

    gIndenter--;
  }

  else {
    os <<
      " none" <<
      endl;
  }

  gIndenter--;
}

void msrTime::printShort (ostream& os) const
{
  os <<
    "Time " <<
    ", timeSymbolKind: " <<
    timeSymbolKindAsString (fTimeSymbolKind) <<
    ", timeIsCompound: " <<
    booleanAsString (fTimeIsCompound) <<
    ", fTimeItemsVector.size(): " <<
    singularOrPlural (
      fTimeItemsVector.size (), "item", "items") <<
    singularOrPlural (
      fTimeItemsVector.size (), "item", "items") <<
    ", line " << fInputLineNumber;
}

ostream& operator<< (ostream& os, const S_msrTime& elt)
{
  elt->print (os);
  return os;
}


}