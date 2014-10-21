/**
 * @file Options.hpp
 * Defines Vampire options.
 */

#ifndef __Options__
#define __Options__

#include "Forwards.hpp"

#include "Debug/Assertion.hpp"

#include "Lib/Allocator.hpp"
#include "Lib/XML.hpp"

namespace Shell {

using namespace Lib;
using namespace Kernel;

class Property;

/**
 * Class that represents Vampire's options.
 * 11/11/2004 Shrigley Hall, completely reimplemented
 */
class Options
{
public:
  enum Tag {
    AGE_WEIGHT_RATIO,
    AIG_BDD_SWEEPING,
    AIG_CONDITIONAL_REWRITING,
    AIG_DEFINITION_INTRODUCTION,
    AIG_DEFINITION_INTRODUCTION_THRESHOLD,
    AIG_FORMULA_SHARING,
    AIG_INLINER,
    ARITY_CHECK,

    BACKWARD_DEMODULATION,
    BACKWARD_SUBSUMPTION,
    BACKWARD_SUBSUMPTION_RESOLUTION,
    BFNT,
    BINARY_RESOLUTION,
    BP_COLLAPSING_PROPAGATION,
    BP_ALLOWED_FM_BALANCE,
    BP_ALMOST_HALF_BOUND_REMOVER,
    BP_ASSIGNMENT_SELECTOR,
    BP_UPDATE_BY_ONE_CONSTRAINT,
    BP_CONFLICT_SELECTOR,
    BP_CONSERVATIVE_ASSIGNMENT_SELECTION,
    BP_FM_ELIMINATION,
    BP_MAX_PROP_LENGTH,
    BP_PROPAGATE_AFTER_CONFLICT,
    BP_START_WITH_PRECISE,
    BP_START_WITH_RATIONAL,

    BP_VARIABLE_SELECTOR,

    COLOR_UNBLOCKING,
    CONDENSATION,

    /** Decode test id */
    DECODE,
    DEMODULATION_REDUNDANCY_CHECK,
    DISTINCT_PROCESSOR,

    EPR_PRESERVING_NAMING,
    EPR_PRESERVING_SKOLEMIZATION,
    EPR_RESTORING_INLINING,
    /**
     * Propagate equalities in formulas, for example
     * (X=Y => X=f(Y)) ---> X=f(X)
     *
     * Such propagation can simplify formulas early in
     * the preprocessing and so help other preprocessing
     * rules (namely dealing with predicate definitions).
     */
    EQUALITY_PROPAGATION,
    EQUALITY_PROXY,
    EQUALITY_RESOLUTION_WITH_DELETION,

    EXTENSIONALITY_ALLOW_POS_EQ,
    EXTENSIONALITY_MAX_LENGTH,
    EXTENSIONALITY_RESOLUTION,

    FLATTEN_TOP_LEVEL_CONJUNCTIONS,
    /** If some of the specified options are set to a forbidden state,
     * vampire will fail to start, or in the CASC mode it will skip
     * such strategies. */
    FORBIDDEN_OPTIONS,
    FORCED_OPTIONS,
    FORWARD_DEMODULATION,
    FORWARD_LITERAL_REWRITING,
    FORWARD_SUBSUMPTION,
    FORWARD_SUBSUMPTION_RESOLUTION,
    /** All literals of set-of-support clauses will be selected */
    FUNCTION_DEFINITION_ELIMINATION,
    FUNCTION_NUMBER,
    GENERAL_SPLITTING,
    GLOBAL_SUBSUMPTION,

    /** We check whether by swapping predicate polarities we can obtain a horn problem */
    HORN_REVEALING,
    /** Generating inference that attempts to do several rewriting at once if it will
     * eliminate literals of the original clause (now we aim just for eliminatin by equality
     * resolution) */
    HYPER_SUPERPOSITION,

    IGNORE_MISSING,
    INCLUDE,
    INCREASED_NUMERAL_WEIGHT,
    INEQUALITY_SPLITTING,
    INPUT_FILE,
    INPUT_SYNTAX,
    INST_GEN_BIG_RESTART_RATIO,
    INST_GEN_INPROCESSING,
    INST_GEN_PASSIVE_REACTIVATION,
    INST_GEN_RESOLUTION_RATIO,
    INST_GEN_RESTART_PERIOD,
    INST_GEN_RESTART_PERIOD_QUOTIENT,
    INST_GEN_SELECTION,
    INST_GEN_WITH_RESOLUTION,
    INTERPRETED_SIMPLIFICATION,

    LATEX_OUTPUT,
    LINGVA_ADDITIONAL_INVARIANTS,
    LITERAL_COMPARISON_MODE,
    LOG_FILE,
    LRS_FIRST_TIME_CHECK,
    LRS_WEIGHT_LIMIT_ONLY,

    MAX_ACTIVE,
    MAX_ANSWERS,
    MAX_INFERENCE_DEPTH,
    MAX_PASSIVE,
    MAX_WEIGHT,
    MEMORY_LIMIT,
    MODE,

    NAME_PREFIX,
    NAMING,
    NICENESS_OPTION,
    NONGOAL_WEIGHT_COEFFICIENT,
    NONLITERALS_IN_CLAUSE_WEIGHT,
    NORMALIZE,

    OUTPUT_AXIOM_NAMES,

