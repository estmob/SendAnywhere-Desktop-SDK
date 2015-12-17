#ifndef SENDANYWHERE_H_
#define SENDANYWHERE_H_

#include <stdbool.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

void sendanywhere_init(const char* apiKey, const char* tokenPath);
void sendanywhere_cleanup();

typedef enum {
    SENDANYWHERE_STATE_UNDEFINED = 0,

    // all task
    SENDANYWHERE_STATE_FINISHED,
    SENDANYWHERE_STATE_ERROR,
    SENDANYWHERE_STATE_PREPARING,

    // trasnfer task
    SENDANYWHERE_STATE_TRANSFERRING,
} SendAnywhereState;

typedef enum {
    SENDANYWHERE_DETAILED_STATE_UNDEFINED = 0,

    // FINISHED
    SENDANYWHERE_DETAILED_STATE_FINISHED_SUCCESS,
    SENDANYWHERE_DETAILED_STATE_FINISHED_CANCEL,
    SENDANYWHERE_DETAILED_STATE_FINISHED_ERROR,

    // ERROR
    SENDANYWHERE_DETAILED_STATE_ERROR_WRONG_API_KEY,
    SENDANYWHERE_DETAILED_STATE_ERROR_SERVER,
    // Send
    SENDANYWHERE_DETAILED_STATE_ERROR_NO_REQUEST,
    SENDANYWHERE_DETAILED_STATE_ERROR_NO_EXIST_FILE,
    // Receive
    SENDANYWHERE_DETAILED_STATE_ERROR_FILE_NO_DOWNLOAD_PATH,
    SENDANYWHERE_DETAILED_STATE_ERROR_FILE_NO_DISK_SPACE,
    SENDANYWHERE_DETAILED_STATE_ERROR_NO_EXIST_KEY,

    // PREPARING
    SENDANYWHERE_DETAILED_STATE_PREPARING_UPDATED_KEY,
    SENDANYWHERE_DETAILED_STATE_PREPARING_UPDATED_FILE_LIST,

    // TRANSFERRING
    SENDANYWHERE_DETAILED_STATE_TRANSFERRING,
} SendAnywhereDetailedState;

typedef void* SendAnywhereTask;

typedef void (*sendanywhere_listener_function)(
                    SendAnywhereState state,
                    SendAnywhereDetailedState detailedState,
                    const void* param,
                    void* userptr);

typedef struct {
    const wchar_t* name;
    const wchar_t* fullPath;
    unsigned long long size;
    unsigned long long sent;
} SendAnywhereTransferFileInfo;

typedef struct {
    unsigned int number;
    SendAnywhereTransferFileInfo* fileInfo;
} SendAnywhereAllTransferFileInfo;

SendAnywhereTask sendanywhere_create_send(
        const wchar_t* files[], unsigned int fileNumber);
SendAnywhereTask sendanywhere_create_receive(
        const wchar_t* key, const wchar_t* destDirPath);
void sendanywhere_close(SendAnywhereTask task);

void sendanywhere_start(SendAnywhereTask task);
bool sendanywhere_is_running(SendAnywhereTask task);
void sendanywhere_wait(SendAnywhereTask task);
void sendanywhere_cancel(SendAnywhereTask task);

void sendanywhere_set_listner(SendAnywhereTask task,
                        sendanywhere_listener_function func, void* userptr);
#ifdef __cplusplus
}
#endif

#endif // SENDANYWHERE_H_

