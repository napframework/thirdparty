#ifndef TURBOACTIVATE_H_INCLUDED
#define TURBOACTIVATE_H_INCLUDED

/*
  Scroll down below the list of includes & defines for
  a list of every function and error code with full
  detailed descriptions of everything.
*/


/* For size_t definition */
#include <stddef.h>

/* Define "uint32_t" for old versions of MS C++ (VS 2008 and below) */
#if !defined(_WIN32) || _MSC_VER >= 1600
#    include <stdint.h>
#else
typedef unsigned int  uint32_t;
typedef unsigned char uint8_t;
#endif

#ifdef _WIN32
#    include <windows.h>

#    ifdef TURBOACTIVATE_EXPORTS
#        ifdef TURBOACTIVATE_STATIC
#            define TURBOACTIVATE_API extern "C"
#        else
#            define TURBOACTIVATE_API extern "C" __declspec(dllexport)
#        endif
#    else
#        ifdef __cplusplus
#            ifdef TURBOACTIVATE_STATIC
#                define TURBOACTIVATE_API extern "C"
#            else
#                define TURBOACTIVATE_API extern "C" __declspec(dllimport)
#            endif
#        else
#            ifdef TURBOACTIVATE_STATIC
#                define TURBOACTIVATE_API
#            else
#                define TURBOACTIVATE_API __declspec(dllimport)
#            endif
#        endif
#    endif

#    if defined(USE_STDCALL_TA_DLL) && !defined(TURBOACTIVATE_STATIC)
#        define TA_CC __stdcall
#    else
#        define TA_CC __cdecl
#    endif
#    define TA_DEPRECATED(func) __declspec(deprecated) func

#    ifndef LIMELM_TYPES
#        define LIMELM_TYPES
typedef LPWSTR  STRTYPE;
typedef LPCWSTR STRCTYPE;
#    endif
#else
#    if __GNUC__ >= 4 || defined(__SUNPRO_CC)
#        ifdef __cplusplus
#            define TURBOACTIVATE_API extern "C" __attribute__((visibility("default")))
#        else
#            define TURBOACTIVATE_API __attribute__((visibility("default")))
#        endif
#    else
#        ifdef __cplusplus
#            define TURBOACTIVATE_API extern "C"
#        else
#            define TURBOACTIVATE_API
#        endif
#    endif

#    define TA_CC
#    ifdef __GNUC__
#        define TA_DEPRECATED(func) func __attribute__((deprecated))
#    else
#        pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#        define TA_DEPRECATED(func) func
#    endif

#    ifndef LIMELM_TYPES
#        define LIMELM_TYPES
typedef char *        STRTYPE;
typedef const char *  STRCTYPE;
typedef int32_t       HRESULT;
typedef char          TCHAR;
#    endif

#    ifdef __ANDROID__
#        include <jni.h>
#    endif
#endif


typedef struct _ACTIVATE_OPTIONS
{
    /* The size, in bytes, of this structure. Set this value to
    the size of the ACTIVATE_OPTIONS structure. */
    uint32_t nLength;

    /* Extra data to pass to the LimeLM servers that will be visible for
    you to see and use. Maximum size is 255 UTF-8 characters. */
    STRCTYPE sExtraData;
} ACTIVATE_OPTIONS, *PACTIVATE_OPTIONS;


/* Flags for the TA_IsGeninueEx() function. */

/* If the user activated using offline activation
   (TA_ActivateRequestToFile(), TA_ActivateFromFile() ), then with this
   flag TA_IsGenuineEx() will still try to validate with the LimeLM
   servers, however instead of returning TA_E_INET (when within the
   grace period) or TA_FAIL (when past the grace period) it will
   instead only return TA_OK (if machine has a valid activation).

   If you still want to get the TA_E_INET error code, without
   deactivating after the grace period has expired, then use
   this flag in tandem with TA_OFFLINE_SHOW_INET_ERR.

   If the user activated using online activation then this flag
   is ignored.
*/
#define TA_SKIP_OFFLINE ((uint32_t)1)

/* If the user activated using offline activation, and you're
   using this flag in tandem with TA_SKIP_OFFLINE, then TA_IsGenuineEx()
   will return TA_E_INET on internet failure instead of TA_OK.

   If the user activated using online activation then this flag
   is ignored.
*/
#define TA_OFFLINE_SHOW_INET_ERR ((uint32_t)2)

typedef struct _GENUINE_OPTIONS
{
    /* The size, in bytes, of this structure. Set this value to
    the size of the GENUINE_OPTIONS structure. */
    uint32_t nLength;

    /* Flags to pass to TA_IsGenuineEx() to control its behavior. */
    uint32_t flags;

    /* How often to contact the LimeLM servers for validation. 90 days recommended. */
    uint32_t nDaysBetweenChecks;

    /* If the call fails because of an internet error,
    how long, in days, should the grace period last (before
    returning deactivating and returning TA_FAIL).

    14 days is recommended. */
    uint32_t nGraceDaysOnInetErr;
} GENUINE_OPTIONS, *PGENUINE_OPTIONS;


/* Flags for the TA_UseTrial() and TA_CheckAndSavePKey() functions. */
#define TA_SYSTEM ((uint32_t)1)
#define TA_USER ((uint32_t)2)