    /** Determines whether predicate definitions will be inlined */
    PREDICATE_DEFINITION_INLINING,
    /** Determines whether predicates with equivalent definitions will be merged into one */
    PREDICATE_DEFINITION_MERGING,
    /** Determines whether SAT solver will be used to discover equivalent predicates */
    PREDICATE_EQUIVALENCE_DISCOVERY,
    PREDICATE_EQUIVALENCE_DISCOVERY_ADD_IMPLICATIONS,
    PREDICATE_EQUIVALENCE_DISCOVERY_RANDOM_SIMULATION,
    PREDICATE_EQUIVALENCE_DISCOVERY_SAT_CONFLICT_LIMIT,
    PREDICATE_INDEX_INTRODUCTION,
    PRINT_CLAUSIFIER_PREMISES,
    PROBLEM_NAME,
    PROOF,
    PROOF_CHECKING,
    /** if non-empty, symbols with this prefix will not be subject to any kind of elimination in preprocessing */
    PROTECTED_PREFIX,

    /** Determines whether (and how) we attempt to answer questions */
    QUESTION_ANSWERING,

    RANDOM_SEED,
    ROW_VARIABLE_MAX_LENGTH,

    SAT_CLAUSE_ACTIVITY_DECAY,
    SAT_CLAUSE_DISPOSER,
    SAT_LEARNT_MINIMIZATION,
    SAT_LEARNT_SUBSUMPTION_RESOLUTION,

    /** @author: ioan
     * Use lingeling in an incremental way from the beginning
     */
    SAT_LINGELING_INCREMENTAL,
    /** enable similar model generation for lingleling */
    SAT_LINGELING_SIMILAR_MODELS,

    SAT_RESTART_FIXED_COUNT,
    SAT_RESTART_GEOMETRIC_INCREASE,
    SAT_RESTART_GEOMETRIC_INIT,
    SAT_RESTART_LUBY_FACTOR,
    SAT_RESTART_MINISAT_INCREASE,
    SAT_RESTART_MINISAT_INIT,
    SAT_RESTART_STRATEGY,
    SAT_SOLVER,
    SAT_VAR_ACTIVITY_DECAY,
    SAT_VAR_SELECTOR,
    /** !!! saturation algorithm: lrs, otter, or discount, inst_gen or tabulation */
    SATURATION_ALGORITHM,
    SELECTION,
    SHOW_ACTIVE,
    SHOW_BLOCKED,
    SHOW_DEFINITIONS,
    SHOW_INTERPOLANT,
    SHOW_NEW,
    SHOW_NEW_PROPOSITIONAL,
    SHOW_NONCONSTANT_SKOLEM_FUNCTION_TRACE,
    SHOW_OPTIONS,
    SHOW_PASSIVE,
    SHOW_PREPROCESSING,
    SHOW_SKOLEMISATIONS,
    SHOW_SYMBOL_ELIMINATION,
    SHOW_THEORY_AXIOMS,
    SIMULATED_TIME_LIMIT,
    SINE_DEPTH,
    SINE_GENERALITY_THRESHOLD,
    SINE_SELECTION,
    SINE_TOLERANCE,
    SMTLIB_CONSIDER_INTS_REAL,
    SMTLIB_FLET_AS_DEFINITION,
    SMTLIB_INTRODUCE_AIG_NAMES,
    SOS,
    //SPLIT_ADD_GROUND_NEGATION,
    SPLIT_AT_ACTIVATION, // should be checked
    //SPLIT_GOAL_ONLY,
    //SPLIT_INPUT_ONLY,
    //SPLIT_POSITIVE,
    SPLITTING,
    SPLITTING_ADD_COMPLEMENTARY,
    SPLITTING_CONGRUENCE_CLOSURE,
    SPLITTING_EAGER_REMOVAL,
    SPLITTING_FLUSH_PERIOD,
    SPLITTING_FLUSH_QUOTIENT,
    SPLITTING_MODEL,
    SPLITTING_NONSPLITTABLE_COMPONENTS,

    STATISTICS,
    SUPERPOSITION_FROM_VARIABLES,
    SYMBOL_PRECEDENCE,

    TABULATION_BW_RULE_SUBSUMPTION_RESOLUTION_BY_LEMMAS,
    TABULATION_FW_RULE_SUBSUMPTION_RESOLUTION_BY_LEMMAS,
    TABULATION_GOAL_AWR,
    TABULATION_GOAL_LEMMA_RATIO,
    TABULATION_INSTANTIATE_PRODUCING_RULES,
    TABULATION_LEMMA_AWR,
    TEST_ID,
    THANKS,
    THEORY_AXIOMS,
    TIME_LIMIT,
    TIME_STATISTICS,
    TRIVIAL_PREDICATE_REMOVAL,

    UNIT_RESULTING_RESOLUTION,
    UNUSED_PREDICATE_DEFINITION_REMOVAL,
    USEDM,

    WEIGHT_INCREMENT,
    WHILE_NUMBER,

    XML_OUTPUT,
   
    ZERO_OPT,

    NUMBER_OF_OPTIONS // must be the last one!
  };

public:
  class StringInt;
  class StringIntMap;
  
  //enums for the bound propagation purpose
  enum BPAlmostHalfBoundingRemoval {
    AHR_BOUNDS_ONLY = 0,
    AHR_OFF = 1,
    AHR_ON = 2
  };

  enum BPAssignmentSelector{
    ASG_ALTERNATIVE = 0,
    ASG_BMP = 1,
    ASG_LOWER = 2,
    ASG_MIDDLE = 3,
    ASG_RANDOM = 4,
    ASG_RATIONAL = 5,
    ASG_SMALLEST = 6,
    ASG_TIGHT = 7,
    ASG_TIGHTISH = 8,
   // ASG_BIGGEST = 9,
    ASG_UPPER = 9
  };
  
  enum BPConflictSelector{
    CS_LEAST_RECENT = 0, 
    CS_MOST_RECENT = 1, 
    CS_SHORTEST_CONSTRAINT = 2
  };
  
