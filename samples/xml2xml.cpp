/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#include <iomanip>      // setw()), set::precision(), ...

#ifndef WIN32
#include <signal.h>
#endif

#include <regex>

#include "version.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "oahBasicTypes.h"
#include "generalOah.h"
#include "xml2xmlOah.h"

#include "musicxml2musicxml.h"

#include "xml2xmlOahDualHandler.h"


using namespace std;

using namespace MusicXML2;


//_______________________________________________________________________________
#ifndef WIN32

static void _sigaction(int signal, siginfo_t *si, void *arg)
{
    cerr << "Signal #" << signal << " catched!" << endl;
    exit(-2);
}

static void catchsigs()
{
	struct sigaction sa;

    memset (&sa, 0, sizeof(struct sigaction));

    sigemptyset (&sa.sa_mask);

    sa.sa_sigaction = _sigaction;
    sa.sa_flags     = SA_SIGINFO;

    sigaction (SIGSEGV, &sa, NULL);
    sigaction (SIGILL, &sa, NULL);
    sigaction (SIGFPE, &sa, NULL);
}

#else
static void catchsigs()	{}
#endif

//_______________________________________________________________________________
int main (int argc, char *argv[])
{
  // create the signals
  // ------------------------------------------------------

	catchsigs();

  // create the OAH dual handler
  // ------------------------------------------------------
  S_xml2xmlOahDualHandler dualHandler;

  try {
    dualHandler =
      xml2xmlOahDualHandler::create (
        argv [0],
        gOutputOstream);
  }
  catch (msrOahException& e) {
    return kInvalidOption;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  // analyze the command line options and arguments
  // ------------------------------------------------------

  try {
    oahHandler::oahHelpOptionsHaveBeenUsedKind
      helpOptionsHaveBeenUsedKind =
        dualHandler->
          applyOptionsAndArgumentsFromArgcAndArgv (
            argc, argv);

    switch (helpOptionsHaveBeenUsedKind) {
      case oahHandler::kHelpOptionsHaveBeenUsedYes:
        return kNoErr;
        break;
      case oahHandler::kHelpOptionsHaveBeenUsedNo:
        // let's go ahead!
        break;
    } // switch
  }
  catch (msrOahException& e) {
    return kInvalidOption;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  string
    inputSourceName =
      gOahOah->fInputSourceName;

  string
    outputFileName =
      gXml2xmlOah->fMusicXMLOutputFileName;

  int
    outputFileNameSize =
      outputFileName.size ();

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGeneralOah->fQuiet) {
    // disable all trace and display options
    /* JMI
    dualHandler->
      enforceOahHandlerQuietness ();
      */
  }

  // welcome message
  // ------------------------------------------------------

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    gLogOstream <<
      "This is xml2xml " << currentVersionNumber () <<
      " from libmusicxml2 v" << musicxmllibVersionStr () <<
      endl;

    gLogOstream <<
      "Launching conversion of ";

    if (inputSourceName == "-")
      gLogOstream <<
        "standard input";
    else
      gLogOstream <<
        "\"" << inputSourceName << "\"";

    gLogOstream <<
      " back to MusicXML" <<
      endl;

    gLogOstream <<
      "Time is " << gGeneralOah->fTranslationDateFull <<
      endl;

    gLogOstream <<
      "MusicXML code will be written to ";
    if (outputFileNameSize) {
      gLogOstream <<
        outputFileName;
    }
    else {
      gLogOstream <<
        "standard output";
    }
    gLogOstream <<
      endl <<
      endl;

    gLogOstream <<
      "The command line is:" <<
      endl;

    gIndenter++;

    gLogOstream <<
      dualHandler->
        commandLineWithShortNamesAsString () <<
      endl;

    gIndenter--;
    gLogOstream <<
      "or:" <<
      endl;
    gIndenter++;

    gLogOstream <<
      dualHandler->
        commandLineWithLongNamesAsString () <<
      endl <<
      endl;

    gIndenter--;
  }
#endif

  // acknoledge end of command line analysis
  // ------------------------------------------------------

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    gLogOstream <<
      "The command line options and arguments have been analyzed" <<
      endl;
  }
#endif

  // sanity check
  // ------------------------------------------------------

  if (inputSourceName == outputFileName) {
    gLogOstream <<
      "\"" << inputSourceName << "\" is both the input and output file name" <<
      endl;

    return 1;
  }

  // do the translation
  // ------------------------------------------------------

  xmlErr err =
    convertMusicXMLBackToMusicXML (
      inputSourceName,
      outputFileName);

  // display the input line numbers for which messages have been issued
  // ------------------------------------------------------

  displayWarningsAndErrorsInputLineNumbers ();

  // print timing information
  // ------------------------------------------------------

  if (gGeneralOah->fDisplayCPUusage)
    timing::gTiming.print (
      gLogOstream);

  // check indentation
  // ------------------------------------------------------
  if (gIndenter != 0) {
    gLogOstream <<
      "### gIndenter final value: "<< gIndenter.getIndent () << " ###" <<
      endl <<
      endl;
  }

  // over!
  // ------------------------------------------------------

  if (err != kNoErr) {
    gLogOstream <<
      "### Conversion from LPSR to LilyPond code failed ###" <<
      endl <<
      endl;

    return 1;
  }

  return 0;
}