/*
When to use TA_USER or TA_SYSTEM
=======================================

TA_USER saves the activation or trial data in the current user's profile if
(and only if) calling the function with TA_SYSTEM had not previously occurred.

TA_SYSTEM saves the activation or trial data in a place accessible by all user
profiles on the system.


In both cases the product key is activated and "locked" to the machine. The
only difference between TA_SYSTEM and TA_USER is where the files are stored.
In other words if one users activates your app on a machine using TA_USER,
another user can activate your app on the same machine, using the same product
key.

If you want to license your software per-user on machines, then use TurboFloat:

https://wyday.com/limelm/help/using-turbofloat/



For almost all cases we recommend using TA_SYSTEM.
*/


/* TA_UseTrial() specific flags:
   =============================
*/

/*
   Use the TA_DISALLOW_VM flag to disallow trials in virtual machines.
   If you use this flag in TA_UseTrial() and the customer's machine is a Virtual
   Machine, then TA_UseTrial() will return TA_E_IN_VM.
*/
#define TA_DISALLOW_VM ((uint32_t)4)

/*
   Use this flag in TA_UseTrial() to tell TurboActivate to use client-side
   unverified trials. For more information about verified vs. unverified trials,
   see here: https://wyday.com/limelm/help/trials/
   Note: unverified trials are unsecured and can be reset by malicious customers.
*/
#define TA_UNVERIFIED_TRIAL ((uint32_t)16)

/*
   Use the TA_VERIFIED_TRIAL flag to use verified trials instead
   of unverified trials. This means the trial is locked to a particular computer.
   The customer can't reset the trial.
*/
#define TA_VERIFIED_TRIAL ((uint32_t)32)


/*
   Gets the handle that will be used for the TurboActivate functions.
   
   
   Returns: 0 on failure. All other return codes are successful.
   
   If you're getting a "0" return code then it's most likely a result
   of TurboActivate being unable to load your TurboActivate.dat file. In
   that case use TA_PDetsFromPath to load the file first before calling
   TA_GetHandle.
*/
TURBOACTIVATE_API uint32_t TA_CC TA_GetHandle(STRCTYPE versionGUID);


/*
   Activates the product on this computer. You must call TA_CheckAndSavePKey()
   with a valid product key or have used the TurboActivate Wizard sometime
   before calling this function.

   If you don't want to pass in extra data you can set "options" to null.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_PKEY, TA_E_INET, TA_E_INET_TIMEOUT,
                          TA_E_INET_TLS, TA_E_INUSE, TA_E_REVOKED,
                          TA_E_INVALID_HANDLE, TA_E_COM, TA_E_EXPIRED,
                          TA_E_EDATA_LONG, TA_E_INVALID_ARGS, TA_E_IN_VM,
                          TA_E_KEY_FOR_TURBOFLOAT, TA_E_ANDROID_NOT_INIT,
                          TA_E_ACCOUNT_CANCELED, TA_E_ENABLE_NETWORK_ADAPTERS
*/
TURBOACTIVATE_API HRESULT TA_CC TA_Activate(uint32_t handle, PACTIVATE_OPTIONS options);


/*
   Get the "activation request" file for offline activation. You must call TA_CheckAndSavePKey()
   with a valid product key or have used the TurboActivate Wizard sometime
   before calling this function.

   If you don't want to pass in extra data you can set "options" to null.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_PKEY, TA_E_INVALID_HANDLE, TA_E_COM,
                          TA_E_EDATA_LONG, TA_E_INVALID_ARGS, TA_E_ANDROID_NOT_INIT,
                          TA_E_ENABLE_NETWORK_ADAPTERS
*/
TURBOACTIVATE_API HRESULT TA_CC TA_ActivationRequestToFile(uint32_t handle, STRCTYPE filename, PACTIVATE_OPTIONS options);


/*
   Activate from the "activation response" file for offline activation.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_PKEY, TA_E_INVALID_HANDLE, TA_E_COM,
                          TA_E_EXPIRED, TA_E_IN_VM, TA_E_ANDROID_NOT_INIT,
                          TA_E_ENABLE_NETWORK_ADAPTERS, TA_E_INVALID_ARGS
*/
TURBOACTIVATE_API HRESULT TA_CC TA_ActivateFromFile(uint32_t handle, STRCTYPE filename);


/*
   Checks and saves the product key.


   Note: If you pass in the TA_SYSTEM flag and you don't have "admin" or "elevated"
   permission then the call will fail.

   If you call this function once in the past with the flag TA_SYSTEM and the calling
   process was an admin process then subsequent calls with the TA_SYSTEM flag will
   succeed even if the calling process is *not* admin/elevated.

   If you want to take advantage of this behavior from an admin process
   (e.g. an installer) but the user hasn't entered a product key then you can
   call this function with a null string:

            TA_CheckAndSavePKey(yourHandle, 0, TA_SYSTEM);

   This will set everything up so that subsequent calls with the TA_SYSTEM flag will
   succeed even if from non-admin processes.

   Note: Calling TA_CheckAndSavePKey(yourHandle, 0, TA_SYSTEM) will return TA_FAIL
         if the system files were correctly setup. It will return TA_E_PERMISSION if
         the system files were not setup due to lack of permissions. The reason it
         returns TA_FAIL instead of TA_OK is because passing in a "NULL" product key
         is not valid.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_PERMISSION, TA_E_INVALID_FLAGS
                          TA_E_ALREADY_ACTIVATED
*/
TURBOACTIVATE_API HRESULT TA_CC TA_CheckAndSavePKey(uint32_t handle, STRCTYPE productKey, uint32_t flags);