  enum BPVariableSelector{
    VS_CONFLICTING = 0, 
    VS_CONFLICTING_AND_COLLAPSING = 1, 
    VS_FIRST = 2, 
    VS_LOOK_AHEAD =3, 
    VS_RANDOM = 4, 
    VS_RECENTLY_CONFLICTING = 5,
    VS_RECENTLY_COLLAPSING = 6,
    VS_TIGHTEST_BOUND = 7

  };
  /**
   * Possible values for function_definition_elimination.
   * @since 29/05/2004 Manchester
   */
  enum FunctionDefinitionElimination {
    FDE_ALL = 0,
    FDE_NONE = 1,
    FDE_UNUSED = 2
  };

  /**
   * Possible values for the input syntax
   * @since 26/08/2009 Redmond
   */
  enum InputSyntax {
    /** syntax of the Simplify prover */
    IS_SIMPLIFY = 0,
    /** syntax of SMTLIB1.2 */
    IS_SMTLIB = 1,
    IS_SMTLIB2 = 2,
    /** syntax of the TPTP prover */
    IS_TPTP = 3, 
    IS_HUMAN = 4, 
    IS_MPS = 5, 
    IS_NETLIB = 6
  };

  /**
   * Possible values for mode_name.
   * @since 06/05/2007 Manchester
   */
  enum Mode {
    MODE_AXIOM_SELECTION,
    MODE_BOUND_PROP,
    MODE_CASC,
    MODE_CASC_EPR,
    MODE_CASC_LTB,
    MODE_CASC_MZR,
    MODE_CASC_SAT,
    MODE_CLAUSIFY,
    MODE_CONSEQUENCE_ELIMINATION,
    MODE_GROUNDING,
    MODE_LTB_BUILD,
    MODE_LTB_SOLVE,
    /** this mode only outputs the input problem, without any preprocessing */
    MODE_OUTPUT,
    MODE_PREPROCESS,
    MODE_PROFILE,
    MODE_PROGRAM_ANALYSIS,   
    MODE_SAT, 
    MODE_SPIDER,
    MODE_VAMPIRE
};

  /** Various options for the output of statistics in Vampire */
  enum Statistics {
    /** changed by the option "--statistics brief" */
    STATISTICS_BRIEF = 0,
    /** changed by the option "--statistics full */
    STATISTICS_FULL = 1,
    /** changed by the option "--statistics off" */
    STATISTICS_NONE = 2
  };

  /** Possible values for sat_solver */
  enum SatSolver {
     BUFFERED_LINGELING = 0,
     BUFFERED_MINISAT = 1,
     BUFFERED_VAMPIRE = 2,
     LINGELING = 3,
     MINISAT = 4,
     VAMPIRE = 5
  };

  /** Possible values for saturation_algorithm */
  enum SaturationAlgorithm {
     DISCOUNT = 0,
     INST_GEN = 1,
     LRS = 2,
     OTTER = 3,
     TABULATION = 4
   };

  /** Possible values for activity of some inference rules */
  enum RuleActivity {
    RA_INPUT_ONLY = 0,
    RA_OFF = 1,
    RA_ON = 2
  };

  enum QuestionAnsweringMode {
    QA_ANSWER_LITERAL = 0,
    QA_FROM_PROOF = 1,
    QA_OFF = 2
  };

  enum InliningMode {
    INL_AXIOMS_ONLY = 0,
    INL_NON_GROWING = 1,
    INL_OFF = 2,
    INL_ON = 3
  };

  enum InterpolantMode {
    INTERP_MINIMIZED = 0,
    INTERP_OFF = 1,
    INTERP_ON = 2
  };

  /** Possible values for splitting */
// Now just on or off
//  enum SplittingMode {
//    SM_INPUT = 0,
//    SM_OFF = 1,
//    SM_SAT = 2
//  };

  enum LiteralComparisonMode {
    LCM_PREDICATE = 0,
    LCM_REVERSE = 1,
    LCM_STANDARD = 2
  };

  enum Condensation {
    CONDENSATION_FAST = 0,
    CONDENSATION_OFF = 1,
    CONDENSATION_ON = 2
  };

  enum Demodulation {
    DEMODULATION_ALL = 0,
    DEMODULATION_OFF = 1,
    DEMODULATION_PREORDERED = 2
  };

  enum Subsumption {
    SUBSUMPTION_OFF = 0,
    SUBSUMPTION_ON = 1,
    SUBSUMPTION_UNIT_ONLY = 2
  };

  enum URResolution {
    URR_EC_ONLY = 0,
    URR_OFF = 1,
    URR_ON = 2
  };

  enum SymbolPrecedence {
    BY_ARITY = 0,
    BY_OCCURRENCE = 1,
    BY_REVERSE_ARITY = 2
  };

  enum SineSelection {
    SS_AXIOMS = 0,
    SS_INCLUDED = 1,
    SS_OFF = 2
  };

  enum Proof {
    PROOF_OFF = 0,
    PROOF_ON = 1,
    PROOF_PROOFCHECK = 2,
    PROOF_TPTP = 3
  };

  /** Values for --equality_proxy */
  enum EqualityProxy {
    EP_R = 0,
    EP_RS = 1,
    EP_RST = 2,
    EP_RSTC = 3,
    /** --equality_proxy=off */
    EP_OFF = 4,
    /** --equality_proxy=on */
    EP_ON = 5
  };

  /** Values for --extensionality_resolution */
  enum ExtensionalityResolution {
    ER_FILTER = 0,
    ER_KNOWN = 1,
    ER_OFF = 2
  };

  enum SatRestartStrategy {
    SRS_FIXED = 0,
    SRS_GEOMETRIC = 1,
    SRS_LUBY = 2,
    SRS_MINISAT = 3,
  };

  enum SatVarSelector {
    SVS_ACTIVE = 0,
    SVS_NICENESS = 1,
    SVS_RECENTLY_LEARNT = 2,
  };

