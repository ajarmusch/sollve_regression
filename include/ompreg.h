//===------ ompreg.h ------------------ OMPREG HEADER FILE ------------------===//
//
// Header file for SOLLVE Regression Tests 
// Borrowed from the SOLLVE Validation and Verification Testsuite
//
//===----------------------------------------------------------------------===//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
int _ompreg_isOffloadingOn = -1;

// Macro for output of information, warning and error messages
#ifdef VERBOSE_MODE
  #define OMPREG_WARNING(message, ...) { \
    printf("[OMPREG_WARNING: %s:%i] " message "\n", __FILENAME__, __LINE__, ##__VA_ARGS__); \
  }
  #define OMPREG_WARNING_IF(condition, message, ...) { \
    if(condition) { \
      printf("[OMPREG_WARNING: %s:%i] " message "\n", __FILENAME__, __LINE__, ##__VA_ARGS__); \
    } \
  }

  #define OMPREG_ERROR(message, ...) { \
    fprintf(stderr, "[OMPREG_ERROR: %s:%i] " message "\n", __FILENAME__, __LINE__, ##__VA_ARGS__); \
  }
  #define OMPREG_ERROR_IF(condition, message, ...) { \
    if(condition) { \
      fprintf(stderr, "[OMPREG_ERROR: %s:%i] " message "\n", __FILENAME__, __LINE__, ##__VA_ARGS__); \
    } \
  }

  #define OMPREG_INFOMSG(message, ...) { \
    printf("[OMPREG_INFO: %s:%i] " message "\n", __FILENAME__, __LINE__, ##__VA_ARGS__); \
  }
  #define OMPREG_INFOMSG_IF(condition, message, ...) { \
    if(condition) { \
      printf("[OMPREG_INFO: %s:%i] " message "\n", __FILENAME__, __LINE__, ##__VA_ARGS__); \
    } \
  }
#else
  #define OMPREG_WARNING(message, ...) {}
  #define OMPREG_WARNING_IF(message, ...) {}
  #define OMPREG_ERROR(message, ...) {}
  #define OMPREG_ERROR_IF(message, ...) {}
  #define OMPREG_INFOMSG(message, ...) {}
  #define OMPREG_INFOMSG_IF(message, ...) {}
#endif // END IF VERBOSE_MODE

#define OMPREG_TEST_OFFLOADING_PROBE \
  _ompreg_isOffloadingOn = 0; \
_Pragma("omp target map (from: _ompreg_isOffloadingOn)") \
  {  _ompreg_isOffloadingOn = !omp_is_initial_device();  }

// Macro for checking if offloading is enabled or not
#define OMPREG_TEST_OFFLOADING { \
  OMPREG_TEST_OFFLOADING_PROBE \
  OMPREG_INFOMSG("Test is running on %s.",(_ompreg_isOffloadingOn)? "device" : "host"); \
}

// Macro for checking if offloading is enabled or not and set a variable with the result
#define OMPREG_TEST_AND_SET_OFFLOADING(var2set) { \
  OMPREG_TEST_OFFLOADING_PROBE \
  OMPREG_INFOMSG("Test is running on %s.",(_ompreg_isOffloadingOn)? "device" : "host"); \
  var2set = _ompreg_isOffloadingOn; \
}
// Macro for setting errors on condition
#define OMPREG_TEST_AND_SET(err, condition) { \
  int _ompreg_conditionEval = condition; \
  err += (_ompreg_conditionEval); \
}
// Macro for setting errors on condition and displaying an error if something went wrong
#define OMPREG_TEST_AND_SET_VERBOSE(err, condition) { \
  int _ompreg_conditionEval = condition; \
  err += (_ompreg_conditionEval); \
  OMPREG_ERROR_IF(_ompreg_conditionEval, " Condition " #condition " failed "); \
}
// Macro for reporting results
#define OMPREG_REPORT(err) { \
  OMPREG_INFOMSG("The value of " #err " is %d.", err); \
  if (_ompreg_isOffloadingOn == -1) \
    printf("[OMPREG_RESULT: %s] Test %s.\n", __FILENAME__, ((err) == 0)? "passed":"failed"); \
  else \
    printf("[OMPREG_RESULT: %s] Test %s on the %s.\n", __FILENAME__, ((err) == 0)? "passed":"failed", (_ompreg_isOffloadingOn)? "device" : "host"); \
}

// Macro for correct exit code
#define OMPREG_RETURN(err) { \
  return ((err) == 0) ? EXIT_SUCCESS : EXIT_FAILURE; \
}

// Macro for report and exit
#define OMPREG_REPORT_AND_RETURN(err) {\
  OMPREG_REPORT(err); \
  OMPREG_RETURN(err); \
}

// Macro to check if it is a shared data environment
#define OMPREG_TEST_SHARED_ENVIRONMENT_PROBE \
  int _ompreg_isSharedEnv = 1; \
  _ompreg_isOffloadingOn = 0; \
_Pragma("omp target map (from: _ompreg_isOffloadingOn) map(to: _ompreg_isSharedEnv)") \
  {  _ompreg_isOffloadingOn = !omp_is_initial_device();  \
     _ompreg_isSharedEnv = 0; \
  }

// Macro to report warning if it is a shared environment
#define OMPREG_TEST_SHARED_ENVIRONMENT {\
  OMPREG_TEST_SHARED_ENVIRONMENT_PROBE \
  OMPREG_WARNING_IF((_ompreg_isOffloadingOn && _ompreg_isSharedEnv == 0),"This tests is running on a shared data environment between host and device. This may cause errors") \
  }

// Macro to report warning if it is a shared environment and set a variable for further use
#define OMPREG_TEST_AND_SET_SHARED_ENVIRONMENT(var2set) {\
  OMPREG_TEST_SHARED_ENVIRONMENT_PROBE \
  OMPREG_WARNING_IF((_ompreg_isOffloadingOn && _ompreg_isSharedEnv == 0),"This tests is running on a shared data environment between host and device. This may cause errors") \
  var2set = (_ompreg_isOffloadingOn && _ompreg_isSharedEnv == 0);\
  }

// Macros to provide thread and team nums if they are not specified
#ifndef OMPREG_NUM_THREADS_DEVICE
  #define OMPREG_NUM_THREADS_DEVICE 8
#endif

#ifndef OMPREG_NUM_TEAMS_DEVICE
  #define OMPREG_NUM_TEAMS_DEVICE 8
#endif

#ifndef OMPREG_NUM_TEAMS_HOST
  #define OMPREG_NUM_TEAMS_HOST 4
#endif

#ifndef OMPREG_NUM_THREADS_HOST
  #define OMPREG_NUM_THREADS_HOST 8
#endif