/*
   Deactivates the product on this computer. Set erasePkey to 1 to erase the stored
   product key, 0 to keep the product key around. If you're using deactivate to let
   a user move between computers it's almost always best to *not* erase the product
   key. This way you can just use TA_Activate() when the user wants to reactivate
   instead of forcing the user to re-enter their product key over-and-over again.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_PKEY, TA_E_ACTIVATE, TA_E_INET,
                          TA_E_INET_TIMEOUT, TA_E_INET_TLS, TA_E_INVALID_HANDLE,
                          TA_E_COM, TA_E_ANDROID_NOT_INIT,
                          TA_E_NO_MORE_DEACTIVATIONS, TA_E_INVALID_ARGS,
                          TA_E_ENABLE_NETWORK_ADAPTERS
*/
TURBOACTIVATE_API HRESULT TA_CC TA_Deactivate(uint32_t handle, char erasePkey);


/*
   Get the "deactivation request" file for offline deactivation.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_PKEY, TA_E_ACTIVATE, TA_E_INVALID_HANDLE,
                          TA_E_COM, TA_E_ANDROID_NOT_INIT, TA_E_INVALID_ARGS,
                          TA_E_ENABLE_NETWORK_ADAPTERS
*/
TURBOACTIVATE_API HRESULT TA_CC TA_DeactivationRequestToFile(uint32_t handle, STRCTYPE filename, char erasePkey);


/*
   Gets the extra data you passed in using TA_Activate().


   lpValueStr
   [out] Pointer to a buffer that receives the value of the string.

   cchValue
   [in] Size (in wide characters on Windows or characters in Unix) of the buffer
        pointed to by the lpValueStr parameter.

   If 'cchValue' is zero, the function returns the required buffer size (in wide characters
   on Windows, characters on Unix) and makes no use of the lpValueStr buffer.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_INVALID_HANDLE, TA_E_INSUFFICIENT_BUFFER
*/
TURBOACTIVATE_API HRESULT TA_CC TA_GetExtraData(uint32_t handle, STRTYPE lpValueStr, int cchValue);


/*
   Gets the value of a custom license field.

   More information on custom license fields: https://wyday.com/limelm/help/license-features/


   lpValueStr
   [out] Pointer to a buffer that receives the value of the string.

   cchValue
   [in] Size (in wide characters on Windows or characters in Unix) of the buffer
        pointed to by the lpValueStr parameter.

   If 'cchValue' is zero, the function returns the required buffer size (in wide characters
   on Windows, characters on Unix) and makes no use of the lpValueStr buffer.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_INVALID_HANDLE, TA_E_INSUFFICIENT_BUFFER
*/
TURBOACTIVATE_API HRESULT TA_CC TA_GetFeatureValue(uint32_t handle, STRCTYPE featureName, STRTYPE lpValueStr, int cchValue);


/*
   Gets the stored product key. NOTE: if you want to check if a product key is valid
   simply call TA_IsProductKeyValid().


   lpValueStr
   [out] Pointer to a buffer that receives the value of the string.

   cchValue
   [in] Size (in wide characters on Windows or characters in Unix) of the buffer
        pointed to by the lpValueStr parameter.

   If 'cchValue' is zero, the function returns the required buffer size (in wide characters
   on Windows, characters on Unix) and makes no use of the lpValueStr buffer.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_PKEY, TA_E_INVALID_HANDLE,
                          TA_E_INSUFFICIENT_BUFFER
*/
TURBOACTIVATE_API HRESULT TA_CC TA_GetPKey(uint32_t handle, STRTYPE lpValueStr, int cchValue);


/*
   Checks whether the computer is activated. This does not re-verify with the activation
   servers. It just checks the cryptographically-signed activation data stored on the computer
   and verify the current hardware-fingerprint "fuzzy matches" the signed activation data.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_INVALID_HANDLE, TA_E_COM,
                          TA_E_IN_VM, TA_E_ANDROID_NOT_INIT, TA_E_ENABLE_NETWORK_ADAPTERS
*/
TURBOACTIVATE_API HRESULT TA_CC TA_IsActivated(uint32_t handle);


/*
   Checks whether the computer is genuinely activated by verifying with the LimeLM servers immediately.
   If reactivation is needed then it will do this as well.

   For almost all use-cases you should use TA_IsGenuineEx(), not this function.


   Returns: TA_OK or TA_E_FEATURES_CHANGED on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_ACTIVATE, TA_E_INET, TA_E_INET_TIMEOUT, TA_E_INET_TLS,
                          TA_E_INVALID_HANDLE, TA_E_COM, TA_E_EXPIRED, TA_E_REVOKED, TA_E_IN_VM,
                          TA_E_FEATURES_CHANGED, TA_E_ANDROID_NOT_INIT, TA_E_ENABLE_NETWORK_ADAPTERS
*/
TURBOACTIVATE_API HRESULT TA_CC TA_IsGenuine(uint32_t handle);