  enum NicenessOption{
    NICENESS_AVERAGE = 0,
    NICENESS_NONE=1,
    NICENESS_SUM = 2,
    NICENESS_TOP = 3,
  };

  enum SatClauseDisposer {
    SCD_GROWING = 0,
    SCD_MINISAT = 1,
  };

  enum SplittingAddComplementary {
    SAC_GROUND = 0,
    SAC_NONE = 1
  };

  enum SplittingModel {
    SM_MIN_ALL = 0,
    SM_MIN_SCO = 1,
    SM_TOTAL = 2
  };

  enum SplittingNonsplittableComponents {
    SNS_ALL = 0,
    SNS_ALL_DEPENDENT = 1,
    SNS_KNOWN = 2,
    SNS_NONE = 3
  };

  enum Sos {
    SOS_ALL = 0,
    SOS_OFF = 1,
    SOS_ON = 2
  };

  enum PredicateEquivalenceDiscoveryMode {
    PED_ALL_ATOMS = 0,
    PED_ALL_FORMULAS = 1,
    PED_DEFINITIONS = 2,
    PED_OFF = 3,
    PED_ON = 4
  };

public:
  Options ();
  void output (ostream&) const;
  void readFromTestId (vstring testId);
  void readOptionsString (vstring testId);
  vstring generateTestId() const;
  bool complete(const Problem&) const;
  bool completeForNNE() const;
  void setForcedOptionValues();
  void checkGlobalOptionConstraints() const;

  void forceIncompleteness() { _forceIncompleteness=true; }

  /**
   * Return the problem name
   *
   * The problem name is computed from the input file name in
   * the @b setInputFile function. If the input file is not set,
   * the problem name is equal to "unknown". The problem name can
   * be set to a specific value using setProblemName().
   */
  vstring problemName () const { return _problemName; }
  void setProblemName(vstring str) { _problemName = str; }

  vstring forcedOptions() const { return _forcedOptions; }
  vstring forbiddenOptions() const { return _forbiddenOptions; }
  vstring testId() const { return _testId; }
  vstring protectedPrefix() const { return _protectedPrefix; }
  Statistics statistics() const { return _statistics; }
  void setStatistics(Statistics newVal) { _statistics=newVal; }
  Proof proof() const { return _proof; }
  bool proofChecking() const { return _proofChecking; }
  int naming() const { return _naming; }
  bool setNaming(int newVal);
  bool flattenTopLevelConjunctions() const { return _flattenTopLevelConjunctions; }
  bool eprPreservingNaming() const { return _eprPreservingNaming; }
  void setEprPreservingNaming(bool newVal) { _eprPreservingNaming = newVal; }
  bool eprPreservingSkolemization() const { return _eprPreservingSkolemization; }
  void setEprPreservingSkolemization(bool newVal) { _eprPreservingSkolemization = newVal; }
  bool eprRestoringInlining() const { return _eprRestoringInlining; }
  void setEprRestoringInlining(bool newVal) { _eprRestoringInlining = newVal; }
  InliningMode predicateDefinitionInlining() const { return _predicateDefinitionInlining; }
  void setPredicateDefinitionInlining(InliningMode newVal) { _predicateDefinitionInlining = newVal; }
  bool predicateDefinitionMerging() const { return _predicateDefinitionMerging; }
  void setPredicateDefinitionMerging(bool newValue) { _predicateDefinitionMerging = newValue; }
  PredicateEquivalenceDiscoveryMode predicateEquivalenceDiscovery() const { return _predicateEquivalenceDiscovery; }
  void setPredicateEquivalenceDiscovery(PredicateEquivalenceDiscoveryMode newValue) { _predicateEquivalenceDiscovery = newValue; }
  bool predicateEquivalenceDiscoveryAddImplications() const { return _predicateEquivalenceDiscoveryAddImplications; }
  bool predicateEquivalenceDiscoveryRandomSimulation() const { return _predicateEquivalenceDiscoveryRandomSimulation; }
  int predicateEquivalenceDiscoverySatConflictLimit() const { return _predicateEquivalenceDiscoverySatConflictLimit; }
  bool predicateIndexIntroduction() const { return _predicateIndexIntroduction; }
  void setPredicateIndexIntroduction(bool newValue) { _predicateIndexIntroduction = newValue; }
  bool aigBddSweeping() const { return _aigBddSweeping; }
  bool aigConditionalRewriting() const { return _aigConditionalRewriting; }
  bool aigDefinitionIntroduction() const { return _aigDefinitionIntroduction; }
  unsigned aigDefinitionIntroductionThreshold() const { return _aigDefinitionIntroductionThreshold; }
  bool aigFormulaSharing() const { return _aigFormulaSharing; }
  bool aigInliner() const { return _aigInliner; }
  Mode mode() const { return _mode; }
  void setMode(Mode newVal);
  InputSyntax inputSyntax() const { return _inputSyntax; }
  void setInputSyntax(InputSyntax newVal) { _inputSyntax = newVal; }
  bool normalize() const { return _normalize; }
  void setNormalize(bool normalize) { _normalize = normalize; }
  vstring include() const { return _include; }
  void setInclude(vstring val) { _include = val; }
  vstring includeFileName (const vstring& relativeName);
  vstring logFile() const { return _logFile; }
  vstring inputFile() const { return _inputFile; }
  int randomSeed() const { return _randomSeed; }
  int rowVariableMaxLength() const { return _rowVariableMaxLength; }
  void setRowVariableMaxLength(int newVal) { _rowVariableMaxLength = newVal; }
  bool printClausifierPremises() const { return _printClausifierPremises; }
  bool showActive() const { return _showActive; }
  bool showBlocked() const { return _showBlocked; }
  bool showDefinitions() const { return _showDefinitions; }
  InterpolantMode showInterpolant() const { return _showInterpolant; }
  bool showNew() const { return _showNew; }
  bool showNewPropositional() const { return _showNewPropositional; }
  bool showNonconstantSkolemFunctionTrace() const { return _showNonconstantSkolemFunctionTrace; }
  void setShowNonconstantSkolemFunctionTrace(bool newVal) { _showNonconstantSkolemFunctionTrace = newVal; }
  bool showOptions() const { return _showOptions; }
  bool showPassive() const { return _showPassive; }
  bool showPreprocessing() const { return _showPreprocessing; }
  bool showSkolemisations() const { return _showSkolemisations; }
  bool showSymbolElimination() const { return _showSymbolElimination; }
  bool showTheoryAxioms() const { return _showTheoryAxioms; }
  bool unusedPredicateDefinitionRemoval() const { return _unusedPredicateDefinitionRemoval; }
  void setUnusedPredicateDefinitionRemoval(bool newVal) { _unusedPredicateDefinitionRemoval = newVal; }
  bool weightIncrement() const { return _weightIncrement; }
  bool useDM() const { return _use_dm; }
  SatSolver satSolver() const { return _satSolver; }
  void setSatSolver(SatSolver newVal) { _satSolver = newVal; }
  SaturationAlgorithm saturationAlgorithm() const { return _saturationAlgorithm; }
  void setSaturationAlgorithm(SaturationAlgorithm newVal) { _saturationAlgorithm = newVal; }
  int selection() const { return _selection; }
  bool setSelection(int newValue);
  bool setInstGenSelection(int newValue);
  vstring latexOutput() const { return _latexOutput; }
  LiteralComparisonMode literalComparisonMode() const { return _literalComparisonMode; }
  bool forwardSubsumptionResolution() const { return _forwardSubsumptionResolution; }
  void setForwardSubsumptionResolution(bool newVal) { _forwardSubsumptionResolution = newVal; }
  Demodulation forwardDemodulation() const { return _forwardDemodulation; }
  bool binaryResolution() const { return _binaryResolution; }
  bool bfnt() const { return _bfnt; }
  void setBfnt(bool newVal) { _bfnt = newVal; }
  URResolution unitResultingResolution() const { return _unitResultingResolution; }
  bool hyperSuperposition() const { return _hyperSuperposition; }
  bool arityCheck() const { return _arityCheck; }
  void setArityCheck(bool newVal) { _arityCheck=newVal; }
  Demodulation backwardDemodulation() const { return _backwardDemodulation; }
  bool demodulationRedundancyCheck() const { return _demodulationRedundancyCheck; }
  void setBackwardDemodulation(Demodulation newVal) { _backwardDemodulation = newVal; }
  Subsumption backwardSubsumption() const { return _backwardSubsumption; }
  void setBackwardSubsumption(Subsumption newVal) { _backwardSubsumption = newVal; }
  Subsumption backwardSubsumptionResolution() const { return _backwardSubsumptionResolution; }
  bool forwardSubsumption() const { return _forwardSubsumption; }
  bool forwardLiteralRewriting() const { return _forwardLiteralRewriting; }

