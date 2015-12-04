#include "sendanywhere.h"
#include <stdio.h>

void callback(SendAnywhereState state, SendAnywhereDetailedState detailedState,
					const void* param, void* userptr) {
	int i;

	if(state == SENDANYWHERE_STATE_PREPARING) {
		if(detailedState == SENDANYWHERE_DETAILED_STATE_PREPARING_UPDATED_FILE_LIST) {
			const SendAnywhereAllTransferFileInfo* files = (const SendAnywhereAllTransferFileInfo*)param;
			for(i=0; i<files->number; ++i) {
				SendAnywhereTransferFileInfo* file = files->fileInfo + i;
				printf("%ls : %lld bytes\n", file->name, file->size);
			}
		}
	} else if(state == SENDANYWHERE_STATE_TRANSFERRING) {
		const SendAnywhereTransferFileInfo* file = (const SendAnywhereTransferFileInfo*)param;
		printf("%ls => %lld/%lld\n", file->fullPath, file->sent, file->size);
	} else if(state == SENDANYWHERE_STATE_FINISHED) {
		switch(detailedState) {
		case SENDANYWHERE_DETAILED_STATE_FINISHED_SUCCESS:
			printf("Transfer finished (success)\n");
			break;
		case SENDANYWHERE_DETAILED_STATE_FINISHED_CANCEL:
			printf("Transfer finished (canceled)\n");
			break;
		case SENDANYWHERE_DETAILED_STATE_FINISHED_ERROR:
			printf("Transfer finished (error!)\n");
			break;
		}
	} else if(state == SENDANYWHERE_STATE_ERROR) {
		switch(detailedState) {
		case SENDANYWHERE_DETAILED_STATE_ERROR_SERVER:
			printf("Network or Server Error!\n");
			break;
		case SENDANYWHERE_DETAILED_STATE_ERROR_FILE_NO_DOWNLOAD_PATH:
			printf("Download path does not exist!\n");
			break;
		case SENDANYWHERE_DETAILED_STATE_ERROR_FILE_NO_DISK_SPACE:
			printf("No disk space!\n");
			break;
		case SENDANYWHERE_DETAILED_STATE_ERROR_NO_EXIST_KEY:
			printf("Wrong KEY!\n");
			break;
		}
	}
}

int main(int argc, char** argv) {
	sendanywhere_init("YOUR_API_KEY", ".sendanywhere-token");

	SendAnywhereTask task = sendanywhere_create_receive(L"KEY", L"/tmp");
	sendanywhere_set_listner(task, callback, 0);

	sendanywhere_start(task);
	sendanywhere_wait(task);

	sendanywhere_cleanup();

    return 0;
}