/*
   Checks whether the computer is genuinely activated by verifying with the LimeLM servers
   after a certain number of days you specify.

   This is meant as a replacement of both TA_IsActivated() and TA_IsGenuine(). Call this at the
   top of your program and let TA_IsGenuineEx() handle all the details.

   This differs with TA_IsGenuine() in 3 major ways:

        1. You can specify how often to verify with the LimeLM servers and it handles
           all the date tracking behind the scenes.


        2. TA_IsGenuineEx() prevents your app from hammering the end-user's network after
           and TA_E_INET error return code by not checking with the LimeLM servers until
           at least 5 hours has passed. If you call TA_IsGenuineEx() after a TA_E_INET return
           and before 5 hours has elapsed then this function will return TA_E_INET_DELAYED.

           (If you give the user the option to recheck with LimeLM, e.g. via a button
           like "Retry now" then call TA_IsGenuine() to immediately retry without waiting 5 hours).


        3. If a TA_E_INET error is being returned, and the grace period has expired,
           then TA_IsGenuineEx() will return TA_FAIL. TA_IsGenuineEx() will continue to try
           contacting the LimeLM servers on subsequent calls (5 hours apart), but you
           should treat the TA_FAIL as a hard failure.


   Returns: TA_OK or TA_E_FEATURES_CHANGED on success. Handle TA_E_INET and TA_E_INET_DELAYED as warnings that
            you should let the end user know about.

            Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_ACTIVATE, TA_E_INET, TA_E_INET_TIMEOUT,
                          TA_E_INET_TLS, TA_E_INET_DELAYED, TA_E_INVALID_HANDLE,
                          TA_E_COM, TA_E_EXPIRED, TA_E_REVOKED, TA_E_INVALID_ARGS,
                          TA_E_INVALID_FLAGS, TA_E_IN_VM, TA_E_FEATURES_CHANGED,
                          TA_E_ANDROID_NOT_INIT, TA_E_ENABLE_NETWORK_ADAPTERS
*/
TURBOACTIVATE_API HRESULT TA_CC TA_IsGenuineEx(uint32_t handle, PGENUINE_OPTIONS options);


/*
   Get the number of days until the next time that the TA_IsGenuineEx() function contacts
   the LimeLM activation servers to reverify the activation.

   You must use the same "nDaysBetweenChecks" and "nGraceDaysOnInetErr" parameters you passed
   to TA_IsGenuineEx() using the GENUINE_OPTIONS structure.

   The number of days remaining until the next reverification with the servers will be put
   in the "DaysRemaining" variable. And if the customer is already in the grace period, then
   the "DaysRemaining" remaining will reflect the number of days left in the grace period and 
   "inGracePeriod" will be 1.

   If both nDaysBetweenChecks and the nGraceDaysOnInetErr have passed, then "DaysRemaining"
   will be 0.

   Also, if TurboActivate determines that system date, time, or timezone are fraudulent then
   "DaysRemaining" will be 0.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_ACTIVATE, TA_E_INVALID_HANDLE
*/
TURBOACTIVATE_API HRESULT TA_CC TA_GenuineDays(uint32_t handle, uint32_t nDaysBetweenChecks, uint32_t nGraceDaysOnInetErr, uint32_t * DaysRemaining, char * inGracePeriod);


/*
   Checks if the product key installed for this product is valid. This does NOT check if
   the product key is activated or genuine. Use TA_IsActivated() and TA_IsGenuine() instead.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_INVALID_HANDLE
*/
TURBOACTIVATE_API HRESULT TA_CC TA_IsProductKeyValid(uint32_t handle);


/*
   Sets the custom proxy to be used by functions that connect to the internet.


   Proxy in the form: http://username:password@host:port/

   Example 1 (just a host): http://127.0.0.1/
   Example 2 (host and port): http://127.0.0.1:8080/
   Example 3 (all 3): http://user:pass@127.0.0.1:8080/

   If the port is not specified, TurboActivate will default to using port 1080 for proxies.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL
*/
TURBOACTIVATE_API HRESULT TA_CC TA_SetCustomProxy(STRCTYPE proxy);


/*
   Get the number of trial days remaining.
   0 days if the trial has expired or has been tampered with
   (1 day means *at most* 1 day, that is it could be 30 seconds)

   You must call TA_UseTrial() at least once in the past before calling this function.
   And you must call this function with the same flags you used with TA_UseTrial().


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_INVALID_HANDLE, TA_E_ALREADY_VERIFIED_TRIAL,
                          TA_E_MUST_USE_TRIAL, TA_E_MUST_SPECIFY_TRIAL_TYPE
*/
TURBOACTIVATE_API HRESULT TA_CC TA_TrialDaysRemaining(uint32_t handle, uint32_t useTrialFlags, uint32_t * DaysRemaining);


/*
   Begins the trial the first time it's called. Calling it again will validate the trial
   data hasn't been tampered with.

   It is recommended that you use the following flag combination: TA_VERIFIED_TRIAL | TA_SYSTEM


   Note: If you pass in the TA_SYSTEM flag and you don't have "admin" or "elevated"
   permission then the call will fail.

   If you call this function once in the past with the flag TA_SYSTEM and the calling
   process was an admin process then subsequent calls with the TA_SYSTEM flag will
   succeed even if the calling process is *not* admin/elevated.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_INVALID_HANDLE, TA_E_INET, TA_E_INET_TIMEOUT,
                          TA_E_INET_TLS, TA_E_PERMISSION, TA_E_COM, TA_E_INVALID_FLAGS,
                          TA_E_IN_VM, TA_E_ANDROID_NOT_INIT,
                          TA_E_ACCOUNT_CANCELED, TA_E_ENABLE_NETWORK_ADAPTERS,
                          TA_E_ALREADY_VERIFIED_TRIAL, TA_E_EXPIRED, TA_E_TRIAL_EXPIRED,
                          TA_E_MUST_SPECIFY_TRIAL_TYPE, TA_E_EDATA_LONG,
                          TA_E_NO_MORE_TRIALS_ALLOWED
*/
TURBOACTIVATE_API HRESULT TA_CC TA_UseTrial(uint32_t handle, uint32_t flags, STRCTYPE extra_data);