  vstring lingvaAdditionalInvariants() const {return _lingvaAdditionalInvariants; }
  int lrsFirstTimeCheck() const { return _lrsFirstTimeCheck; }
  int lrsWeightLimitOnly() const { return _lrsWeightLimitOnly; }
  bool setLrsFirstTimeCheck(int newVal);
  int simulatedTimeLimit() const { return _simulatedTimeLimit; }
  void setSimulatedTimeLimit(int newVal) { _simulatedTimeLimit = newVal; }
  int maxInferenceDepth() const { return _maxInferenceDepth; }
  SymbolPrecedence symbolPrecedence() const { return _symbolPrecedence; }
  /**
   * Return time limit in deciseconds, or 0 if there is no time limit
   */
  int timeLimitInDeciseconds() const { return _timeLimitInDeciseconds; }
  static int readTimeLimit(const char* val);
  size_t memoryLimit() const { return _memoryLimit; }
  int inequalitySplitting() const { return _inequalitySplitting; }
  long maxActive() const { return _maxActive; }
  long maxAnswers() const { return _maxAnswers; }
  void setMaxAnswers(int newVal) { _maxAnswers = newVal; }
  long maxPassive() const { return _maxPassive; }
  int maxWeight() const { return _maxWeight; }
  int ageRatio() const { return _ageRatio; }
  int weightRatio() const { return _weightRatio; }
  bool superpositionFromVariables() const { return _superpositionFromVariables; }
  bool equalityPropagation() const { return _equalityPropagation; }
  void setEqualityPropagation(bool newVal) { _equalityPropagation = newVal; }
  EqualityProxy equalityProxy() const { return _equalityProxy; }
  RuleActivity equalityResolutionWithDeletion() const { return _equalityResolutionWithDeletion; }
  ExtensionalityResolution extensionalityResolution() const { return _extensionalityResolution; }
  unsigned extensionalityMaxLength() const { return _extensionalityMaxLength; }
  bool extensionalityAllowPosEq() const { return _extensionalityAllowPosEq; }
  
  float nongoalWeightCoefficient() const { return _nongoalWeightCoefficient; }
  bool setNongoalWeightCoefficient(float newVal);
  Sos sos() const { return _sos; }
  void setSos(Sos newVal) { _sos = newVal; }
  FunctionDefinitionElimination functionDefinitionElimination() const { return _functionDefinitionElimination; }
  bool outputAxiomNames() const { return _outputAxiomNames; }
  void setOutputAxiomNames(bool newVal) { _outputAxiomNames = newVal; }
  QuestionAnsweringMode questionAnswering() const { return _questionAnswering; }
  void setQuestionAnswering(QuestionAnsweringMode newVal) { _questionAnswering = newVal; }
  vstring xmlOutput() const { return _xmlOutput; }
  vstring thanks() const { return _thanks; }

