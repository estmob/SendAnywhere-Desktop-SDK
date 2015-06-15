#ifndef PAPRIKA_TRANSFER_H_
#define PAPRIKA_TRANSFER_H_

#include <stdbool.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
#define PAPRIKA_EXTERN __declspec(dllexport)
#else
#define PAPRIKA_EXTERN
#endif /* _WIN32 */

PAPRIKA_EXTERN void paprika_set_apikey(const char* key);

// auth function
typedef void* PaprikaAuthToken;

PAPRIKA_EXTERN PaprikaAuthToken paprika_auth_create();
PAPRIKA_EXTERN PaprikaAuthToken paprika_auth_create_with_deviceid(
                                    const char* id, const char* password);
PAPRIKA_EXTERN void paprika_auth_close(PaprikaAuthToken auth);

PAPRIKA_EXTERN const char* paprika_auth_get_device_id(PaprikaAuthToken auth);
PAPRIKA_EXTERN const char* paprika_auth_get_device_password(PaprikaAuthToken auth);

// option function
typedef enum {
    PAPRIKA_OPTION_API_SERVER = 0,
    PAPRIKA_OPTION_RESERVED1,
    PAPRIKA_OPTION_RESERVED2,
    PAPRIKA_OPTION_PROFILE_NAME,
} PaprikaOptionKey;

typedef void* PaprikaOption;

PAPRIKA_EXTERN PaprikaOption paprika_option_create();
PAPRIKA_EXTERN void paprika_option_close(PaprikaOption option);

PAPRIKA_EXTERN void paprika_option_set_value(PaprikaOption option,
                        PaprikaOptionKey key, const void* value);

// task function

typedef enum {
    PAPRIKA_STATE_UNKNOWN = 0,

    // all task
    PAPRIKA_STATE_FINISHED,
    PAPRIKA_STATE_ERROR,
    PAPRIKA_STATE_PREPARING,

    // trasnfer task
    PAPRIKA_STATE_TRANSFERRING,
} PaprikaState;

typedef enum {
    PAPRIKA_DETAILED_STATE_UNKNOWN = 0,

    // all task
    PAPRIKA_DETAILED_STATE_FINISHED_SUCCESS,
    PAPRIKA_DETAILED_STATE_FINISHED_CANCEL,
    PAPRIKA_DETAILED_STATE_FINISHED_ERROR,

    PAPRIKA_DETAILED_STATE_ERROR_WRONG_API_KEY,
    PAPRIKA_DETAILED_STATE_ERROR_RESERVED5,
    PAPRIKA_DETAILED_STATE_ERROR_RESERVED6,
    PAPRIKA_DETAILED_STATE_ERROR_RESERVED7,
    PAPRIKA_DETAILED_STATE_ERROR_RESERVED8,

    PAPRIKA_DETAILED_STATE_PREPARING_UPDATED_DEVICE_ID,
    PAPRIKA_DETAILED_STATE_RESERVED10,

    // transfer task
    /* PAPRIKA_TASK_UPLOAD, PAPRIKA_TASK_UPLOAD_REQ, PAPRIKA_TASK_UPLOAD_RES,
       PAPRIKA_TASK_DOWNLOAD, PAPRIKA_TASK_DOWNLOAD_REQ, PAPRIKA_TASK_DOWNLOAD_RES */
    PAPRIKA_DETAILED_STATE_RESERVED11,
    PAPRIKA_DETAILED_STATE_PREPARING_UPDATED_KEY,
    PAPRIKA_DETAILED_STATE_RESERVED13,
    PAPRIKA_DETAILED_STATE_RESERVED14,
    PAPRIKA_DETAILED_STATE_PREPARING_UPDATED_FILE_LIST,
    PAPRIKA_DETAILED_STATE_RESERVED16,

    PAPRIKA_DETAILED_STATE_RESERVED17,
    PAPRIKA_DETAILED_STATE_RESERVED18,
    PAPRIKA_DETAILED_STATE_RESERVED19,
    PAPRIKA_DETAILED_STATE_RESERVED20,
    PAPRIKA_DETAILED_STATE_RESERVED21,
    PAPRIKA_DETAILED_STATE_RESERVED22,
    PAPRIKA_DETAILED_STATE_RESERVED23,

    PAPRIKA_DETAILED_STATE_RESERVED24,
    PAPRIKA_DETAILED_STATE_RESERVED25,

    // PAPRIKA_TASK_UPLOAD
    PAPRIKA_DETAILED_STATE_ERROR_NO_REQUEST,
    PAPRIKA_DETAILED_STATE_ERROR_NO_EXIST_FILE,

    // PAPRIKA_TASK_DOWNLOAD
    PAPRIKA_DETAILED_STATE_ERROR_FILE_NO_DOWNLOAD_PATH,
    PAPRIKA_DETAILED_STATE_ERROR_FILE_NO_DISK_SPACE,

    // PAPRIKA_TASK_DOWNLOAD, PAPRIKA_TASK_DELETE_KEY, PAPRIKA_TASK_QUERY_KEY
    PAPRIKA_DETAILED_STATE_ERROR_NO_EXIST_KEY,

} PaprikaDetailedState;


typedef void* PaprikaTask;

typedef void (*paprika_listener_function)(
                    PaprikaState state,
                    PaprikaDetailedState detailedState,
                    const void* param,
                    void* userptr);

typedef struct {
    const wchar_t* name;
    const wchar_t* fullPath;
    unsigned long long size;
    unsigned long long sent;
} PaprikaTransferFileState;

typedef struct {
    unsigned int number;
    PaprikaTransferFileState* fileState;
} PaprikaAllTransferFileState;


// send/recv function
PAPRIKA_EXTERN PaprikaTask paprika_create_upload(
        const wchar_t* files[], unsigned int fileNumber,
        const wchar_t* wishKey, bool async, bool secureKey);

PAPRIKA_EXTERN PaprikaTask paprika_create_download(
        const wchar_t* key, const wchar_t* destDirPath);


// general function for task
PAPRIKA_EXTERN void paprika_close(PaprikaTask task);

PAPRIKA_EXTERN void paprika_start(PaprikaTask task);
PAPRIKA_EXTERN void paprika_wait(PaprikaTask task);
PAPRIKA_EXTERN void paprika_cancel(PaprikaTask task);
PAPRIKA_EXTERN bool paprika_is_running(PaprikaTask task);

PAPRIKA_EXTERN void paprika_set_auth(PaprikaTask task, PaprikaAuthToken auth);
PAPRIKA_EXTERN void paprika_set_option(PaprikaTask task, PaprikaOption option);
PAPRIKA_EXTERN void paprika_set_listner(PaprikaTask task,
                        paprika_listener_function func, void* userptr);

#ifdef __cplusplus
}
#endif

#endif // PAPRIKA_TRANSFER_H_