/*
   Generate a "verified trial" offline request file. This file will then need to be submitted to LimeLM.
   You will then need to use the TA_UseTrialVerifiedFromFile() function with the response file from LimeLM
   to actually start the trial.


   The easier solution is to use verified trials online. Simply use the TA_UseTrial() function with
   the TA_VERIFIED_TRIAL | TA_SYSTEM flags.



   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_INVALID_HANDLE, TA_E_PERMISSION, TA_E_COM
                          TA_E_ANDROID_NOT_INIT, TA_E_ENABLE_NETWORK_ADAPTERS,
                          TA_E_EDATA_LONG, TA_E_INVALID_ARGS
*/
TURBOACTIVATE_API HRESULT TA_CC TA_UseTrialVerifiedRequest(uint32_t handle, STRCTYPE filename, STRCTYPE extra_data);


/*
   Use the "verified trial response" from LimeLM to start the verified trial.



   To use verified trials online, simply use the TA_UseTrial() function with the TA_VERIFIED_TRIAL | TA_SYSTEM
   flags.


   Note: If you pass in the TA_SYSTEM flag and you don't have "admin" or "elevated"
   permission then the call will fail.

   If you call this function once in the past with the flag TA_SYSTEM and the calling
   process was an admin process then subsequent calls with the TA_SYSTEM flag will
   succeed even if the calling process is *not* admin/elevated.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_INVALID_HANDLE, TA_E_PERMISSION, TA_E_COM
                          TA_E_INVALID_FLAGS, TA_E_IN_VM,
                          TA_E_ANDROID_NOT_INIT, TA_E_ENABLE_NETWORK_ADAPTERS,
                          TA_E_MUST_SPECIFY_TRIAL_TYPE, TA_E_INVALID_ARGS
*/
TURBOACTIVATE_API HRESULT TA_CC TA_UseTrialVerifiedFromFile(uint32_t handle, STRCTYPE filename, uint32_t flags);


/*
   Extends the trial using a trial extension created in LimeLM.

   You must call TA_UseTrial() at least once in the past before calling this function.
   And you must call this function with the same flags you used with TA_UseTrial().


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_INET, TA_E_INET_TIMEOUT, TA_E_INET_TLS,
                          TA_E_INVALID_HANDLE, TA_E_TRIAL_EUSED, TA_E_TRIAL_EEXP,
                          TA_E_MUST_USE_TRIAL, TA_E_MUST_SPECIFY_TRIAL_TYPE
*/
TURBOACTIVATE_API HRESULT TA_CC TA_ExtendTrial(uint32_t handle, uint32_t useTrialFlags, STRCTYPE trialExtension);


/*
The lease callback function type with a user-defined pointer.
*/
typedef void(TA_CC * TrialCallbackType)(uint32_t, void *);

/*
   Set the function that will be called by a separate background thread that notifies
   your app of trial expiration (either naturally, or because of customer fraud).

   TA_SetTrialCallback() also allow you to pass allong "context" in the form
   of a pointer that you defined. This is passed back along by the TurboActivate
   Library when the callback function is called.

   The trial callback function should handle everything defined below (see:
   "Possible callback statuses" at the bottom of this header).


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_INVALID_HANDLE
*/
TURBOACTIVATE_API HRESULT TA_CC TA_SetTrialCallback(uint32_t handle, TrialCallbackType callback, void * userDefinedPtr);


/*
   Loads the "TurboActivate.dat" file from a path rather than loading it
   from the same dir as TurboActivate.dll on Windows or the app that
   uses libTurboActivate.dylib / libTurboActivate.so on Mac / Linux.

   You can load multiple *.dat files for licensing multiple products within
   one process. You can switch between the products by using the
   TA_GetHandle() with the VersionGUID of the product you want to use
   licensing for.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_PDETS


   Note: Once the product details file has loaded, all subsequent calls to "PDetsFromPath"
   will return TA_FAIL
*/
TURBOACTIVATE_API HRESULT TA_CC TA_PDetsFromPath(STRCTYPE filename);


/*
   Loads the "TurboActivate.dat" file from a byte array rather than loading it
   from the same dir as TurboActivate.dll on Windows or the app that
   uses libTurboActivate.dylib / libTurboActivate.so on Mac / Linux.

   You can load multiple *.dat files for licensing multiple products within
   one process. You can switch between the products by using the
   TA_GetHandle() with the VersionGUID of the product you want to use
   licensing for.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_PDETS


   Note: Once the product details file has loaded, all subsequent calls to "PDetsFromPath"
   will return TA_FAIL
*/
TURBOACTIVATE_API HRESULT TA_CC TA_PDetsFromByteArray(const uint8_t * byteArr, size_t byteArrLen);


/*
   This function allows you to set a custom folder to store the activation
   data files. For normal use we do not recommend you use this function.

   Only use this function if you absolutely must store data into a separate
   folder. For example if your application runs on a USB drive and can't write
   any files to the main disk, then you can use this function to save the activation
   data files to a directory on the USB disk.

   If you are using this function (which we only recommend for very special use-cases)
   then you must call this function on every start of your program at the very top of
   your app before any other functions are called.

   The directory you pass in must already exist. And the process using TurboActivate
   must have permission to create, write, and delete files in that directory.


   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_INVALID_HANDLE
*/
TURBOACTIVATE_API HRESULT TA_CC TA_SetCustomActDataPath(uint32_t handle, STRCTYPE directory);


/* Flags for the TA_IsDateValid() function. */

/* With this flag, TA_IsDateValid() will return TA_OK if the passed in "date_time"
   has not expired and the system dates have not been tampered with. Otherwise, 
   TA_FAIL will be returned.
*/
#define TA_HAS_NOT_EXPIRED ((uint32_t)1)


