Send Anywhere Native Desktop SDK
===

Initialize
---
```
void sendanywhere_init(const char* apiKey, const char* tokenPath);
void sendanywhere_cleanup();
```

### void sendanywhere_init(const char* apiKey, const char* tokenPath)
Initialize Send Anywhere library.

Parameters |                                  |
-----------|----------------------------------|
apiKey     | Your API key.                    |
tokenPath  | A token path for saving session. |

### void sendanywhere_cleanup() ###
Cleanup library.


Create task
---
```
SendAnywhereTask sendanywhere_create_send(
                      const wchar_t* files[],
                      unsigned int fileNumber);
SendAnywhereTask sendanywhere_create_receive(
                     const wchar_t* key,
                     const wchar_t* destDirPath);

void sendanywhere_close(SendAnywhereTask task);
```

### SendAnywhereTask sendanywhere_create_send(const wchar_t* files[], unsigned int fileNumber)
Initialize Send Anywhere library.

Parameters |                |
-----------|----------------|
files      | Files to send. |
fileNumber | Number of file.|

Return |                      |
-------|----------------------|
value  | A handle to new task |

### SendAnywhereTask sendanywhere_create_receive(const wchar_t* key, const wchar_t* destDirPath)
Start task for sending or receiving.
Initialize Send Anywhere library.

Parameters |                    |
-----------|--------------------|
key        | The KEY of sender. |
destDirPath| Save path.         |

Return |                      |
-------|----------------------|
value  | A handle to new task |

### void sendanywhere_close(SendAnywhereTask task)
Close a handle.

Parameters |                       |
-----------|-----------------------|
task       | A Handle to the task. |


Control task
---
```
void sendanywhere_start(SendAnywhereTask task);

bool sendanywhere_is_running(SendAnywhereTask task);

void sendanywhere_wait(SendAnywhereTask task);

void sendanywhere_cancel(SendAnywhereTask task);
```

### void sendanywhere_start(SendAnywhereTask task)
Start task for sending or receiving.

Parameters |                       |
-----------|-----------------------|
task       | A Handle to the task. |

### bool sendanywhere_is_running(SendAnywhereTask task)
Indicates whether the task is running.

Parameters |                       |
-----------|-----------------------|
task       | A Handle to the task. |

Return |                                               |
-------|-----------------------------------------------|
value  | True if the task is running, false otherwise. |

### void sendanywhere_wait(SendAnywhereTask task)
Wait until task is finished.

Parameters |                       |
-----------|-----------------------|
task       | A Handle to the task. |

### void sendanywhere_cancel(SendAnywhereTask task)
Cancel the task to stop.

Parameters |                       |
-----------|-----------------------|
task       | A Handle to the task. |


Listener for task
---
```
typedef void (*sendanywhere_listener_function)(
                    SendAnywhereState state,
                    SendAnywhereDetailedState detailedState,
                    const void* param,
                    void* userptr);
                    
void sendanywhere_set_listner(
          SendAnywhereTask task,
          sendanywhere_listener_function func,
          void* userptr);
```
```
typedef enum {
    SENDANYWHERE_STATE_UNDEFINED,

    SENDANYWHERE_STATE_FINISHED,
    SENDANYWHERE_STATE_ERROR,
    SENDANYWHERE_STATE_PREPARING,

    SENDANYWHERE_STATE_TRANSFERRING,

    SENDANYWHERE_STATE_NOTIFICATION,
} SendAnywhereState;

typedef enum {
    SENDANYWHERE_DETAILED_STATE_UNDEFINED,

    SENDANYWHERE_DETAILED_STATE_FINISHED_SUCCESS,
    SENDANYWHERE_DETAILED_STATE_FINISHED_CANCEL,
    SENDANYWHERE_DETAILED_STATE_FINISHED_ERROR,

    SENDANYWHERE_DETAILED_STATE_ERROR_WRONG_API_KEY,
    SENDANYWHERE_DETAILED_STATE_ERROR_SERVER,

    SENDANYWHERE_DETAILED_STATE_ERROR_NO_REQUEST,
    SENDANYWHERE_DETAILED_STATE_ERROR_NO_EXIST_FILE,

    SENDANYWHERE_DETAILED_STATE_ERROR_FILE_NO_DOWNLOAD_PATH,
    SENDANYWHERE_DETAILED_STATE_ERROR_FILE_NO_DISK_SPACE,
    SENDANYWHERE_DETAILED_STATE_ERROR_NO_EXIST_KEY,

    SENDANYWHERE_DETAILED_STATE_PREPARING_UPDATED_KEY,
    SENDANYWHERE_DETAILED_STATE_PREPARING_UPDATED_FILE_LIST,

    SENDANYWHERE_DETAILED_STATE_TRANSFERRING,
} SendAnywhereDetailedState;
```

```
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
```


### void sendanywhere_set_listner(SendAnywhereTask task, sendanywhere_listener_function func, void* userptr);
Register a callback to be invoked when transfer event occurs.

### typedef void (*sendanywhere_listener_function)(SendAnywhereState state, SendAnywhereDetailedState detailedState, const void* param, void* userptr);

SendAnywhereState (SENDANYWHERE_STATE_) | SendAnywhereDetailedState (SENDANYWHERE_DETAILED_STATE_) | param           |
-------------|----------------------------- |-----------------|
FINISHED     | FINISHED_SUCCESS             |                 |
             | FINISHED_CANCEL              |                 |
             | FINISHED_ERROR               |                 |
ERROR        | ERROR_WRONG_API_KEY          |                 |
             | ERROR_SERVER                 |                 |
             | ERROR_NO_REQUEST             |                 |
             | ERROR_NO_EXIST_FILE          |                 |
             | ERROR_NO_EXIST_KEY           |                 |
             | ERROR_FILE_NO_DOWNLOAD_PATH  |                 |
             | ERROR_FILE_NO_DISK_SPACE     |                 |
             | ERROR_FILE_DISK_NOT_MOUNTED  |                 |
PREPARING    | PREPARING_UPDATED_KEY        | const wchar_t*  |
             | PREPARING_UPDATED_FILE_LIST  | const SendAnywhereAllTransferFileInfo* |
TRANSFERRING | TRANSFERRING                 | const SendAnywhereTransferFileInfo* |


* Flow Step
  * PREPARING_UPDATED_KEY
    * PREPARING_UPDATED_FILE_LIST
      * TRANSFERRING
      * TRANSFERRING
      * ...
      * TRANSFERRING
      * TRANSFERRING
        * FINISHED_SUCCESS
        * FINISHED_CANCEL
        * ERROR
          * FINISHED_ERROR
