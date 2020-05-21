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

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "versions.h"

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "lpsr2LilypondOah.h"

#include "msr2LpsrInterface.h"

#include "msr2LpsrTranslator.h"

using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_lpsrScore buildLpsrScoreFromMsrScore (
  const S_msrScore mScore,
  S_msrOah         msrOpts,
  S_lpsrOah        lpsrOpts,
  ostream&         logOstream)
{
  // sanity check
  msrAssert (
    mScore != 0,
    "mScore is null");

  clock_t startClock = clock ();

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    string separator =
      "%--------------------------------------------------------------";

    logOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 3: translating the MSR into a LPSR" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an msr2LpsrTranslator
  msr2LpsrTranslator
    translator (
      logOstream,
      mScore);

  // build the LPSR score
  translator.buildLpsrScoreFromMsrScore ();

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 3",
    "translate MSR to LPSR",
    timingItem::kMandatory,
    startClock,
    endClock);

  // get the LPSR score
  S_lpsrScore
    lpScore =
      translator.getLpsrScore ();

  // return it
  return lpScore;
}

//_______________________________________________________________________________
void displayLpsrScore (
  const S_lpsrScore lpScore,
  S_msrOah          msrOpts,
  S_lpsrOah         lpsrOpts,
  ostream&          logOstream)
{
  // sanity check
  msrAssert (
    lpScore != 0,
    "lpScore is null");

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  logOstream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the LPSR as text" <<
    endl <<
    separator <<
    endl <<
    endl <<
    lpScore <<
    separator <<
    endl <<
    endl;

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display the LPSR",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayLpsrScore_OptionalPass (
  S_lpsrScore lpScore,
  S_msrOah    msrOpts,
  S_lpsrOah   lpsrOpts)
{
  // display it
  displayLpsrScore (
    lpScore,
    msrOpts,
    lpsrOpts,
    gLogOstream);

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after LPSR score display: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
S_lpsrScore convertMsrScoreToLpsrScore (
  S_msrScore mScore,
  string     passNumber)
{
  S_lpsrScore lpScore;

  if (gLpsr2LilypondOah->fNoLilypondCode) {
    gLogOstream <<
      "Option '-nolpc, -no-lilypond-code' is set, no LPSR is created" <<
      endl <<
      endl;
  }
  else {
    lpScore =
      buildLpsrScoreFromMsrScore (
        mScore,
        gMsrOah,
        gLpsrOah,
        gLogOstream);
  }

  if (gIndenter != 0) {
    if (! gGeneralOah->fQuiet) {
      stringstream s;

      s <<
        "gIndenter value after pass 3: "<<
        gIndenter.getIndent ();

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! lpScore && ! gLpsr2LilypondOah->fNoLilypondCode) {
    gLogOstream <<
      "### Conversion from MSR to LPSR failed ###" <<
      endl <<
      endl;

    exit (2);
  }

  return lpScore;
}


}