/*
   Checks if the string in the form "YYYY-MM-DD HH:mm:ss" is a valid
   date/time. The date must be in UTC time and "24-hour" format. If your
   date is in some other time format first convert it to UTC time before
   passing it into this function.

   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL, TA_E_INVALID_FLAGS, TA_E_INVALID_HANDLE
*/
TURBOACTIVATE_API HRESULT TA_CC TA_IsDateValid(uint32_t handle, STRCTYPE date_time, uint32_t flags);


/*
   Gets the version number of the currently used TurboActivate library.
   This is a useful alternative for platforms which don't support file meta-data
   (like Linux, FreeBSD, and other unix variants).

   The version format is:  Major.Minor.Build.Revision

   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL
*/
TURBOACTIVATE_API HRESULT TA_CC TA_GetVersion(uint32_t * MajorVersion, uint32_t * MinorVersion, uint32_t * BuildVersion, uint32_t * RevisionVersion);


#ifdef __ANDROID__
/*
   If you're using TurboActivate from Android, and you're not using the JNI interface,
   then you must first initialize TurboActivate to use JNIEnv from your app. For purely
   native apps you can get this using the "android_main" function.

   Returns: TA_OK on success. Handle all other return codes as failures.

   Possible return codes: TA_OK, TA_FAIL
*/
TURBOACTIVATE_API HRESULT TA_CC TA_InitAndroid(JNIEnv * env);
#endif


/*
    General Success and Failure return codes.
*/
#define TA_OK ((HRESULT)0L)
#define TA_FAIL ((HRESULT)1L)


/*
 MessageId: TA_E_PKEY
 Message code (in Hex): 0x2
 Message code (in Decimal): 2

 MessageText:

 Invalid product key
*/
#define TA_E_PKEY ((HRESULT)0x00000002L)


/*
 MessageId: TA_E_ACTIVATE
 Message code (in Hex): 0x3
 Message code (in Decimal): 3

 MessageText:

 The product needs to be activated.
*/
#define TA_E_ACTIVATE ((HRESULT)0x00000003L)


/*
 MessageId: TA_E_INET
 Message code (in Hex): 0x4
 Message code (in Decimal): 4

 MessageText:

 Connection to the server failed.

 More information here: https://wyday.com/limelm/help/faq/#internet-error
*/
#define TA_E_INET ((HRESULT)0x00000004L)


/*
 MessageId: TA_E_INUSE
 Message code (in Hex): 0x5
 Message code (in Decimal): 5

 MessageText:

 The product key has already been activated with the maximum number of computers.
*/
#define TA_E_INUSE ((HRESULT)0x00000005L)


/*
 MessageId: TA_E_REVOKED
 Message code (in Hex): 0x6
 Message code (in Decimal): 6

 MessageText:

 The product key has been revoked.
*/
#define TA_E_REVOKED ((HRESULT)0x00000006L)


/*
 MessageId: TA_E_PDETS
 Message code (in Hex): 0x8
 Message code (in Decimal): 8

 MessageText:

 The product details file "TurboActivate.dat" failed to load.

 On Windows the TurboActivate.dat must be in the same folder as the TurboActivate.dll file.
 On Unix (Linux / Mac OS X / etc.) the TurboActivate.dat file must be in the same
 folder as the executable calling the TurboActivate functions. Also, on Windows,
 if you're using static linking with the TurboActivate library then the TurboActivate.dat
 must be in the same folder as the executable (or DLL) using the TurboActivate library
 functions.

 On Mac OS X this can be confusing because a ".app" file isn't really a file
 at all, it's just a folder (or "bundle") that contains assets, and a few levels
 of directories like this:

 MyApp.app/
     Contents/
         Info.plist
         MacOS/
         Resources/

 In a Mac OS X application bundle the "actual executable" is inside the "MacOS"
 folder. That's where your should put TurboActivate.dat.


 If you can't or don't want to include TurboActivate.dat alongside TurboActivate.dll
 on Windows or alongside your app on Unix, or you just want to rename TurboActivate.dat
 to something else, then your only option is to call TA_PDetsFromPath().
*/
#define TA_E_PDETS ((HRESULT)0x00000008L)


/*
 MessageId: TA_E_TRIAL
 Message code (in Hex): 0x9
 Message code (in Decimal): 9

 MessageText:

 The trial data has been corrupted, using the oldest date possible.
*/
#define TA_E_TRIAL ((HRESULT)0x00000009L)


/*
 MessageId: TA_E_TRIAL_EUSED
 Message code (in Hex): 0xC
 Message code (in Decimal): 12

 MessageText:

 The trial extension has already been used.
*/
#define TA_E_TRIAL_EUSED ((HRESULT)0x0000000CL)


/*
 MessageId: TA_E_EXPIRED
 Message code (in Hex): 0xD
 Message code (in Decimal): 13

 MessageText:

 The activation has expired or the system time has been tampered
 with. Ensure your time, timezone, and date settings are correct.

 If you know for sure the offline activation (or offline verified trial)
 has not expired, then the problem is fraudulent date/time/timezone.
 To fix the problem do the following in order, don't skip a step:

 1. Fix the timezone on the device.
 2. Fix the date on the device.
 3. Fix the time on the device.
 4. Restart the device.

 Step 1 is critical. As is step 4.
*/
#define TA_E_EXPIRED ((HRESULT)0x0000000DL)


/*
 MessageId: TA_E_TRIAL_EEXP
 Message code (in Hex): 0xD
 Message code (in Decimal): 13

 MessageText:

 The trial extension has expired.
*/
#define TA_E_TRIAL_EEXP ((HRESULT)0x0000000DL)