  bool zeroOpt() const{ return _zeroOpt;}

  bool globalSubsumption() const { return _globalSubsumption; }
  /** true if calling set() on non-existing options does not result in a user error */
  bool ignoreMissing() const { return _ignoreMissing; }
  /** set the "ignore missing options" value to true or false */
  void setIgnoreMissing(bool newVal) { _ignoreMissing = newVal; }
  bool increasedNumeralWeight() const { return _increasedNumeralWeight; }
  bool theoryAxioms() const { return _theoryAxioms; }
  void setTheoryAxioms(bool newValue) { _theoryAxioms = newValue; }
  bool interpretedSimplification() const { return _interpretedSimplification; }
  void setInterpretedSimplification(bool val) { _interpretedSimplification = val; }
  Condensation condensation() const { return _condensation; }
  RuleActivity generalSplitting() const { return _generalSplitting; }
  vstring namePrefix() const { return _namePrefix; }
  bool timeStatistics() const { return _timeStatistics; }
//  bool splitAddGroundNegation() const { return _splitAddGroundNegation; }
  bool splitAtActivation() const { return _splitAtActivation; }
//  bool splitGoalOnly() const { return _splitGoalOnly; }
//  bool splitInputOnly() const { return _splitInputOnly; }
//  bool splitPositive() const { return _splitPositive; }
  bool splitting() const { return _splitting; }
//  void setSplitting(SplittingMode newVal) { _splitting = newVal; }
  bool nonliteralsInClauseWeight() const { return _nonliteralsInClauseWeight; }

  unsigned sineDepth() const { return _sineDepth; }
  unsigned sineGeneralityThreshold() const { return _sineGeneralityThreshold; }
  SineSelection sineSelection() const { return _sineSelection; }
  void setSineSelection(SineSelection val) { _sineSelection=val; }
  float sineTolerance() const { return _sineTolerance; }

  bool smtlibConsiderIntsReal() const { return _smtlibConsiderIntsReal; }
  void setSmtlibConsiderIntsReal( bool newVal ) { _smtlibConsiderIntsReal = newVal; }
  bool smtlibFletAsDefinition() const { return _smtlibFletAsDefinition; }
  bool smtlibIntroduceAIGNames() const { return _smtlibIntroduceAIGNames; }

  bool colorUnblocking() const { return _colorUnblocking; }
  bool distinctProcessor() const { return _distinctProcessor; }
  bool hornRevealing() const { return _hornRevealing; }
  bool trivialPredicateRemoval() const { return _trivialPredicateRemoval; }

  bool tabulationBwRuleSubsumptionResolutionByLemmas() const { return _tabulationBwRuleSubsumptionResolutionByLemmas; }
  bool tabulationFwRuleSubsumptionResolutionByLemmas() const { return _tabulationFwRuleSubsumptionResolutionByLemmas; }
  int tabulationGoalAgeRatio() const { return _tabulationGoalAgeRatio; }
  int tabulationGoalWeightRatio() const { return _tabulationGoalWeightRatio; }
  int tabulationGoalRatio() const { return _tabulationGoalRatio; }
  int tabulationLemmaRatio() const { return _tabulationLemmaRatio; }
  bool tabulationInstantiateProducingRules() const { return _tabulationInstantiateProducingRules; }
  int tabulationLemmaAgeRatio() const { return _tabulationLemmaAgeRatio; }
  int tabulationLemmaWeightRatio() const { return _tabulationLemmaWeightRatio; }

  float instGenBigRestartRatio() const { return _instGenBigRestartRatio; }
  bool instGenInprocessing() const { return _instGenInprocessing; }
  bool instGenPassiveReactivation() const { return _instGenPassiveReactivation; }
  int instGenResolutionRatioInstGen() const { return _instGenResolutionRatioInstGen; }
  int instGenResolutionRatioResolution() const { return _instGenResolutionRatioResolution; }
  int instGenRestartPeriod() const { return _instGenRestartPeriod; }
  float instGenRestartPeriodQuotient() const { return _instGenRestartPeriodQuotient; }
  int instGenSelection() const { return _instGenSelection; }
  bool instGenWithResolution() const { return _instGenWithResolution; }

  float satClauseActivityDecay() const { return _satClauseActivityDecay; }
  SatClauseDisposer satClauseDisposer() const { return _satClauseDisposer; }
  bool satLearntMinimization() const { return _satLearntMinimization; }
  bool satLingelingIncremental() const { return _satLingelingIncremental; }
  bool satLingelingSimilarModels() const { return _satLingelingSimilarModels; }
  bool satLearntSubsumptionResolution() const { return _satLearntSubsumptionResolution; }
  int satRestartFixedCount() const { return _satRestartFixedCount; }
  float satRestartGeometricIncrease() const { return _satRestartGeometricIncrease; }
  int satRestartGeometricInit() const { return _satRestartGeometricInit; }
  int satRestartLubyFactor() const { return _satRestartLubyFactor; }
  float satRestartMinisatIncrease() const { return _satRestartMinisatIncrease; }
  int satRestartMinisatInit() const { return _satRestartMinisatInit; }
  SatRestartStrategy satRestartStrategy() const { return _satRestartStrategy; }
  float satVarActivityDecay() const { return _satVarActivityDecay; }
  SatVarSelector satVarSelector() const { return _satVarSelector; }

  NicenessOption nicenessOption() const { return _nicenessOption; }

