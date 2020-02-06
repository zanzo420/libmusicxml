/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class msrHarmonyDegree : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

/*
Degree elements
  can then add, subtract, or alter from these
  starting points.

      <harmony>
        <root>
          <root-step>B</root-step>
        </root>
        <kind>dominant</kind>
        <degree>
          <degree-value>5</degree-value>
          <degree-alter>1</degree-alter>
          <degree-type>alter</degree-type>
        </degree>
        <degree>
          <degree-value>9</degree-value>
          <degree-alter>1</degree-alter>
          <degree-type>add</degree-type>
        </degree>
      </harmony>
*/

    enum msrHarmonyDegreeTypeKind {
      kHarmonyDegreeTypeAdd,
      kHarmonyDegreeTypeAlter,
      kHarmonyDegreeTypeSubstract };

    static string harmonyDegreeTypeKindAsString (
      msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmonyDegree> create (
      int                      inputLineNumber,
      int                      harmonyDegreeValue,
      msrAlterationKind        harmonyDegreeAlterationKind,
      msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);

    /* JMI
    SMARTP<msrHarmonyDegree> createHarmonyDegreeNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrHarmonyDegree> createHarmonyDegreeDeepCopy ( // JMI ???
      S_msrPart containingPart);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHarmonyDegree (
      int                      inputLineNumber,
      int                      harmonyDegreeValue,
      msrAlterationKind        harmonyDegreeAlterationKind,
      msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);

    virtual ~msrHarmonyDegree ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setHarmonyDegreeHarmonyUpLink (
                            S_msrHarmony harmonyUpLink);

    S_msrHarmony          getHarmonyDegreeHarmonyUpLink () const
                              { return fHarmonyDegreeHarmonyUpLink; }

    int                   getHarmonyDegreeValue () const
                              { return fHarmonyDegreeValue; }

    msrAlterationKind     getHarmonyDegreeAlterationKind () const
                              { return fHarmonyDegreeAlterationKind; }

    msrHarmonyDegreeTypeKind
                          getHarmonyDegreeTypeKind () const
                              { return fHarmonyDegreeTypeKind; }

    // services
    // ------------------------------------------------------

    int                   harmonyDegreeAsSemitones () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                harmonyDegreeKindAsString () const;
    string                harmonyDegreeKindAsShortString () const;

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // upLinks
    S_msrHarmony          fHarmonyDegreeHarmonyUpLink;

    int                   fHarmonyDegreeValue;
    msrAlterationKind     fHarmonyDegreeAlterationKind;
    msrHarmonyDegreeTypeKind
                          fHarmonyDegreeTypeKind;
};
typedef SMARTP<msrHarmonyDegree> S_msrHarmonyDegree;
EXP ostream& operator<< (ostream& os, const S_msrHarmonyDegree& elt);

//______________________________________________________________________________
class msrHarmony : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmony> createWithoutVoiceUplink (
      int                      inputLineNumber,
      // no harmonyVoiceUpLink yet
      msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
      msrHarmonyKind           harmonyKind,
      string                   harmonyKindText,
      int                      harmonyInversion,
      msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
      rational                 harmonySoundingWholeNotes,
      rational                 harmonyDisplayWholeNotes,
      int                      harmonyStaffNumber,
      msrTupletFactor          harmonyTupletFactor,
      rational                 harmonyWholeNotesOffset);

    SMARTP<msrHarmony> createHarmonyNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrHarmony> createHarmonyDeepCopy ( // JMI ???
      S_msrVoice containingVoice);

    // applications API
    // ------------------------------------------------------

    static SMARTP<msrHarmony> createWithVoiceUplink (
      int                      inputLineNumber,
      S_msrVoice               harmonyVoiceUpLink,
      msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
      msrHarmonyKind           harmonyKind,
      string                   harmonyKindText,
      int                      harmonyInversion,
      msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
      rational                 harmonySoundingWholeNotes,
      rational                 harmonyDisplayWholeNotes,
      int                      harmonyStaffNumber,
      msrTupletFactor          harmonyTupletFactor,
      rational                 harmonyWholeNotesOffset);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHarmony (
      int                      inputLineNumber,
      S_msrVoice               harmonyVoiceUpLink,
      msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
      msrHarmonyKind           harmonyKind,
      string                   harmonyKindText,
      int                      harmonyInversion,
      msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
      rational                 harmonySoundingWholeNotes,
      rational                 harmonyDisplayWholeNotes,
      int                      harmonyStaffNumber,
      msrTupletFactor          harmonyTupletFactor,
      rational                 harmonyWholeNotesOffset);

    virtual ~msrHarmony ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
    void                  setHarmonyNoteUpLink (
                            S_msrNote note)
                              { fHarmonyNoteUpLink = note; }

    S_msrNote             getHarmonyNoteUpLink () const
                             { return fHarmonyNoteUpLink; }

    void                  setHarmonyVoiceUpLink (
                            S_msrVoice voice)
                              { fHarmonyVoiceUpLink = voice; }

    S_msrVoice            getHarmonyVoiceUpLink () const
                             { return fHarmonyVoiceUpLink; }

    // whole notes
    void                  setHarmonySoundingWholeNotes (
                            rational wholeNotes)
                              { fMeasureElementSoundingWholeNotes = wholeNotes; }

    rational              getHarmonySoundingWholeNotes () const
                              { return fMeasureElementSoundingWholeNotes; }

    void                  setHarmonyDisplayWholeNotes (
                            rational wholeNotes)
                              { fHarmonyDisplayWholeNotes = wholeNotes; }

    rational              getHarmonyDisplayWholeNotes () const
                              { return fHarmonyDisplayWholeNotes; }

    // offset
    rational              getHarmonyWholeNotesOffset () const
                              { return fHarmonyWholeNotesOffset; }

    // root
    msrQuarterTonesPitchKind
                          getHarmonyRootQuarterTonesPitchKind () const
                              { return fHarmonyRootQuarterTonesPitchKind; }

    // kind
    msrHarmonyKind        getHarmonyKind () const
                              { return fHarmonyKind; }

    string                getHarmonyKindText () const
                              { return fHarmonyKindText; }

    // degrees
    const list<S_msrHarmonyDegree>&
                          getHarmonyDegreesList ()
                              { return fHarmonyDegreesList; }

    // inversion
    int                   getHarmonyInversion () const
                              { return fHarmonyInversion; }

    // bass
    msrQuarterTonesPitchKind
                          getHarmonyBassQuarterTonesPitchKind () const
                              { return fHarmonyBassQuarterTonesPitchKind; }

    // staff
    void                  setHarmonyStaffNumber (
                            int staffNumber)
                              { fHarmonyStaffNumber = staffNumber; }

    int                   getHarmonyStaffNumber () const
                             { return fHarmonyStaffNumber; }

    // tuplet factor
    void                  setHarmonyTupletFactor (
                            msrTupletFactor tupletFactor);

    msrTupletFactor       getHarmonyTupletFactor () const
                              { return fHarmonyTupletFactor; }

    // frame
    void                  setHarmonyFrame (
                            S_msrFrame frame);

    const S_msrFrame&     getHarmonyFrame () const
                              { return fHarmonyFrame; }

    // position in measure
    void                  setHarmonyPositionInMeasure (
                            rational positionInMeasure);

  public:

    // services
    // ------------------------------------------------------

    void                  appendHarmonyDegreeToHarmony (
                            S_msrHarmonyDegree harmonyDegree)
                              {
                                fHarmonyDegreesList.push_back (
                                  harmonyDegree);
                              }

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrNote             fHarmonyNoteUpLink;
    S_msrVoice            fHarmonyVoiceUpLink;

    // whole notes
    rational              fHarmonyDisplayWholeNotes;

    // offset
    rational              fHarmonyWholeNotesOffset;

    // root
    msrQuarterTonesPitchKind
                          fHarmonyRootQuarterTonesPitchKind;

    // kind
    msrHarmonyKind        fHarmonyKind;
    string                fHarmonyKindText;

    // inversion
    int                   fHarmonyInversion;

    // bass
    msrQuarterTonesPitchKind
                          fHarmonyBassQuarterTonesPitchKind;

    // degrees
    list<S_msrHarmonyDegree>
                          fHarmonyDegreesList;

    // staff
    int                   fHarmonyStaffNumber;

    // tuplet factor
    msrTupletFactor       fHarmonyTupletFactor;

    // frame
    S_msrFrame            fHarmonyFrame;
};
typedef SMARTP<msrHarmony> S_msrHarmony;
EXP ostream& operator<< (ostream& os, const S_msrHarmony& elt);