/*
 MessageId: TA_E_COM
 Message code (in Hex): 0xB
 Message code (in Decimal): 11

 MessageText:

 The hardware id couldn't be generated due to an error in the COM setup.
 Re-enable Windows Management Instrumentation (WMI) in your group policy
 editor or reset the local group policy to the default values. Contact
 your system admin for more information.

 This error is Windows only.

 This error can also be caused by the user (or another program) disabling
 the "Windows Management Instrumentation" service. Make sure the "Startup type"
 is set to Automatic and then start the service.


 To further debug WMI problems open the "Computer Management" (compmgmt.msc),
 expand the "Services and Applications", right click "WMI Control" click
 "Properties" and view the status of the WMI.
*/
#define TA_E_COM ((HRESULT)0x0000000BL)


/*
 MessageId: TA_E_INSUFFICIENT_BUFFER
 Message code (in Hex): 0xE
 Message code (in Decimal): 14

 MessageText:

 The the buffer size was too small. Create a larger buffer and try again.
*/
#define TA_E_INSUFFICIENT_BUFFER ((HRESULT)0x0000000EL)


/*
 MessageId: TA_E_PERMISSION
 Message code (in Hex): 0xF
 Message code (in Decimal): 15

 MessageText:

 Insufficient system permission. Either start your process as an
 admin / elevated user or call the function again with the
 TA_USER flag instead of the TA_SYSTEM flag.

 If you get this TA_E_PERMISSION error from a function that doesn't
 accept flags (or doesn't accept TA_USER / TA_SYSTEM flags) then
 the problem is that something on the system messed with the permissions
 of the files and TurboActivate failed to fix the problem. Run your app,
 and thus the function, from an admin/elevated instance to let TurboActivate
 fix the problem for the customer.
*/
#define TA_E_PERMISSION ((HRESULT)0x0000000FL)


/*
 MessageId: TA_E_INVALID_FLAGS
 Message code (in Hex): 0x10
 Message code (in Decimal): 16

 MessageText:

 The flags you passed to TA_CheckAndSavePKey(...) or TA_UseTrial(...)
 were invalid (or missing). Flags like "TA_SYSTEM" and "TA_USER"
 are mutually exclusive -- you can only use one or the other.
*/
#define TA_E_INVALID_FLAGS ((HRESULT)0x00000010L)


/*
 MessageId: TA_E_IN_VM
 Message code (in Hex): 0x11
 Message code (in Decimal): 17

 MessageText:

 The function failed because this instance of your program
 if running inside a virtual machine / hypervisor and you've
 prevented the function from running inside a VM.
*/
#define TA_E_IN_VM ((HRESULT)0x00000011L)

/*
 MessageId: TA_E_EDATA_LONG
 Message code (in Hex): 0x12
 Message code (in Decimal): 18

 MessageText:

 The "extra data" was too long. You're limited to 255 UTF-8 characters.
 Or, on Windows, a Unicode string that will convert into 255 UTF-8
 characters or less.
*/
#define TA_E_EDATA_LONG ((HRESULT)0x00000012L)


/*
 MessageId: TA_E_INVALID_ARGS
 Message code (in Hex): 0x13
 Message code (in Decimal): 19

 MessageText:

 The arguments passed to the function are invalid. Double check your logic.
*/
#define TA_E_INVALID_ARGS ((HRESULT)0x00000013L)


/*
 MessageId: TA_E_KEY_FOR_TURBOFLOAT
 Message code (in Hex): 0x14
 Message code (in Decimal): 20

 MessageText:

 The product key used is for TurboFloat, not TurboActivate.
*/
#define TA_E_KEY_FOR_TURBOFLOAT ((HRESULT)0x00000014L)


/*
 MessageId: TA_E_INET_DELAYED
 Message code (in Hex): 0x15
 Message code (in Decimal): 21

 MessageText:

 TA_IsGenuineEx() previously had a TA_E_INET error, and instead
 of hammering the end-user's network, TA_IsGenuineEx() is waiting
 5 hours before rechecking on the network.
*/
#define TA_E_INET_DELAYED ((HRESULT)0x00000015L)


/*
 MessageId: TA_E_FEATURES_CHANGED
 Message code (in Hex): 0x16
 Message code (in Decimal): 22

 MessageText:

 If TA_IsGenuine() or TA_IsGenuineEx() reactivated and the custom
 license fields have changed, then this will be the return
 code. Treat this as a success.
*/
#define TA_E_FEATURES_CHANGED ((HRESULT)0x00000016L)


/*
 MessageId: TA_E_ANDROID_NOT_INIT
 Message code (in Hex): 0x17
 Message code (in Decimal): 23

 MessageText:

 You didn't call the TA_InitAndroid() function before using
 the TurboActivate functions. This only applies to using TurboActivate
 on Android -- specifically using TA from native code. You don't
 need to call this from Java.
*/
#define TA_E_ANDROID_NOT_INIT ((HRESULT)0x00000017L)


/*
 MessageId: TA_E_NO_MORE_DEACTIVATIONS
 Message code (in Hex): 0x18
 Message code (in Decimal): 24

 MessageText:

 This product key had a limited number of allowed deactivations.
 No more deactivations are allowed for the product key. This product
 is still activated on this computer.
*/
#define TA_E_NO_MORE_DEACTIVATIONS ((HRESULT)0x00000018L)


/*
 MessageId: TA_E_ACCOUNT_CANCELED
 Message code (in Hex): 0x19
 Message code (in Decimal): 25

 MessageText:

 Can't activate or start a verified trial because the LimeLM
 account is cancelled.
*/
#define TA_E_ACCOUNT_CANCELED ((HRESULT)0x00000019L)