  void setMemoryLimit(size_t newVal) { _memoryLimit = newVal; }
  void setInputFile(const vstring& newVal);
  void setTimeLimitInSeconds(int newVal) { _timeLimitInDeciseconds = 10*newVal; }
  void setTimeLimitInDeciseconds(int newVal) { _timeLimitInDeciseconds = newVal; }
  int getTimeLimit(){return _timeLimitInDeciseconds;}
  int getWhileNumber(){return _whileNumber;}
  int getFunctionNumber(){return _functionNumber;}
//   // standard ways of creating options
  XMLElement toXML() const;
  bool outputSuppressed() const;
  void set(const vstring& name, const vstring& value);
  void set(const char* name, const char* value);
  void setShort(const char* name, const char* value);

  int nonGoalWeightCoeffitientNumerator() const { return _nonGoalWeightCoeffitientNumerator; }
  int nonGoalWeightCoeffitientDenominator() const { return _nonGoalWeightCoeffitientDenominator; }

  SplittingNonsplittableComponents splittingNonsplittableComponents() const { return _splittingNonsplittableComponents; }
  SplittingAddComplementary splittingAddComplementary() const { return _splittingAddComplementary; }
  int splittingFlushPeriod() const { return _splittingFlushPeriod; }
  float splittingFlushQuotient() const { return _splittingFlushQuotient; }
  bool splittingEagerRemoval() const { return _splittingEagerRemoval; }
  bool splittingCongruenceClosure() const { return _splittingCongruenceClosure; }
  SplittingModel splittingModel() const { return _splittingModel; }

  void setProof(Proof p) { _proof = p; }
  bool bpEquivalentVariableRemoval() const { return _equivalentVariableRemoval; }
  unsigned bpMaximalPropagatedEqualityLength() const { return _maximalPropagatedEqualityLength; }
  BPAlmostHalfBoundingRemoval bpAlmostHalfBoundingRemoval() const {return _bpAlmostHalfBoundingRemoval;}
  bool bpFmElimination () const {return _bpFmElimination;}
  unsigned bpAllowedFMBalance() const { return _bpAllowedFMBalance; }
  BPAssignmentSelector bpAssignmentSelector() const {return _bpAssignmentSelector; }
  bool bpCollapsingPropagation() const {return _bpCollapsingPropagation; }
  unsigned bpUpdatesByOneConstraint() const {return _updatesByOneConstraint; }
  bool bpConservativeAssignmentSelection() const {return _bpConservativeAssignmentSelection; }
  BPConflictSelector bpConflictSelector() const {return _bpConflictSelector; }
  bool backjumpTargetIsDecisionPoint() const { return _backjumpTargetIsDecisionPoint; }
  bool bpPropagateAfterConflict() const {return _bpPropagateAfterConflict; }
  BPVariableSelector bpVariableSelector() const {return _bpVariableSelector; }
  bool bpSelectUnusedVariablesFirst() const {return _selectUnusedVariablesFirst; }
  bool bpStartWithPrecise() const { return _bpStartWithPrecise; }
  bool bpStartWithRational() const { return _bpStartWithRational;}
  
  CLASS_NAME(Options);
  USE_ALLOCATOR(Options);

  /** first is option name, second option value */
  typedef pair<vstring, vstring> OptionSpec;
  typedef Stack<OptionSpec> OptionSpecStack;
  static void readOptionsString(vstring testId, OptionSpecStack& assignments);

private:
  void set(const char* name, const char* value, int index);

private:
  class Constants;

  int _ageRatio;
  bool _aigBddSweeping;
  bool _aigConditionalRewriting;
  bool _aigDefinitionIntroduction;
  unsigned _aigDefinitionIntroductionThreshold;
  bool _aigFormulaSharing;
  bool _aigInliner;
  bool _arityCheck;
  
  bool _backjumpTargetIsDecisionPoint;
  Demodulation _backwardDemodulation;
  Subsumption _backwardSubsumption;
  Subsumption _backwardSubsumptionResolution;
  bool _bfnt;
  bool _binaryResolution;
  unsigned _bpAllowedFMBalance;
  BPAlmostHalfBoundingRemoval _bpAlmostHalfBoundingRemoval;
  BPAssignmentSelector _bpAssignmentSelector;
  bool _bpCollapsingPropagation;
  BPConflictSelector _bpConflictSelector;
  bool _bpConservativeAssignmentSelection;
  bool _bpFmElimination;
  bool _bpPropagateAfterConflict;
  bool _bpStartWithPrecise;
  bool _bpStartWithRational;
  BPVariableSelector _bpVariableSelector;

  bool _colorUnblocking;
  Condensation _condensation;

  bool _demodulationRedundancyCheck;
  bool _distinctProcessor;

  bool _eprPreservingNaming;
  bool _eprPreservingSkolemization;
  bool _eprRestoringInlining;
  bool _equalityPropagation;
  EqualityProxy _equalityProxy;
  RuleActivity _equalityResolutionWithDeletion;
  bool _equivalentVariableRemoval;
  ExtensionalityResolution _extensionalityResolution;
  unsigned _extensionalityMaxLength;
  bool _extensionalityAllowPosEq;
  
  bool _flattenTopLevelConjunctions;
  vstring _forbiddenOptions;
  bool _forceIncompleteness;
  vstring _forcedOptions;
  Demodulation _forwardDemodulation;
  bool _forwardLiteralRewriting;
  bool _forwardSubsumption;
  bool _forwardSubsumptionResolution;
  FunctionDefinitionElimination _functionDefinitionElimination;
  int _functionNumber;
  
  RuleActivity _generalSplitting;
  bool _globalSubsumption;

  bool _hornRevealing;
  bool _hyperSuperposition;

