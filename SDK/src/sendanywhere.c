#include "sendanywhere.h"
#include "internal/paprika.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static PaprikaAuthToken gToken;
static const char* gTokenPath;
static PaprikaOption gOption;

void sendanywhere_init(const char* apiKey, const char* tokenPath) {
	paprika_set_apikey(apiKey);

	gTokenPath = tokenPath;

	if(!gToken) {
		char deivec_id[64] = {0,};
		char deivec_password[64] = {0,};

		FILE * pFile;
		pFile = fopen(gTokenPath, "r");
		if(pFile) {
			fscanf(pFile, "%s %s", deivec_id, deivec_password);
			fclose(pFile);
		}

		if(strlen(deivec_id) > 0 && strlen(deivec_password) > 0) {
			gToken = paprika_auth_create_with_deviceid(deivec_id, deivec_password);
		} else {
			gToken = paprika_auth_create();
		}
	}

	if(!gOption) {
		gOption = paprika_option_create();
	}
	paprika_option_set_value(gOption, PAPRIKA_OPTION_PROFILE_NAME, L"Send Anywhere SDK");
}

void sendanywhere_cleanup() {
	paprika_auth_close(gToken);
	paprika_option_close(gOption);
}


typedef struct {
	PaprikaTask task;
	sendanywhere_listener_function listener;
	void* listenerUserPtr;
} TaskData;

static void callbackFunc(PaprikaState pState,
                    PaprikaDetailedState pDetailedState,
                    const void* param,
                    void* userptr) {
	TaskData* task = (TaskData*)userptr;
	PaprikaTask pTask = ((TaskData*)task)->task;
	sendanywhere_listener_function listener = task->listener;

	SendAnywhereState state = SENDANYWHERE_STATE_UNDEFINED;
	SendAnywhereDetailedState detailedState = SENDANYWHERE_DETAILED_STATE_UNDEFINED;

#define CONVERT(STATE, DSTATE) \
	} else if(pState == PAPRIKA_STATE_##STATE && \
		pDetailedState == PAPRIKA_DETAILED_STATE_##STATE##_##DSTATE) { \
		state = SENDANYWHERE_STATE_##STATE; \
		detailedState = SENDANYWHERE_DETAILED_STATE_##STATE##_##DSTATE;

	if(0) {
		CONVERT(FINISHED, SUCCESS);
		CONVERT(FINISHED, CANCEL);
		CONVERT(FINISHED, ERROR);

		CONVERT(ERROR, WRONG_API_KEY);
		CONVERT(ERROR, NO_REQUEST);
		CONVERT(ERROR, NO_EXIST_FILE);
		CONVERT(ERROR, FILE_NO_DOWNLOAD_PATH);
		CONVERT(ERROR, FILE_NO_DISK_SPACE);
		CONVERT(ERROR, NO_EXIST_KEY);

		CONVERT(PREPARING, UPDATED_KEY);
		CONVERT(PREPARING, UPDATED_FILE_LIST);
	} else if(pState == PAPRIKA_STATE_ERROR) {
		state = SENDANYWHERE_STATE_ERROR;
		detailedState = SENDANYWHERE_DETAILED_STATE_ERROR_SERVER;
	} else if(pState == PAPRIKA_STATE_TRANSFERRING) {
		state = SENDANYWHERE_STATE_TRANSFERRING;
		detailedState = SENDANYWHERE_DETAILED_STATE_TRANSFERRING;
	} else if(pDetailedState == PAPRIKA_DETAILED_STATE_PREPARING_UPDATED_DEVICE_ID) {
		const char* deivec_id = paprika_auth_get_device_id(gToken);
		const char* deivec_password  = paprika_auth_get_device_password(gToken);

		FILE * pFile;
		pFile = fopen(gTokenPath, "w");
		fprintf(pFile, "%s %s", deivec_id, deivec_password);
		fclose(pFile);
	}

#undef CONVERT

	if(listener &&
		state != SENDANYWHERE_STATE_UNDEFINED &&
		detailedState != SENDANYWHERE_DETAILED_STATE_UNDEFINED) {
		listener(state, detailedState, param, task->listenerUserPtr);
	}
}


SendAnywhereTask sendanywhere_create_send(
        const wchar_t* files[], unsigned int fileNumber) {
	TaskData* task = (TaskData*)malloc(sizeof(TaskData));

	((TaskData*)task)->task = paprika_create_upload(files, fileNumber, PAPRIKA_TRANSFER_DIRECT);

	return task;
}

SendAnywhereTask sendanywhere_create_receive(
        const wchar_t* key, const wchar_t* destDirPath) {
	TaskData* task = (TaskData*)malloc(sizeof(TaskData));

	((TaskData*)task)->task = paprika_create_download(key, destDirPath);

	return task;
}

void sendanywhere_start(SendAnywhereTask task) {
	PaprikaTask pTask = ((TaskData*)task)->task;

    paprika_set_auth(pTask, gToken);
    paprika_set_option(pTask, gOption);
    paprika_set_listner(pTask, callbackFunc, task);

    paprika_start(pTask);
}

void sendanywhere_cancel(SendAnywhereTask task) {
	PaprikaTask pTask = ((TaskData*)task)->task;
    paprika_cancel(pTask);
}

bool sendanywhere_is_running(SendAnywhereTask task) {
	PaprikaTask pTask = ((TaskData*)task)->task;
    paprika_is_running(pTask);
}

void sendanywhere_wait(SendAnywhereTask task) {
	PaprikaTask pTask = ((TaskData*)task)->task;
    paprika_wait(pTask);
}

void sendanywhere_close(SendAnywhereTask task) {
	PaprikaTask pTask = ((TaskData*)task)->task;
    paprika_close(pTask);

    free(task);
}

void sendanywhere_set_listner(SendAnywhereTask task,
                        sendanywhere_listener_function func, void* userptr) {
	((TaskData*)task)->listener = func;
	((TaskData*)task)->listenerUserPtr = userptr;
}