/*
 MessageId: TA_E_ALREADY_ACTIVATED
 Message code (in Hex): 0x1A
 Message code (in Decimal): 26
 
 MessageText:

 You can't use a product key because your app is already activated
 with a product key. To use a new product key, then first deactivate using
 either the TA_Deactivate() or TA_DeactivationRequestToFile().
*/
#define TA_E_ALREADY_ACTIVATED ((HRESULT)0x0000001AL)


/*
 MessageId: TA_E_INVALID_HANDLE
 Message code (in Hex): 0x1B
 Message code (in Decimal): 27

 MessageText:

 The handle is not valid. To get a handle use TA_GetHandle().
*/
#define TA_E_INVALID_HANDLE ((HRESULT)0x0000001BL)


/*
 MessageId: TA_E_ENABLE_NETWORK_ADAPTERS
 Message code (in Hex): 0x1C
 Message code (in Decimal): 28

 MessageText:

 There are network adapters on the system that are disabled and
 TurboActivate couldn't read their hardware properties (even after trying
 and failing to enable the adapters automatically). Enable the network adapters,
 re-run the function, and TurboActivate will be able to "remember" the adapters
 even if the adapters are disabled in the future.

 Note: The network adapters do not need an active Internet connections. They just
       need to not be disabled. Whether they are or are not connected to the
       internet/intranet is not important and does not affect this error code at all.


 On Linux you'll get this error if you don't have any real network adapters attached.
 For example if you have no "eth[x]", "wlan[x]", "en[x]", "wl[x]", "ww[x]", "sl[x]",
 "em[x]", or "p[x]p[y]" network interface devices.

 See: https://wyday.com/limelm/help/faq/#disabled-adapters
*/
#define TA_E_ENABLE_NETWORK_ADAPTERS ((HRESULT)0x0000001CL)


/*
 MessageId: TA_E_ALREADY_VERIFIED_TRIAL
 Message code (in Hex): 0x1D
 Message code (in Decimal): 29

 MessageText:

 The trial is already a verified trial. You need to use the "TA_VERIFIED_TRIAL"
 flag. Can't "downgrade" a verified trial to an unverified trial.
*/
#define TA_E_ALREADY_VERIFIED_TRIAL ((HRESULT)0x0000001DL)


/*
 MessageId: TA_E_TRIAL_EXPIRED
 Message code (in Hex): 0x1E
 Message code (in Decimal): 30

 MessageText:

 The trial has expired. You must request a trial extension from the company.
*/
#define TA_E_TRIAL_EXPIRED ((HRESULT)0x0000001EL)


/*
 MessageId: TA_E_MUST_SPECIFY_TRIAL_TYPE
 Message code (in Hex): 0x1F
 Message code (in Decimal): 31

 MessageText:

 You must specify the trial type (TA_UNVERIFIED_TRIAL or TA_VERIFIED_TRIAL).
 And you can't use both flags. Choose one or the other. We recommend TA_VERIFIED_TRIAL.
*/
#define TA_E_MUST_SPECIFY_TRIAL_TYPE ((HRESULT)0x0000001FL)


/*
 MessageId: TA_E_MUST_USE_TRIAL
 Message code (in Hex): 0x20
 Message code (in Decimal): 32

 MessageText:

 You must call TA_UseTrial() before you can get the number of trial days remaining.
*/
#define TA_E_MUST_USE_TRIAL ((HRESULT)0x00000020L)


/*
 MessageId: TA_E_NO_MORE_TRIALS_ALLOWED
 Message code (in Hex): 0x21
 Message code (in Decimal): 33

 MessageText:

 In the LimeLM account either the trial days is set to 0, OR the account is set
 to not auto-upgrade and thus no more verified trials can be made.
*/
#define TA_E_NO_MORE_TRIALS_ALLOWED ((HRESULT)0x00000021L)


/*
 MessageId: TA_E_BROKEN_WMI
 Message code (in Hex): 0x22
 Message code (in Decimal): 34

 MessageText:

 The WMI repository on the computer is broken. To fix the WMI repository
 see the instructions here:

 https://wyday.com/limelm/help/faq/#fix-broken-wmi
*/
#define TA_E_BROKEN_WMI ((HRESULT)0x00000022L)


/*
 MessageId: TA_E_INET_TIMEOUT
 Message code (in Hex): 0x23
 Message code (in Decimal): 35
 
 MessageText:
 
 The connection to the server timed out because a long period of time
 elapsed since the last data was sent or received.
*/
#define TA_E_INET_TIMEOUT ((HRESULT)0x00000023L)


/*
 MessageId: TA_E_INET_TLS
 Message code (in Hex): 0x24
 Message code (in Decimal): 36

 MessageText:

 The secure connection to the activation servers failed due to a TLS or certificate
 error. This is most often caused by MITM (man-in-the-middle) attempts on corporate
 networks or, if on Unix operating systems (macOS, Linux, BSD, etc.), it's caused
 by out-of-date or missing "CA certificates". This means either keeping your system
 itself up-to-date, or manually updating the CA certs.

 More information here: https://wyday.com/limelm/help/faq/#internet-error
*/
#define TA_E_INET_TLS ((HRESULT)0x00000024L)


/* Possible callback statuses from the TrialCallbackType function: */


/* Callback-status value used when the trial has expired.
*/
#define TA_CB_EXPIRED ((HRESULT)0x00000000UL)

/* Callback-status value used when the trial has expired due to date/time fraud.
*/
#define TA_CB_EXPIRED_FRAUD ((HRESULT)0x00000001UL)


#endif /* TURBOACTIVATE_H_INCLUDED */