  /** if true, then calling set() on non-existing options will not result in a user error */
  bool _ignoreMissing;
  vstring _include;
  /** if this option is true, Vampire will add the numeral weight of a clause
   * to its weight. The weight is defined as the sum of binary sizes of all
   * integers occurring in this clause. This option has not been tested and
   * may be extensive, see Clause::getNumeralWeight()
   */
  bool _increasedNumeralWeight;
  int _inequalitySplitting;
  vstring _inputFile;
  InputSyntax _inputSyntax;
  float _instGenBigRestartRatio;
  bool _instGenInprocessing;
  bool _instGenPassiveReactivation;
  int _instGenResolutionRatioInstGen;
  int _instGenResolutionRatioResolution;
  int _instGenRestartPeriod;
  float _instGenRestartPeriodQuotient;
  int _instGenSelection;
  bool _instGenWithResolution;
  bool _interpretedSimplification;

  vstring _latexOutput;
  vstring _lingvaAdditionalInvariants;

  LiteralComparisonMode _literalComparisonMode;
  vstring _logFile;
  int _lrsFirstTimeCheck;
  int _lrsWeightLimitOnly;

  long _maxActive;
  int _maxAnswers;
  int _maxInferenceDepth;
  long _maxPassive;
  int _maxWeight;
  unsigned _maximalPropagatedEqualityLength;
  size_t _memoryLimit;
  Mode _mode;

  vstring _namePrefix;
  int _naming;
  NicenessOption _nicenessOption;
  float _nongoalWeightCoefficient;
  int _nonGoalWeightCoeffitientDenominator;
  int _nonGoalWeightCoeffitientNumerator;
  bool _nonliteralsInClauseWeight;
  bool _normalize;

  bool _outputAxiomNames;

  InliningMode _predicateDefinitionInlining;
  bool _predicateDefinitionMerging;
  PredicateEquivalenceDiscoveryMode _predicateEquivalenceDiscovery;
  bool _predicateEquivalenceDiscoveryAddImplications;
  bool _predicateEquivalenceDiscoveryRandomSimulation;
  int _predicateEquivalenceDiscoverySatConflictLimit;
  bool _predicateIndexIntroduction;
  bool _printClausifierPremises;
  vstring _problemName;
  Proof _proof;
  bool _proofChecking;
  
  vstring _protectedPrefix;

  QuestionAnsweringMode _questionAnswering;

  int _randomSeed;
  int _rowVariableMaxLength;

  float _satClauseActivityDecay;
  SatClauseDisposer _satClauseDisposer;
  bool _satLearntMinimization;
  bool _satLearntSubsumptionResolution;
  /** Lingeling incremental and similar models options */
  bool _satLingelingIncremental;
  bool _satLingelingSimilarModels;

  int _satRestartFixedCount;
  float _satRestartGeometricIncrease;
  int _satRestartGeometricInit;
  int _satRestartLubyFactor;
  float _satRestartMinisatIncrease;
  int _satRestartMinisatInit;
  SatRestartStrategy _satRestartStrategy;
  float _satVarActivityDecay;
  SatVarSelector _satVarSelector;
  SatSolver _satSolver;
  SaturationAlgorithm _saturationAlgorithm;
  int _selection;
  bool _selectUnusedVariablesFirst;
  bool _showActive;
  bool _showBlocked;
  bool _showDefinitions;
  InterpolantMode _showInterpolant;
  bool _showNew;
  bool _showNewPropositional;
  bool _showNonconstantSkolemFunctionTrace;
  bool _showOptions;
  bool _showPassive;
  bool _showPreprocessing;
  bool _showSkolemisations;
  bool _showSymbolElimination;
  bool _showTheoryAxioms;
  int _simulatedTimeLimit;
  unsigned _sineDepth;
  unsigned _sineGeneralityThreshold;
  SineSelection _sineSelection;
  float _sineTolerance;
  bool _smtlibConsiderIntsReal;
  bool _smtlibFletAsDefinition;
  bool _smtlibIntroduceAIGNames;
  Sos _sos;
  //bool _splitAddGroundNegation;
  bool _splitAtActivation;
  //bool _splitGoalOnly;
  //bool _splitInputOnly;
  //bool _splitPositive;
  bool _splitting;
  SplittingAddComplementary _splittingAddComplementary;
  bool _splittingCongruenceClosure;
  bool _splittingEagerRemoval;
  unsigned _splittingFlushPeriod;
  float _splittingFlushQuotient;
  SplittingNonsplittableComponents _splittingNonsplittableComponents;
  SplittingModel _splittingModel;
  Statistics _statistics;
  bool _superpositionFromVariables;
  SymbolPrecedence _symbolPrecedence;

  int _tabulationBwRuleSubsumptionResolutionByLemmas;
  int _tabulationFwRuleSubsumptionResolutionByLemmas;
  int _tabulationGoalAgeRatio;
  int _tabulationGoalWeightRatio;
  int _tabulationGoalRatio;
  int _tabulationLemmaRatio;
  bool _tabulationInstantiateProducingRules;
  int _tabulationLemmaAgeRatio;
  int _tabulationLemmaWeightRatio;
  vstring _testId;
  vstring _thanks;
  bool _theoryAxioms;
  /** Time limit in deciseconds */
  int _timeLimitInDeciseconds;
  bool _timeStatistics;
  bool _trivialPredicateRemoval;

  URResolution _unitResultingResolution;
  bool _unusedPredicateDefinitionRemoval;
  unsigned _updatesByOneConstraint;
  bool _use_dm;
  bool _weightIncrement;
  int _weightRatio;
  int _whileNumber;

  vstring _xmlOutput;

  bool _zeroOpt;

  // various read-from-string-write options
  static void readAgeWeightRatio(const char* val, int& ageRatio, int& weightRatio, char separator=':');
  static vstring boolToOnOff(bool);
  void outputValue(ostream& str,int optionTag) const;
  friend class Shell::LTB::Builder;

public:
  // the following two functions are used by Environment
  bool onOffToBool(const char* onOff,const char* option);
}; // class Options

}

#endif

