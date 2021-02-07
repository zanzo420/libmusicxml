/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef MSVC
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"
#include "musicxmlOah.h"

#include "xml2gmnInsiderHandler.h"
#include "xml2gmnRegularHandler.h"

#include "musicxml2mxmlTreeInterface.h"
#include "mxmlTree2msrSkeletonBuilderInterface.h"
#include "mxmlTree2msrTranslatorInterface.h"

#include "msr2msrInterface.h"
#include "msr2mxmlTreeInterface.h"
#include "mxmlTree2guidoTranlatorInterface.h"

#include "mxmlTree.h"
#include "musicxml2guido.h"


using namespace std;

namespace MusicXML2
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/

//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
static xmlErr xmlFile2guidoWithHandler (
  SXMLFile&     xmlfile,
  std::ostream& out,
  std::ostream& err,
  S_oahHandler  handler)
{
  if (gGlobalMxmlTreeOahGroup->getTraceMusicXMLTree ()) {
    gLogStream <<
      endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl <<
      "xmlFile2guidoWithHandler(), xmlfile contains:" <<
      endl << endl;

    ++gIndenter;

    xmlfile->print (gLogStream);
    gLogStream << endl << endl;

    --gIndenter;

    gLogStream <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl << endl;
  }

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalGeneralOahGroup->getQuiet ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // get the original mxmlTree
  // ------------------------------------------------------

  Sxmlelement
    originalMxmlTree =
      xmlfile->elements ();

  // the MSR score
  // ------------------------------------------------------

  S_msrScore firstMsrScore;

  // create the skeleton of the first MSR from the originalMxmlTree (pass 2a)
  // ------------------------------------------------------

  try {
    firstMsrScore =
      convertMxmlTreeToMsrScoreSkeleton (
        originalMxmlTree,
        gGlobalMsrOahGroup,
        "Pass 2a",
        "Create the skeleton of the first MSR from the originalMxmlTree");
  }
  catch (msgMxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2gmnInsiderOahGroup->getQuitAfterPass2a ()) {
    err <<
      endl <<
      "Quitting after pass 2a as requested" <<
      endl;

    return kNoErr;
  }

  // populate the MSR skeleton from MusicXML data (pass 2b)
  // ------------------------------------------------------

  try {
    populateMsrSkeletonFromMxmlTree (
      originalMxmlTree,
      firstMsrScore,
        "Pass 2b",
        "Populate the MSR skeleton from MusicXML data");
  }
  catch (msgMxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2gmnInsiderOahGroup->getQuitAfterPass2b ()) {
    err <<
      endl <<
      "Quitting after pass 2b as requested" <<
      endl;

    return kNoErr;
  }

  // convert the first MSR score into a second MSR (pass 3)
  // ------------------------------------------------------

  S_msrScore secondMsrScore;

  try {
    secondMsrScore =
      convertMsrScoreToMsrScore (
        firstMsrScore,
        gGlobalMsrOahGroup,
        gGlobalMsr2msrOahGroup,
        "Pass 3",
        "Convert the first MSR into a second MSR");
  }
  catch (msgMxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // convert the second MSR into a second mxmlTree (pass 4)
  // ------------------------------------------------------

  Sxmlelement secondMxmlTree;

  try {
    secondMxmlTree =
      convertMsrScoreToMxmltree (
        secondMsrScore,
        gGlobalMsrOahGroup,
        "Pass 4",
        "Convert the second MSR into a second mxmlTree",
        timingItem::kMandatory);
  }
  catch (msgMxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // generate Guido from the second mxmlTree (pass 5)
  // ------------------------------------------------------

  string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

  try {
    generateGuidoFromMxmlTree (
      secondMxmlTree,
      outputFileName,
      err,
      "Pass 5",
      "Convert  the second mxmlTree into Guido text");
  }
  catch (msgMxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  return kNoErr;
}

//_______________________________________________________________________________
static xmlErr xmlFile2guidoWithOptionsVector (
  SXMLFile&            xmlfile,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
	Sxmlelement st;

	if (xmlfile) {
	  st = xmlfile->elements ();

    if (st) {
      if (st->getName () == "score-timewise")
        return kUnsupported;
    }
  }

	else {
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  err <<
    "xmlFile2guido(), xmlfile is NULL" <<
    endl;
#endif
#endif

    return kInvalidFile;
	}

  // the fake executable name
  // ------------------------------------------------------

  string fakeExecutableName = "xml2gmn";

  // reset the global indenter
  // ------------------------------------------------------

  gIndenter.resetToZero ();

  // create the global log indented output stream
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (out, err);

	// print the options vector
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
      displayOptionsVector (options, err);
#endif
#endif

  // are there 'insider' and/or 'regular' options present?
  // ------------------------------------------------------

  bool insiderOptions = false;
  bool regularOptions = false;

	for (unsigned int i = 1; i < options.size (); ++i) {
	  string optionName = options [i].first;

		if (optionName == K_INSIDER_OPTION_NAME) {
		  insiderOptions = true;
		}
		if (optionName == K_REGULAR_OPTION_NAME) {
		  regularOptions = true;
		}
	} // for

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "xmlFile2guido()" <<
    ", insiderOptions: " << booleanAsString (insiderOptions) <<
    ", regularOptions: " << booleanAsString (regularOptions) <<
    endl;
#endif
#endif

  if (insiderOptions && regularOptions) {
    stringstream s;

    s <<
      "options '-insider' and '-regular' cannot be used together";

    oahError (s.str ());
  }

  // here, at most one of insiderOptions and regularOptions is true

  // create an xml2gmn insider OAH handler
  // ------------------------------------------------------

  // create an insider xml2gmn OAH handler
  S_xml2gmnInsiderHandler
    insiderOahHandler =
      xml2gmnInsiderHandler::create (
        fakeExecutableName,
        fakeExecutableName + " insider OAH handler with options vector");

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOptions) {
    // use the insider xml2gmn OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular xml2gmn OAH handler
    handler =
      xml2gmnRegularHandler::create (
        fakeExecutableName,
        fakeExecutableName + " regular OAH handler",
        insiderOahHandler);
  }

  // handle the command line options and arguments
  // ------------------------------------------------------

  try {
    // handle the options from the options vector
    oahElementHelpOnlyKind
      helpOnlyKind =
        handler->
          handleOptionsFromOptionsVector (
            fakeExecutableName,
            options);

    // have help options been used?
    switch (helpOnlyKind) {
      case kElementHelpOnlyYes:
        return kNoErr; // quit now
        break;
      case kElementHelpOnlyNo:
        // go ahead
        break;
    } // switch
  }
  catch (msgOahException& e) {
    displayException (e, gOutputStream);
    return kInvalidOption;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### " <<
      fakeExecutableName <<
      " gIndenter value after options ands arguments checking: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // do the job
  // ------------------------------------------------------

  xmlFile2guidoWithHandler (
    xmlfile,
    out,
    err,
    handler);

  return kNoErr;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfile2guido (
  const char *         fileName,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromFile (
	      fileName,
	      "Pass 1",
	      "Create an mxmlTree reading a MusicXML file");

	if (xmlfile) {
		return
		  xmlFile2guidoWithOptionsVector (
		    xmlfile, options, out, err);
	}

	return kInvalidFile;
}

xmlErr musicxmlFile2guidoWithHandler (
  const char * fileName,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromFile (
	      fileName,
	      "Pass 1",
	      "Create an mxmlTree reading a MusicXML file");

	if (xmlfile) {
		return
		  xmlFile2guidoWithHandler (
		    xmlfile, out, err, handler);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2guido (
  FILE *               fd,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromFd (
	      fd,
	      "Pass 1",
	      "Create an mxmlTree reading a MusicXML descriptor");

	if (xmlfile) {
		return
		  xmlFile2guidoWithOptionsVector (
		    xmlfile, options, out, err);
	}

	return kInvalidFile;
}

xmlErr musicxmlFd2guidoWithHandler (
  FILE *       fd,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromFd (
	      fd,
	      "Pass 1",
	      "Create an mxmlTree reading a MusicXML descriptor");

	if (xmlfile) {
		return
		  xmlFile2guidoWithHandler (
		    xmlfile, out, err, handler);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2guido (
  const char *         buffer,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromString (
	      buffer,
	      "Pass 1",
	      "Create an mxmlTree reading a MusicXML buffer");

	// call xmlFile2guido() even if xmlfile is null,
	// to handle the help options if any
  return
    xmlFile2guidoWithOptionsVector (
      xmlfile, options, out, err);

	return kInvalidFile;
}

xmlErr musicxmlString2guidoWithHandler (
  const char * buffer,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromString (
	      buffer,
	      "Pass 1",
	      "Create an mxmlTree reading a MusicXML buffer");

	// call xmlFile2guido() even if xmlfile is null,
	// to handle the help options if any
  return
    xmlFile2guidoWithHandler (
      xmlfile, out, err, handler);

	return kInvalidFile;
}


}